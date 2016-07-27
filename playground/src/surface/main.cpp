/**
 *
 * FabiC.2016-07-07
 *
 */

#include <iostream>

#include "surface/ui/surface.hpp"
#  include "surface/xcb/inline-decls.hpp"


/**
 * MAIN !
 */
int main(int argc, const char *argv[])
{
  using std::cout;
  using std::endl;

  using sf::xcb::Xcb;
  using sf::xcb::Window;
  using SurfaceWin = sf::ui::Surface;

  auto xcb_ = std::make_shared< Xcb >();

//  auto surf_ = std::make_shared< SurfaceWin >( xcb_ );
  auto surf_ = SurfaceWin::_construct< SurfaceWin >( xcb_ );

  surf_->map();

  xcb_->run();

  return 0;
}
