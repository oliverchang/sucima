#ifndef _WHEEL_CONTROLLER_H_
#define _WHEEL_CONTROLLER_H_

class WheelController {
 public:
  void SetConfiguration(int spin_angle, int spin_strength, int speed);
};

extern WheelController g_wheel_controller;

#endif
