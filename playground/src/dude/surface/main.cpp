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

  logtrace << "main(): Current working directory : "
           << boost::filesystem::current_path();

  auto xcb_ = std::make_shared< Xcb >();

  auto buffers_ = std::make_shared< sf::TextBuffers >();

  buffers_->openFile("experiments/clang/fixtures/bare.cpp");

  // auto surf_ = std::make_shared< SurfaceWin >( xcb_ );
  // ^ DAMN: inheritable_shared_from_this<> hack :-/
  auto surf_ = SurfaceWin::_construct< SurfaceWin >(xcb_, buffers_);

  surf_->map();

  xcb_->run();

  logtrace << "main(): finished, good bye..." ;

  return 0;
}
