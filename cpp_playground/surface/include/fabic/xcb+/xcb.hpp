
#include <cassert>
#include <exception>
#include <xcb/xcb.h>

#include "fabic/logging.hpp"

namespace fabic {

  class Xcb
  {
  public:
    typedef Xcb& self;

    /**
     * the XCB_* are #define-s from xcb.h
     * For typed-return value of connection_has_error().
     */
    enum class ConnectionError : int {
      CONN_OK = 0,

        /** xcb connection errors because of socket, pipe and other stream errors. */
        CONN_ERROR                   = XCB_CONN_ERROR,
        /** xcb connection shutdown because of extension not supported */
        CONN_CLOSED_EXT_NOTSUPPORTED = XCB_CONN_CLOSED_EXT_NOTSUPPORTED,
        /** malloc(), calloc() and realloc() error upon failure, for eg ENOMEM */
        CONN_CLOSED_MEM_INSUFFICIENT = XCB_CONN_CLOSED_MEM_INSUFFICIENT,
        /** Connection closed, exceeding request length that server accepts. */
        CONN_CLOSED_REQ_LEN_EXCEED   = XCB_CONN_CLOSED_REQ_LEN_EXCEED,
        /** Connection closed, error during parsing display string. */
        CONN_CLOSED_PARSE_ERR        = XCB_CONN_CLOSED_PARSE_ERR,
        /** Connection closed because the server does not have a screen matching the display. */
        CONN_CLOSED_INVALID_SCREEN   = XCB_CONN_CLOSED_INVALID_SCREEN,
        /** Connection closed because some FD passing operation failed */
        CONN_CLOSED_FDPASSING_FAILED = XCB_CONN_CLOSED_FDPASSING_FAILED,

      // (for bounds checking)
      _LAST_ENUM_SENTINEL
    };


    // Exceptions
    struct base_exception : std::exception {};
    struct unexpected_return_value_from_xcb : base_exception {};
    struct xcb_connection_in_error_condition : base_exception {};

  private:
    xcb_connection_t *connection_ = nullptr;
    int screenNumber = -1;

  // Prevent inadvertent copies.
  protected:
    Xcb(const Xcb&) = delete;
    Xcb& operator=(const Xcb &) = delete;

  public:

    Xcb() {
      this->connect();
    }

    /**
     * Desctructor, disconnects if needs be.
     */
    virtual ~Xcb()
    {
      if (this->connection_ != nullptr)
        this->disconnect();
    }

    xcb_connection_t& getConnection()
    {
      if (this->connection_ == nullptr)
        throw base_exception();

      return *this->connection_;
    }

    /**
     * xcb_connect(const char *displayname, int *screenp)
     *
     * @brief Connects to the X server.
     * @param displayname: The name of the display.
     * @param screenp: A pointer to a preferred screen number.
     * @return A newly allocated xcb_connection_t structure.
     *
     * Connects to the X server specified by @p displayname. If @p
     * displayname is @c NULL, uses the value of the DISPLAY environment
     * variable. If a particular screen on that server is preferred, the
     * int pointed to by @p screenp (if not @c NULL) will be set to that
     * screen; otherwise the screen will be set to 0.
     *
     * Always returns a non-NULL pointer to a xcb_connection_t, even on failure.
     * Callers need to use xcb_connection_has_error() to check for failure.
     * When finished, use xcb_disconnect() to close the connection and free
     * the structure.
     */
    self connect(const char *displayName = nullptr)
    {
      logtrace << "Xcb: Connecting to "
               << (displayName ? displayName : "default display.");

      if (this->connection_ != nullptr)
        throw base_exception();

      this->connection_ = xcb_connect(displayName, &this->screenNumber);

      this->throw_if_connection_in_error();

      logtrace << "Xcb: connected (probably).";

      return *this;
    }


    /**
     * xcb_disconnect();
     *
     * @brief Closes the connection.
     * @param c: The connection.
     *
     * Closes the file descriptor and frees all memory associated with the
     * connection @c c. If @p c is @c NULL, nothing is done.
     */
    void disconnect()
    {
      logtrace << "Xcb: Disconnecting from server...";

      if (this->connection_ == nullptr)
        throw base_exception();

      xcb_disconnect(this->connection_);

      logtrace << "Xcb: Disconnected.";

      this->connection_ = nullptr;
    }


