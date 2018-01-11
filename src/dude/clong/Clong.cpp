
#include <stdexcept>
#include <clang/AST/ASTContext.h>
// #include <clang/AST/ASTConsumer.h>

#include "dude/clang/TypePrinter.hpp"
#include "dude/util/Terminal.hpp"

#include "Clong.hpp"

namespace clong {
namespace plugin {


  // ctor
  Clong::Clong(CompilerInstance& Instance,
               std::set<std::string> ParsedTemplates,
               const char *PQXXOptionsString)
    : Instance(Instance),
      ParsedTemplates(ParsedTemplates),
      PQXX_( PQXXOptionsString )
    {
      Preprocessor& PP = Instance.getPreprocessor();
      PP.addPPCallbacks( // PP takes ownership.
          llvm::make_unique<PPCallbacksTracker>(Ignore,
                                                CallbackCalls, PP) );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  // dtor
  Clong::~Clong()
  {
  }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  void
    Clong::InvestigateASTContextTypes( ASTContext& context )
    {
      TPush log("Here's the types we've got from calling"
                " ASTContext::getTypes(), these are printed out"
                " thanks to Clang's TypePrinter::print().");

      // const SmallVectorImpl<Type *>&
      auto& types = context.getTypes();

#if 1
      TypePrinter printer(context.getPrintingPolicy(), /* indentation */ 2);

      const char *placeholder = "";

      for(const Type *type : types) {
        *log << twhite << "- " << tnormal;
        printer.print(type, Qualifiers(), *log, placeholder);
        *log << tendl;
      }
#else
      *log << tred << "Clong::InvestigateASTContextTypes(): "
                      "TypePrinter thing is *disabled*" << treset << tendl;
#endif
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  // Output the trace given its data structure and a stream.
  int
    Clong::OutputPreprocessorTrace( llvm::raw_ostream &OS )
    {
      // Mark start of document.
      OS << "---\n";

      for (std::vector<CallbackCall>::const_iterator I = CallbackCalls.begin(),
          E = CallbackCalls.end();
          I != E; ++I) {
        const CallbackCall &Callback = *I;
        OS << "- Callback: " << Callback.Name << "\n";

        for (auto AI = Callback.Arguments.begin(), AE = Callback.Arguments.end();
            AI != AE; ++AI) {
          const Argument &Arg = *AI;
          OS << "  " << Arg.Name << ": " << Arg.Value << "\n";
        }
      }

      // Mark end of document.
      OS << "...\n";

      return 0;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  void
    Clong::InitPostgresDatabase()
    {
      TPush log("Clong::InitPostgresDatabase()");

      pqxx::transaction<> TXN( PQXX_ );

      *log << "- Database: " << twhite << PQXX_.dbname() << tnormal << tendl;

      TXN.exec(R"( -- DROP TABLE IF EXISTS decl ;
                   CREATE TABLE IF NOT EXISTS decl (
                     id               SERIAL PRIMARY KEY NOT NULL,
                     kind             SMALLINT NOT NULL,
                     context_id       INT NULL REFERENCES decl (id),
                     -- file_id       INT NULL,
                     -- namespace_id  INT NULL REFERENCES decl (id),
                     -- class_id      INT NULL REFERENCES decl (id),
                     -- codebase_id   INT NULL,
                     name       TEXT NOT NULL,
                     fq_name    TEXT NULL,
                     definition_id  INT NULL REFERENCES decl (id),
                     is_definition  BOOL DEFAULT FALSE,
                     code       TEXT NULL,
                     start_line INT NULL,
                     end_line   INT NULL
                   ); )");

      TXN.commit();
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  const FileEntry*
    Clong::getFileEntry(const Decl& D) const
    {
      const SourceManager& SrcMgr = Context_->getSourceManager();
      SourceLocation LocStart = SrcMgr.getSpellingLoc( D.getLocStart() );
      FileID FID = SrcMgr.getFileID( LocStart );
      const FileEntry *FE = SrcMgr.getFileEntryForID( FID );
      return FE;
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  bool
    Clong::isInMainFile(const Decl& D) const
    {
      const SourceManager& SrcMgr = Context_->getSourceManager();
      SourceLocation SpellLoc = SrcMgr.getSpellingLoc( D.getLocation() );
      return SrcMgr.isInMainFile( SpellLoc );
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  const PrintingPolicy&
    Clong::getPrintingPolicy() const {
      return Context_->getPrintingPolicy();
    }

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

  StringRef
    Clong::getSourceCode(SourceRange SR) const
    {
      if (SR.isInvalid()) {
        return "<<(!) Invalid SourceRange [Clong::getSourceCode()] (!)>>";
      }

      const SourceManager& SrcMgr = Context_->getSourceManager();

      SourceLocation LocStart = SrcMgr.getSpellingLoc( SR.getBegin() );
      SourceLocation LocEnd   = SrcMgr.getSpellingLoc( SR.getEnd()   );

      assert(SrcMgr.getFileID(LocStart) == SrcMgr.getFileID(LocStart)
          && "We're _not_ handling SourceRange/s that span multiple buffers.");

      const char *start = SrcMgr.getCharacterData( LocStart );
      const char *end   = SrcMgr.getCharacterData( LocEnd   );
      size_t length = end - start +1 ;

      assert(start != nullptr && "Ist not good");
      assert(end   != nullptr && "Ist not good");
      assert(end >= start && "Dude, WTF?!");

      auto Str = StringRef(start, length);
      if (Str.empty())
        Str = "<< AIN'T NO CODE! >>";

      return Str;
    }

} // plugin ns.
} // clong ns.
