/** @file PID.hpp
 *  @brief PID controller.
 *
 *  @authors Meelik Kiik
 *  @date 9. October 2016
 *  @version 0.1
 */

#ifndef TUUM_PID_H
#define TUUM_PID_H

#include "platform.hpp"

namespace tuum {

  extern float gP, gI, gD;

  class PID
  {
  public:
    typedef float pid_size_t;

    PID();

    pid_size_t run(pid_size_t, float);

    void setP(pid_size_t);
    void setI(pid_size_t);
    void setD(pid_size_t);
    void setPID(pid_size_t, pid_size_t, pid_size_t);

    pid_size_t _p();
    pid_size_t _i();
    pid_size_t _d();

  private:
    pid_size_t P, I, D, m_val;
    pid_size_t m_p, m_i, m_d;
    pid_size_t m_lerr;
  };

}

#endif
