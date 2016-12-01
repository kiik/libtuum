/** @file tuum_navigation.hpp
 *  Navigation system interface.
 *
 *  @authors Meelik Kiik
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 2 December 2015
 */

#ifndef TUUM_NAVIGATION_H
#define TUUM_NAVIGATION_H

#include "rtxmath.hpp"

#include "Ball.hpp"
#include "Goal.hpp"
#include "Robot.hpp"

#include "tuum_EntityFilter.hpp"

namespace tuum {

  class Navigation
  {
  public:
    Navigation(EntityFilter*);

    void init();
    void run();

    void updateEntities();

    int countValidBalls();

    Transform calcBallPickupPos(Transform*);
    Transform calcAllyGoalPos(Transform*);

    vec2i calcGoalShootPos(Transform*);

    Ball* getNearestBall();

    BallSet* getBalls();
    GoalSet* getGoals();

    Goal* getOpponentGoal();
    Goal* getAllyGoal();

    Robot* getAlly();

    static void setup(EntityFilter*);
    static void preProcess();
    static void process();

  private:
    EntityFilter* gEntityFilter;
  };

  extern Navigation* gNavigation;

}

#endif
