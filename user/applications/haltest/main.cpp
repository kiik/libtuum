
#include <iostream>

#include "application.hpp"

#include "MotorDriver.hpp"
#include "MotionControl.hpp"

#include "hal.hpp"


int main(){
  printf("Tuum hardware abstraction layer test application.\n");

  //const char* dev = "/dev/ttyUSB0";
  //tuum::MotionControl* test = new tuum::MotionControl(dev, 115200);
  //test->forward(19);
  //test->turn(180);
  tuum::hal::setup();
  //tuum::hal::hw.getMotionControl()->testSequence();

  bool running = true;
  while(running) {
    tuum::hal::process();
  }

  return 0;
}
