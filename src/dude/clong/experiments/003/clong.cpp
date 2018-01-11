//===- traverse-translation-unit-001.cpp ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// FABIC / 2017-12-19
//
// Experiment -- traversing a translation unit _without_resorting to
// `RecursiveASTVisitor<...>` and writing stuff to a MySQL database
// by means of the SOCI database abstraction library.
//
// See basic SOCI example `misc/soci-mysql-example-01.cpp`
//
// SEG. FAULT !
//
// Tried various cargo cult techniques ; including having a break ;
// Cooked a pizza, had dinner ;
// Tried rebuilding SOCI  with my other Clang ;
// Tried my other Clang hand-compiled install.
// Tried to load the core dumps (LLDB crashed -_-)
// Tried running the whole thing from within LLDB, but couldn't get a backtrace.
// `
//  `
//   And finally: tried mysql-connector-c++ (v1.1.9), which worked fine.
//   But it does resort to exceptions too -_-
//   Aaah !!
//
//   EDIT: Trying PostgreSql with libpqxx, appears ok.
//
// Usage :
//
//   $ clang++ -Xclang -load -Xclang build/translation-unit/003/clong.so -std=c++1z tutu/test-004.cpp
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"

#include <type_traits>

#include "soci/soci.h"
#include "soci/mysql/soci-mysql.h"

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

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
    llvm::errs() << "\n~~~ PIMPL! WE'RE AT CLASS Dummy CONSTRUCTOR (!) [ctor] ~~~\n\n";

    //soci::session sql("mysql://db=clong user=fabi password='haiku'");
    //
    // ^ the sole act of connecting causes a seg. fault -_-
    //   and it has nothing to do with the instantiation/execution
    //   of our TraverseTUnitConsumer/Action classes.
    //

    //soci::session sql("mysql://db=clong user=fabi password='WRONGPASS'");
    // ^ SOCI will raise an exception if it couldn't connect, which
    //   does trigger std::terminate().

    // Let's try mysql-connector-c++
    try_connect_with_mysql_connector();
    try_connect_with_postgres_pqxx();
  }

  /**
   * mysql-connector-c++ 1.1.9 seems to not cause a crash !
   */
  static void try_connect_with_mysql_connector()
  {
    sql::mysql::MySQL_Driver *driver =
      sql::mysql::get_mysql_driver_instance();

    try {
      // 1) try to connect with an incorrect password.
      sql::Connection *con =
        driver->connect("tcp://127.0.0.1:3306", "fabi", "WrongPassword");
    }
    //catch(sql::SQLException const &ex) {
    // ^ /me just want to make sure we can catch things with the
    //   standard ex. base class.
    catch(std::exception const &ex) {
      // 1") so as to catch an ex.
      llvm::errs() << "INFO: Good, we caught the exception as expected: "
                   << ex.what() << "\n" ;

      // 2) Retry connection with the correct credentials.
      try {
        sql::Connection *con =
          driver->connect("tcp://127.0.0.1:3306", "fabi", "haiku");

        sql::Statement *stmt = con->createStatement();

        stmt->execute("USE clong");
        stmt->execute("DROP TABLE IF EXISTS test");
        stmt->execute("CREATE TABLE IF NOT EXISTS test (id INT, label VARCHAR(32), PRIMARY KEY (id))");

        stmt->execute("INSERT INTO test (id, label) VALUES (1, 'label 1')");
        stmt->execute("INSERT INTO test (id, label) VALUES (2, 'label 2')");
        stmt->execute("INSERT INTO test (id, label) VALUES (3, 'label 3')");

        llvm::errs() << "~~ Dude! mysql-connector-c++ seems to work!";
        llvm::errs() << "         (at least it doesn't have Clang crash)";

        delete stmt;
        delete con;
      }
      // 2") catch things with their `sql::SQLException` sub-type.
      catch(sql::SQLException const &ex) {
        llvm::errs() << "ERROR: Ouch! Caught some exception: "
                     << ex.what() << "\n" ;
      }
    }
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
            name       TEXT NOT NULL
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
  class TraverseTUnitConsumer : public ASTConsumer {
  private:
    CompilerInstance &Instance;
    std::set<std::string> ParsedTemplates;
    ASTContext *astContext_ = nullptr;
    soci::session SQL;

  public:
    /// Ctor
    TraverseTUnitConsumer(CompilerInstance &Instance,
                           std::set<std::string> ParsedTemplates)
        : Instance(Instance), ParsedTemplates(ParsedTemplates)
    {
      llvm::errs() << "\n~~~ TraverseTUnitConsumer() [ctor] ~~~\n\n";
    }

    /**
     */
    void HandleTranslationUnit(ASTContext& context) override;

  private:
    /**
     * Dispatches Decl node processing to the VisitXXX() methods.
     *
     * @param D Any concrete Decl sub-type.
     */
    void VisitDecl(const Decl *D);

    /**
     * Iterate over the sub-nodes of the decl. context `DC`, invoking
     * `VisitDecl()`.
     */
    void VisitDeclContext(const DeclContext *DC);

    /**
     */
    void VisitTranslationUnit(const TranslationUnitDecl *ND);

    /**
     */
    void VisitNamespace(const NamespaceDecl *ND);

    /**
     */
    void VisitFunction(const FunctionDecl *FD);

  private:
    void VisitVarDecl(const VarDecl *VD);
  };

  // [overriden, entry point]
  void TraverseTUnitConsumer::HandleTranslationUnit(ASTContext& context)
  {
    llvm::errs() << "\n-- TraverseTUnitConsumer::HandleTranslationUnit() : BEGIN ! --\n";

    try {
      // SQL.open("mysql://db=clong user=fabi password='haiku'");
      // soci::session sql("mysql://db=clong user=fabi password='haiku'");

      this->astContext_ = &context;

      clang::TranslationUnitDecl *TU = context.getTranslationUnitDecl();

      VisitDecl( TU );

      // SQL.close();
    }
    catch(std::exception const &ex) {
      llvm::errs() << "ERROR: Ouch! Caught some exception: "
                   << ex.what() << "\n" ;
    }

    llvm::errs() << "-- TraverseTUnitConsumer::HandleTranslationUnit() : END ! --\n";
  }

  void TraverseTUnitConsumer::VisitDecl(const Decl *D)
  {
      // is-a VarDecl
      if (const ParmVarDecl *PV = dyn_cast<ParmVarDecl>(D)) {
        llvm::errs() << "    ` [ParmVarDecl] (!!): "
          << PV->getQualifiedNameAsString() << "\n";;
        VisitVarDecl( PV );
      }
      // is-a VarDecl
      else if (const ImplicitParamDecl *IP = dyn_cast<ImplicitParamDecl>(D)) {
        llvm::errs() << " > [ ImplicitParamDecl ] " << IP->getQualifiedNameAsString() << "\n";
      }
      // is-a DeclaratorDecl & Redeclarable<VarDecl>
      else if (const VarDecl *VD = dyn_cast<VarDecl>(D)) {
        llvm::errs() << " > [ VarDecl ] " << VD->getQualifiedNameAsString() << " -- "
          << "\n";
      }
      else if (const FileScopeAsmDecl *VD = dyn_cast<FileScopeAsmDecl>(D)) {
        llvm::errs() << " > [ FileScopeAsmDecl ] ( !!! )" << "\n";
      }
      /*
         DeclContext concretes.
       */
      else if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(D)) {
        VisitFunction( FD );
      }
      else if (const CXXRecordDecl *RD = dyn_cast<CXXRecordDecl>(D)) {
        llvm::errs() << " > [ CXXRecordDecl ] " << RD->getQualifiedNameAsString() << "\n";
        VisitDeclContext( RD );
      }
      else if (const RecordDecl *RD = dyn_cast<RecordDecl>(D)) {
        llvm::errs() << " > [ RecordDecl ] " << RD->getQualifiedNameAsString() << "\n";
        VisitDeclContext( RD );
      }
      else if (const EnumDecl *ED = dyn_cast<EnumDecl>(D)) {
        llvm::errs() << " > [ EnumDecl ]\n";
        VisitDeclContext( ED );
      }
      // enum/struct/union/class (we should never enter this for we should
      // have matched an earlier case.
      else if (const TagDecl *TD = dyn_cast<TagDecl>(D)) {
        llvm::errs() << " > [ TagDecl ] (??)\n";
        VisitDeclContext( TD );
      }
      else if (const NamespaceDecl *ND = dyn_cast<NamespaceDecl>(D)) {
        VisitNamespace( ND );
      }
      else if (const TranslationUnitDecl *TU = dyn_cast<TranslationUnitDecl>(D)) {
        VisitTranslationUnit( TU );
      }
      else if (const BlockDecl *BD = dyn_cast<BlockDecl>(D)) {
        llvm::errs() << " > [ BlockDecl ]" << "\n";
        VisitDeclContext( BD );
      }
      else if (const ExternCContextDecl *EC = dyn_cast<ExternCContextDecl>(D)) {
        llvm::errs() << " > [ ExternCContextDecl ]" << "\n";
        VisitDeclContext( EC );
      }
      else if (const ExportDecl *ED = dyn_cast<ExportDecl>(D)) {
        llvm::errs() << " > [ ExportDecl ]" << "\n";
        VisitDeclContext( ED );
      }
      else if (const DeclContext *DC = dyn_cast<DeclContext>(D)) {
        llvm::errs() << " > [ DeclContext ] (!!!) " << "\n";
        VisitDeclContext( DC );
      }
      else if (const TypedefNameDecl *TD = dyn_cast<TypedefNameDecl>(D)) {
        llvm::errs() << " > [ TypedefNameDecl ]\n";
      }
      else if (const TypedefDecl *TD = dyn_cast<TypedefDecl>(D)) {
        llvm::errs() << " > [ TypedefDecl ]\n";
      }
      else if (const TypeAliasDecl *TD = dyn_cast<TypeAliasDecl>(D)) {
        llvm::errs() << " > [ TypeAliasDecl ]\n";
      }
      else if (const TypeDecl *TD = dyn_cast<TypeDecl>(D)) {
        llvm::errs() << " > [ TypeDecl ]\n";
      }
      else if (const LabelDecl *LD = dyn_cast<LabelDecl>(D)) {
        llvm::errs() << " > [ LabelDecl ]: " << LD->getQualifiedNameAsString() << "\n";
      }
      else if (const EnumConstantDecl *EC = dyn_cast<EnumConstantDecl>(D)) {
        llvm::errs() << " > [ EnumConstantDecl ]: " << EC->getQualifiedNameAsString() << "\n";
      }
      else if (const IndirectFieldDecl *FD = dyn_cast<IndirectFieldDecl>(D)) {
        llvm::errs() << " > [ IndirectFieldDecl ] -_-" << "\n";
      }
      // is-a DeclaratorDecl & Mergeable<FieldDecl>
      else if (const FieldDecl *FD = dyn_cast<FieldDecl>(D)) {
        llvm::errs() << " > [ FieldDecl ]: " << FD->getQualifiedNameAsString() << "\n";
      }
      // is-a ValueDecl
      else if (const DeclaratorDecl *DD = dyn_cast<DeclaratorDecl>(D)) {
        llvm::errs() << " > [ DeclaratorDecl ]: " << DD->getQualifiedNameAsString() << "\n";
      }
      // is-a NamedDecl
      else if (const ValueDecl *VD = dyn_cast<ValueDecl>(D)) {
        llvm::errs() << " > [ ValueDecl ]: " << VD->getQualifiedNameAsString() << "\n";
      }
      else if (const NamedDecl *ND = dyn_cast<NamedDecl>(D)) {
        llvm::errs() << " > [ NamedDecl ]: "
          << ND->getNameAsString()
          << " -- " << ND->getQualifiedNameAsString()
          << "\n";
      }
      else if (const EmptyDecl *LD = dyn_cast<EmptyDecl>(D)) {
        llvm::errs() << " > [ EmptyDecl ]: " << "\n";
      }
      else {
        llvm::errs() << " > [ unknown-decl or unnamed-decl (?) ]\n";
      }
  }

  void TraverseTUnitConsumer::VisitDeclContext(const DeclContext *DC)
  {
    clang::DeclContext::decl_iterator iter     = DC->decls_begin(),
                                      end_iter = DC->decls_end();

    for( /**/ ; iter != end_iter ; iter++)
    {
      const Decl *D = *iter;

      llvm::errs() << '.';

      VisitDecl( D );
    }
  }

  void TraverseTUnitConsumer::VisitTranslationUnit(const TranslationUnitDecl *ND)
  {
    llvm::errs() << "TranslationUnit (...)" << "\n";
    VisitDeclContext( ND );
  }

  void TraverseTUnitConsumer::VisitNamespace(const NamespaceDecl *ND)
  {
    llvm::errs() << " > Namespace "<< ND->getQualifiedNameAsString() << "\n";
    VisitDeclContext( ND );
  }

  void TraverseTUnitConsumer::VisitFunction(const FunctionDecl *FD)
  {
    llvm::errs() << " > Function "
      << (FD->isThisDeclarationADefinition() ? "def.: " : "decl: ")
      << FD->getQualifiedNameAsString() << "\n";

    // Iterate over this function's arguments.
    auto it     = FD->param_begin();
    auto it_end = FD->param_end();

    for( ; it != it_end; it++) {
      const ParmVarDecl *P = *it;
      llvm::errs() << "    ` (*) " << P->getQualifiedNameAsString() << "\n";
    }

    // Walk the re-declaration chain.
    const FunctionDecl *decl = FD;
    while(nullptr != (decl = decl->getPreviousDecl())) {
      llvm::errs() << "    ` (!) RE-DECL. (!)";
      VisitFunction( decl );
    }

    VisitDeclContext( FD );

#if 0
    // Function _definitions_ have their arguments disposed as sub-nodes,
    // this isn't the case of function declarations.
    clang::DeclContext::decl_iterator iter     = FD->decls_begin(),
                                      end_iter = FD->decls_end();
    for( /**/ ; iter != end_iter ; iter++)
    {
      const Decl *D = *iter;

      if (const ParmVarDecl *PV = dyn_cast<ParmVarDecl>(D)) {
        llvm::errs() << "    ` ParmVarDecl: "
                     << PV->getQualifiedNameAsString()
                     << "\n";;
      }
    }
#endif // 0

    llvm::errs() << "    ` Done with function.\n";
  }

    void TraverseTUnitConsumer::VisitVarDecl(const VarDecl *VD)
    {
      using clang::QualType;
      using clang::Type;

      // Beware: that thing is some sort of smart-pointer.
      // This is actually a Type*
      QualType qtype = VD->getTypeSourceInfo()->getType();
      Type::TypeClass tc = qtype->getTypeClass();

      using TC = clang::Type::TypeClass;
      switch( tc ) {
        case TC::Builtin: {
          llvm::errs() << "        ` [builtin] ";
          auto type = cast<BuiltinType>(qtype);
          llvm::errs() << type->getName( this->astContext_->getLangOpts()) << "\n";
          break;
        }
        default:
          llvm::errs() << "(!!!) unknown type class\n";
      }
    }

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
