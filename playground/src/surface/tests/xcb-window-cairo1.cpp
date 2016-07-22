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

//  win_->initCairoSurface();
//  win_->surface().fill(sf::rgba<>(96, 128, 32, 128));

  //win_->surface().fill(sf::rgba<>(96, 128, 32, 128));

  win_->setHandleExportCallback(
      [](Window& win, uint16_t width, uint16_t height)
      {
        logtrace << "HEY!! " << width << 'x' << height
                 << " / Cairo surface status : " << win.surface().status();
        std::random_device r;
//        win.surface().fill(
//            sf::rgba<unsigned long>(r(), r(), r(), r())
//        );
        auto sf = win.surface();

        auto dim = sf.dimensions();
        const auto a = r();
        const auto b = r();
        long long x = (a * dim.width())  / r.max();
        long long y = (b * dim.height()) / r.max();

        sf.move_to(sf::Vector<>(x, y));
        sf.rel_line_to(sf::Vector<>(64, 92));
        sf.set_line_width(10);
        sf.source_rgba(sf::rgba<double>(1.0, .9, .7, .75));
        sf.stroke();
      }
    );

  xcb_->run();

  return 0;
}
