
#ifndef TUUM_DETECT_H
#define TUUM_DETECT_H

#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/coroutine2/coroutine.hpp>

using namespace boost;

namespace tuum {

  // Entity Detection State
  template<class T>
  struct EDS {
    int mn_h = -4; // Entity removal health condition
    int lo_h = 1;  // Entity detection health condition
    int med_h = 3;

    std::vector<T*> objs;
    std::vector<T*> tmp_objs;
    mutex mLock;

    std::vector<T*>* getEntities() {
      mutex::scoped_lock scoped_lock(mLock);
      return &objs;
    }

    std::vector<T*>* getTmpEntities() {
      mutex::scoped_lock scoped_lock(mLock);
      return &tmp_objs;
    }

    typename boost::coroutines2::asymmetric_coroutine<T*>::pull_type getAllEntities() {
      EDS* that = this;

      typename boost::coroutines2::asymmetric_coroutine<T*>::pull_type routine(
        [that](typename boost::coroutines2::asymmetric_coroutine<T*>::push_type& sink){
          for(auto& o : that->objs) sink(o);
          for(auto& o : that->tmp_objs) sink(o);
      });

      return routine;
    }

    void processProbableEntity(T* obj) {
      T* probable_entity = nullptr;
      double p = 0.0, _p;

      // Calculate entity similarity probabilities
      for(auto& o : this->getAllEntities()) {
        // TODO: entity object should implement this probability method
        _p = stateProbability(o->getTransform(), obj->getTransform());

        if(_p > p) {
          p = _p;
          probable_entity = o;
        }
      }

      // Create or update balls
      if (p < 0.01) {
        tmp_objs.push_back(obj);
      } else if (probable_entity != nullptr) {
        probable_entity->beliefUpdate(*obj->getTransform());
        probable_entity->setBlob(*obj->getBlob());
        delete(obj);
      }
    }

    int size() { return objs.size(); }
    int probableSize() { return tmp_objs.size(); }

    void update() {
      {
        for(auto& b : objs) {
          b->decayTick();
        }

        for(auto& b : tmp_objs) {
          b->decayTick();
        }
      }

      int health = mn_h;

      //FIXME: Memory leaks?
      tmp_objs.erase(std::remove_if(tmp_objs.begin(), tmp_objs.end(),
        [health](T*& obj_ptr) {
          return obj_ptr->getHealth() < health;
        }), tmp_objs.end());

      {
        auto it = tmp_objs.begin();
        while(it != tmp_objs.end()) {
          if((*it)->getHealth() > med_h) {
            objs.push_back(*it);
            it = tmp_objs.erase(it);
          } else it++;
        }
      }

      {
        auto it = objs.begin();
        while(it != objs.end()) {
          if((*it)->getHealth() < lo_h) {
            tmp_objs.push_back(*it);
            it = objs.erase(it);
          } else it++;
        }
      }
    }

  };

}

#endif
