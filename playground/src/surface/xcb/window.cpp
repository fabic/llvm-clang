
#include "fabic/surface/xcb/window.hpp"
#include "fabic/surface/xcb/inline-decls.hpp"
//#include "fabic/surface/ui/elements-inlines.hpp"
#include "fabic/logging.hpp"

namespace sf {
  namespace xcb {


    Window::Window(xcb_shared_ptr xcb_)
        : xcb_( xcb_ )
        , _xid( 0 ) // fixme: 0 ?
    { }


    // TODO: drop this one ?
    Window::Window(xcb_shared_ptr xcb_, xcb_window_t xid)
        : xcb_( xcb_ )
        , _xid(xid)
    { }


    Window::~Window() { }


    xcb_visualtype_t *
      Window::getVisualType()
    {
      if (this->visual_type_ != nullptr)
        return this->visual_type_;

      this->visual_type_ = xcb_aux_get_visualtype(
          this->xcb_->getXcbConnectionPtr(),
          this->xcb_->getScreenNumber(),
          this->get_attributes()->visual // xcb_visualid_t
      );

      return this->visual_type_;
    }


    std::unique_ptr< xcb_get_window_attributes_reply_t >
      Window::get_attributes()
    {
      auto conn_ = this->xcb_->getXcbConnectionPtr();

      auto cookie = xcb_get_window_attributes(conn_, this->getXid());

      auto attrs_ =
          xcb_get_window_attributes_reply(
              conn_,
              cookie,
              NULL // TODO: generic event thing ?
          );

      if (attrs_ == nullptr)
        throw got_nullptr_from_xcb_ex();

      return std::unique_ptr< xcb_get_window_attributes_reply_t >( attrs_ );
    }


    std::unique_ptr< xcb_get_geometry_reply_t >
      Window::get_geometry()
    {
      auto conn_ = this->xcb_->getXcbConnectionPtr();

      auto drawable = this->getXid();

      auto cookie = xcb_get_geometry(conn_, drawable);

      xcb_generic_error_t *error_ = nullptr;

      auto geom_ =
          xcb_get_geometry_reply(
              conn_,
              cookie,
              &error_
          );

      if (geom_ == nullptr)
        throw got_nullptr_from_xcb_ex();

      assert( error_ == nullptr );
      // TODO: ^ to sthg with it ? throw ? log ?

      return std::unique_ptr< xcb_get_geometry_reply_t >( geom_ );
    }


    // virtual btw.
    Window::self_ptr
      Window::create(
          uint16_t width,
          uint16_t height,
          window_shared_ptr parentWindow
      )
    {
      if (parentWindow == nullptr)
        parentWindow = this->xcb_->getRootWindow();

      auto screen = this->xcb_->getScreenInfo();

      constexpr uint32_t bitmask =
          XCB_CW_BACK_PIXEL
        | XCB_CW_BORDER_PIXEL
        | XCB_CW_EVENT_MASK;

      MaskValues<bitmask> attributes;

      attributes[ XCB_CW_EVENT_MASK ] =
          XCB_EVENT_MASK_EXPOSURE
        | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW
        | XCB_EVENT_MASK_KEY_PRESS    | XCB_EVENT_MASK_KEY_RELEASE
        | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
        | XCB_EVENT_MASK_POINTER_MOTION
          ;

      attributes[ XCB_CW_BACK_PIXEL ]   = 0x00ddccbb; // screen.white_pixel;
      attributes[ XCB_CW_BORDER_PIXEL ] = screen.black_pixel;

      uint32_t   value_mask = attributes.bitmask();
      uint32_t * value_list = attributes.data();

      auto wid = this->xcb_->generate_xid();

      uint16_t       borderWidth = 10;
      uint16_t       windowClass = XCB_WINDOW_CLASS_INPUT_OUTPUT;
      xcb_visualid_t visualXid   = XCB_COPY_FROM_PARENT;

      // See definition at `/usr/include/xcb/xproto.h:5564`
      // https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html#helloworld
      // http://rosettacode.org/wiki/Window_creation/X11#XCB
      xcb_void_cookie_t _cookie =
          xcb_create_window(
              this->xcb_->getXcbConnectionPtr(),  // xcb_connection_t *c
              XCB_COPY_FROM_PARENT,        // uint8_t          depth
              wid,                         // xcb_window_t     wid
              parentWindow->getXid(),      // xcb_window_t     parent
              0, 0,                        // coordinates (int16_t x, int16_t y)
              width, height,               // dimensions  (int16_t w, int16_t h)
              borderWidth,                 // uint16_t         border_width
              windowClass,                 // uint16_t         _class
              visualXid,                   // xcb_visualid_t   visual
              value_mask,                  // uint32_t         value_mask
              value_list                   // const uint32_t   *value_list
          );

      Xcb::assert_void_cookie( _cookie );

      this->_xid = wid;

      this->xcb_->registerWindow( this->shared_from_this() );

      this->initCairoSurface();

      return this;
    }


