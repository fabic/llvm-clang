//===- traverse-translation-unit-001.cpp ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// FABIC / 2017-12-20
//
// I've got a better understanding of that AST and how to traverse it manually.
// Let's investigate Clang's `RecursiveASTVisitor< Derived >` with a new look.
//
// Done here: importing Clang's TypePrinter.cpp file, which does not have a
//            corresponding .h header --> so as to print out all the Type/s
//            the ASTContext knows about.
//            See method `TraverseTUnitConsumer::InvestigateASTContextTypes()`
//
//            And it does work ^^
//
// Usage :
//
//   $ clang++ -Xclang -load -Xclang build/translation-unit/004/clong.so -std=c++1z tutu/test-004.cpp
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"

#include "/home/fabi/dev/llvm-clang/llvm-clang/clang/lib/AST/TypePrinter.cpp"
#include <type_traits>

#include <pqxx/pqxx>

using namespace clang;

/**
 * Core dumped : in search for WTF the problem may be, most probably
 *               some sort of concurrency problem (SOCI ain't thread
 *               safe, which is okay).
 *
 * We want to know this shared module (clong.so) is loaded/initialized
 * more than once.
 *
 * It turns out `TraverseTUnitConsumer` is instantiated once,
 * and `TraverseTUnitAction` twice (per translation unit (cc1 subprocess)).
 */
struct Dummy {
  explicit Dummy()
  {
    llvm::errs() << "\n~~~ PIMPL! WE'RE AT CLASS Dummy CONSTRUCTOR (!) [ctor] ~~~\n";
    llvm::errs() <<   "~~~  ` checking PostgreSql connectivity.               ~~~\n\n";

    try_connect_with_postgres_pqxx();
  }

  /**
   * PostgreSql connectivity through `libpqxx`.
   *
   * * <https://www.tutorialspoint.com/postgresql/postgresql_c_cpp.htm>
   * * <https://github.com/Pr0Wolf29/libpqxx-example>
   * * <https://www.postgresql.org/docs/7.2/static/libpqplusplus.html>
   */
  static void try_connect_with_postgres_pqxx()
  {
    llvm::errs() << "\nPQXX: Dummy::try_connect_with_postgres_pqxx(): BEGIN.\n";

    try {
      // Connect to the database.  In practice we may have to pass some
      // arguments to say where the database server is, and so on.
      //pqxx::connection c;

      pqxx::connection conn(
          "dbname = clong user = fabi password = haiku "
          " hostaddr = 127.0.0.1 port = 5432"
        );

      if (conn.is_open()) {
        llvm::errs() << "PQXX: Opened database successfully: "
                     << conn.dbname() << "\n";
      } else {
        llvm::errs() << "PQXX: Can't open database" << "\n";
      }

      // Start a transaction.  In libpqxx, you always work in one.
      pqxx::work work(conn);

      // work::exec1() executes a query returning a single row of data.
      // We'll just ask the database to return the number 1 to us.
      pqxx::row row = work.exec1("SELECT 1");

      // Look at the first and only field in the row, parse it as an integer,
      // and print it.
      llvm::errs() << row[0].as<int>() << "\n";

      work.exec(R"(
          DROP TABLE IF EXISTS decls ;
          CREATE TABLE IF NOT EXISTS decls (
            id         INT PRIMARY KEY NOT NULL,
            namespace  TEXT NULL,
            class      TEXT NULL,
            name       TEXT NOT NULL,
            fqname     TEXT NULL,
            code       TEXT NULL,
            defn_id    INT NULL,
            is_defn    BOOL DEFAULT FALSE,
            file_id    INT NULL,
            start_line INT NULL,
            end_line   INT NULL
          ) ;
        )");

      work.exec(R"(
        INSERT INTO decls (id, name) VALUES
          (1, 'main'),
          (2, '__libc_start_main') ;
        )");

      {
        pqxx::result rset = work.exec("SELECT * FROM decls ORDER BY name ;");

        for(const auto& row : rset) {
          static_assert( std::is_same<decltype(row), pqxx::row const&>::value,
            "PQXX: (!!) we're expecting pqxx::row& from result sets.");
          llvm::errs() << "`-> "
            << "ID #" << row[0].as<int>()
            << ", namespace: " << row[1].c_str()
            << ", name: " << row[2].as<std::string>()
            << "\n" ;
        }
      }

      // Commit your transaction.  If an exception occurred before this
      // point, execution will have left the block, and the transaction will
      // have been destroyed along the way.  In that case, the failed
      // transaction would implicitly abort instead of getting to this point.
      work.commit();

      conn.disconnect ();
     }
     catch (const std::exception &e) {
      llvm::errs() << "PQXX: ERROR: Ouch! Caught an exception: "
                   << e.what() << "\n";
    }

    llvm::errs() << "PQXX: Dummy::try_connect_with_postgres_pqxx(): END.\n";
  }
} __dummy ;


