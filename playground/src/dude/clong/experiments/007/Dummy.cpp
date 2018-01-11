
// #include "llvm/Support/raw_ostream.h"
#include <type_traits>
#include <pqxx/pqxx>

#include "dude/util/Terminal.hpp"

namespace clong {
namespace util {

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
    /// ctor
    explicit Dummy()
    {
      ((terrs().yellow() << "PIMPL! Dummy() [ctor]\n")
        .indent().pad() << "` checking PostgreSql connectivity.\n")
        .reset();
      try_connect_with_postgres_pqxx();
    }

    /// dtor
    ~Dummy()
    {
      ((terrs(-1).unindent().pad().yellow()
        << "PIMPL! ~Dummy() [dtor]\n")
        .pad() << " ` Goodbye.\n")
        .reset();
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
      (terrs() << "PQXX: Dummy::try_connect_with_postgres_pqxx(): BEGIN.\n")
        .indent();

      try {
        // Connect to the database.  In practice we may have to pass some
        // arguments to say where the database server is, and so on.
        //pqxx::connection c;

        pqxx::connection conn(
            "dbname = clong user = fabi password = haiku "
            " hostaddr = 127.0.0.1 port = 5432"
          );

        if (conn.is_open()) {
          ((terrs().green() << "PQXX: Opened database successfully: ")
            .white() << conn.dbname() << "\n")
            .reset() ;
        } else {
          (terrs().red() << "PQXX: Can't open database" << "\n").reset();
        }

        // Start a transaction.  In libpqxx, you always work in one.
        pqxx::work work(conn);

        // work::exec1() executes a query returning a single row of data.
        // We'll just ask the database to return the number 1 to us.
        pqxx::row row = work.exec1("SELECT 1");

        // Look at the first and only field in the row, parse it as an integer,
        // and print it.
        terrs() << row[0].as<int>() << "\n";

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
            terrs() << "`-> "
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
        (((terrs().red().bold().reverse() << "PQXX: ERROR:")
          .red() << " Ouch! Caught an exception: ")
          .white() << "“" << e.what() << "”\n")
          .reset();
      }

      terrs(-1).unindent().pad()
        << "PQXX: Dummy::try_connect_with_postgres_pqxx(): END.\n";
    }
  }  __DUMMY ;

} // util ns.
} // clong ns.
