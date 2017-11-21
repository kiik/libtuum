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

  Entity::Entity():
    m_dead_frames(0),
    m_matched(false)
  {
    mId = Entity::newId();
  }

  Entity::Entity(const Blob& blob):
    Entity()
  {
    mBlob = blob;
    Vec2i r = mBlob.getCentroid();
    mTransform.setX(r.x); mTransform.setY(r.y);
  }

  Entity::Entity(const Transform& transform, const Blob& blob):
    Entity()
  {
    mTransform = transform;
    mBlob = blob;
  }

  void Entity::match(Blob bl)
  {
    // printf("[Entity::match]#TODO: Match\n");
    mBlob_ = bl;
    m_matched = true;
  }

  float Entity::matchPercent(Blob bl)
  {
    auto c1 = mBlob.getCentroid(), c2 = bl.getCentroid();

    float d = c1.distanceTo(c2);
    // printf("c1=%s, c2=%s, d=%.2f\n", c1.toString().c_str(), c2.toString().c_str(), d);

    if(d < 1.0) return 1;

    // printf("#TODO: MATCH PERCENT d=%.2f\n", d);

    return 0;
  }

  bool Entity::matched()
  {
    return m_matched;
  }

  void Entity::tick()
  {
    if(m_matched)
    {
      // Update blob
      mBlob = mBlob_;
      mTransform.setPosition(mBlob.getCentroid());

      m_dead_frames = 0;
      m_matched = false;
    }
    else
    {
      // increase death timer
      m_dead_frames++;
    }
  }

  int Entity::deadFrameCount()
  {
    return m_dead_frames;
  }

  size_t Entity::getId() { return mId; }

  Blob* Entity::getBlob() { return &mBlob; }
  void Entity::setBlob(const Blob& o) { mBlob = o; }

  int Entity::getHealth() { return m_health; }

  Transform* Entity::getTransform() { return &mTransform; }


  void Entity::beliefUpdate(Transform transform) {
    mTransform.setPosition(transform.getX(), transform.getY());
    if(m_health < 8) m_health += 2;
  }

  void Entity::decayTick() {
    m_health--;
  }

  std::string Entity::toString() {
    return format("<Entity #%lu, df=%i, (%i,%i)>", mId, m_dead_frames, mTransform.getX(), mTransform.getY());
  }

}
