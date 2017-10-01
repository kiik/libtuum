/** @file tuum_agent.cpp
 *  @brief Autonomous agent subsystem. Hardware control management middleware.
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @version 0.1
 *  @date 18. June 2017
 */

#include <boost/bind.hpp>

#include "tuum_hal.hpp"

#include "tuum_agent.hpp"

namespace tuum {

  Subsystem::TypeVar Agent::Type;

  Agent::Agent():
    gNav(nullptr),
    m_ai_en(false),
    m_ai_log_clk(1000)
  {

  }

  void Agent::setup()
  {
    int err_flag = 0;

    if(getSubsystemHandle<tuum::Navigator*>(Navigator::GetType(), gNav) > 0) {
      RTXLOG("Navigator present.");

      gNav->setMotionHandler(boost::bind(&Agent::move_ai, this, _1));
    } else err_flag = -1;

    if(err_flag < 0) {
      RTXLOG(tuum::format("Error - Missing module. (code=%i)", err_flag), LOG_ERR);
    } else {
      m_init = true;
    }
  }

  void Agent::process()
  {

  }

  int Agent::move(Vec2d mvec)
  {
    if(m_ai_en) stop_ai();  //TODO: If conflict enter into some emergency state?

    return -1;
  }

  int Agent::move_ai(Navigator::mvec_t mvec)
  {
    if(!m_ai_en) {
      stop_ai();
      return 0;
    }

    hal::hw.getMotionControl()->aiMove(mvec);

    return 1;
  }

  int Agent::start_ai()
  {
    m_ai_en = true;
    return 1;
  }

  void Agent::stop_ai()
  {
    m_ai_en = false;
    //TODO: hardware stop
  }

}
