# include "surface/xcb+.hpp"
# include "util/logging.hpp"

/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  loginfo << argv[0]
          << "Basically displays an X window through the XCB+ implementation.";

  using sf::xcb::Xcb;
  using sf::xcb::Window;

  auto xcb_ = std::make_shared< Xcb >();

  auto win_ = std::make_shared< Window >( xcb_ );

  win_->create(320, 240, nullptr);

  win_->map();

  xcb_->run();

  return 0;
}
