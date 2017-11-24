/** @file Entity.hpp
 *  @brief Field entity class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.4
 *  @date 3. December 2015
 */

#ifndef TUUM_ENTITY_H
#define TUUM_ENTITY_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "tuum_math.hpp"

#include "Blob.hpp"

namespace tuum {

  class Entity;

  typedef boost::shared_ptr<Entity> EntityPtr;

  class Entity
  {
  public:
    static size_t newId();

    Entity();
    Entity(const Blob&);
    Entity(const Transform&, const Blob&);

    ~Entity();

    void  match(Blob);
    float matchPercent(Blob);

    void  match(Transform, Blob);
    float matchPercent(Transform, Blob);

    bool matched();

    void tick();

    size_t getId();
    int    getHealth();

    Blob* getBlob();
    void  setBlob(const Blob&);

    Transform* getTransform();

    virtual std::string toString();

    int deadFrameCount();

    // Legacy:
    void beliefUpdate(Transform);
    void decayTick();

  protected:
    Blob mBlob, mBlob_;
    Transform mTransform;

    static size_t id_seq;

    size_t mId;
    int m_health = 0, m_dead_frames;
    bool m_matched;
  };

}

#endif
