/** @file tuum_physics.hpp
 *  @brief Physics system interface. Manages geometrical calculations over many entities.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 *  @date 3. December 2015
*/

#ifndef TUUM_PHYSICS_H
#define TUUM_PHYSICS_H

#include "tuum_entities.hpp"
#include "tuum_EntityFilter.hpp"

namespace tuum {

  class Physics
  {
  public:
    Physics(EntityFilter*);

    Entity* rayCast(const double&, const double& = 0);

  protected:
    EntityFilter* entityFilter;
  };

}

#endif
