
// #include <llvm/Support/FileSystem.h>
// Let's go with the one that throws:
#include <experimental/filesystem>

#include "Clong.hpp"
#include "dude/util/Terminal.hpp"

/*
 * https://github.com/bendiken/lmdbxx
 * http://lmdbxx.sourceforge.net/
 */

namespace clong {
namespace plugin {

  bool
    Clong::InitLMDB()
    {
      TPush log ("YEAH");

      namespace fs = llvm::sys::fs;

      assert(LMDBDatabasePathName.size() > 2
          && "^ temporary assertion to protect myself.");

      if (fs::is_directory(LMDBDatabasePathName)) {
        fs::remove(LMDBDatabasePathName);
      }

      fs::create_directory(LMDBDatabasePathName);

      LMDB_ = ::lmdb::env::create();
      LMDB_.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
      LMDB_.open(LMDBDatabasePathName.data(), 0, 0664);

      if (false) {
        /* Create and open the LMDB environment: */
        auto env = ::lmdb::env::create();
        env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
        env.open("./database.mdb", 0, 0664);

        /* Insert some key/value pairs in a write transaction: */
        auto wtxn = ::lmdb::txn::begin(env);
        auto dbi = ::lmdb::dbi::open(wtxn, nullptr);
        dbi.put(wtxn, "username", "jhacker");
        dbi.put(wtxn, "email", "jhacker@example.org");
        dbi.put(wtxn, "fullname", "J. Random Hacker");
        wtxn.commit();

        /* Fetch key/value pairs in a read-only transaction: */
        auto rtxn = ::lmdb::txn::begin(env, nullptr, MDB_RDONLY);
        auto cursor = ::lmdb::cursor::open(rtxn, dbi);
        std::string key, value;
        while (cursor.get(key, value, MDB_NEXT)) {
          std::printf("key: '%s', value: '%s'\n", key.c_str(), value.c_str());
        }
        cursor.close();
        rtxn.abort();
      }

      /* The enviroment is closed automatically. */
      return true;
    }


} // plugin ns.
} // clong ns.
