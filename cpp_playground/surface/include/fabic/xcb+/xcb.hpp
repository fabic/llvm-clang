#ifndef FABIC_XCB_H
#define FABIC_XCB_H

#include "fabic/xcb+/typedefs.hpp"

namespace fabic {
namespace xcb {

  using std::make_shared;
  using std::shared_ptr;
  using std::make_pair;
  using std::pair;



  /**
   * Entry point, manages an XCB connection, keeps track of Window-s, etc...
   *
   * Note: the `xcb_void_cookie_t` obscur type pervasively used as XCB's
   *       functions return value is defined as :
   *
   *       typedef struct {
   *           unsigned int sequence;  // < Sequence number
   *       } xcb_void_cookie_t;
   *
   * @link https://www.x.org/releases/X11R7.7/doc/index.html
   * @link https://www.x.org/wiki/guide/client-ecosystem/
   * @link https://www.x.org/wiki/guide/xlib-and-xcb/
   * @link https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html
   * @link http://rosettacode.org/wiki/Window_creation/X11#XCB
   */
  class Xcb
    : public std::enable_shared_from_this< Xcb >
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

    window_shared_ptr rootWindow = nullptr;

    windows_map_t windows;

  // Prevent inadvertent copies.
  protected:
    Xcb(const Xcb&) = delete;
    Xcb& operator=(const Xcb &) = delete;

  public:

    /**
     * Constructor, automatically connects (to the default display/screen).
     */
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

    /**
     * @return a reference (not a pointer).
     *
     * @throws
     */
    xcb_connection_t& getConnection()
    {
      if (this->connection_ == nullptr)
        throw base_exception();

      return *this->connection_;
    }

    /**
     * @throws
     */
    xcb_connection_t *
      getXcbConnectionPtr()
    {
      if (this->connection_ == nullptr)
        throw base_exception();

      return this->connection_;
    }

    /**
     * @brief Connects to the X server.
     *        `xcb_connect(const char *displayname, int *screenp)`
     *
     * @param displayname: The name of the display.
     * @param screenp:     A pointer to a preferred screen number.
     *
     * @return A newly allocated `xcb_connection_t` structure.
     *
     * @throws
     *
     * Connects to the X server specified by @p displayname. If @p
     * displayname is @c NULL, uses the value of the DISPLAY environment
     * variable. If a particular screen on that server is preferred, the
     * int pointed to by @p screenp (if not @c NULL) will be set to that
     * screen; otherwise the screen will be set to 0.
     *
     * Always returns a non-NULL pointer to a xcb_connection_t, even on failure.
     *
     * Callers need to use `xcb_connection_has_error()` to check for failure.
     *
     * When finished, use `xcb_disconnect()` to close the connection and free
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
     * @brief Closes the connection.
     *        `xcb_disconnect()`
     *
     * @param c: The connection.
     *
     * Closes the file descriptor and frees all memory associated with the
     * connection @c c. If @p c is @c NULL, nothing is done.
     */
    self disconnect()
    {
      logtrace << "Xcb: Disconnecting from server...";

      if (this->connection_ == nullptr)
        throw base_exception();

      xcb_disconnect(this->connection_);

      logtrace << "Xcb: Disconnected.";

      this->connection_ = nullptr;

      return *this;
    }

    /**
     * @brief Allocates an XID for a new object.
     *        `uint32_t xcb_generate_id(xcb_connection_t *c)`
     *
     * @param c: The connection.
     *
     * @return A newly allocated XID.
     *
     * Allocates an XID for a new object. Typically used just prior to
     * various object creation functions, such as `xcb_create_window()`.
     */
    xid_t generate_xid()
    {
      return xcb_generate_id( this->connection_ );
    }

