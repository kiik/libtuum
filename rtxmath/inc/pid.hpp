/** @file pid.h
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @date May 2017
 */

#ifndef HYDRO_PID_H
#define HYDRO_PID_H

#include "platform.hpp"

namespace tuum {

  #define PID_DIRECT 1
  #define PID_REVERSE 2

  struct pid_t {
    double kP, kI, kD;
    double min, max, SV;
    time_ms_t deltaTimeStep;

    time_ms_t _t;
    double _P, _I, _D;
    double _lastProcessValue, _err, _derr, out;
    int _direction;
  };


  struct libPID_t {
    void (*init)(struct pid_t*);
    void (*set_tuning)(double, double, double, struct pid_t*);
    void (*set_limit)(double, double, struct pid_t*);
    void (*set_period_ms)(time_ms_t, struct pid_t*);

    struct pid_t (*NewPID)(double, double, double, double, double);

    void (*pid_tick)(double, struct pid_t*);
    void (*pid_vel_tick)(double, struct pid_t*);
  };

  extern const struct libPID_t libPID;

}

#endif
