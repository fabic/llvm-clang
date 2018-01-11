
#include "dude/lexer/lexer.hpp"
#include "dude/util/logging.hpp"

namespace dude {
namespace lexer {

  // ctor
  Lexer::Lexer(File& file)
    : _file(file),
      _it(file.content().cbegin()),
      _it_begin(file.content().begin()),
      _it_end(file.content().cend())
  {
  }


  char
    Lexer::next_character()
    {
      if (have_reached_eof()) {
        throw dude::ex::yet_undefined_exception(
            "Lexer::next_character(): EOF -_-"
            " can't read past the end-of-file.");
      }

      char ch = *_it++;

      return ch;
    }


  void
    Lexer::put_back_character(char ch)
    {
      if (_it == _it_begin) {
        throw dude::ex::yet_undefined_exception(
            "Lexer::put_back_character(): Can't reasonably put back a character"
            " before the start of the string -_-");
      }

      _it--;

      if (*_it != ch) {
        logwarn << "Beware: the character you just put back doesn't match"
                   " the one in the previous position.";
      }
    }


  bool // static btw.
    Lexer::is_symbol_character(char ch)
    {
      switch(ch) {
        case '/': case '*': case '#': case '@': case '^': case '%': case '&':
        case '|': case '.': case ':': case '-': case '+': case '>': case '<':
        case '=': case '(': case ')': case '~': case '!': case '"': case '\'':
        case '`': case '[': case ']': case '{': case '}': case '$': case ';':
        case ',': case '\\':
          return true;
        default:
          return false;
      }
    }


  Token
    Lexer::next_token()
    {
      if (have_reached_eof()) {
        return Token( Token::Kind::EOF );
      }

      char ch = next_character();

      // Branch upon "special" single characters".
      switch(ch) {
        case '\0':
          return Token( Token::Kind::nullbyte );
      }

      // Other branch conditions :

      // Consume blank space.
      if (is_blank_character(ch)) {
        put_back_character(ch);
        return try_lex_a_bunch_of_blank_space();
      }
      // Single slash '/' symbol, comment block `/* ... */`,
      // multiple consecutive lines of `// ...` comments.
      else if (ch == '/') {
        char nch = next_character();
        // ¿ Start of a comment block ?
        if (nch == '*') {
          put_back_character(nch);
          put_back_character( ch);
          return try_lex_comment_block();
        }
        // Start of a '//' line comment ?
        else if (nch == '/') {
          put_back_character(nch);
          put_back_character( ch);
          return try_lex_double_slashed_consecutive_comment_lines();
        }
        else {
          put_back_character(nch);
          return Token(Token::Kind::symbol, &_it[-1], 1);
        }
      }
      // Double-quoted "..." string :
      else if (ch == '"') {
        put_back_character(ch);
        return try_lex_double_quoted_string();
      }
      // Symbols
      else if (is_symbol_character(ch)) {
        return Token(Token::Kind::symbol, &_it[-1], 1);
      }
      // Identifier
      else if (may_character_start_identifier(ch)) {
        put_back_character(ch);
        return try_lex_identifier();
      }
      // Numbers
      else if (is_digit_character(ch)) {
        put_back_character(ch);
        return try_lex_numeric_literal();
      }
      // ERROR - Consume any character we do not know,
      //         and form a 'whatever' token with it.
      else {
        const char * ptr_start = &*_it;
        const char * ptr_end   = nullptr;
        do {
          switch(ch) {
            case '\0':
            case ' ': case '\t': case '\n': case '\r':
              ;
            case '/':
            default:
              ch = next_character();
          }
        } while( !have_reached_eof());
        ptr_end = &*_it;
        put_back_character(ch);
        return Token(Token::Kind::whatever, ptr_start, ptr_end);
      }

      return Token();
    }


  Token
    Lexer::try_lex_a_bunch_of_blank_space()
    {
      const char * blank_start_ptr = &*_it;
      const char * blank_end_ptr   = nullptr;

      bool has_eol_chr = false;

      do {
        char ch = next_character();
        if (! is_blank_character(ch)) {
          put_back_character(ch);
          break;
        }
        else if (is_eol(ch)) {
          has_eol_chr = true;
          break;
        }
      } while( true );

      blank_end_ptr = &*_it;

      return Token(Token::Kind::blank_w_eol, blank_start_ptr, blank_end_ptr);
    }


