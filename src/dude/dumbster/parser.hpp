#ifndef _DUDE_DUMBSTER_PARSER_H
#define _DUDE_DUMBSTER_PARSER_H

#include <list>

# include "dude/dumbster/fragments.hpp"
# include "dude/lexer/lexer.hpp"
# include "dude/filesystem/file.hpp"

namespace dude {
namespace dumbster {

  using dude::ex::yet_undefined_exception;
  using dude::fs::File;

  using dude::lexer::Lexer;
  using dude::lexer::Token;

  namespace xfs = boost::filesystem;

  /**
   */
  class Parser {
    protected:
      File _file;
      Lexer _alex;
      std::list<Token> _tokens;
      std::list<Token>::iterator current_token_it_;
      std::list<Token> _stack;
      Fragment *fragments_;
    public:
      explicit Parser(xfs::path fileName);

      /// Fetch one more token from the lexer.
      Token& next_token();

      /// Rewind back one token.
      Token& rewind_token();

      /// Returns the current token from the `_tokens` list (of lexed tokens).
      Token& token();

      /// TRUE if current token is a single-byte character matching `ch`.
      bool matchSymbol(char ch);

      /// Push onto the parsing `_stack` a (possibly multi-byte character `mch`
      /// that is expected by some [non-]terminal.
      void pushExpectedSymbol(const char *mch);

      template<typename... TokenCtorArgs>
        void pushExpectedToken(TokenCtorArgs&&... args)
        {
          _stack.emplace_front(std::forward<TokenCtorArgs>(args)...);
        }

      /// Pop one token off the top of the “expected tokens stack”.
      /// `tok` argument is compared against the token about to be poped off,
      /// (and an exception is thrown in case both do not match).
      Token popExpectedToken(const Token& tok);

      /// Tells whether `tok` matches a token that is expected by some
      /// non-terminal.
      bool isExpected(const Token& tok);

      void parse();
      void dev_parse();

      Fragment * nt_whatever(Fragment *previous, int depth);
      Fragment * nt_curly_block(Fragment *preceding, int depth);
      void debug_print_ast(std::ostream& os);
  };

} // dumbster ns.
} // dude ns.

#endif // _DUDE_DUMBSTER_PARSER_H
