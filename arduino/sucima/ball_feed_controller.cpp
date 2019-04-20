#include <Arduino.h>

#include "ball_feed_controller.h"
#include "connections.h"

namespace {

constexpr int kMotorPwm = 200;
constexpr int kSensorThreshold = 300;
constexpr int kMinimumMillis = 200;
constexpr int kFeedDeadline = 1000;
constexpr int kReverseTime = 500;

}

BallFeedController g_ball_feed_controller;

void BallFeedController::FeedOne() {
  state_ = kFeeding;
  last_millis_ = millis();
  analogWrite(kPinMotorFeedPwm, kMotorPwm);
}

void BallFeedController::Loop() {
  switch (state_) {
  case kJammed:
    if (millis() - last_millis_ >= kReverseTime) {
      FeedOne();
    }
    break;
  case kFeeding:
    if (analogRead(kSensorPin) < kSensorThreshold &&
        millis() - last_millis_ > kMinimumMillis) {
      state_ = kIdle;
      analogWrite(kPinMotorFeedPwm, 0);
      ball_was_fed_ = true;
    } else if (millis() - last_millis_ >= kFeedDeadline) {
      // Ball jam detected.
      state_ = kJammed;
      last_millis_ = millis();
      analogWrite(kPinMotorFeedDir, kMotorFeedJamDir);
    }
    break;
  default:
    ball_was_fed_ = false;
    break;
  }
}

bool BallFeedController::BallWasFed() {
  return ball_was_fed_;
}
