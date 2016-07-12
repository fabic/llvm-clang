#ifndef FABIC_XCB_EVENTS_H
#define FABIC_XCB_EVENTS_H

#include <xcb/xproto.h>

namespace fabic {
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
  , MOTION_NOTIFY = XCB_MOTION_NOTIFY // 6
  , ENTER_NOTIFY = XCB_ENTER_NOTIFY // 7
  , LEAVE_NOTIFY = XCB_LEAVE_NOTIFY // 8
  , FOCUS_IN = XCB_FOCUS_IN // 9
  , FOCUS_OUT = XCB_FOCUS_OUT // 10
  , KEYMAP_NOTIFY = XCB_KEYMAP_NOTIFY // 11
  , EXPOSE = XCB_EXPOSE // 12
  , GRAPHICS_EXPOSURE = XCB_GRAPHICS_EXPOSURE // 13
  , NO_EXPOSURE = XCB_NO_EXPOSURE // 14
  , VISIBILITY_NOTIFY = XCB_VISIBILITY_NOTIFY // 15
  , CREATE_NOTIFY = XCB_CREATE_NOTIFY // 16
  , DESTROY_NOTIFY = XCB_DESTROY_NOTIFY // 17
  , UNMAP_NOTIFY = XCB_UNMAP_NOTIFY // 18
  , MAP_NOTIFY = XCB_MAP_NOTIFY // 19
  , MAP_REQUEST = XCB_MAP_REQUEST // 20
  , REPARENT_NOTIFY = XCB_REPARENT_NOTIFY // 21
  , CONFIGURE_NOTIFY = XCB_CONFIGURE_NOTIFY // 22
  , CONFIGURE_REQUEST = XCB_CONFIGURE_REQUEST // 23
  , GRAVITY_NOTIFY = XCB_GRAVITY_NOTIFY // 24
  , RESIZE_REQUEST = XCB_RESIZE_REQUEST // 25
  , CIRCULATE_NOTIFY = XCB_CIRCULATE_NOTIFY // 26
  , CIRCULATE_REQUEST = XCB_CIRCULATE_REQUEST // 27
  , PROPERTY_NOTIFY = XCB_PROPERTY_NOTIFY // 28
  , SELECTION_CLEAR = XCB_SELECTION_CLEAR // 29
  , SELECTION_REQUEST = XCB_SELECTION_REQUEST // 30
  , SELECTION_NOTIFY = XCB_SELECTION_NOTIFY // 31
  , COLORMAP_NOTIFY = XCB_COLORMAP_NOTIFY // 32
  , CLIENT_MESSAGE = XCB_CLIENT_MESSAGE // 33
  , MAPPING_NOTIFY = XCB_MAPPING_NOTIFY // 34
  , GE_GENERIC = XCB_GE_GENERIC // 35
};


/**
 * Now that's quite dangerous certainly ;
 * hopefully won't this blow in someone face at some point.
 */
union EventsUnion {
  xcb_generic_event_t generic;

  xcb_key_press_event_t     key_press;   // XCB_KEY_PRESS 2
    xcb_key_release_event_t key_release; // XCB_KEY_RELEASE 3

  EventType type() const
  {
    return (EventType) // C-style cast here too, not good.
      (this->generic.response_type & ~0x80) ;
  }
};


class Event {
private:
  xcb_shared_ptr xcb_;
  std::unique_ptr< EventsUnion > union_;

private:
  inline void _assert_not_null_union() const
    throw (xcb_exception)
  {
    if (this->union_ == nullptr)
      throw xcb_exception();
  }

public:
  explicit Event( xcb_shared_ptr xcb_ )
    : xcb_( xcb_ )
  { }

  bool next();

  EventType type() const noexcept;

};


} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_EVENTS_H