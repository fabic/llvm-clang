#ifndef FABIC_XCB_IMPL_H
#define FABIC_XCB_IMPL_H

#include "xcb.hpp"

TL_NS_BEGIN
namespace xcb {


// // // // // // // // // // // // // // // // // // // // // // // //

/**
 * todo: merge all inlines in one file if at all possible?
 */
inline
bool Event::next()
{
  this->union_ =
    std::unique_ptr< EventsUnion >(
        (EventsUnion *)
          xcb_wait_for_event( this->xcb_->getXcbConnectionPtr() )
        // ^ dangerous blind C-style casted, beware !
      );

  return this->union_ != nullptr;
}


/**
 * Short-cut for the EventsUnion::type() method.
 */
inline
EventType Event::type() const noexcept
  {
    _assert_not_null_union();
    return this->union_->type();
  }

// // // // // // // // // // // // // // // // // // // // // // // //

inline
xcb_connection_t&
  Xcb::getConnection()
{
  if (this->connection_ == nullptr)
    throw base_exception();

  return *this->connection_;
}


inline
xcb_connection_t *
  Xcb::getXcbConnectionPtr()
{
  if (this->connection_ == nullptr)
    throw base_exception();

  return this->connection_;
}


inline
int Xcb::getScreenNumber() const
{
  return this->screenNumber;
}


inline
xid_t
  Xcb::generate_xid()
{
  return xcb_generate_id( this->connection_ );
}


inline
Xcb::ConnectionError
  Xcb::connection_has_error()
{
  int retv = xcb_connection_has_error(this->connection_);

  if (retv > static_cast<int>(ConnectionError::_LAST_ENUM_SENTINEL))
    throw unexpected_return_value_from_xcb();

  return static_cast<ConnectionError>(retv);
}


inline
Xcb::self
  Xcb::flush()
{
  int status = xcb_flush( this->connection_ );

  if (status <= 0)
    throw base_exception();

  return *this;
}


/// private btw.
inline
void Xcb::throw_if_connection_in_error() {
  if (this->connection_has_error() != ConnectionError::CONN_OK)
    throw xcb_connection_in_error_condition();
}


// static btw.
inline
void
  Xcb::assert_void_cookie(xcb_void_cookie_t cookie)
{
  assert( cookie.sequence > 0 );
}


} // xcb ns.
TL_NS_END

#endif // FABIC_XCB_IMPL_H