  Token
    Lexer::try_lex_comment_block()
    {
      const char * lexem_start_ptr = &*_it;
      const char * lexem_end_ptr   = nullptr;

      char ch = next_character();
      char nch = next_character();

      if (ch != '/' && nch != '*') {
        put_back_character(nch);
        put_back_character( ch);
        throw dude::ex::yet_undefined_exception(
            "TODO: try_lex_comment_block(): impl. backtracking ?");
      }

      while( ! have_reached_eof() )
      {
        ch = next_character();

        // Match the `*/` end of block comment.
        if (ch == '*') {
          nch = next_character();
          if (nch == '/') {
            logtrace << "Lexer: got one comment block !";
            break;
          }
        }

      }

      // todo: err. handling in case we reached EOF before matching the
      //       actual comment block end "*/" ?
      lexem_end_ptr = &*_it;

      return Token(Token::Kind::comment, lexem_start_ptr, lexem_end_ptr);
    }


  Token
    Lexer::try_lex_double_slashed_consecutive_comment_lines()
    {
      const char * lexem_start_ptr = &*_it;
      const char * lexem_end_ptr   = nullptr;

      char ch = '\0', nch = '\0';

      while( ! have_reached_eof() )
      {
         ch = next_character();
        nch = next_character();

        if (ch != '/' && nch != '/') {
          put_back_character(nch);
          put_back_character( ch);
          break;
        }

        while(!have_reached_eof() && (ch = next_character() != '\n')) { }
        // if (have_reached_eof()) { break; } // => end-of-file (hence we're not
                                           //    putting back the character.

        // TODO: comsume leading blanks in hope of matching '//' again and
        //       have other comment lines (with trailing blanks) consumed.
      }

      lexem_end_ptr = &*_it;

      return Token(Token::Kind::comment, lexem_start_ptr, lexem_end_ptr);
    }


  Token
    Lexer::try_lex_identifier()
    {
      const char * lexem_start_ptr = &*_it;
      const char * lexem_end_ptr   = nullptr;

      while( ! have_reached_eof() )
      {
        char ch = next_character();
        if (!is_digit_character(ch)
            && !may_character_start_identifier(ch)) {
          put_back_character(ch);
          break;
        }
      }

      lexem_end_ptr = &*_it;

      return Token(Token::Kind::identifier, lexem_start_ptr, lexem_end_ptr);
    }


  Token
    Lexer::try_lex_numeric_literal()
    {
      const char * lexem_start_ptr = &*_it;
      const char * lexem_end_ptr   = nullptr;

      while( ! have_reached_eof() )
      {
        char ch = next_character();
        if (!is_digit_character(ch)
            && true) { // todo: continue impl.
          put_back_character(ch);
          break;
        }
      }

      lexem_end_ptr = &*_it;

      return Token(Token::Kind::number, lexem_start_ptr, lexem_end_ptr);
    }


  Token
    Lexer::try_lex_double_quoted_string()
    {
      const char * lexem_start_ptr = &*_it;
      const char * lexem_end_ptr   = nullptr;

      // Consume start of string character '"' :
      char ch = next_character();
      if (ch != '"') {
        throw dude::ex::yet_undefined_exception(
            "Trying to lex a double-quoted string, "
            "but first character is not '\"'" );
      }

      char nch = '\0';

      while( ! have_reached_eof() )
      {
        ch = next_character();
        // Skip escaped double-quote \" :
        if (ch == '\\' && (nch = next_character()) == '"' )
          continue;
        else if (ch == '"')
          break;
      }

      lexem_end_ptr = &*_it;

      return Token(Token::Kind::string, lexem_start_ptr, lexem_end_ptr);
    }


  bool // static btw.
    Lexer::may_character_start_identifier(char ch)
    {
      // See Clang's `Lexer::LexTokenInternal(...)`
      //     [src/clong/_clang/Lex/Lexer.cpp]
      switch(ch) {
        // C99 6.4.2: Identifiers.
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
          return true;
        default:
          return false;
      }
    }


} // lexer ns.
} // dude ns.

