/** @file tuum_agent.hpp
 *  @brief Autonomous agent subsystem. Hardware control management middleware.
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @version 0.1
 *  @date 18. June 2017
 */

#ifndef TUUM_AGENT_H
#define TUUM_AGENT_H

#include "tuum_navigator.hpp"

#include "tuum_system.hpp"

namespace tuum {

  class Navigator;

  class Agent : public Subsystem
  {
  public:
    static Subsystem::TypeVar Type;
    static Subsystem::Type GetType() { return &Agent::Type; }

  public:
    Agent();

    void setup();
    void process();

    Subsystem::Type getType() { return Agent::GetType(); }

    // Motion vector relative to robot, unit is meters.
    int move(Vec2d);     // Manual call target
    int move_ai(Navigator::MotionDelta_t);  // Autonomous call target

    int start_ai();
    void stop_ai();

  protected:
    bool m_init, m_ai_en;
    soft_clk_t m_ai_log_clk;

    Navigator *gNav;
  };

}

#endif
