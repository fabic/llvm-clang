#ifndef SURFACE_XCB_H
#define SURFACE_XCB_H

#include "surface/xcb/typedefs.hpp"
#include "surface/xcb/exceptions.hpp"
#include "surface/xcb/events.hpp"
#include "surface/xcb/mask_values.hpp"
#include "surface/xcb/window.hpp"

namespace sf {
  namespace xcb {

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
     * * <https://www.x.org/releases/X11R7.7/doc/index.html>
     * * <https://www.x.org/releases/X11R7.7/doc/man/man3/index.xhtml>
     * * <https://www.x.org/wiki/guide/client-ecosystem/>
     * * <https://www.x.org/wiki/guide/xlib-and-xcb/>
     * * <https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html>
     * * <http://rosettacode.org/wiki/Window_creation/X11#XCB>
     * * <https://xcb.freedesktop.org/manual/modules.html>
     * * [`x11-helper.c` of the “Rofi window switcher”](https://github.com/DaveDavenport/rofi/blob/master/source/x11-helper.c)
     */
    class Xcb
        : public std::enable_shared_from_this< Xcb >
    {
    public:
      typedef Xcb* self;
      typedef std::map<xcb_window_t, window_shared_ptr> windows_map_t;

      /**
       * the XCB_* are #define-s from xcb.h
       * For typed-return value of `connection_has_error()`.
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
      Xcb();

      /**
       * Desctructor, disconnects if needs be.
       */
      virtual ~Xcb();

      /**
       * @return a reference (not a pointer).
       *
       * @throws
       */
      xcb_connection_t& getConnection();

      /**
       * @throws
       */
      xcb_connection_t * getXcbConnectionPtr();

      int getScreenNumber() const;

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
      self connect(const char *displayName = nullptr);


      /**
       * @brief Closes the connection.
       *        `xcb_disconnect()`
       *
       * @param c: The connection.
       *
       * Closes the file descriptor and frees all memory associated with the
       * connection @c c. If @p c is @c NULL, nothing is done.
       */
      self disconnect();

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
      xid_t generate_xid();

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
      ConnectionError connection_has_error();

    protected:
      /// Helper that throws an exception if something has gone wrong
      /// with the connection.
      void throw_if_connection_in_error();

    public:
      /**
       * @brief Forces any buffered output to be written to the server.
       *        `int xcb_flush(xcb_connection_t *c);`
       * @param c: The connection to the X server.
       * @return > @c 0 on success, <= @c 0 otherwise.
       *
       * Forces any buffered output to be written to the server. Blocks
       * until the write is complete.
       */
      self flush();

      /**
       * @param screenNbr The X screen number; default value -1 means the screen
       *                  we got upon connecting (see `connect()`, `this->screenNumber`).
       */
      screen_ref_t& getScreenInfo(int screenNbr = -1);

      /**
       *
       */
      window_shared_ptr getRootWindow();

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
      static void assert_void_cookie(xcb_void_cookie_t cookie);

      /**
       * Enter the event loop.
       *
       * \return this
       */
      virtual Xcb * run();

      /**
       * Add window `win_` to our map of windows we know about.
       *
       * @return this
       */
      self registerWindow(window_shared_ptr win_);

      window_shared_ptr lookupWindow(xcb_window_t window_xid);

      /**
       * Create a default-configured window.
       */
      window_shared_ptr
      createWindowSimple(
          window_shared_ptr parentWindow = nullptr,
          uint16_t          width  = 320,
          uint16_t          height = 200
      );

    protected:
      void _handleEvent(const Event& event);
      void _dispatchEvent(
          const Event& event,
          xcb_window_t target_window_xid );
    };

  } // xcb ns.
} // sf ns.

#endif // SURFACE_XCB_H
