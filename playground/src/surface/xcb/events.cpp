
# include "fabic/surface/xcb/events.hpp"

namespace sf {
  namespace xcb {


    Event::EventDescription_cref_t
      Event::description()
          const throw( base_exception )
    {
      const auto it = this->_event_descriptions.find( this->type() );

      if (it == this->_event_descriptions.end())
        throw base_exception();

      return it->second;
    }


    /**
     * Initialization of static const data member `Event::_event_descriptions`
     * which is admittedly useless it in general except for some debug/logging
     * abilities at this early development stage...
     */
    const Event::EventDescriptionMap_t Event::_event_descriptions {
          { EventType::KEY_PRESS,         EventDescription(EventType::KEY_PRESS,      "KEY PRESS",      "xcb_key_press_event_t") }
        , { EventType::KEY_RELEASE,       EventDescription(EventType::KEY_RELEASE,    "KEY RELEASE",    "xcb_key_release_event_t") }
        , { EventType::BUTTON_PRESS,      EventDescription(EventType::BUTTON_PRESS,   "BUTTON_PRESS") }
        , { EventType::BUTTON_RELEASE,    EventDescription(EventType::BUTTON_RELEASE, "BUTTON_RELEASE") }
        , { EventType::MOTION_NOTIFY,     EventDescription(EventType::MOTION_NOTIFY,  "MOTION_NOTIFY") }
        , { EventType::ENTER_NOTIFY,      EventDescription(EventType::ENTER_NOTIFY,   "ENTER_NOTIFY") }
        , { EventType::LEAVE_NOTIFY,      EventDescription(EventType::LEAVE_NOTIFY,   "LEAVE_NOTIFY") }
        , { EventType::FOCUS_IN,          EventDescription(EventType::FOCUS_IN,       "FOCUS_IN") }
        , { EventType::FOCUS_OUT,         EventDescription(EventType::FOCUS_OUT,      "FOCUS_OUT") }
        , { EventType::KEYMAP_NOTIFY,     EventDescription(EventType::KEYMAP_NOTIFY,  "KEYMAP_NOTIFY") }
        , { EventType::EXPOSE,            EventDescription(EventType::EXPOSE,         "EXPOSE",         "xcb_expose_event_t") }
        , { EventType::GRAPHICS_EXPOSURE, EventDescription(EventType::GRAPHICS_EXPOSURE, "GRAPHICS_EXPOSURE") }
        , { EventType::NO_EXPOSURE,       EventDescription(EventType::NO_EXPOSURE,       "NO_EXPOSURE") }
        , { EventType::VISIBILITY_NOTIFY, EventDescription(EventType::VISIBILITY_NOTIFY, "VISIBILITY_NOTIFY") }
        , { EventType::CREATE_NOTIFY,     EventDescription(EventType::CREATE_NOTIFY,   "CREATE_NOTIFY") }
        , { EventType::DESTROY_NOTIFY,    EventDescription(EventType::DESTROY_NOTIFY,  "DESTROY_NOTIFY") }
        , { EventType::UNMAP_NOTIFY,      EventDescription(EventType::UNMAP_NOTIFY,    "UNMAP_NOTIFY") }
        , { EventType::MAP_NOTIFY,        EventDescription(EventType::MAP_NOTIFY,      "MAP_NOTIFY") }
        , { EventType::MAP_REQUEST,       EventDescription(EventType::MAP_REQUEST,     "MAP_REQUEST") }
        , { EventType::REPARENT_NOTIFY,   EventDescription(EventType::REPARENT_NOTIFY, "REPARENT_NOTIFY") }
        , { EventType::CONFIGURE_NOTIFY,  EventDescription(EventType::CONFIGURE_NOTIFY,  "CONFIGURE_NOTIFY") }
        , { EventType::CONFIGURE_REQUEST, EventDescription(EventType::CONFIGURE_REQUEST, "CONFIGURE_REQUEST") }
        , { EventType::GRAVITY_NOTIFY,    EventDescription(EventType::GRAVITY_NOTIFY,    "GRAVITY_NOTIFY") }
        , { EventType::RESIZE_REQUEST,    EventDescription(EventType::RESIZE_REQUEST,    "RESIZE_REQUEST") }
        , { EventType::CIRCULATE_NOTIFY,  EventDescription(EventType::CIRCULATE_NOTIFY,  "CIRCULATE_NOTIFY") }
        , { EventType::CIRCULATE_REQUEST, EventDescription(EventType::CIRCULATE_REQUEST, "CIRCULATE_REQUEST") }
        , { EventType::PROPERTY_NOTIFY,   EventDescription(EventType::PROPERTY_NOTIFY,   "PROPERTY_NOTIFY") }
        , { EventType::SELECTION_CLEAR,   EventDescription(EventType::SELECTION_CLEAR,   "SELECTION_CLEAR") }
        , { EventType::SELECTION_REQUEST, EventDescription(EventType::SELECTION_REQUEST, "SELECTION_REQUEST") }
        , { EventType::SELECTION_NOTIFY,  EventDescription(EventType::SELECTION_NOTIFY,  "SELECTION_NOTIFY") }
        , { EventType::COLORMAP_NOTIFY,   EventDescription(EventType::COLORMAP_NOTIFY,   "COLORMAP_NOTIFY") }
        , { EventType::CLIENT_MESSAGE,    EventDescription(EventType::CLIENT_MESSAGE,    "CLIENT_MESSAGE") }
        , { EventType::MAPPING_NOTIFY,    EventDescription(EventType::MAPPING_NOTIFY,    "MAPPING_NOTIFY") }
        , { EventType::GE_GENERIC,        EventDescription(EventType::GE_GENERIC,        "GE_GENERIC") }
    };

  } // xcb ns.
} // sf ns.
