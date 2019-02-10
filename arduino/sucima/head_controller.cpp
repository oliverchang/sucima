#include <Arduino.h>
#include "head_controller.h"
#include "connections.h"

namespace {

static constexpr int kTrajectorySteps = 160;
static constexpr int kTrajectoryStepSize = 10;
static constexpr int kLeftRightSteps = 18;

}

HeadController g_head_controller;

HeadController::HeadController() :
    stepper_lr_(200, kStepperLR0, kStepperLR1, kStepperLR2, kStepperLR3),
    stepper_traj_(200, kStepperUD0, kStepperUD1, kStepperUD2, kStepperUD3) {
  stepper_lr_.setSpeed(20);
  stepper_traj_.setSpeed(40);
}

void HeadController::Calibrate() {
  DebugLog("Calibrating head...\n");
  for (int i = 0; i < kLeftRightSteps; i++) {
    StepLeftRight(-1);
    delay(100);
  }

  for (int i = 0; i < kTrajectorySteps / kTrajectoryStepSize; i++) {
    StepTrajectory(1);
    delay(100);
  }

  StepLeftRight(kLeftRightSteps / 2);

  stepper_lr_pos_ = 0;
  stepper_traj_pos_ = kTrajectorySteps / kTrajectoryStepSize;
  DebugLog("Done.\n");
}

void HeadController::SetOrientation(int8_t position, uint8_t trajectory) {
  DebugLog("Setting orientation position=");
  DebugLog(position);
  DebugLog(", trajectory=");
  DebugLog(trajectory);
  DebugLog("\n");

  StepLeftRight(position - stepper_lr_pos_); 
  StepTrajectory(trajectory - stepper_traj_pos_);
}

void HeadController::StepTrajectory(int steps) {
  stepper_traj_.step(steps * kTrajectoryStepSize);
  stepper_traj_pos_ += steps;
}

void HeadController::StepLeftRight(int steps) {
  stepper_lr_.step(-steps);
  stepper_lr_pos_ += steps;
}
