#ifndef _SPIN_MATH_H_
#define _SPIN_MATH_H_

// Angle between wheels.
constexpr int kWheelAngle = 120;
// Maximum PWM speed.
constexpr int kMinSpeed = 50;
constexpr int kMaxSpeed = 255;
constexpr int kMaxSpeedSetting = 20;
constexpr int kMaxSpinSetting = 20;

struct WheelSpeeds {
  int left = 0;
  int right = 0;
  int bottom = 0;
};

WheelSpeeds GetWheelSpeeds(int spin_angle, int spin_strength, int speed);

#endif
