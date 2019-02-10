#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../sucima/spin_math.h"

void TestGetWheelSpeeds(int spin_angle, int spin_strength, int speed,
                        int bottom, int left, int right) {
  printf("Testing spin_angle=%d, spin_strength=%d, speed=%d\n",
         spin_angle, spin_strength, speed);
  WheelSpeeds speeds = GetWheelSpeeds(spin_angle, spin_strength, speed);

  bool failed = false;
  if (speeds.bottom != bottom) {
    printf("expected bottom=%d, got %d\n", bottom, speeds.bottom);
    failed = true;
  }

  if (speeds.left != left) {
    printf("expected left=%d, got %d\n", left, speeds.left);
    failed = true;
  }

  if (speeds.right != right) {
    printf("expected right=%d, got %d\n", right, speeds.right);
    failed = true;
  }

  if (failed)
    abort();
}

int main() {
  // Varying backspin.
  TestGetWheelSpeeds(180, 20, 20, 255, 0, 0);
  TestGetWheelSpeeds(180, 20, 10, 152, 0, 0);
  TestGetWheelSpeeds(180, 10, 20, 170, 42, 42);
  TestGetWheelSpeeds(180, 10, 10, 101, 25, 25);
  TestGetWheelSpeeds(180, 5, 20, 127, 63, 63);
  TestGetWheelSpeeds(180, 1, 20, 93, 80, 80);
  TestGetWheelSpeeds(180, 1, 1, 22, 19, 19);
  TestGetWheelSpeeds(180, 0, 20, 85, 85, 85);

  // Varying topspin
  TestGetWheelSpeeds(0, 20, 20, -85, 170, 170);
  TestGetWheelSpeeds(0, 20, 10, -50, 101, 101);
  TestGetWheelSpeeds(0, 10, 20, 0, 127, 127);
  TestGetWheelSpeeds(0, 10, 10, 0, 76, 76);
  TestGetWheelSpeeds(0, 5, 20, 42, 106, 106);
  TestGetWheelSpeeds(0, 1, 20, 76, 89, 89);
  TestGetWheelSpeeds(0, 0, 20, 85, 85, 85);

  // Varying right sidespin
  TestGetWheelSpeeds(90, 20, 20, 84, -62, 232);
  TestGetWheelSpeeds(90, 20, 10, 50, -37, 138);
  TestGetWheelSpeeds(90, 10, 20, 84, 11, 158);
  TestGetWheelSpeeds(90, 10, 10, 50, 6, 94);
  TestGetWheelSpeeds(90, 5, 20, 85, 48, 121);
  TestGetWheelSpeeds(90, 1, 20, 85, 77, 92);
  TestGetWheelSpeeds(90, 0, 20, 85, 85, 85);

  // Varying left sidespin
  TestGetWheelSpeeds(270, 20, 20, 85, 232, -62);
  TestGetWheelSpeeds(270, 20, 10, 50, 138, -37);
  TestGetWheelSpeeds(270, 10, 20, 85, 158, 11);
  TestGetWheelSpeeds(270, 10, 10, 50, 94, 6);
  TestGetWheelSpeeds(270, 5, 20, 85, 121, 48);
  TestGetWheelSpeeds(270, 1, 20, 85, 92, 77);
  TestGetWheelSpeeds(270, 0, 20, 85, 85, 85);

  // Mixed spin
  // top right 45 degrees.
  TestGetWheelSpeeds(45, 20, 20, -35, 41, 249);
  TestGetWheelSpeeds(45, 10, 20, 24, 63, 167);

  // top right 60 degrees.
  TestGetWheelSpeeds(60, 20, 20, 0, 0, 254);
  TestGetWheelSpeeds(60, 10, 20, 42, 42, 169);

  // top left 45 degrees.
  TestGetWheelSpeeds(315, 20, 20, -35, 249, 41);
  TestGetWheelSpeeds(315, 10, 20, 24, 167, 63);

  // top left 60 degrees.
  TestGetWheelSpeeds(300, 20, 20, 0, 254, 0);
  TestGetWheelSpeeds(300, 10, 20, 42, 169, 42);

  // bottom right 45 degrees.
  TestGetWheelSpeeds(135, 20, 20, 205, -79, 128);
  TestGetWheelSpeeds(135, 10, 20, 145, 2, 106);

  // bottom left 45 degrees.
  TestGetWheelSpeeds(225, 20, 20, 205, 128, -79);
  TestGetWheelSpeeds(225, 10, 20, 145, 106, 2);

  // edge cases.
  TestGetWheelSpeeds(100, 20, 0, 0, 0, 0);
  TestGetWheelSpeeds(1, 20, 20, -84, 167, 172);
  TestGetWheelSpeeds(1, 1, 1, 18, 20, 21);
  TestGetWheelSpeeds(1, 20, 1, -19, 39, 40);
  TestGetWheelSpeeds(1, 1, 20, 76, 89, 89);

  printf("all tests passed\n");
  return 0;
}