    /**
     * @brief Test whether the connection has shut down due to a fatal error.
     *        `xcb_connection_has_error(xcb_connection_t *c)`
     *
     * @param c: The connection.
     *
     * @return > 0 if the connection is in an error state; 0 otherwise.
     *
     * @throws
     *
     * Some errors that occur in the context of an `xcb_connection_t` are
     * unrecoverable. When such an error occurs, the connection is shut down
     * and further operations on the `xcb_connection_t` have no effect, but
     * memory will not be freed until `xcb_disconnect()` is called on the
     * `xcb_connection_t`.
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

    /**
     * @brief Forces any buffered output to be written to the server.
     *        `int xcb_flush(xcb_connection_t *c);`
     * @param c: The connection to the X server.
     * @return > @c 0 on success, <= @c 0 otherwise.
     *
     * Forces any buffered output to be written to the server. Blocks
     * until the write is complete.
     */
    int flush()
    {
      int status = xcb_flush( this->connection_ );

      if (status <= 0)
        throw base_exception();

      return status;
    }

  protected:
    /// Helper that throws an exception if something has gone wrong
    /// with the connection.
    void throw_if_connection_in_error() {
      if (this->connection_has_error() != ConnectionError::CONN_OK)
        throw xcb_connection_in_error_condition();
    }

  public:
    /**
     * @param screenNbr The X screen number; default value -1 means the screen
     *                  we got upon connecting (see `connect()`, `this->screenNumber`).
     */
    screen_ref_t&
      getScreenInfo(int screenNbr = -1)
    {
      screenNbr = screenNbr > -1 ?
        screenNbr : this->screenNumber;

      xcb_screen_iterator_t iter =
        xcb_setup_roots_iterator(
            xcb_get_setup( this->connection_ )
          );
      // todo: ^ see if we may have error conditions with these 2 xcb_... functions.

      while( iter.rem > 0 && screenNbr > 0) {
        xcb_screen_next( &iter );
        screenNbr--;
      }

      xcb_screen_t * screen = iter.data;

      assert( screen != nullptr );

      return *screen;
    }

    /**
     *
     */
    window_shared_ptr
      getRootWindow()
    {
      if (this->rootWindow != nullptr)
        return this->rootWindow;

      xcb_screen_t& screen = this->getScreenInfo();
      xcb_window_t rootXid = screen.root;

      pair<windows_map_t::iterator, bool> retv =
        this->windows.emplace(
            make_pair(rootXid, make_shared< Window >(this->shared_from_this(), rootXid))
          );

      bool failed = retv.second == false;

      if (failed) {
        throw base_exception();
      }

      auto inserted_it = retv.first;

      window_shared_ptr win = inserted_it->second;

      this->rootWindow = win;

      return this->rootWindow;
    }

    /**
     * FIXME: can't tell if it's ok to check for the bundled `sequence`
     *        member of that void/obscur type, most probably not.
     *
     * But this static has the side-effect that we do check for XCB's functions
     * return values (at least does it prevents compiler warnings for unused
     * variables where we do retrieve the cookie "for code readability".)
     *
     * We'll supposed the cookie thing contains a protocol message sequence
     * number or something like that.
     */
    static
    void
      assert_void_cookie(xcb_void_cookie_t cookie)
    {
      assert( cookie.sequence > 0 );
    }


    /**
     * Enter the event loop.
     */
    virtual void
      run()
    {
      logdebug << "Xcb::run() : begin.";

      this->flush();

      xcb_generic_event_t * event;

      while (nullptr != (event = xcb_wait_for_event( this->connection_ )))
      {
        // todo: What is this masking agaisnt ~0x80 about ?
        auto e = event->response_type & ~0x80; // uint8_t btw.

        switch (e) {
          case XCB_EXPOSE: {
            logtrace << "Xcb::run(): EXPOSE !";
            xcb_expose_event_t *ev = (xcb_expose_event_t *) event;
            break;
          }

          case XCB_BUTTON_PRESS: {
            /* Handle the ButtonPress event type */
            xcb_button_press_event_t *ev = (xcb_button_press_event_t *) event;

            /* ... */

            break;
          }

          case XCB_KEY_PRESS: {
            logtrace << "Xcb::run(): KEY PRESSED !";
            break;
          }

          // Unknown event type, ignore it.
          default: {
            logtrace << "Xcb::run(): Event " << e << " is unknown to us.";
            break;
          }
        }

        free( event );
      } // end of iteration waiting for events.

      logdebug << "Xcb::run() : end.";
    }
  };

} // xcb ns.
} // fabic ns.

#endif // FABIC_XCB_H
