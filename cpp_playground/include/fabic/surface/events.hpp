#ifndef FABIC_XCB_EVENTS_H
#define FABIC_XCB_EVENTS_H

#include <map>
#include "fabic/surface/typedefs.hpp"

TL_NS_BEGIN
namespace xcb { // todo: rename to tk:: ns.

/** Convenient enumeration for those scattered #define-s `XCB_...`
 * that relate to events.
 *
 * Related field being `response_type` of `xcb_generic_event_t`.
      typedef struct {
          uint8_t   response_type;
          ...
      } xcb_generic_event_t;
 */
enum class EventType : uint8_t {
    KEY_PRESS       = XCB_KEY_PRESS      // 2
  , KEY_RELEASE     = XCB_KEY_RELEASE    // 3
  , BUTTON_PRESS    = XCB_BUTTON_PRESS   // 4
  , BUTTON_RELEASE  = XCB_BUTTON_RELEASE // 5
  , MOTION_NOTIFY   = XCB_MOTION_NOTIFY // 6
  , ENTER_NOTIFY    = XCB_ENTER_NOTIFY // 7
  , LEAVE_NOTIFY    = XCB_LEAVE_NOTIFY // 8
  , FOCUS_IN        = XCB_FOCUS_IN // 9
  , FOCUS_OUT       = XCB_FOCUS_OUT // 10
  , KEYMAP_NOTIFY     = XCB_KEYMAP_NOTIFY // 11
  , EXPOSE            = XCB_EXPOSE // 12
  , GRAPHICS_EXPOSURE = XCB_GRAPHICS_EXPOSURE // 13
  , NO_EXPOSURE       = XCB_NO_EXPOSURE // 14
  , VISIBILITY_NOTIFY = XCB_VISIBILITY_NOTIFY // 15
  , CREATE_NOTIFY     = XCB_CREATE_NOTIFY // 16
  , DESTROY_NOTIFY    = XCB_DESTROY_NOTIFY // 17
  , UNMAP_NOTIFY      = XCB_UNMAP_NOTIFY // 18
  , MAP_NOTIFY        = XCB_MAP_NOTIFY // 19
  , MAP_REQUEST       = XCB_MAP_REQUEST // 20
  , REPARENT_NOTIFY   = XCB_REPARENT_NOTIFY // 21
  , CONFIGURE_NOTIFY  = XCB_CONFIGURE_NOTIFY // 22
  , CONFIGURE_REQUEST = XCB_CONFIGURE_REQUEST // 23
  , GRAVITY_NOTIFY    = XCB_GRAVITY_NOTIFY // 24
  , RESIZE_REQUEST    = XCB_RESIZE_REQUEST // 25
  , CIRCULATE_NOTIFY  = XCB_CIRCULATE_NOTIFY // 26
  , CIRCULATE_REQUEST = XCB_CIRCULATE_REQUEST // 27
  , PROPERTY_NOTIFY   = XCB_PROPERTY_NOTIFY // 28
  , SELECTION_CLEAR   = XCB_SELECTION_CLEAR // 29
  , SELECTION_REQUEST = XCB_SELECTION_REQUEST // 30
  , SELECTION_NOTIFY  = XCB_SELECTION_NOTIFY // 31
  , COLORMAP_NOTIFY   = XCB_COLORMAP_NOTIFY // 32
  , CLIENT_MESSAGE    = XCB_CLIENT_MESSAGE // 33
  , MAPPING_NOTIFY    = XCB_MAPPING_NOTIFY // 34
  , GE_GENERIC        = XCB_GE_GENERIC // 35
};


/**
 * Now that's quite dangerous certainly ;
 * hopefully won't this blow in someone face at some point.
 */
union EventsUnion {
  xcb_generic_event_t  _generic;

  xcb_key_press_event_t     _key_press;   // XCB_KEY_PRESS 2
    xcb_key_release_event_t _key_release; // XCB_KEY_RELEASE 3

  xcb_expose_event_t  _expose;            // XCB_EXPOSE 12


  inline EventType type() const
  {
    return (EventType) // C-style cast here too, not good.
      (this->_generic.response_type & ~0x80) ;
    // TODO: Find out what is this last ^ bit about, actually ?
  }
};


/**
 *
 */
class EventDescription {
private:
  EventType    _type;
  const char * _name;
  const char * _struct_name;

public:
  EventDescription(EventType type, const char * name, const char * xcb_struct = nullptr)
    : _type(type), _name(name), _struct_name(xcb_struct)
  {
    assert( name != nullptr );
  }

  EventType    type()        const { return this->_type; }
  const char * name()        const { return this->_name; }
  const char * struct_name() const { return this->_struct_name; }
};


/**
 *
 */
class Event {
public:
  typedef std::map<EventType, EventDescription> EventDescriptionMap_t;
  typedef const EventDescription& EventDescription_cref_t;

private:
  xcb_shared_ptr xcb_;
  std::unique_ptr< EventsUnion > union_;

private:
  static const EventDescriptionMap_t _event_descriptions;

private:
  inline void _assert_not_null_union() const
    throw (xcb_exception)
  {
    if (this->union_ == nullptr)
      throw xcb_exception();
  }

  inline void _throw_if_not_typed(EventType type) const
    throw(xcb_event_type_mismatch_exception)
  {
    if (this->type() != type)
      throw xcb_event_type_mismatch_exception();
  }

public:
  explicit Event( xcb_shared_ptr xcb_ )
    : xcb_( xcb_ )
  { }

  bool next();

  EventType type() const noexcept;

  EventDescription_cref_t description() const
    throw(xcb_exception);

  /**
   * @return the target window for this event; or 0 if n/a.
   */
  xcb_window_t window_xid() const
  {
    xcb_window_t window_xid = 0;

    switch( this->type() )
    {
      case EventType::EXPOSE:      window_xid = this->expose()->window; break;
      case EventType::KEY_PRESS:   window_xid = this->key_press()->event; break;
      case EventType::KEY_RELEASE: window_xid = this->key_release()->event; break;
      default:
        ;
    }

    return window_xid;
  }

  inline const xcb_expose_event_t *expose() const
  {
    this->_throw_if_not_typed( EventType::EXPOSE );
    return &this->union_->_expose;
  }

  inline const xcb_key_press_event_t *key_press() const
  {
    this->_throw_if_not_typed( EventType::KEY_PRESS );
    return &this->union_->_key_press;
  }

  inline const xcb_key_release_event_t *key_release() const
  {
    this->_throw_if_not_typed( EventType::KEY_RELEASE );
    return &this->union_->_key_release;
  }

};


} // xcb ns.
TL_NS_END

#endif // FABIC_XCB_EVENTS_H
