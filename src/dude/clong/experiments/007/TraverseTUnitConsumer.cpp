
#include "TraverseTUnitConsumer.hpp"
#include "PimplIOVisitor.hpp"
#include "dude/util/Terminal.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>

namespace clong {
namespace plugin {

  // ctor
  TraverseTUnitConsumer::TraverseTUnitConsumer(
      CompilerInstance &Instance,
      std::set<std::string> ParsedTemplates
    ) : Instance(Instance), ParsedTemplates(ParsedTemplates)
  {
    (terrs().yellow() << "TraverseTUnitAction() [ctor]\n")
      .reset()
      .indent();
  }


  // dtor
  TraverseTUnitConsumer::~TraverseTUnitConsumer()
  {
    (terrs(-1).unindent().pad()
      .yellow() << "~TraverseTUnitAction() [dtor]\n")
      .reset();
  }


  // [overriden, entry point]
  void
    TraverseTUnitConsumer::HandleTranslationUnit(ASTContext& context)
    {
      (terrs().magenta() << "TraverseTUnitConsumer::HandleTranslationUnit() [BEGIN]\n")
        .reset()
        .indent();

      terrs() << "` getASTAllocatedMemory() -> "
              << context.getASTAllocatedMemory() << "\n";
      terrs() << "` getSideTableAllocatedMemory() -> "
              << context.getSideTableAllocatedMemory() << "\n";

      try {
        PimplIOVisitor visitor( context );

        clang::TranslationUnitDecl *TU = context.getTranslationUnitDecl();

        // if (false)
        //  InvestigateASTContextTypes( context );

        (terrs().white() << "~  ~  ~  X  ~  ~  ~\n").reset();

        visitor.TraverseDecl( TU );

        auto& map = visitor.getRepository().getArtifactsMap();

        ((((terrs().blue() << "~~> Artifacts repository has ")
          .white() << map.size() << " elements.")
          .blue()  << " Estimated memory size: ")
          .white().bold() << map.getMemorySize() << " Bytes\n")
          .reset();
      }
      catch(const std::exception &ex) {
        (((terrs().lf().pad().red().bold().reverse() << "ERROR: ")
          .red().bold() << "Ouch! Caught some exception: ")
          .red() << ex.what() << "\n\n")
          .reset();
      }

      (terrs(-1)
        .unindent().pad()
        .magenta() << "TraverseTUnitConsumer::HandleTranslationUnit() [END]\n")
        .reset();
    }


  void
    TraverseTUnitConsumer::InvestigateASTContextTypes( ASTContext& context )
    {
      // const SmallVectorImpl<Type *>&
      auto& types = context.getTypes();

      TypePrinter printer(context.getPrintingPolicy(), /* indentation */ 2);

      const char *placeholder = ""; // "hey"

      for(const Type *type : types) {
        llvm::errs() << " - ";
        printer.print(type, Qualifiers(), llvm::errs(), placeholder);
        llvm::errs() << "\n";
      }

      (terrs().magenta() << "InvestigateASTContextTypes(): END.\n").reset();
    }

} // plugin ns.
} // clong ns.
