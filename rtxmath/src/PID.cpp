/** @file PID.cpp
 *  @brief PID controller implementation.
 *
 *  @authors Meelik Kiik
 *  @date 9. October 2016
 *  @version 0.1
 */

#include "PID.hpp"

namespace tuum {

  float gP = 0.6;
  float gI = 0.4;
  float gD = 0.04;

  PID::PID():
    P(gP), I(gI), D(gD),
    m_p(0.0), m_i(0.0), m_d(0.0),
    m_lerr(0)
  {

  }

  PID::pid_size_t PID::run(pid_size_t err, float dt) {
    m_p = err;

    m_i += err;

    float i_lim = 0.5;

    if(m_i > i_lim) m_i = i_lim;
    else if(m_i < -i_lim) m_i = -i_lim;

    m_d = (err - m_lerr) / dt;
    m_lerr = err;

    return P * m_p + I * m_i + D * m_d;
  }

  void PID::setP(pid_size_t v) { P = v; }
  void PID::setI(pid_size_t v) { I = v; }
  void PID::setD(pid_size_t v) { D = v; }

  void PID::setPID(pid_size_t p, pid_size_t i, pid_size_t d) {
    setP(p); setI(i), setD(d);
  }

  PID::pid_size_t PID::_p() { return m_p * gP; }
  PID::pid_size_t PID::_i() { return m_i * gI; }
  PID::pid_size_t PID::_d() { return m_d * gD; }

}
