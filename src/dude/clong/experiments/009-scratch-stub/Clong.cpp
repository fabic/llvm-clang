
#include <stdexcept>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>

#include "clong/clang/TypePrinter.hpp"
#include "dude/util/Terminal.hpp"

#include "Clong.hpp"

namespace clong {
namespace plugin {


  // ctor
  Clong::Clong(
      CompilerInstance& Instance,
      std::set<std::string> ParsedTemplates)
    : Instance(Instance),
      ParsedTemplates(ParsedTemplates)
    {
    }


  // dtor
  Clong::~Clong()
  {
  }


  // [overriden, entry point]
  void
    Clong::HandleTranslationUnit(ASTContext& context)
    {
      TPush log ("HEY!");

      *log << "`-> getASTAllocatedMemory() -> "
           << context.getASTAllocatedMemory() << tendl;
      *log << "`-> getSideTableAllocatedMemory() -> "
           << context.getSideTableAllocatedMemory() << tendl;

      try {
        clang::TranslationUnitDecl *TU = context.getTranslationUnitDecl();

        if (true)
          InvestigateASTContextTypes(context);

        throw std::runtime_error("Hola! that's an exception!");
      }
      catch(const std::exception &ex) {
        *log << ex;
      }
    }


  void
    Clong::InvestigateASTContextTypes( ASTContext& context )
    {
      TPush log("Here's the types we've got from calling"
                " ASTContext::getTypes(), these are printed out"
                " thanks to Clang's TypePrinter::print().");

      // const SmallVectorImpl<Type *>&
      auto& types = context.getTypes();

      TypePrinter printer(context.getPrintingPolicy(), /* indentation */ 2);

      const char *placeholder = "";

      for(const Type *type : types) {
        *log << twhite << "- " << tnormal;
        printer.print(type, Qualifiers(), *log, placeholder);
        *log << tendl;
      }
    }

} // plugin ns.
} // clong ns.