    /**
     * xcb_connection_has_error(xcb_connection_t *c);
     *
     * @brief Test whether the connection has shut down due to a fatal error.
     * @param c: The connection.
     * @return > 0 if the connection is in an error state; 0 otherwise.
     *
     * Some errors that occur in the context of an xcb_connection_t
     * are unrecoverable. When such an error occurs, the
     * connection is shut down and further operations on the
     * xcb_connection_t have no effect, but memory will not be freed until
     * xcb_disconnect() is called on the xcb_connection_t.
     *
     * @return XCB_CONN_ERROR, because of socket errors, pipe errors or other stream errors.
     * @return XCB_CONN_CLOSED_EXT_NOTSUPPORTED, when extension not supported.
     * @return XCB_CONN_CLOSED_MEM_INSUFFICIENT, when memory not available.
     * @return XCB_CONN_CLOSED_REQ_LEN_EXCEED, exceeding request length that server accepts.
     * @return XCB_CONN_CLOSED_PARSE_ERR, error during parsing display string.
     * @return XCB_CONN_CLOSED_INVALID_SCREEN, because the server does not have a screen matching the display.
     */
    ConnectionError connection_has_error()
    {
      int retv = xcb_connection_has_error(this->connection_);

      if (retv > static_cast<int>(ConnectionError::_LAST_ENUM_SENTINEL))
        throw unexpected_return_value_from_xcb();

      return static_cast<ConnectionError>(retv);
    }

  protected:
    void throw_if_connection_in_error() {
      if (this->connection_has_error() != ConnectionError::CONN_OK)
        throw xcb_connection_in_error_condition();
    }

  public:
    /**
     *
     */
    const xcb_screen_t *
      getScreenInfo(int screenNbr = -1)
    {
      screenNbr = screenNbr > -1 ?
        screenNbr : this->screenNumber;

      xcb_screen_iterator_t iter =
        xcb_setup_roots_iterator(
            xcb_get_setup( this->connection_ )
          );

      while( iter.rem > 0 && screenNbr > 0) {
        xcb_screen_next( &iter );
        screenNbr--;
      }

      xcb_screen_t * screen = iter.data;

      return screen;
    }


    /**
     * @brief Creates a window
     *
     * @param c The connection
     * @param depth Specifies the new window's depth (TODO: what unit?).
     *
     * The special value `XCB_COPY_FROM_PARENT` means the depth is taken from the
     * \a parent window.
     *
     * @param wid The ID with which you will refer to the new window, created by
     * `xcb_generate_id`.
     *
     * @param parent The parent window of the new window.
     * @param x The X coordinate of the new window.
     * @param y The Y coordinate of the new window.
     * @param width The width of the new window.
     * @param height The height of the new window.
     * @param border_width TODO:
     * \n
     * Must be zero if the `class` is `InputOnly` or a `xcb_match_error_t` occurs.
     * @param _class A bitmask of #xcb_window_class_t values.
     * @param _class \n
     * @param visual Specifies the id for the new window's visual.
     * \n
     * The special value `XCB_COPY_FROM_PARENT` means the visual is taken from the
     * \a parent window.
     * @param value_mask A bitmask of #xcb_cw_t values.
     * @return A cookie
     *
     * Creates an unmapped window as child of the specified \a parent window. A
     * CreateNotify event will be generated. The new window is placed on top in the
     * stacking order with respect to siblings.
     *
     * The coordinate system has the X axis horizontal and the Y axis vertical with
     * the origin [0, 0] at the upper-left corner. Coordinates are integral, in terms
     * of pixels, and coincide with pixel centers. Each window and pixmap has its own
     * coordinate system. For a window, the origin is inside the border at the inside,
     * upper-left corner.
     *
     * The created window is not yet displayed (mapped), call `xcb_map_window` to
     * display it.
     *
     * The created window will initially use the same cursor as its parent.
     */
    xcb_void_cookie_t
    create_window (uint8_t           depth = XCB_COPY_FROM_PARENT,
                       xcb_window_t      wid,
                       xcb_window_t      parent,
                       int16_t           x,
                       int16_t           y,
                       uint16_t          width,
                       uint16_t          height,
                       uint16_t          border_width,
                       uint16_t          _class,
                       xcb_visualid_t    visual,
                       uint32_t          value_mask,
                       const uint32_t   *value_list)
    {
    }
      };
    }

} // fabic ns.
