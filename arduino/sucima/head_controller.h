#ifndef _HEAD_CONTROLLER_H_
#define _HEAD_CONTROLLER_H_

#include <Stepper.h>

class HeadController {
 public:
  HeadController();
  void Calibrate();
  void StepTrajectory(int steps);
  void StepLeftRight(int steps);
  void SetOrientation(int8_t position, uint8_t trajectory);

 private:
  Stepper stepper_traj_; 
  Stepper stepper_lr_; 
  int stepper_lr_pos_ = 0;
  int stepper_traj_pos_ = 0;
};

extern HeadController g_head_controller;

#endif
