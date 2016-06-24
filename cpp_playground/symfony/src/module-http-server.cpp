
# include "fabic/module/http/module-http-server.hpp"

namespace di = fabic::di;

void
hey_di_register_services(di::container_shared_ptr_t container)
{
  logtrace << "HEY! that's module-http-server ;- Hura!" ;
}


// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_dll/tutorial.html#boost_dll.tutorial.searching_for_a_symbol_in_multiple_plugins
BOOST_DLL_ALIAS(
    hey_di_register_services,
    di_register_services
)
