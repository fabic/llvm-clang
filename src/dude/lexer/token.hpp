#ifndef _DUDE_LEXER_TOKEN_H
#define _DUDE_LEXER_TOKEN_H

#include <string>
#include "dude/util/exceptions.hpp"

/// Damn C header -__-
///   “ /usr/include/libio.h:62:14: note: expanded from macro 'EOF'
///     # define EOF (-1) ”
#ifdef EOF
#  undef EOF
#endif

namespace dude {
namespace lexer {

  /**
   */
  struct Cursor {
    /// There's always a first line, even if file is empty, true ?
    int _line   = 1;
    int _column = 0;
  };


  /**
   */
  struct Selection {
    Cursor _start;
    Cursor _end;
  };

  /**
   */
  struct Line {
    Selection _selection;
  };

  /**
   */
  struct Token {
    public:
      using size_t = std::string::size_type;

      /** Token kinds.
       */
      enum Kind : unsigned short {
        NIL = 0, EOF, nullbyte, whatever, blank, blank_w_eol, number,
        string, identifier, symbol, null_pointer, keyword,
        slash_symbol, backslash_symbol, star_symbol, comment
      };

    public:
      Kind   _kind;

      /// Pointer into the source buffer to the start of the lexed token.
      const char * start_;

      /// Character count composing the lexem, from the `start_`.
      size_t _count;
    public:
      explicit Token(Kind k = Kind::NIL)
        : _kind(k), start_(nullptr), _count(0) {}

      Token(Kind k, const char *start, const char *end)
        : _kind(k), start_(start), _count(end-start) {}

      Token(Kind k, const char *start, size_t count)
        : _kind(k), start_(start), _count(count) {}

      Token(Kind k, const char *start)
        : _kind(k), start_(start), _count(strlen(start)) {}

      bool is_nil() const noexcept { return _kind == Kind::NIL; }
      bool is_eof() const noexcept { return _kind == Kind::EOF; }

      bool is_blank() const noexcept {
        return _kind == Kind::blank || _kind == Kind::blank_w_eol ;
      }

      bool is_blank_with_eol() const { return _kind == Kind::blank_w_eol; }
      bool is_eol() const { return is_blank_with_eol(); }

      bool is_null_byte() const noexcept { return _kind == Kind::nullbyte; }
      bool is_whatever() const noexcept { return _kind == Kind::whatever; }
      bool is_comment() const noexcept { return _kind == Kind::comment; }
      bool is_symbol() const noexcept { return _kind == Kind::symbol; }
      bool is_identifier() const noexcept { return _kind == Kind::identifier; }
      bool is_number() const noexcept { return _kind == Kind::number; }
      bool is_string() const noexcept { return _kind == Kind::string; }

      /**
       * Test if this token is a single character symbol matching `ch`.
       */
      bool is_symbol(char ch) const;

      const char * cbegin() const { return start_; }
      const char * cend() const { return start_ + _count; }

      std::string raw_text() const;
      std::string text() const;
      char first_character() const;

      const char *kind_as_text();
  };

  // ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~

  /**
   * Comparison operator== of two tokens.
   */
  inline bool
    operator==(const Token& lhs, const Token& rhs)
    {
      return lhs._kind == rhs._kind
        && lhs._count == rhs._count
        && ( (lhs.start_ == nullptr && rhs.start_ == nullptr)
            || (0 == strncmp(lhs.start_, rhs.start_, lhs._count))
           );
      ;
    }

  /// Return TRUE if tokens `lhs` and `rhs` are different, i.e. do not
  /// represent the same lexed thing.
  inline bool
    operator!=(const Token& lhs, const Token& rhs)
    {
      return false == operator==(lhs, rhs);
    }

} // lexer ns.
} // dude ns.

#endif // _DUDE_LEXER_TOKEN_H

