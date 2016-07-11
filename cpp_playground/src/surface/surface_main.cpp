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

using namespace fabic;

/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  using std::cout;
  using std::endl;
  using xcb::Xcb;
  using xcb::Window;
  using sf::Surface;

  auto xcb_ = std::make_shared< Xcb >();

  // auto win1_ = xcb_->createWindowSimple();
  // win1_->map();

  // auto surf_ = std::make_shared< Surface >(
  //     xcb_
  //   );

  auto surf_ = (new Surface( xcb_ ))->shared_from_base< Surface >();
  // auto surf_ = std::shared_ptr< tk::Element >(new Surface( xcb_ ))->shared_from_this< Surface >();

  surf_->map();

  xcb_->flush();

  xcb_->run();

  return 0;
}
