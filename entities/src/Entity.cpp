/** @file Entity.hpp
 *  @brief Field entity class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.4
 *  @date 3. December 2015
 */

#include <sstream>
#include <string>

#include "platform.hpp"
#include "Entity.hpp"

namespace tuum {

  size_t Entity::id_seq = 1;

  size_t Entity::newId() {
    return id_seq++;
  }

  Entity::Entity() {
    mId = Entity::newId();
  }

  Entity::Entity(const Blob& blob):
    Entity()
  {
    mBlob = blob;
    vec2i r = mBlob.getCentroid();
    mTransform.setX(r.x); mTransform.setY(r.y);
  }

  Entity::Entity(const Transform& transform, const Blob& blob):
    Entity()
  {
    mTransform = transform;
    mBlob = blob;
  }

  size_t Entity::getID() { return mId; }

  Blob* Entity::getBlob() { return &mBlob; }

  int Entity::getHealth() { return m_health; }

  Transform* Entity::getTransform() { return &mTransform; }


  void Entity::beliefUpdate(Transform transform) {
    int x = mTransform.getX()*0.2 + transform.getX()*0.8;
    int y = mTransform.getY()*0.2 + transform.getY()*0.8;
    mTransform.setPosition(x, y);

    if(m_health < 35) m_health += 2;
  }

  void Entity::beliefUpdate() {
    m_health--;
  }

  std::string Entity::toString() {
    return format("<Entity #%lu, %i, (%i,%i)>", mId, m_health, mTransform.getX(), mTransform.getY());
  }

}
