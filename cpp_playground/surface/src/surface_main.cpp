/** surface/src/main.cpp
 *
 * FabiC.2016-07-07
 *
 * http://rosettacode.org/wiki/Window_creation/X11#XCB
 *
 * https://www.x.org/wiki/guide/client-ecosystem/
 * https://www.x.org/wiki/guide/xlib-and-xcb/
 * https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html
 *
 * https://www.x.org/releases/X11R7.7/doc/index.html
 *
 * https://cairographics.org/manual/index.html
 * https://cairographics.org/manual/cairo-XCB-Surfaces.html
 *https://www.cairographics.org/examples/
 *
 * https://xcb.freedesktop.org/PublicApi/
 *
 * https://developer.gnome.org/pango/unstable/
 */

#include <iostream>

# include "fabic/surface/surface.hpp"

/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  using std::cout;
  using std::endl;
  using fabic::xcb::Xcb;
  using fabic::xcb::Window;
  using fabic::sf::Surface;

  auto xcb_ = std::make_shared< Xcb >();

  auto win1_ = xcb_->createWindowSimple();

  win1_->map();

  auto surf_ = std::make_shared< Surface >(
      xcb_
    );

  surf_->map();

  xcb_->flush();

  xcb_->run();

  return 0;
}
