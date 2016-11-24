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

#include "tuum_math.hpp"

#include "Blob.hpp"

namespace tuum {

  /**
   *  class KalmanFilterObject
   *    KalmanState m_kfState[2]; // Last, current states
   *    ...?
   *
   *    void kPredict() // Velocity projection
   *    void kSense(Transform newMeasurement) // Velocity change from measurements
   *    void kEvaluate() // Updates m_kfState
   *    Transform kalmanProcess() // runs filter methods and returns probable new Transform
   *
   *  class Entity : KalmanFilterObject
   *    update(Transform transform):
   *      transform = this->kalmanProcess(Transform)
   *      ...
   *
   */

  class Entity
  {
  public:
    enum TypeId {
      None,
      Ball,
      Goal_Blue,
      Goal_Yellow,
    };

    static size_t newId();

    Entity();
    Entity(const Blob&);
    Entity(const Transform&, const Blob&);

    size_t getID();
    int getHealth();
    Blob* getBlob();

    Transform* getTransform();

    void beliefUpdate(Transform);
    void beliefUpdate();


    std::string toString();

  protected:
    static size_t id_seq;

    size_t mId;
    int m_health = 0;

    Blob mBlob;
    Transform mTransform;
  };

}

#endif
