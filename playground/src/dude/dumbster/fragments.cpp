#include "dude/dumbster/fragments.hpp"
#include "dude/util/logging.hpp"

namespace dude {
namespace dumbster {

  Fragment::Fragment(Kind k)
    : _kind(k)
  {  }


  void
    Fragment::push_token(Token *tok)
    {
      _tokens.push_back(tok);
    }


  void
    Fragment::pop_token()
    {
      _tokens.pop_back();
    }


  void
    Fragment::set_previous_fragment(Fragment *prev)
    {
      if (previous_ != nullptr)
        logwarn << "Fragment's previous ptr is not null o_o";
      previous_ = prev;
    }


  void
    Fragment::set_next_fragment(Fragment *nex)
    {
      if (next_ != nullptr)
        logwarn << "Fragment's next ptr is not null o_o";
      next_ = nex;
    }


} // dumbster ns.
} // dude ns.
