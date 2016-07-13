
#include "fabic/surface/window.hpp"
#include "fabic/surface/window-inlines.hpp"
#include "fabic/surface/elements-inlines.hpp"
#include "fabic/surface/xcb-inlines.hpp"

TL_NS_BEGIN
namespace xcb {


// todo: create();

Window::Window(xcb_shared_ptr xcb_)
  : Block( nullptr )
  , xcb_( xcb_ )
  , windowXid( 0 ) // fixme: 0 ?
{ }


// TODO: drop this one ?
Window::Window(xcb_shared_ptr xcb_, xcb_window_t xid)
  : Block( nullptr )
  , xcb_( xcb_ )
  , windowXid(xid)
{ }


Window::~Window() { }


xcb_visualtype_t *
Window::getVisualType()
{
  if (this->visual != nullptr)
    return this->visual;

  this->visual = xcb_aux_get_visualtype(
      this->xcb_->getXcbConnectionPtr(),
      this->xcb_->getScreenNumber(),
      this->get_attributes()->visual // xcb_visualid_t
    );

  return this->visual;
}


std::unique_ptr<xcb_get_window_attributes_reply_t>
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
    throw Xcb::base_exception();

  return std::unique_ptr<xcb_get_window_attributes_reply_t>(attrs_);
}


std::unique_ptr<xcb_get_geometry_reply_t>
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
    throw Xcb::base_exception();

  assert( error_ == nullptr );

  return std::unique_ptr<xcb_get_geometry_reply_t>(geom_);
}


// virtual btw.
Window::self_ptr
  Window::create(
      uint16_t          width,
      uint16_t          height,
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

  attributes[ XCB_CW_BACK_PIXEL ]   = screen.black_pixel;
  attributes[ XCB_CW_BORDER_PIXEL ] = screen.white_pixel;

  uint32_t value_mask  = attributes.bitmask();
  uint32_t* value_list = attributes.data();

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

  this->windowXid = wid;

  this->xcb_->registerWindow( this->shared_from_base< Window >() );

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

  this->computePositionning(
      tk::pixels_dimensions_t(width, height),
      tk::pixels_position_t(x, y)
    );

  this->render();

  // todo: ^ get the computed bbox and ensure min. window dimensions (prevent
  // todo:   too small window size).
  // todo: eventually have an aspect-ratio thing wrt. to window content, like
  // todo: when we have 2+ split-editor...
}


// virtual override btw.
tk::pixels_dimensions_t
  Window::preComputePositionning(
    tk::pixels_dimensions_t dimensions,
    tk::pixels_position_t   position
  )
{
  this->attributes()->positionning()->dimensions( dimensions );
  //this->attributes()->positionning()->xy(x, y); // todo: position( position
  // ) ??

  return tk::Element::preComputePositionning(dimensions, position);
}


// virtual override btw.
void Window::render()
{
  this->_surface.initXcb(
      this->xcb()->getXcbConnectionPtr(),
      this->getDrawableXid(),
      this->getVisualType(),
      this->attributes()->positionning()->dimensions()
  );

  tk::Element::render();
}


} // xcb ns.
TL_NS_END
