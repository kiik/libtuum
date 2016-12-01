
#ifndef TUUM_ENTITY_FILTER_H
#define TUUM_ENTITY_FILTER_H

#include "tuum_json.hpp"
#include "tuum_cmv.hpp"

#include "tuum_entities.hpp"
#include "tuum_detect.hpp"

namespace tuum {

  //TODO: replace this function's usage with an analoguous Entity method
  static double stateProbability(Transform* t1, Transform* t2) {
    const double A = 125.0;
    double px = A*gauss_prob2(t1->getX(), 120, t2->getX());
    double py = A*gauss_prob2(t1->getY(), 120, t2->getY());
    return 2*px*py / (px+py);
  }

  //typedef std::vector<Feature*> FeatureSet;
  typedef std::vector<Ball*> BallSet;
  typedef std::vector<Goal*> GoalSet;
  typedef std::vector<Robot*> RobotSet;

  typedef EDS<Ball>  BallDetect;
  typedef EDS<Goal>  GoalDetect;
  typedef EDS<Robot> RobotDetect;

  class EntityFilter : public JSONSerializable<EntityFilter>
  {
  public:
    typedef Entity entity_t;
    typedef std::vector<entity_t> EntitySet;

    EntityFilter();

    void process();
    void digest(BlobSet*);

    int digestBall(Blob&);
    int digestGoal(Blob&);

    void toJSON(json&);
    void fromJSON(json);

    BallSet* getBalls() { return &(mBallDetect.objs); }
    GoalSet* getGoals() { return &(mGoalDetect.objs); }


  protected:
    BallDetect mBallDetect;
    GoalDetect mGoalDetect;

    Timer mDebugTmr;
  };

}

#endif