    // virtual btw.
    void
    Window::handleEvent(const Event& event)
    {
      logtrace << "Window::handleEvent(): begin." ;

      switch( event.type() )
      {
      case EventType::EXPOSE: {
        auto ex_ = event.expose();
        this->handleEventExpose(ex_->width, ex_->height, ex_->x, ex_->y);
        break;
      }
      case EventType::KEY_PRESS: {
        break;
      }
      case EventType::KEY_RELEASE: {
        break;
      }
      default:
        logtrace << "Window::handleEvent(): skipping unknown event." ;
      }

      logtrace << "Window::handleEvent(): end." ;
    }


    // virtual btw.
    void Window::handleEventExpose(
        uint16_t width,
        uint16_t height,
        uint16_t x,
        uint16_t y
    )
    {
      logtrace << "Window::handleEventExpose(): "
               << "width = " << width << ", height = " << height
               << ", x = " << x << ", y = " << y ;

      assert (! (width & 0x8000) );
      assert (! (height & 0x8000) );

      // Drop eventual sign-bit for we'll be using int16_t from now on.
      width  &= ~0x8000;
      height &= ~0x8000;

      this->_surface.setSize( Dimensions<>(width, height) );

      this->render();

//      tk::Rectangle<> rect(
//          tk::Rectangle<>::position_t(0, 0),
//          tk::Rectangle<>::dimensions_t(width, height)
//        );
//  this->computePositionning( rect );
//  this->render();

      // todo: ^ get the computed bbox and ensure min. window dimensions (prevent
      // todo:   too small window size).
      // todo: eventually have an aspect-ratio thing wrt. to window content, like
      // todo: when we have 2+ split-editor...

      if (this->handle_expose_callback_ != nullptr)
        this->handle_expose_callback_(*this, width, height);

      this->flush();
    }


    Window::self_ptr
      Window::initCairoSurface()
    {
      auto geometry = this->get_geometry();

      Dimensions<> dimensions(
          geometry->width,
          geometry->height
      );

      this->_surface.initXCB(
          this->xcb_->getXcbConnectionPtr(),
          this->getDrawableXid(),
          this->getVisualType(),
          dimensions
        );

      return this;
    }

    // virtual btw.
    Window::self_ptr
      Window::render()
    {
      auto sf = this->surface();

      sf.set_operator(CAIRO_OPERATOR_CLEAR);
      sf.paint();

      sf.set_operator(CAIRO_OPERATOR_OVER);

      sf.source_rgba(rgba<>(255, 64, 32, 127));

      sf.set_line_width(20);

      auto dim = sf.dimensions();

      sf.move_to(Vector<>(0, 0));
      sf.line_to(Vector<>(dim.width(), dim.height()));

      sf.move_to(Vector<>(dim.width(), 0));
      sf.line_to(Vector<>(0, dim.height()));

      sf.stroke();

      return this;
    }

    // virtual override btw.
//    tk::Rectangle<>
//    Window::computePositionning(tk::Rectangle<> rect)
//    {
//      this->boundingBox( rect );
//
//      // forward processing to parent impl.
//      return tk::Element::computePositionning( rect );
//    }


      // virtual override btw.
//    void Window::render()
//    {
//      this->_surface.initXCB(
//          this->xcb()->getXcbConnectionPtr(),
//          this->getDrawableXid(),
//          this->getVisualType(),
//          this->attributes()->positionning()->dimensions()
//      );
//
//      tk::Rectangle<> rect;
//      //auto a = rect.dimensions().x();
//
//
//      tk::Element::render();
//    }


  } // xcb ns.
} // sf ns.
