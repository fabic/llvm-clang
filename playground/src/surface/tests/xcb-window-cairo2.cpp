# include "fabic/surface/xcb+.hpp"

/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  loginfo << argv[0]
          << "Displays an X window through XCB+,"
              " and setup an XCB Cairo Surface.";

  using sf::xcb::Xcb;
  using sf::xcb::Window;
  using sf::cairo::Surface;

  auto xcb_ = std::make_shared< Xcb >();

  auto win_ = std::make_shared< Window >( xcb_ );

  win_->create(320, 240, nullptr);
  win_->map();
  xcb_->run();

  return 0;
}
