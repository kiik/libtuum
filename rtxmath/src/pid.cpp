/** @file pid.c
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @date May 2017
 */

#include "pid.hpp"

namespace tuum {

  void pid_init(struct pid_t* pid) {
    pid->kP = 0; pid->kI = 0; pid->kD = 0;
    pid->min = 0; pid->max = 0; pid->SV = 0;

    pid->out = 0;
    pid->deltaTimeStep = 20;

    pid->_t = millis();
    pid->_I = 0; pid->_lastProcessValue = 0; pid->_err = 0;
    pid->_direction = PID_DIRECT;
  }

  void pid_set_tuning(double kp, double ki, double kd, struct pid_t* pid) {
    if(pid->kP < 0 || pid->kI < 0 || pid->kD < 0) return;

    double dT_sec = ((double)pid->deltaTimeStep) / 1000.0;

    pid->kP = kp;
    pid->kI = ki * dT_sec;
    pid->kD = kd * dT_sec;

    if(pid->_direction == PID_REVERSE) {
      pid->kP = (0 - pid->kP);
      pid->kI = (0 - pid->kI);
      pid->kD = (0 - pid->kD);
    }
  }

  void pid_set_limit(double mn, double mx, struct pid_t* pid) {
    if(mn > mx) return;

    pid->min = mn;
    pid->max = mx;

    if(pid->out > mx) pid->out = mx;
    else if(pid->out < mn) pid->out = mn;
  }

  void pid_set_period_ms(time_ms_t dt, struct pid_t* pid) {
    if(dt <= 0) return;

    double scale = (double)dt / (double)pid->deltaTimeStep;

    pid->kI *= scale;
    pid->kD /= scale;

    pid->deltaTimeStep = dt;
  }

  void pid_tick(double pv, struct pid_t* pid)
  {
    time_ms_t t = millis();
    time_ms_t dt = t - pid->_t;

    if(dt < pid->deltaTimeStep) return;

    double err = pid->SV - pv;

    pid->_I += pid->kI * err;
    if(pid->_I > pid->max) pid->_I = pid->max;
    else if(pid->_I < pid->min) pid->_I = pid->min;

    double delta_pv = pv - pid->_lastProcessValue;

    pid->out = pid->kP * err + pid->_I - pid->kD * delta_pv;
    if(pid->out > pid->max) pid->out = pid->max;
    else if(pid->out < pid->min) pid->out = pid->min;

    pid->_lastProcessValue = pv;
    pid->_t = t;
  }

  void pid_vel_tick(double pv, struct pid_t* pid)
  {
    time_ms_t t = millis();
    time_ms_t dt = t - pid->_t;

    if(dt < pid->deltaTimeStep) return;

    double err = pid->SV - pv, delta_err = err - pid->_err, d2_err = delta_err - pid->_derr;

    pid->_I = pid->kI * err;
    if(pid->_I > pid->max) pid->_I = pid->max;
    else if(pid->_I < pid->min) pid->_I = pid->min;

    pid->_P = pid->kP * delta_err;
    pid->_D = -1 * pid->kD * d2_err;
    pid->out = pid->_lastProcessValue + pid->_P + pid->_I + pid->_D;

    if(pid->out > pid->max) pid->out = pid->max;
    else if(pid->out < pid->min) pid->out = pid->min;

    pid->_lastProcessValue = pv;
    pid->_t = t;
    pid->_err = err;
    pid->_derr = delta_err;
  }

  struct pid_t pid_create(double kp, double ki, double kd, double mn, double mx)
  {
    struct pid_t pid;

    pid_init(&pid);
    pid.kP = kp; pid.kI = ki; pid.kD = kd;

    pid_set_limit(mn, mx, &pid);
    pid_set_period_ms(20, &pid);

    return pid;
  }

  const struct libPID_t libPID = {
    .init = pid_init,
    .set_tuning = pid_set_tuning,
    .set_limit = pid_set_limit,
    .set_period_ms = pid_set_period_ms,

    .NewPID = pid_create,

    .pid_tick = pid_tick,
    .pid_vel_tick = pid_vel_tick
  };

}
