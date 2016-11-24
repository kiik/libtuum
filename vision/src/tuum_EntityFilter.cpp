
#include "tuum_vision.hpp"

#include "tuum_EntityFilter.hpp"

namespace tuum {

  EntityFilter::EntityFilter()
  {

  }

  void EntityFilter::process()
  {

  }

  void EntityFilter::digest(const BlobSet* blobs)
  {
    for(auto it = blobs->begin(); it != blobs->end(); it++) {

    }
  }

  void EntityFilter::toJSON(json& out) {
    //TODO
  }

  void EntityFilter::fromJSON(json in) {
    //TODO
  }

}
