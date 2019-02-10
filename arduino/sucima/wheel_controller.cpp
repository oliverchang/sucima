#include <Arduino.h>

#include "connections.h"
#include "spin_math.h"
#include "wheel_controller.h"

WheelController g_wheel_controller;

void WheelController::SetConfiguration(int spin_angle, int spin_strength, int speed) {
  DebugLog("Setting wheel configuration spin_angle=");
  DebugLog(spin_angle);
  DebugLog(", spin_strength=");
  DebugLog(spin_strength);
  DebugLog(", speed=");
  DebugLog(speed);
  DebugLog("\n");

  WheelSpeeds speeds = GetWheelSpeeds(spin_angle, spin_strength, speed);
  digitalWrite(kPinMotorBottomDir, speeds.bottom < 0 ? kForwardDir : kBackwardDir);
  analogWrite(kPinMotorBottomPwm, abs(speeds.bottom));
  digitalWrite(kPinMotorLeftDir, speeds.left < 0 ? kForwardDir : kBackwardDir);
  analogWrite(kPinMotorLeftPwm, abs(speeds.left));
  digitalWrite(kPinMotorRightDir, speeds.right < 0 ? kForwardDir : kBackwardDir);
  analogWrite(kPinMotorRightPwm, abs(speeds.right));
}
