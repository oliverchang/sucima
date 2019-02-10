#include <Arduino.h>
#include <string.h>

#include "ball_feed_controller.h"
#include "connections.h"
#include "drill_controller.h"
#include "head_controller.h"
#include "wheel_controller.h"

DrillController g_drill_controller;

namespace {

constexpr int kSampleWheelSpinTime = 1000;

void PrintBall(const Ball& ball) {
  DebugLog("spin_angle=");
  DebugLog(ball.spin_angle());
  DebugLog(" spin_strength=");
  DebugLog(ball.spin_strength());
  DebugLog(" traj=");
  DebugLog(ball.trajectory());
  DebugLog(" pos=");
  DebugLog(ball.position());
  DebugLog(" speed=");
  DebugLog(ball.speed());
  DebugLog("\n");
}

unsigned long NextBallTime(int balls_per_min) {
  return millis() + 60000 / balls_per_min;
}

}

void DrillController::SetDrillLength(size_t length) {
  if (length >= kMaxDrillLength)
    length = kMaxDrillLength;

  drill_length_ = length;
}

void DrillController::SetDrill(size_t i, const Ball& ball) {
  if (i >= kMaxDrillLength)
    return;

  drill_[i] = ball;
  DebugLog("Setting drill ");
  DebugLog(i);
  DebugLog(" to ");
  PrintBall(ball);
}

void DrillController::SetBallsPerMinute(int balls_per_min) {
  DebugLog("Setting balls per min to ");
  DebugLog(balls_per_min);
  DebugLog("\n");
  balls_per_min_ = balls_per_min;
  next_ball_time_ = NextBallTime(balls_per_min_);

  if (balls_per_min_) {
    state_ = DRILL;
    cur_ = 0;
    PrepareBall(drill_[cur_]);
  } else {
    state_ = NONE;
    g_wheel_controller.SetConfiguration(0, 0, 0);
  }
}

void DrillController::SampleBall(const Ball& ball) {
  if (state_ != NONE)
    return;

  DebugLog("Sampling ");
  PrepareBall(ball);
  state_ = SAMPLE;
  g_ball_feed_controller.FeedOne();
  PrintBall(ball);
}

void DrillController::Loop() {
  g_ball_feed_controller.Loop();

  switch (state_) {
  case SAMPLE:
    if (g_ball_feed_controller.BallWasFed()) {
      state_ = SAMPLE_FED;
      next_ball_time_ = millis() + kSampleWheelSpinTime;
    }
    break;
  case SAMPLE_FED:
    if (millis() >= next_ball_time_) {
      state_ = NONE;
      g_wheel_controller.SetConfiguration(0, 0, 0);
    }
    break;
  case DRILL:
    if (!balls_per_min_)
      return;

    if (g_ball_feed_controller.BallWasFed()) {
      // Prepare the next ball.
      PrepareBall(drill_[cur_]);
    }

    if (millis() < next_ball_time_)
      return;

    g_ball_feed_controller.FeedOne();
    next_ball_time_ = NextBallTime(balls_per_min_);

    ++cur_;
    if (cur_ >= drill_length_)
      cur_ = 0;

    break;
  case NONE:
  default:
    break;
  }
}

void DrillController::PrepareBall(const Ball& ball) {
  g_head_controller.SetOrientation(ball.position(), ball.trajectory());
  g_wheel_controller.SetConfiguration(
      ball.spin_angle(), ball.spin_strength(), ball.speed());
}

