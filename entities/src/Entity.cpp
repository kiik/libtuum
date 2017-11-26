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
    m_alive_frames(0),
    m_matched(false)
  {
    mId = Entity::newId();
  }

  Entity::Entity(const Blob& blob):
    Entity()
  {
    mBlob = blob;
    mBlob_ = blob;

    // Vec2i r = mBlob.getCentroid();

    //TODO: Camera to world projection logic
    /*
    int H = 800;
    int P0_x = 1280 / 2, P0_y = 100;

    mTransform.setX(r.x - P0_x);
    mTransform.setY((H - r.y - P0_y));
    */
  }

  // (H - r.y - P0_y) * 10 = Y -> y = H - C + Y / 10

  Entity::Entity(const Transform& transform, const Blob& blob):
    Entity()
  {
    mTransform = transform;
    mBlob = blob;
    mBlob_ = blob;
  }

  Entity::~Entity()
  {

  }

  void Entity::match(Transform tfm, Blob bl)
  {
    // printf("[Entity::match]#TODO: Match\n");
    mBlob_ = bl;
    mTransform = tfm; // gmField->fn_T_cameraToWorld({bl.c_x, bl.c_y});

    m_matched = true;
  }

  float Entity::matchPercent(Transform tfm, Blob bl)
  {
    auto c1 = mBlob.getCentroid(), c2 = bl.getCentroid();

    // printf("dbg match e#%lu: mBlob.area=%lu, bl.area=%lu\n", mId, mBlob.area(), bl.area());

    if(bl.area() / float(mBlob.area()) < 0.8) return 0.0;

    // Projection transformation is applied at this point, so distance should be close to real value.
    float d = c1.distanceTo(c2);

    // printf("Entity#%lu c1=%s, c2=%s, d=%.2f\n", mId, c1.toString().c_str(), c2.toString().c_str(), d);

    if(d < 100.0) {
      return 1.0;
    }

    // printf("#TODO: MATCH PERCENT d=%.2f\n", d);

    return 0.0;
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

      m_dead_frames = 0;
      m_alive_frames = m_alive_frames >= 5 ? 5 : m_alive_frames + 1;

      m_matched = false;
    }
    else
    {
      // increase death timer
      m_dead_frames++;
      m_alive_frames = m_alive_frames <= 0 ? 0 : m_alive_frames - 1;
    }
  }

  int Entity::deadFrameCount()
  {
    return m_dead_frames;
  }

  int Entity::aliveFrameCount()
  {
    return m_alive_frames;
  }

  bool Entity::isAlive()
  {
    return m_alive_frames > 2;
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
