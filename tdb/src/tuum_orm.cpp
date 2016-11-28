
#include <memory>
#include <vector>
#include <sstream>
#include <map>
#include <sqlite3.h>

#include "tuum_logger.hpp"
#include "tuum_orm.hpp"
#include "tuum_db.hpp"

namespace tuum { namespace db {

  std::string get_columns_sql(const value_map& dat)
  {
    std::stringstream columns;

    columns << '(';
    const char DELIMIT = ',';

    bool delim = false;
    for(auto it = dat.begin(); it != dat.end(); it++) {
      if(delim) columns << DELIMIT;
      else delim = true;

      columns << it.key();
    }

    columns << ')';

    return columns.str();
  }

  int orm(Model* m)
  {
    value_map dat = m->__cols__();

    std::string columns = get_columns_sql(dat);
    std::string table = m->__table__();
    printf("ORM_BIND: %s  (%lu)\n", columns.c_str(), dat.size());

    char* zErrMsg = 0;
    sqlite3* db = gHandle;
    int rc;

    std::string sql = m->__table_sql__();

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if(rc != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    return 0;
  }

  int add(Model* m)
  {
    if(m->__table__() == "") return -1;

    value_map dat = m->__row__();

    //RTXLOG("TODO:", LOG_ERR);

    std::stringstream columns;
    std::stringstream values;

    columns << '(';
    values << '(';
    const char DELIMIT = ',';

    bool delim = false;
    for(auto it = dat.begin(); it != dat.end(); it++) {
      if(delim) {
        columns << DELIMIT;
        values << DELIMIT;
      } else {
        delim = true;
      }

      //printf("FIELD: %s => %s\n", it.key().c_str(), it.value().get<std::string>().c_str());
      columns << it.key();

      //TODO: Type handling
      std::string val = it.value();
      values << '\'' << val << '\'';
    }

    columns << ')';
    values << ')';

    std::stringstream sql;

    sql << "INSERT INTO " << m->__table__() << ' ';
    sql << columns.str() << " VALUES " << values.str() << ';';
    RTXLOG(sql.str());

    char* zErrMsg = 0;
    sqlite3* db = gHandle;
    int rc;

    rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
    if(rc != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    return 0;
  }

}}
