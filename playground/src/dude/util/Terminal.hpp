#ifndef _PIMPLIO_Terminal_HPP
#define _PIMPLIO_Terminal_HPP

//#include <llvm/Support/FormattedStream.h>
//#include "clong/llvm/FormattedStream.hpp"
#include <llvm/Support/raw_ostream.h>

#include "dude/hacks/routines.hpp"


// Client code may want to override this.
#ifndef TERRS
# define TERRS(COLOR, STUFF) ((terrs().COLOR() << STUFF).reset()
#endif

#ifndef TLOGERR
#  define TLOGERR(FROM, WHAT) (                                       \
      ((terrs().red().bold().reverse() << "(!) " << FROM << " (!)")   \
        .white() << ' ' << WHAT << ' ')                               \
        .reset()                                                      \
    )
#endif

#ifndef TLOG
#  define TLOG(FROM, WHAT) (                                          \
      ((terrs().blue().bold() << FROM )                               \
        .white() << ' ' << WHAT << ' ')                               \
        .reset()                                                      \
    )
#endif

namespace clong {

  // using namespace llvm;
  using llvm::format_object_base;
  using llvm::formatv_object_base;
  using llvm::FormattedBytes;
  using llvm::FormattedNumber;
  using llvm::FormattedString;
  using llvm::SmallVectorImpl;
  using llvm::StringRef;
  using llvm::raw_ostream;

  enum TermCtrl {
    tendl = 0,
    treset, trset,
    tnormal, treverse, tbold,
    tred, tgreen, tblue, tyellow, tmagenta, tcyan, twhite,
    tpush, tpop // TODO //
  };

  /**
   * We're extending LLVM's `formatted_raw_ostream` so as to have a somewhat
   * simpler interface for writting out to terminals, with added benefits such
   * as :
   *   - "automatic" indentation (by means of a trick, see `terrs(N)`);
   *   - FIXME: override `write_impl()` for this.
   *   - Fluent verbs for setting colors.
   *
   * EDIT: LLVM's FormattedStream code was imported (2018-01-01).
   * EDIT: Got rid of FormattedStream, now extending `raw_ostream` directly.
   */
  class Terminal : public raw_ostream {
  private:
    using Base  = raw_ostream;

  private:
    raw_ostream& ROS_;
    unsigned Line_ = 0,
             Column_ = 0,
             Margin_ = 0;

    llvm::SmallVector<unsigned, 16> indentStack;

  private:
    /// Overriden from _llvm::formatted_raw_ostream & llvm::raw_ostream.
    void write_impl(const char *Ptr, size_t Size) override;
    uint64_t current_pos() const override;
    ///
    void SpacePadLeftMargin();
  public:
    static constexpr size_t defaultIndentCountChars = 2;

  public:
    /// Constructor.
    explicit Terminal(raw_ostream& ros);

    Terminal& changeColor(enum Colors Color,
                             bool Bold = false,
                             bool BG = false) override;

    Terminal& resetColor()   override  { ROS_.resetColor(); return *this; }
    Terminal& reverseColor() override  { ROS_.reverseColor(); return *this; }
    bool is_displayed() const override { return ROS_.is_displayed(); }
    bool has_colors() const override   { return ROS_.is_displayed(); }

    Terminal& push(unsigned charCount = defaultIndentCountChars);
    Terminal& pop();

    /// Emit a new-line character.
    Terminal& endl() { *this << '\n'; return *this; }

    /// Alias for `endl()`.
    Terminal& lf() { return endl(); }

    /// Change only the bold attribute, leave color as-is.
    /// Note that setting bold to false after it has been set to true won't work.
    Terminal& bold(bool bold = true, bool bg = false) {
      return changeColor(Colors::SAVEDCOLOR, bold, bg);
    }

    Terminal& red(bool bold = false, bool bg = false) {
      return changeColor(Colors::RED, bold, bg);
    }

    Terminal& green(bool bold = false, bool bg = false) {
      return changeColor(Colors::GREEN, bold, bg);
    }

    Terminal& yellow(bool bold = false, bool bg = false) {
      return changeColor(Colors::YELLOW, bold, bg);
    }

    Terminal& blue(bool bold = false, bool bg = false) {
      return changeColor(Colors::BLUE, bold, bg);
    }

    Terminal& magenta(bool bold = false, bool bg = false) {
      return changeColor(Colors::MAGENTA, bold, bg);
    }

    Terminal& cyan(bool bold = false, bool bg = false) {
      return changeColor(Colors::CYAN, bold, bg);
    }

