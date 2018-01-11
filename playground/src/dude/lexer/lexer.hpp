#ifndef _DUDE_LEXER_LEXER_H
#define _DUDE_LEXER_LEXER_H

#include <istream>
#include <string>
#include <vector>

#include "dude/lexer/token.hpp"
#include "dude/util/logging.hpp"
#include "dude/filesystem/file.hpp"

namespace dude {
namespace lexer {

  using dude::fs::File;

  /**
   */
  class Lexer {
    protected:
      File&   _file;
      /// Iterator into the string buffer – points at the next character that
      /// will be consumed.
      File::string_t::const_iterator _it; // todo: use '_' suffix: it's a ptr
      File::string_t::const_iterator _it_begin;
      File::string_t::const_iterator _it_end;
      Cursor  _cursor;
    public:
      /**
       * * FIXME: we do strange things there setting the `istream` buffer
       *   to the `_source_text` buffer – see impl. notes :
       *   <http://en.cppreference.com/w/cpp/io/basic_filebuf/setbuf>
       */
      explicit Lexer(File& file);

      /**
       * Check if we've reached the end of the file content buffer.
       * Note that we do not consider a `\0` byte as a special marker
       * for end-of-string.  */
      inline bool have_reached_eof() const;
      char next_character();

      /// Rewind the `_it` iterator one character backward.
      void put_back_character(char ch); // rewind() ?

      /** Consume characters and produce a token.
       */
      Token next_token();
      Token try_lex_a_bunch_of_blank_space();
      Token try_lex_comment_block();
      Token try_lex_identifier();
      Token try_lex_numeric_literal();
      Token try_lex_double_quoted_string();
      Token try_lex_double_slashed_consecutive_comment_lines();

      static inline bool is_blank_character(char ch);
      static inline bool is_eol(char ch);
      static bool is_symbol_character(char ch);
      static inline bool is_ascii_letter(char ch);
      static inline bool is_digit_character(char ch);
      static bool may_character_start_identifier(char ch);
  };


  // ~ ~ INLINES DEFINITIONS ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  bool
    Lexer::have_reached_eof() const
    {
      return _it >= _it_end;
    }

  bool // static btw.
    Lexer::is_blank_character(char ch)
    {
      return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
    }

  bool // static btw.
    Lexer::is_eol(char ch)
    {
      return ch == '\n';
    }

  bool // static btw.
    Lexer::is_ascii_letter(char ch)
    {
      return (ch >= 'A' && ch <= 'Z')
          || (ch >= 'a' && ch <= 'z');
    }

  bool // static btw.
    Lexer::is_digit_character(char ch)
    {
      return ch >= '0' && ch <= '9';
    }

} // lexer ns.
} // dude ns.

#endif // _DUDE_LEXER_LEXER_H
