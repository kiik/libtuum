
#include <cstring>
#include <sqlite3.h>

#include "tuum_db.hpp"

#include "tdb_query.hpp"

namespace tuum { namespace db {

  query_t::query_t()
  {

  }

  query_t::query_t(value_t tbl):
    m_table(tbl)
  {

  }

  query_t::~query_t()
  {

  }

  void query_t::init(value_t tbl)
  {
    m_table = tbl;
  }

  query_t* query_t::filter(value_map in) {
    for(auto it = in.begin(); it != in.end(); it++) {
      m_filters[it.key()] = it.value();
    }

    return this;
  }

  int query_t::exec(rows_t& out) {
    if(run() < 0) return -1;
    out = _d;
    return 0;
  }

  int query_t::exec(row_t& out) {
    if(run() < 0) return -1;
    if(_d.size() == 0) return 0;

    for(auto col = _d[0].begin(); col != _d[0].end(); col++) {
      out[col.key()] = col.value();
    }

    return 0;
  }

  int query_t::compile() {
    std::stringstream sql;

    sql << "SELECT * FROM " << m_table;

    sql << " WHERE ";
    bool delim = false;
    for(auto it = m_filters.begin(); it != m_filters.end(); it++) {
      if(!delim) delim = true;
      else sql << " AND ";

      sql << format("%s = %s", it.key(), it.value());
    }

    _q = sql.str();
    return 0;
  }

  int query_t::execute() {
    sqlite3* db = gHandle;
    char *zErrMsg = 0;
    int rc;

    printf("exec %s\n", _q.c_str());
    rc = sqlite3_exec(db, _q.c_str(), query_t::query_callback, this, &zErrMsg);
    if(rc != SQLITE_OK) {
      RTXLOG(format("SQL error: %s\n", zErrMsg), LOG_ERR);
      sqlite3_free(zErrMsg);
      return -1;
    }

    return 0;
  }

  int query_t::run() {
    if(!isReady()) return -1;

    _d.clear();
    _q.clear();

    if(compile() < 0) return -2;
    if(execute() < 0) return -3;

    return 0;
  }

  int query_t::hook(int argc, char** argv, char** coln)
  {
    value_map row;

    int i;
    for(i=0; i < argc; i++) {
      if(argv[i])
        row[coln[i]] = argv[i];
    }
    _d.push_back(row);

    return 0;
  }

}}
