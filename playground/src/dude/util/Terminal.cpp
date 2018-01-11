#include "dude/util/Terminal.hpp"

namespace clong {

  /*
   * "Global" func. ...
   */
  Terminal&
    terrs()
    {
      static Terminal T( llvm::errs() );
      return T;
    }


  // ctor
  Terminal::Terminal(raw_ostream& ros)
    : Base(/* unbuffered */ true)
    , ROS_(ros)
  { /* noop */ }


  uint64_t
    Terminal::current_pos() const
    {
      //return ROS_.current_pos();
      // ^ Can't: private member -_-
      return 7; // FIXME: returning some random number...
    }


  Terminal&
    Terminal::changeColor(enum Colors Color, bool Bold, bool BG)
    {
      // We need to pad before changing the color since
      // the background may be reversed.
      SpacePadLeftMargin();
      ROS_.changeColor(Color, Bold, BG);
      return *this;
    }


  Terminal&
    Terminal::push(unsigned charCount)
    {
      indentStack.push_back( charCount );
      Margin_ += charCount;
      return *this;
    }


  Terminal&
    Terminal::pop() {
      if (indentStack.size() > 0) {
        unsigned count = indentStack.back();
        indentStack.pop_back();
        assert(count <= Margin_);
        Margin_ -= count;
      }
      else {
        *this << "\n(!!!) Terminal::pop(): Nothing left to pop off the indentation stack -_- (!!!)\n\n";
      }
      return *this;
    }


  void
    Terminal::SpacePadLeftMargin()
    {
      if (Column_ != 0)
        return;
      ROS_.indent(Margin_);
      Column_ = Margin_;
    }


  // Note to self: that thing was unexpectedly messy to get right.
  void
    Terminal::write_impl(const char *Ptr, size_t Size)
    {
      size_t i = 0, j = 0;

      for( ; i < Size; i++)
      {
        char ch = Ptr[i];
        if (ch != '\n')
          continue;

        SpacePadLeftMargin();

        size_t len = i-j+1;
        ROS_.write(Ptr+j, len);

        j = i+1;
        Line_++;
        Column_ = 0;
      }
      // ^ We leave this loop when `i` == Size.
      //   However 2 situations occur (but really are "the same thing"):
      //     - j == 0 : no LF was found.
      //     - j > 0 but j < Size : There remains a last portion of the input
      //                          string that does not end with a LF and hence
      //                          was not yet output.

      if (j < Size) {
        SpacePadLeftMargin();
        ROS_.write(Ptr+j, i);
        Column_ += i-j;
      }
    }


  Terminal&
    Terminal::operator<<(TermCtrl x)
    {
      switch(x) {
      case tendl:    endl(); break;
      case trset:
      case treset:   reset(); break;
      case tnormal:  normal(); break;
      case treverse: reverse(); break;
      case tbold:    bold(); break;
      case tred:     red(); break;
      case tgreen:   green(); break;
      case tblue:    blue(); break;
      case tyellow:  yellow(); break;
      case tmagenta: magenta(); break;
      case tcyan:    cyan(); break;
      case twhite:   white(); break;
      default:
        llvm_unreachable("Ouch!");
      }
      return *this;
    }

  // ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~ x ~~

  // ctor
  TPush::TPush(const char *message)
    : Term_( terrs() )
    , Text_( message )
  {
    if (Text_.empty())
      Text_ = Routine()();
    if (!Text_.empty())
      Term_ << tnormal << Text_ << tendl;
    Term_.push();
  }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  /// Destructor.
  TPush::~TPush()
  {
    Term_.pop();
    if (!Text_.empty())
      Term_ << tnormal << Text_ << tendl;
  }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  Terminal&
    TPush::heading(const char *heading)
    {
      Term_.changeColor(Terminal::Colors::WHITE, true, false);
      Term_.changeColor(Terminal::Colors::YELLOW, true, true);
      Term_ << heading ;
      Term_.reset();
      return Term_;
    }

} // clong ns.
