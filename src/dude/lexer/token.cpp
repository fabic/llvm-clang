#include "dude/lexer/token.hpp"

namespace dude {
namespace lexer {

  std::string
    Token::raw_text() const
    {
      return std::string(cbegin(), cend());
    }


  std::string
    Token::text() const
    {
      if (start_ == nullptr)
        throw dude::ex::yet_undefined_exception("Argh! Token#start_ is a nullptr !");
      else if (_count == 0)
        throw dude::ex::yet_undefined_exception("Argh! Token#count_ is 0 (no characters, wtf?).");

      if (false && is_comment()) { // FIXME -_-
        size_t i = 0, j = 0, k = 0;
        std::string retv (_count, '^');
        const char *str = start_;

        // Skip "/*"
        if (str[i] == '/') i++;
        if (str[i] == '*') i++;

        while(true)
        {
          // Skip leading "* " or "*\n".
          if (str[i] == '*' && (str[i+1] == ' ' || str[i+1] == '\n')) {
            i += 2;
          }

          // `i` will hold the start of this comment line.
          // Consume characters until the end-of-line.
          j = i;
          bool eoc = false;
          while (str[j] != '\n'
              && !(eoc = str[j] == '*' && str[j+1] == '/')
              // Bounds checking is ^ slightly incorrect here,
              // but we're supposed to meet the "*/" marking the
              // end comment block before we seg. fault.
              && j < _count)
          {
            j++;
          }

          // Consume the line-feed.
          if (!eoc) { j++; }
          // and copy to the destination buffer.
          retv.replace(k, j-i, str+i, j-i);
          k += j-i;

          // Reached comment's end ⇒ exit !
          if (str[j] == '*' && str[j+1] == '/') { break; }

          // Consumme leading whitespaces (imperfect).
          i = j;
          while (str[i] == ' ' && i < _count) { i++; }
          if (str[i] == '*' && str[i+1] != '/') { i++; }

          // and start over.
        }

        retv.resize(k);

        return retv;
      }

      return raw_text();
    }


  char
    Token::first_character() const
    {
      if (start_ == nullptr || _count == 0)
        throw dude::ex::yet_undefined_exception("Token: No pointer or character count is 0.");
      return start_[0];
    }


  bool
    Token::is_symbol(char ch) const
    {
      if (!is_symbol())
        return false;
      else if (_count != 1) {
        throw dude::ex::yet_undefined_exception(
            "Token::is_symbol(char): character _count is != 1, impl. is"
            "limited to one-byte characters at the moment.");
      }

      return first_character() == ch;
    }


  const char *
    Token::kind_as_text()
  {
    switch(_kind) {
      case Kind::NIL:
        return "NIL (undefined Nil token)";
      case Kind::EOF:
        return "EOF – end-of-file";
      case Kind::nullbyte:
        return "NULL \\0 byte";
      case Kind::whatever:
        return "whatever";
      case Kind::blank:
        return "blank";
      case Kind::blank_w_eol:
        return "blank_w_eol";
      case Kind::number:
        return "number";
      case Kind::string:
        return "string";
      case Kind::identifier:
        return "identifier";
      case Kind::symbol:
        return "symbol";
      case Kind::null_pointer:
        return "null_pointer";
      case Kind::keyword:
        return "keyword";
      case Kind::comment:
        return "comment";
      default:
        return "UNKNOWN TOKEN KIND !";
    }
  }

} // lexer ns.
} // dude ns.