    Terminal& white(bool bold = false, bool bg = false) {
      return changeColor(Colors::WHITE, bold, bg);
    }

    Terminal& reset() {
      resetColor();
      return *this;
    }

    /// Synonym for `reset()`.
    Terminal& normal() { return reset(); }

    Terminal& reverse() {
      reverseColor();
      return *this;
    }

    Terminal& operator<<(TermCtrl x);

    // ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~
    // ~~  Overriden operators from Base class (for Terminal& chaining) ~  ~~
    // ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~  ~~

    Terminal& operator<<(const char *Str) { Base::operator<<(Str); return *this; }
    Terminal& operator<<(StringRef Str)   { Base::operator<<(Str); return *this; }
    Terminal& operator<<(char C)          { Base::operator<<(C); return *this; }
    Terminal& operator<<(unsigned char C) { Base::operator<<(C); return *this; }
    Terminal& operator<<(signed char C)   { Base::operator<<(C); return *this; }

    Terminal& operator<<(const std::string &Str) {
      Base::operator<<(Str);
      return *this;
    }

    Terminal& operator<<(const SmallVectorImpl<char> &Str) {
      Base::operator<<(Str);
      return *this;
    }

    Terminal& operator<<(unsigned long N) { Base::operator<<(N); return *this; }
    Terminal& operator<<(long N)          { Base::operator<<(N); return *this; }
    Terminal& operator<<(unsigned long long N) { Base::operator<<(N); return *this; }
    Terminal& operator<<(long long N)   { Base::operator<<(N); return *this; }
    Terminal& operator<<(const void *P) { Base::operator<<(P); return *this; }

    Terminal& operator<<(unsigned int N) { Base::operator<<(N); return *this; }
    Terminal& operator<<(int N)          { Base::operator<<(N); return *this; }
    Terminal& operator<<(double N)       { Base::operator<<(N); return *this; }

    // Formatted output, see the format() function in Support/Format.h.
    Terminal& operator<<(const format_object_base &Fmt) {
      Base::operator<<(&Fmt);
      return *this;
    }

    // Formatted output, see the leftJustify() function in Support/Format.h.
    Terminal& operator<<(const FormattedString &FS) {
      Base::operator<<(FS);
      return *this;
    }

    // Formatted output, see the formatHex() function in Support/Format.h.
    Terminal& operator<<(const FormattedNumber &FN) {
      Base::operator<<(FN);
      return *this;
    }

    // Formatted output, see the formatv() function in Support/FormatVariadic.h.
    Terminal& operator<<(const formatv_object_base &fob) {
      Base::operator<<(fob);
      return *this;
    }

    // Formatted output, see the format_bytes() function in Support/Format.h.
    Terminal& operator<<(const FormattedBytes &FB) {
      Base::operator<<(FB);
      return *this;
    }

  public:
    Terminal& operator<<(const std::exception& ex) {
      *this << tendl << tred << tbold << treverse << " (!) ERROR (!) "
            << tred << ": Ouch! Caught some exception: "
            << twhite << ex.what() << tendl << tendl;
      return *this;
    }
  };

  /// terrs(N) - This returns a reference to a Terminal,
  /// which is-a raw_fd_ostream, for standard error.
  ///
  /// Use it like: terrs() << "foo" << "bar";
  ///              terrs().red.reverse() << "baz";
  ///
  Terminal& terrs();

  /**
   * RAII helper for pushing & poping off the indentation stack automatically.
   *
   * TODO: have it take a closure for eventual client...
   * TODO: ex. TPush _yup("blah blah);
   *           _yup.OnExit([] => { some stuff });
   *           _yup.OnExit([] => { some other stuff });
   */
  class TPush {
  private:
    using Routine = dude::hack::Routine;

  private:
    Terminal& Term_;
    std::string Text_ ;

  public:
    /// Constructor.
    explicit __attribute__ ((noinline))
      TPush(const char *message = "");

    // ctor
    __attribute__ ((always_inline))
    explicit TPush(const StringRef Str) : TPush(Str.data()) { }


    /// Destructor.
    ~TPush();

    //
    // BEWARE! we're introducing some pointer-like behavior here
    //         _to a different type (Terminal vs. TPush).
    //         => and this can be confusing (!)
    //
    Terminal& operator*()  { return Term_; }
    // Terminal* operator->() { return &Term_; }
    // TODO: ^ see if we gain sthg by doing this.

    Terminal& heading(const char *heading);
  };

} // clong ns.

#endif // _PIMPLIO_Terminal_HPP
