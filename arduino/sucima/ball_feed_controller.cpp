#include <Arduino.h>

#include "ball_feed_controller.h"
#include "connections.h"

namespace {

constexpr int kMotorPwm = 180;
constexpr int kSensorThreshold = 150;
constexpr int kMinimumMillis = 200;
constexpr int kFeedDeadline = 1000;
constexpr int kReverseTime = 500;

}

BallFeedController g_ball_feed_controller;

void BallFeedController::FeedOne() {
  state_ = FEEDING;
  last_millis_ = millis();
  digitalWrite(kPinMotorFeedDir, kMotorFeedDir);
  analogWrite(kPinMotorFeedPwm, kMotorPwm);
}

void BallFeedController::Stop() {
  state_ = IDLE;
  analogWrite(kPinMotorFeedPwm, 0);
}

void BallFeedController::Loop() {
  switch (state_) {
  case FEEDING:
    jammed_ = false;
    ball_was_fed_ = false;

    if (analogRead(kSensorPin) < kSensorThreshold &&
        millis() - last_millis_ > kMinimumMillis) {
      Stop();
      ball_was_fed_ = true;
    } else if (millis() - last_millis_ >= kFeedDeadline) {
      // Ball jam detected.
      jammed_ = true;
    }
    break;
  default:
    break;
  }
}

void BallFeedController::ReverseJam() {
  digitalWrite(kPinMotorFeedDir, kMotorFeedJamDir);
  analogWrite(kPinMotorFeedPwm, kMotorPwm);
}

bool BallFeedController::BallWasFed() {
  return ball_was_fed_;
}

bool BallFeedController::IsJammed() {
  return jammed_;
}
