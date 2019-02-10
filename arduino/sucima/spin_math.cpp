#include "spin_math.h"
#include "connections.h"

#include <math.h>

namespace {

constexpr float kPi = 3.14159265358979323846f;

// Top two rows are components of the 3 wheel vectors (bottom, left, right).
//   bottom + left + right = result
// Last row is |bottom| + |left| + |right| = total_speed
//
// 0               sqrt(3)/2         -sqrt(3)/2
// 1              -0.5              -0.5
// 1               1                 1
//
// Matrix inverse is:
// is
//
// 0               2/3               1/3
//  sqrt(3)/3     -1/3               1/3
// -sqrt(3)/3     -1/3               1/3
const float solution[3][3] = {
  {0, 2.0f/3, 1.0f/3},
  {0.57735f, -1.0f/3, 1.0f/3},
  {-0.57735f, -1.0f/3, 1.0f/3},
};

float ToRadians(float degrees) {
  return degrees * kPi / 180.0f;
}

int NormalizeAngle(int angle) {
  return (angle + 360) % 360;
}

}

WheelSpeeds GetWheelSpeeds(int spin_angle, int spin_strength, int speed) {
  WheelSpeeds result;
  if (speed == 0)
    return result;

  int total_speed = kMinSpeed + speed * (kMaxSpeed - kMinSpeed) / kMaxSpeedSetting;
  float spin_mult = static_cast<float>(spin_strength) / kMaxSpinSetting;
  float spin_magnitude = total_speed * spin_mult;

  // Flip angle to get direction of spin.
  spin_angle = NormalizeAngle(spin_angle + 180);
  // Convert into CCW angle starting from positive X.
  spin_angle = NormalizeAngle(360 - spin_angle + 90);

  float target_x = spin_magnitude * cos(ToRadians(spin_angle));
  float target_y = spin_magnitude * sin(ToRadians(spin_angle));

  result.bottom =
    target_x * solution[0][0] +
    target_y * solution[0][1] +
    total_speed * solution[0][2];

  result.left =
    target_x * solution[1][0] +
    target_y * solution[1][1] +
    total_speed * solution[1][2];

  result.right =
    target_x * solution[2][0] +
    target_y * solution[2][1] +
    total_speed * solution[2][2];

  return result;
}
