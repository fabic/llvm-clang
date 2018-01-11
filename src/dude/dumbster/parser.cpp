#include "dude/dumbster/parser.hpp"
#include "dude/util/logging.hpp"

namespace dude {
namespace dumbster {

  Parser::Parser(xfs::path fileName)
    : _file(fileName),
      _alex(_file),
      current_token_it_(_tokens.begin()),
      fragments_(nullptr)
  { }


  Token&
    Parser::next_token()
    {
      current_token_it_++;

      if (current_token_it_ == _tokens.end()) {
        Token tok = _alex.next_token();
        _tokens.push_back(std::move(tok));
        current_token_it_--;
      }

      return *current_token_it_ ;
    }


  Token&
    Parser::rewind_token()
    {
      --current_token_it_ ;

      if (current_token_it_ == _tokens.end()) {
        throw dude::ex::yet_undefined_exception(
            "Parser::rewind_token(): can't rewind past the beginning lad -_-");
      }

      return *current_token_it_ ;
    }


  Token&
    Parser::token()
    {
      if (current_token_it_ == _tokens.end()) {
        throw dude::ex::yet_undefined_exception(
            "Parser::token(): Huh! We've got _no_ current token !");
      }

      return *current_token_it_;
    }


  bool
    Parser::matchSymbol(char ch)
    {
      return token().is_symbol(ch);
    }


  void
    Parser::pushExpectedSymbol(const char * mch)
    {
      //_stack.emplace_front(Token::Kind::symbol, mch);
      pushExpectedToken(Token::Kind::symbol, mch);
    }


  Token
    Parser::popExpectedToken(const Token& tok)
    {
      Token kot = _stack.front();

      _stack.pop_front();

      if (kot != tok) {
        throw dude::ex::yet_undefined_exception(
            "Parser::popExpectedToken(): Dude, that's bad, the token you're"
            "trying to pop off the stack does _not_ match the token sitting"
            "at the top: ");
      }

      return kot;
    }


  bool
    Parser::isExpected(const Token& tok)
    {
      for(const Token& kot : _stack) {
        if (tok == kot)
          return true;
      }

      return false;
    }


  void
    Parser::parse()
    {
      logtrace << "Parser::parse(): BEGIN.";

      assert(fragments_ == nullptr);

      fragments_ = new Fragment();

      pushExpectedToken(Token::Kind::EOF);

      nt_whatever(fragments_, 1);

      // Consume tokens that where not lexed
      // and issue warnings about these.
      int notLexedCount = 0;
      while(true) {
        Token& tok = next_token();

        notLexedCount++;

        if (tok.is_eof()) {
          logdebug << "Parser::parse(): Got EOF, ok";
          break;
        }

        logwarn << "Parser::parse(): Remaining token that wasn't parsed: "
                << tok.kind_as_text();
      }

      if (notLexedCount > 1) {
        logerror << "Parser::parse(): " << notLexedCount
                 << " tokens were not lexed, that's not okay." ;
      }

      logtrace << "Parser::parse(): END";
    }


  Fragment *
    Parser::nt_whatever(Fragment *previous, int depth)
    {
      logtrace << depth << ") nt_whatever: start.";

      Fragment *first_fragment = new Fragment();
      Fragment *current = first_fragment;

      while(true)
      {
        while (true)
        {
          Token& tok = next_token();

          if (isExpected(tok)) {
            rewind_token();
            logdebug << depth << ") nt_whatever: matched an expected token: "
                     << tok.kind_as_text()
                     << " [RETURNING TO CALLER]" ;
            goto my_first_goto_in_a_while;
          }

          current->push_token(&tok);

          // if (tok.is_blank()) {
          //   loginfo << "nt_whatever(): skipping blank(s) token."
          //           << tok.text();
          // }

          // ';' terminated => STATEMENT
          if (tok.is_symbol(';')) {
            current->set_kind(Fragment::Kind::statement);
            logtrace << depth << ") nt_whatever: got one fragment !";
            break;
          }
          // {...} BLOCK "ANTECEDENT"
          else if (tok.is_symbol('{')) {
            current->set_kind(Fragment::Kind::block_antecedent);
            current->pop_token();
            rewind_token();
            current = nt_curly_block(current, depth+1);
            break;
          }

          previous = current;
        }

        // Create a new fragment for accumulating tokens.
        current = new Fragment();

        current->set_previous_fragment(previous);
        previous->set_next_fragment(current);
      }

my_first_goto_in_a_while:
      logtrace << depth << ") nt_whatever: finished.";
      return first_fragment;
    }


  Fragment *
    Parser::nt_curly_block(Fragment *preceding, int depth)
    {
      logtrace << depth << ") nt_curly_block: begin.";

      Token& tok = next_token();

      assert( matchSymbol('{') );

      pushExpectedSymbol("}");

      Fragment *block = new Fragment(Fragment::Kind::block);
      block->push_token(&tok);
      block->set_previous_fragment( preceding );
      if (preceding != nullptr)
        preceding->set_next_fragment( block );

      while(true) {
        nt_whatever(block, depth+1);

        tok = next_token();
        if (tok.is_symbol('}')) {
          logdebug << depth << ") nt_curly_block: matched expected '}' symbol"
                               "– end-of-block – ok, returning to caller.";
          break;
        }

      }

      logtrace << depth << ") nt_curly_block: end.";

      return block;
    }


  void
    Parser::debug_print_ast(std::ostream& os)
    {
      loginfo << "Parser::debug_print_ast(): START.";

      Fragment *current = fragments_;

      do {
        os << "FRAGMENT KIND: " << (int)current->kind() << " {" << std::endl;
        auto tokens = current->tokens();
        for(Token *tok : tokens) {
          os << '[' << tok->text() << ']';
        }
        os << "} [" << (int)current->kind() << "]" << std::endl;
      } while( (current = current->next()) != nullptr );

      loginfo << "Parser::debug_print_ast(): END.";
    }


  void
    Parser::dev_parse()
    {
      logtrace << "Parser::parse(): begin.";

      Lexer alex (_file);

      while( true )
      {
        //Token tok = alex.next_token();
        Token& tok = next_token();

        if (tok.is_eof()) {
          loginfo << "Parser reached EOF.";
          break;
        }
        else if (tok.is_nil()) {
          logwarn << "Parser got a NIL token";
          break;
        }
        else if (tok.is_null_byte()) {
          logwarn << "Parser got a \\0 byte, ignoring it.";
        }
        else if (tok.is_blank()) {
          loginfo << "Parser got blanks.";
        }
        else if (tok.is_symbol()) {
          loginfo << "Parser got a symbol :"
                  << tok.first_character();
        }
        else if (tok.is_identifier()) {
          loginfo << "Parser got an identifier : "
                  << tok.text();
        }
        else if (tok.is_number()) {
          loginfo << "Parser got a number : "
                  << tok.text();
        }
        else if (tok.is_string()) {
          loginfo << "Parser got a string : "
                  << tok.text();
        }
        else if (tok.is_whatever()) {
          logwarn << "Parser got a “whatever” token from the lexer :"
                  << tok.raw_text();
        }
        else if (tok.is_comment()) {
          loginfo << "Parser got a comment (!) : \n"
                  //<< tok.text() ;
                  << tok.raw_text() ;
        }
        else {
          logwtf << "Got an unexpected token: "
                 << static_cast<short>(tok._kind);
        }
      }

      logtrace << "Parser::parse(): end.";
    }



} // psr ns.
} // dude ns.
