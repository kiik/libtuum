
#include "torm_tests.hpp"

#include "models/tuum_Cache.hpp"

namespace tuum { namespace db {

  int run_file_tests() {
    tuum::db::FileORM file;

    file.setPath("assets/test2.ppl");
    file.setData("HEADER\nBODY\nCONTENTS");

    //add(&file);

    // Create query object
    Query::row_t res;

    // Create file query
    tuum::db::value_map flt = {{"path", "assets/test2.ppl"}};
    auto q = file.query()->filter(flt);

    // Execute query
    if(q->exec(res) < 0) {
      RTXLOG("Query tests failed!", LOG_ERR);
      return -1;
    } else {
      printf("Results: %lu\n", res.size());

      /*
      auto dat = res;
      for(auto row = dat.begin(); row != dat.end(); row++) {
        printf("ROW:\n");
        auto r = *row;
        for(auto col = r.begin(); col != r.end(); col++) {
          printf("%s => %s\n", col->first.c_str(), col->second.c_str());
        }
        printf("\n");
      }*/

      auto r = res;
      for(auto col = r.begin(); col != r.end(); col++) {
        printf("%s => %s\n", col.key().c_str(), col.value().get<std::string>().c_str());
      }
      printf("\n");

      RTXLOG("Query tests passed.");
    }

    return 0;
  }

  int run_cache_tests() {
    Cache::orm_init();

    Cache obj;

    //obj.setName("test_cache");
    //obj.setData("TEST_DATA");

    //db::add(&obj);

    tuum::db::value_map flt = {{"name", "test_cache"}};
    auto q = obj.query()->filter(flt);

    Query::row_t res;
    if(q->exec(res) < 0) {
      RTXLOG("Query tests failed!", LOG_ERR);
      return -1;
    } else {
      printf("Results: %lu\n", res.size());
      auto r = res;
      for(auto col = r.begin(); col != r.end(); col++) {
        printf("%s => %s\n", col.key().c_str(), col.value().get<std::string>().c_str());
      }
      printf("\n");

      RTXLOG("Query tests passed.");
    }
  }

  int run_orm_tests() {
    DB_SAFE();

    RTXLOG("Running orm tests...");
    run_cache_tests();
    // db::commit();

    return 0;
  }


}}
