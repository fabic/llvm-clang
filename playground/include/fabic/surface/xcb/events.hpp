#ifndef SURFACE_XCB_EVENTS_H
#define SURFACE_XCB_EVENTS_H

#include <map>
#include "fabic/surface/xcb/typedefs.hpp"
#include "fabic/surface/xcb/exceptions.hpp"

namespace sf {
  namespace xcb {

    /** Convenient enumeration for those scattered #define-s `XCB_...`
     * that relate to events.
     *
     * Related field being `response_type` of `xcb_generic_event_t` :
     *
     * <pre>
     *     typedef struct {
     *         uint8_t   response_type;
     *         ...
     *     } xcb_generic_event_t;
     * </pre>
     */
    enum class EventType : uint8_t {
        KEY_PRESS         = XCB_KEY_PRESS         //  2
      , KEY_RELEASE       = XCB_KEY_RELEASE       //  3
      , BUTTON_PRESS      = XCB_BUTTON_PRESS      //  4
      , BUTTON_RELEASE    = XCB_BUTTON_RELEASE    //  5
      , MOTION_NOTIFY     = XCB_MOTION_NOTIFY     //  6
      , ENTER_NOTIFY      = XCB_ENTER_NOTIFY      //  7
      , LEAVE_NOTIFY      = XCB_LEAVE_NOTIFY      //  8
      , FOCUS_IN          = XCB_FOCUS_IN          //  9
      , FOCUS_OUT         = XCB_FOCUS_OUT         // 10
      , KEYMAP_NOTIFY     = XCB_KEYMAP_NOTIFY     // 11
      , EXPOSE            = XCB_EXPOSE            // 12
      , GRAPHICS_EXPOSURE = XCB_GRAPHICS_EXPOSURE // 13
      , NO_EXPOSURE       = XCB_NO_EXPOSURE       // 14
      , VISIBILITY_NOTIFY = XCB_VISIBILITY_NOTIFY // 15
      , CREATE_NOTIFY     = XCB_CREATE_NOTIFY     // 16
      , DESTROY_NOTIFY    = XCB_DESTROY_NOTIFY    // 17
      , UNMAP_NOTIFY      = XCB_UNMAP_NOTIFY      // 18
      , MAP_NOTIFY        = XCB_MAP_NOTIFY        // 19
      , MAP_REQUEST       = XCB_MAP_REQUEST       // 20
      , REPARENT_NOTIFY   = XCB_REPARENT_NOTIFY   // 21
      , CONFIGURE_NOTIFY  = XCB_CONFIGURE_NOTIFY  // 22
      , CONFIGURE_REQUEST = XCB_CONFIGURE_REQUEST // 23
      , GRAVITY_NOTIFY    = XCB_GRAVITY_NOTIFY    // 24
      , RESIZE_REQUEST    = XCB_RESIZE_REQUEST    // 25
      , CIRCULATE_NOTIFY  = XCB_CIRCULATE_NOTIFY  // 26
      , CIRCULATE_REQUEST = XCB_CIRCULATE_REQUEST // 27
      , PROPERTY_NOTIFY   = XCB_PROPERTY_NOTIFY   // 28
      , SELECTION_CLEAR   = XCB_SELECTION_CLEAR   // 29
      , SELECTION_REQUEST = XCB_SELECTION_REQUEST // 30
      , SELECTION_NOTIFY  = XCB_SELECTION_NOTIFY  // 31
      , COLORMAP_NOTIFY   = XCB_COLORMAP_NOTIFY   // 32
      , CLIENT_MESSAGE    = XCB_CLIENT_MESSAGE    // 33
      , MAPPING_NOTIFY    = XCB_MAPPING_NOTIFY    // 34
      , GE_GENERIC        = XCB_GE_GENERIC        // 35
      , _FIRST_SENTINEL   = XCB_KEY_PRESS  ///< first known event type (lowest value for bounds checking = 2).
      , _LAST_SENTINEL    = XCB_GE_GENERIC ///< last known event type (highest value for bounds checking = 35).
    };


    /**
     * Union struct. which shields us away from the C-style logic
     * of "simulated data inheritance" by means of pointers to structures
     * that have to be casted based on some criteria like typically a type
     * property.
     *
     * For XCB the actual event struct. is selected through the
     * `xcb_generic_event_t.response_type` struct. field.
     *
     * Now that's quite dangerous certainly, but not more than if we played
     * it C-style ; anyway we have opportunities here of shielding client codes
     * by means of assertions &/ exceptions if needs be.
     */
    union EventsUnion {
      xcb_generic_event_t  _generic;

