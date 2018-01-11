#ifndef _DUDE_DUMBSTER_FRAGMENTS_H
#define _DUDE_DUMBSTER_FRAGMENTS_H

#include <memory>
#include <list>
# include "dude/lexer/token.hpp"

namespace dude {
namespace dumbster {

  using dude::lexer::Token;

  /**
   */
  class Fragment {
    public:
      enum Kind : short {
        NIL = 0, statement, block, parenthesized, bracketized,
        block_antecedent
      };
      using tokens_list_ref_t = std::list<Token *> & ;
    protected:
      Fragment *previous_ = nullptr;
      Fragment *next_     = nullptr;
      Fragment *inner_    = nullptr;
      std::list<Token *> _tokens;
      Kind _kind;
    public:
      explicit Fragment(Kind k = Kind::NIL);

      void push_token(Token *tok);
      void pop_token();

      inline void set_kind(Kind k) { _kind = k; }
      inline Kind kind() const { return _kind; }
      inline Fragment * next() { return next_; }

      void set_previous_fragment(Fragment *frag);
      void set_next_fragment(Fragment *frag);

      tokens_list_ref_t tokens() { return _tokens; }
  };


  /**
   */
  class Block : public Fragment {
    protected:
      //std::shared_ptr<Fragment> inner_;
  };

} // dumbster ns.
} // dude ns.

#endif // _DUDE_DUMBSTER_FRAGMENTS_H
