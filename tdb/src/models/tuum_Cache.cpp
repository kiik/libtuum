
#include "models/tuum_Cache.hpp"

namespace tuum {

  bool Cache::sOrmInit = false;

  void Cache::orm_init() {
    if(Cache::sOrmInit) return;
    Cache obj;
    db::orm(&obj);
    Cache::sOrmInit = true;
  }

  Cache::Cache():
    m_id("id"), m_name("name"), m_data("data")
  {
    db::Model::init();
  }

}