      xcb_key_press_event_t     _key_press;   // XCB_KEY_PRESS 2
      xcb_key_release_event_t _key_release; // XCB_KEY_RELEASE 3

      xcb_expose_event_t  _expose;            // XCB_EXPOSE 12

      // todo: add here some further struct. from XCB as needed.

      /// Helper for getting the XCB event type.
      inline EventType type() const
      {
        return (EventType) // C-style cast here too, not good.
            (this->_generic.response_type & ~0x80) ;
        // TODO: Find out what is this last ^ bit about, actually ?
      }
    };


    /** Generally irrelevant class that describe a given XCB event ;
     * essentially for debugging purposes. */
    class EventDescription {
    private:
      EventType    _type;
      const char * _name;
      const char * _struct_name;

    public:
      /**
       * Ctor, see `Event::_event_descriptions` static const map<>
       * and its declaration at `events.cpp`
       *
       * \param type
       * \param name       event name / description.
       * \param xcb_struct name of the corresponding "concrete" XCB struct.
       */
      EventDescription(
          EventType    type,
          const char * name,
          const char * xcb_struct = nullptr)
        : _type(type),
          _name(name),
          _struct_name(xcb_struct)
      {
        assert( name != nullptr );
      }

      EventType    type()        const { return this->_type; }
      const char * name()        const { return this->_name; }
      const char * struct_name() const { return this->_struct_name; }
    };


    /**
     * Basic wrapper around getting XCB events, one at a time.
     *
     * Events are handed to us by XCB as pointers to heap-allocated
     * `xcb_generic_event` structures that we're responsible for freeing :
     *
     * * We're C-style casting it as `EventsUnion` ;
     * * and wrap it in a `unique_ptr<>` for automatic memory management.
     */
    class Event {
    public:
      typedef std::map<EventType, EventDescription> EventDescriptionMap_t;
      typedef const EventDescription& EventDescription_cref_t;

    private:
      xcb_shared_ptr xcb_;
      std::unique_ptr< EventsUnion > union_;

    private:
      /// A mapping of XCB event type to event description, essentially
      /// for debugging purposes, see `events.cpp` for declaration.
      static const EventDescriptionMap_t _event_descriptions;

    private:
      /// Assert guard that ensures the `union_` property ain't nullptr.
      void _assert_not_null_union() const
        throw (base_exception)
      {
        if (this->union_ == nullptr)
          throw base_exception();
      }

      /// Helper for asserting the current exception is of the given `type`.
      void _throw_if_not_typed(EventType type) const
        throw( event_type_mismatch_ex )
      {
        if (this->type() != type)
          throw event_type_mismatch_ex();
      }

    public:
      /// Ctor
      explicit Event( xcb_shared_ptr xcb_ ) : xcb_(xcb_ )
      { }

      /// Get  next XCB event.
      bool next();

      /**
       * Short-cut for the EventsUnion::type() method.
       * \return the type of the current event.
       */
      EventType type() const noexcept {
        _assert_not_null_union();
        return this->union_->type();
      }

      /// @return a description for the current event.
      EventDescription_cref_t description() const throw( base_exception );

      /**
       * @return the target window X ID for this event; or 0 if n/a.
       */
      xcb_window_t window_xid() const
      {
        xcb_window_t window_xid = 0;

        switch( this->type() ) {
        case EventType::EXPOSE:      window_xid = this->expose()->window; break;
        case EventType::KEY_PRESS:   window_xid = this->key_press()->event; break;
        case EventType::KEY_RELEASE: window_xid = this->key_release()->event; break;
        default:
          ;
        }

        return window_xid;
      }

      inline const xcb_expose_event_t * expose() const
      {
        this->_throw_if_not_typed( EventType::EXPOSE );
        return &this->union_->_expose;
      }

      inline const xcb_key_press_event_t * key_press() const
      {
        this->_throw_if_not_typed( EventType::KEY_PRESS );
        return &this->union_->_key_press;
      }

      inline const xcb_key_release_event_t * key_release() const
      {
        this->_throw_if_not_typed( EventType::KEY_RELEASE );
        return &this->union_->_key_release;
      }
    };

  } // xcb ns.
} // sf ns.

#endif // SURFACE_XCB_EVENTS_H
