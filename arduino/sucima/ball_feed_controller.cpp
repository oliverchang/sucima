#include <Arduino.h>

#include "ball_feed_controller.h"
#include "connections.h"

namespace {

constexpr int kSensorThreshold = 300;
constexpr int kMinimumMillis = 200;

}

BallFeedController g_ball_feed_controller;

void BallFeedController::FeedOne() {
  analogWrite(kPinMotorFeedPwm, 140);
  last_millis_ = millis();
}

void BallFeedController::Loop() {
  if (analogRead(kSensorPin) < kSensorThreshold &&
      millis() - last_millis_ > kMinimumMillis) {
    analogWrite(kPinMotorFeedPwm, 0);
    ball_was_fed_ = true;
  } else {
    ball_was_fed_ = false;
  }
}

bool BallFeedController::BallWasFed() {
  return ball_was_fed_;
}
