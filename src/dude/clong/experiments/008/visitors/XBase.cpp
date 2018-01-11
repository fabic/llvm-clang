#include <cassert>
#include <exception>

#include "XBase.hpp"
#include "../PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  // ctor
  XBase::XBase(PimplIOVisitor& PV) : Pimp_(PV)
  { /* noop */ }


  StringRef
    XBase::getSourceCode(SourceRange SR) const
    {
      if (SR.isInvalid()) {
        SR = getSourceRange();
        if (SR.isInvalid())
            throw std::invalid_argument("Could not come up with a valid SourceRange (!)");
      }

      const SourceManager& SrcMgr = Pimp_.getContext().getSourceManager();

      SourceLocation LocStart = SrcMgr.getSpellingLoc( SR.getBegin() );
      SourceLocation LocEnd   = SrcMgr.getSpellingLoc( SR.getEnd()   );

      assert(SrcMgr.getFileID(LocStart) == SrcMgr.getFileID(LocStart)
          && "We're _not_ handling SourceRange/s that span multiple buffers.");

      const char *start = SrcMgr.getCharacterData( LocStart );
      const char *end   = SrcMgr.getCharacterData( LocEnd   );
      size_t length = end - start ;

      assert(start != nullptr && "Ist not good");
      assert(end   != nullptr && "Ist not good");
      assert(end >= start && "Dude, WTF?!");

      auto Str = StringRef(start, length);
      if (Str.empty())
        Str = "<< AIN'T NO CODE! >>";

      return Str;
    }

  const FileEntry*
    XBase::getFileEntry(const Decl& D) const
    {
      return Pimp_.getFileEntry( D );
    }

  void
    XBase::PrintType(const TypeSourceInfo* TSI, const IdentifierInfo* I)
    {
      assert(TSI != nullptr);
      QualType QT = TSI->getType();
      PrintType(QT, I);
    }

  void
    XBase::PrintType(const QualType QT, const IdentifierInfo* I)
    {
      StringRef PlaceHolder = I != nullptr ? I->getName() : "<?>";
      PrintType(QT, PlaceHolder);
    }

  void
    XBase::PrintType(const Type* T, const IdentifierInfo* I)
    {
      StringRef PlaceHolder = I != nullptr ? I->getName() : "<?>";
      PrintType(T, PlaceHolder);
    }

  void
    XBase::PrintType(const TypeSourceInfo* TSI, StringRef PlaceHolder)
    {
      assert(TSI != nullptr);
      QualType QT = TSI->getType();
      PrintType(QT, PlaceHolder);
    }

  void
    XBase::PrintType(const QualType QT, StringRef PlaceHolder)
    {
      raw_ostream& ros = terrs();
      Pimp_.getTypePrinter().print(QT, ros, PlaceHolder);
    }

  void
    XBase::PrintType(const Type* T, StringRef PlaceHolder)
    {
      PrintType(QualType(T,0), PlaceHolder);
    }

} // plugin ns.
} // clong ns.
