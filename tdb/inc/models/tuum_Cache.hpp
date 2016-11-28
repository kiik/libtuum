
#ifndef TUUM_CACHE_H
#define TUUM_CACHE_H

#include "tuum_orm.hpp"

using namespace tuum::db;

namespace tuum {

  class Cache : public db::Model
  {
  public:
    Cache();

    static void orm_init();

    int getId() { return m_id.get(); }

    std::string getName() { return m_name.get(); }
    std::string getData() { return m_data.get(); }

    void setName(std::string v) { m_name.set(v); }
    void setData(std::string v) { m_data.set(v); }

  protected:
    db::Column<db::sql::Integer> m_id;
    db::Column<db::sql::Text> m_name;
    db::Column<db::sql::Text> m_data;

  public:
    const std::string __table__() const { return std::string("cache"); }

    const std::string __table_sql__() const {
      return "CREATE TABLE IF NOT EXISTS cache("  \
            "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
            "NAME           TEXT UNIQUE NOT NULL," \
            "DATA           TEXT    NOT NULL );";
    }

    value_map __row__() {
      value_map out;
      //if(m_id.assigned)
      //  out[m_id.name] = m_id.value;

      if(m_name.assigned)
        out[m_name.name] = m_name.value;

      if(m_data.assigned)
        out[m_data.name] = m_data.value;

      return out;
    }

    value_map __cols__() {
      value_map out;
      out[m_id.name] = 0;
      out[m_name.name] = m_name.value;
      out[m_data.name] = m_data.value;

      return out;
    }

  private:
    static bool sOrmInit;
  };

}

#endif