namespace clong {


  /**
   *
   */
  class PimplIOVisitor :
          public RecursiveASTVisitor< PimplIOVisitor >
  {
  private:
    using Base = RecursiveASTVisitor< PimplIOVisitor > ;
    using Self = PimplIOVisitor ;

  private:
    ASTContext *Context;

  public:
    /// Ctor
    explicit PimplIOVisitor(ASTContext *Context)
      : Context(Context) {}

    /**
     */
    bool VisitRecordDecl(RecordDecl *decl) {
      llvm::errs() << "! [RecordDecl          ] "
                   << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitCXXRecordDecl(CXXRecordDecl *Declaration)
    {
      llvm::errs() << "! [CXXRecordDecl       ] "
                   << Declaration->getQualifiedNameAsString() ;

      FullSourceLoc FullLocation = Context->getFullLoc( Declaration->getLocStart() );
      if (FullLocation.isValid()) {
        llvm::outs() << " ["
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "]";
      }

      llvm::errs() << "\n";
      return true;
    }

    /**
     */
    bool VisitEnumDecl(EnumDecl *decl) {
      llvm::errs() << "! [EnumDecl            ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitEnumConstantDecl(EnumConstantDecl *decl) {
      llvm::errs() << "! [EnumConstantDecl    ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitFieldDecl(FieldDecl *decl) {
      llvm::errs() << "! [FieldDecl           ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitFunctionDecl(FunctionDecl *decl) {
      llvm::errs() << "! [FunctionDecl        ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitNamespaceDecl(NamespaceDecl *decl) {
      llvm::errs() << "! [NamespaceDecl       ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitTagDecl(TagDecl *decl) {
      llvm::errs() << "! [TagDecl             ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitTranslationUnitDecl(TranslationUnitDecl *decl) {
      llvm::errs() << "! [TranslationUnitDecl ] ";

#if 0
      FullSourceLoc FullLocation = Context->getFullLoc( decl->getLocStart() );
      // failed
      llvm::errs() << FullLocation.getManager().getFilename( FullLocation )
                   << "\n";
#endif //0
#if 0
      // failed
      if (FullLocation.isValid())
        llvm::outs() << "  ` Found declaration at "
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "\n";
#endif //0

      llvm::errs() << "\n";

      return true;
    }

    /**
     */
    bool VisitTypeDecl(TypeDecl *decl) {
      llvm::errs() << "! [TypeDecl            ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitTypeAliasDecl(TypeAliasDecl *decl) {
      llvm::errs() << "! [TypeAliasDecl       ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitTypedefDecl(TypedefDecl *decl) {
      llvm::errs() << "! [TypedefDecl         ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitTypedefNameDecl(TypedefNameDecl *decl) {
      llvm::errs() << "! [TypedefNameDecl     ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitValueDecl(ValueDecl *decl) {
      llvm::errs() << "! [ValueDecl           ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitVarDecl(VarDecl *decl) {
      llvm::errs() << "! [VarDecl             ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    /**
     */
    bool VisitParmVarDecl(ParmVarDecl *decl) {
      llvm::errs() << "! [ParmVarDecl         ] " << decl->getQualifiedNameAsString() << "\n";
      return true;
    }

    bool TraverseTranslationUnitDecl(TranslationUnitDecl *decl);
    bool TraverseFunctionDecl(FunctionDecl *decl);

  };

  // ~~ ~~ ~~ PimplIOVisitor impl. ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~

  bool
    PimplIOVisitor::TraverseTranslationUnitDecl(TranslationUnitDecl *decl) {
      return Base::TraverseTranslationUnitDecl( decl );
    }

  bool
    PimplIOVisitor::TraverseFunctionDecl(FunctionDecl *decl) {
      return Base::TraverseFunctionDecl( decl );
    }


  // ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
  // ~~ ~~ ~~ ~ TraverseTUnitConsumer ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
  // ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~


  /**
   *
   */
  class TraverseTUnitConsumer : public ASTConsumer {
  private:
    CompilerInstance &Instance;
    std::set<std::string> ParsedTemplates;

  public:
    /// Ctor
    TraverseTUnitConsumer(CompilerInstance &Instance,
                           std::set<std::string> ParsedTemplates)
        : Instance(Instance), ParsedTemplates(ParsedTemplates)
    {
      llvm::errs() << "\n~~~ TraverseTUnitConsumer() [ctor] ~~~\n\n";
    }

    /**
     * Entry point: We're run somehow when the/a trans. unit is available.
     */
    void HandleTranslationUnit(ASTContext& context) override;

  private:
    /**
     * <http://clang.llvm.org/doxygen/classclang_1_1ASTContext.html#a151571ff7d2efdb6b9b6026773916056>
     */
    void InvestigateASTContextTypes( ASTContext& context );
  };


  // [overriden, entry point]
  void TraverseTUnitConsumer::HandleTranslationUnit(ASTContext& context)
  {
    llvm::errs() << "\n-- TraverseTUnitConsumer::HandleTranslationUnit() : BEGIN ! --\n";
    llvm::errs() << "\n--  ` getASTAllocatedMemory() -> "
                 << context.getASTAllocatedMemory() << "\n";
    llvm::errs() << "\n--  ` getSideTableAllocatedMemory() -> "
                 << context.getSideTableAllocatedMemory() << "\n";

    try {
      PimplIOVisitor visitor( &context );

      clang::TranslationUnitDecl *TU = context.getTranslationUnitDecl();

      visitor.TraverseDecl( TU );

      InvestigateASTContextTypes( context );
    }
    catch(std::exception const &ex) {
      llvm::errs() << "ERROR: Ouch! Caught some exception: "
                   << ex.what() << "\n" ;
    }

    llvm::errs() << "-- TraverseTUnitConsumer::HandleTranslationUnit() : END ! --\n";
  }


  void
    TraverseTUnitConsumer::InvestigateASTContextTypes( ASTContext& context )
    {
      // const SmallVectorImpl<Type *>&
      auto& types = context.getTypes();

      TypePrinter printer(context.getPrintingPolicy(), /* indentation */ 2);

      constexpr char *placeholder = ""; // "hey"

      for(const Type *type : types) {
        llvm::errs() << " - ";
        printer.print(type, Qualifiers(), llvm::errs(), placeholder);
        llvm::errs() << "\n";
      }

      llvm::errs() << "-- InvestigateASTContextTypes(): END.";
    }

  // ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
  // ~~ ~~ ~~ ~~  TraverseTUnitAction  ~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
  // ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~


  /**
   *
   */
  class TraverseTUnitAction : public PluginASTAction {
    std::set<std::string> ParsedTemplates;
  public:
    /**
     * CTOR
     */
    explicit TraverseTUnitAction()
    {
      llvm::errs() << "\n~~~ TraverseTUnitAction() [ctor]\n\n ~~~";
    }

    /**
     */
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   llvm::StringRef) override {
      return llvm::make_unique<TraverseTUnitConsumer>(CI, ParsedTemplates);
    }

    // Automatically run the plugin after the main AST action
    PluginASTAction::ActionType getActionType() override {
      return AddAfterMainAction;
    }

    /**
     */
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override
    {
      if (!args.empty() && args[0] == "help")
        PrintHelp(llvm::errs());

      for (unsigned i = 0, e = args.size(); i != e; ++i) {
        llvm::errs() << "TraverseTranslationUnit arg = " << args[i] << "\n";
      }

      return true;
    }

    /**
     */
    void PrintHelp(llvm::raw_ostream& ros) {
      ros << "Help for TraverseTranslationUnit plugin goes here\n";
    }

  };

} // clong ns.


static FrontendPluginRegistry::Add<clong::TraverseTUnitAction>
  X("print-fns", "print function names");
