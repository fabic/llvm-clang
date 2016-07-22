# include "fabic/surface/xcb+.hpp"
# include "fabic/surface/cairo/cairo.hpp"
# include "fabic/logging.hpp"

#include <random>

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

  win_->initCairoSurface();
  win_->surface().fill(sf::rgba<>(96, 128, 32, 128));

  //win_->surface().fill(sf::rgba<>(96, 128, 32, 128));

  win_->setHandleExportCallback(
      [](Window& win, uint16_t width, uint16_t height)
      {
        logtrace << "HEY!! " << width << 'x' << height;
        std::minstd_rand r;
        win.surface().fill(
            sf::rgba<unsigned long>(r(), r(), r(), r())
        );
      }
    );

  xcb_->run();

  return 0;
}
