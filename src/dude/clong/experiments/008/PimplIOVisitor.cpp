
#include <cassert>
#include <exception>
#include "PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  // ctor
  PimplIOVisitor::PimplIOVisitor(ASTContext& Context)
    : Context(Context)
    , Printer(Context.getPrintingPolicy())
  {
    (terrs().cyan() << "PimplIOVisitor : BEGIN.\n")
      .push();
  }


  // dtor
  PimplIOVisitor::~PimplIOVisitor()
  {
    terrs().pop().cyan() << "PimplIOVisitor : END.\n";
  }

  const FileEntry*
    PimplIOVisitor::getFileEntry(const Decl& D) const
    {
      const SourceManager& SrcMgr = Context.getSourceManager();
      SourceLocation LocStart = SrcMgr.getSpellingLoc( D.getLocStart() );
      FileID FID = SrcMgr.getFileID( LocStart );
      const FileEntry *FE = SrcMgr.getFileEntryForID( FID );
      return FE;
    }


  bool
    PimplIOVisitor::isInMainFile(const Decl& D) const
    {
      const SourceManager& SrcMgr = Context.getSourceManager();
      SourceLocation SpellLoc = SrcMgr.getSpellingLoc( D.getLocation() );
      return SrcMgr.isInMainFile( SpellLoc );
    }


  bool
    PimplIOVisitor::TraverseDeclContextHelper(DeclContext *DC)
    {
      if (!DC)
        return true;

      (terrs().bold().white() << "PUSH: " << DC->getDeclKindName() << "\n")
        .reset()
        .push();

      declContextStack.push( DC );

      // TODO repo.AddDeclContext( DC );

      bool retv = Base::TraverseDeclContextHelper( DC );

      const DeclContext *dc = declContextStack.pop();

      // Protect me, against myself.
      assert(dc == DC && "DeclContext that was poped isn't the same we pushed in.");

      terrs().pop();

      return retv;
    }


  void
    Repository::Add(std::shared_ptr<XDecl> D)
    {
      Key_t K = KeyOf( D );
      std::shared_ptr<XBase> X = std::static_pointer_cast<XBase>( D );
      auto pair = Artifacts.insert( std::make_pair(K, X) );
      bool ok = pair.second;
      if (!ok)
        // throw std::runtime_error("Decl. is already in repository (!)");
        (terrs().red() << "(!) Decl. is already in repository (!).\n").normal();
    }

  // void
  //   Repository::AddDeclContext(const DeclContext *DC)
  //   {
  //     const Decl *D = dyn_cast<Decl>( DC );
  //     assert( D != nullptr && "Ouch! Could not obtain a Decl* by casting a DeclContext* (!)");
  //     AddDecl( D );
  //   }
  //

} // plugin ns.
} // clong ns.
