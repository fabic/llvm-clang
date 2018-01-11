
#include <cassert>
#include <exception>
#include "PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

namespace clong {
namespace plugin {

  bool
    PimplIOVisitor::TraverseDeclContextHelper(DeclContext *DC) {
      if (!DC)
        return true;

      terrs() << "PUSH: " << DC->getDeclKindName() << "\n";

      terrs().indent();

      declContextStack.push( DC );

      repo.AddDeclContext( DC );

      bool retv = Base::TraverseDeclContextHelper( DC );

      const DeclContext *dc = declContextStack.pop();

      // Protect me, against myself.
      assert(dc == DC && "DeclContext that was poped isn't the same we pushed in.");

      terrs().unindent();

      return retv;
    }


  bool
    PimplIOVisitor::TraverseTranslationUnitDecl(TranslationUnitDecl *decl)
    {
      return Base::TraverseTranslationUnitDecl( decl );
    }


  void
    PimplIOVisitor::Repository::AddDecl(const Decl *D)
    {
      auto pair = Artifacts.insert( std::make_pair(D, Artifact(D)) );
      bool ok = pair.second;
      if (!ok)
        // throw std::runtime_error("Decl. is already in repository (!)");
        (terrs().red() << "(!) Decl. is already in repository (!).\n").normal();
    }

  void
    PimplIOVisitor::Repository::AddDeclContext(const DeclContext *DC)
    {
      const Decl *D = dyn_cast<Decl>( DC );
      assert( D != nullptr && "Ouch! Could not obtain a Decl* by casting a DeclContext* (!)");
      AddDecl( D );
    }


  bool
    PimplIOVisitor::VisitNamedDecl(NamedDecl *D)
    {
      (terrs().magenta() << "[NamedDecl]["  << D->getDeclKindName() << "] \n")
        .reset();
      repo.AddDecl( D );
      return true;
    }


  bool
    PimplIOVisitor::VisitTypeDecl(TypeDecl *D)
    {
#if 1
      StringRef name = D->getName();
      (terrs().blue() << "[TypeDecl][" << D->getDeclKindName() << "] ")
        .reset() << "name: " << name
                 << ", fqn: " << D->getQualifiedNameAsString() ;

      terrs() << ", type: ";

      QualType T = Context.getTypeDeclType( D );
      IdentifierInfo *I = D->getIdentifier();
      StringRef identifier = I ? I->getName() : "";

      Printer.print(T, terrs(), identifier);

      FullSourceLoc FullLocation = Context.getFullLoc( D->getLocStart() );
      StringRef fileName = FullLocation.getManager().getFilename( FullLocation );

      if (FullLocation.isValid()) {
        llvm::outs() << ", loc.: "
                     << fileName << ':'
                     << FullLocation.getSpellingLineNumber() << ':'
                     << FullLocation.getSpellingColumnNumber();
      }

      // repo.AddDecl( D );

      terrs() << "\n";
#endif //0
      return true;
    }

  bool
    PimplIOVisitor::VisitRecordDecl(RecordDecl *D)
    {
      (terrs().yellow() << "[RecordDecl]["  << D->Decl::getDeclKindName() << "] \n")
        .reset();
      return true;
    }

  bool
    PimplIOVisitor::VisitCXXRecordDecl(CXXRecordDecl *D)
    {
      (terrs().yellow() << "[CXXRecordDecl]["  << D->Decl::getDeclKindName() << "] \n")
        .reset();
      return true;
    }

  bool
    PimplIOVisitor::VisitDeclaratorDecl(DeclaratorDecl *D)
    {
      IdentifierInfo *I = D->getIdentifier();
      StringRef identifier = I ? I->getName() : "X";

      (terrs().white() << "[DeclaratorDecl]["  << D->getDeclKindName() << "] ")
        .reset() << " identifier: " << identifier ;

      const TypeSourceInfo *TSI = D->getTypeSourceInfo();
      if (TSI == nullptr) {
        terrs() << ", type: NONE";
      }
      else {
        const QualType T = TSI->getType();
        terrs() << ", type: ";
        Printer.print(T, terrs(), identifier);
      }

      FullSourceLoc FullLocation = Context.getFullLoc( D->getLocStart() );
      StringRef fileName = FullLocation.getManager().getFilename( FullLocation );

      if (FullLocation.isValid()) {
        llvm::outs() << ", loc.: "
                     << fileName << ':'
                     << FullLocation.getSpellingLineNumber() << ':'
                     << FullLocation.getSpellingColumnNumber();
      }

      terrs() << "\n";
      return true;
    }


  bool
    PimplIOVisitor::TraverseFunctionDecl(FunctionDecl *decl)
    {
      return Base::TraverseFunctionDecl( decl );
    }


  bool
    PimplIOVisitor::VisitFunctionDecl(FunctionDecl *D)
    {
      FunctionVisitor V (D, this);
      return V.Visit();
    }

} // plugin ns.
} // clong ns.
