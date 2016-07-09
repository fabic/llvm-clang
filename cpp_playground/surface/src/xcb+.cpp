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

# include "fabic/xcb+/xcb+.hpp"

/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  using std::cout;
  using std::endl;
  using fabic::xcb::Xcb;
  using fabic::xcb::Window;

  auto xcb_ = std::make_shared< Xcb >();

  //auto win1 = Window::createSimpleWindow(xcb_);

  auto win1 = xcb_->createWindowSimple();

  win1->map();

  xcb_->flush();

  {
    cout << "sizeof..." << endl
         << sizeof(int) << endl
         << sizeof(short int) << endl
         << sizeof(short) << endl
         ;

    using fabic::util::popcount;

    cout << "popcount..." << endl
         << popcount<int>(128 | 32 | 8 | 2) << endl
         << popcount(128 | 32 | 16 | 8 | 2 | 1) << endl
         ;

    using fabic::util::bit_index;

    cout << "bit_index..." << endl
         << bit_index<uint32_t>(8, 8|4|2|1) << endl
         << bit_index<uint32_t>(8, 8|4|2) << endl
         << bit_index<uint32_t>(8, 8|4) << endl
         << bit_index<uint32_t>(8, 8) << endl
         << bit_index<uint32_t>(8, 32|16|8|4|2|1) << endl
         << bit_index<uint32_t>(8, 16|8|4|2|1) << endl
         ;
  }

  xcb_->run();

  return 0;
}
