/** @file platform.hpp
 *  General computer-platform related functionalities.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 27. November 2015
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include <unistd.h>
#include <stdint.h>

#include "platform.t.hpp"

typedef size_t time_ms_t;
typedef long long time_us_t;

time_ms_t millis();
time_us_t micros();


struct Timer {
  unsigned long int _start = 0;
  unsigned long int period;
  unsigned long int _end = 0;

  void start(int = 0) {
    _start = millis();
    _end = _start + period;
  }

  void start(size_t T) {
    _start = millis();
    _end = _start + T;
  }

  bool isTime() {
    return millis() >= _end;
  }

  void setPeriod(uint32_t T) {
    period = T;
  }

  void trigger() {
    _end = millis();
  }

};

struct soft_clk_t {
  time_ms_t T, _t;

  soft_clk_t():
    T(0), _t(0)
  {

  }

  soft_clk_t(time_ms_t period)
  {
    init(period);
  }

  void init(time_ms_t period)
  {
    T = period;
    _t = millis();
  }

  bool tick(time_ms_t t = 0)
  {
    if(t == 0) t = millis();

    if(t >= _t) {
      _t = t + T;
      return true;
    }

    return false;
  }

  void wait(time_ms_t t = 0)
  {
    if(t == 0) t = millis();
    if(_t <= t) return;

    time_ms_t dt = _t - t;

    if(dt > 1) {
      usleep(dt * 1000);
    }
  }

};

#include "tuum_logger.hpp"

#endif
