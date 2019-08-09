#include <Arduino.h>
#include <string.h>

#include "ball_feed_controller.h"
#include "connections.h"
#include "drill_controller.h"
#include "head_controller.h"
#include "wheel_controller.h"

DrillController g_drill_controller;

namespace {

constexpr int kSampleWheelSpinTime = 1500;
constexpr int kMaxJamCount = 4;
constexpr int kJamReverseTime = 500;

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

}

DrillController::DrillController() {
  ResetDrillOrder();
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

void DrillController::SetRandom(bool random) {
  random_ = random;
  if (random) {
    ShuffleDrill();
  } else {
    ResetDrillOrder();
  }
}

void DrillController::ResetDrillOrder() {
  for (size_t i = 0; i < kMaxDrillLength; ++i) {
    indexes_[i] = i;
  }
}

void DrillController::ShuffleDrill() {
  ResetDrillOrder();

  for (size_t i = 0; i < drill_length_ - 1; ++i) {
    size_t new_i = random(i, drill_length_);
    size_t tmp = indexes_[i];
    indexes_[i] = indexes_[new_i];
    indexes_[new_i] = tmp;
  }
}

void DrillController::SetBallsPerMinute(int balls_per_min) {
  DebugLog("Setting balls per min to ");
  DebugLog(balls_per_min);
  DebugLog("\n");
  balls_per_min_ = balls_per_min;

  if (balls_per_min_) {
    state_ = DRILL;
    cur_ = 0;
    next_ball_time_ = NextBallTime();
    PrepareBall(CurBall());
  } else {
    state_ = DRILL_STOPPING;
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
      jam_count_ = 0;
      state_ = SAMPLE_FED;
      next_ball_time_ = NextBallTime();
    } else if (g_ball_feed_controller.IsJammed()) {
      HandleJam();
      return;
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
      jam_count_ = 0;
      PrepareBall(CurBall());
    } else if (g_ball_feed_controller.IsJammed()) {
      HandleJam();
      return;
    }

    if (millis() < next_ball_time_)
      return;

    g_ball_feed_controller.FeedOne();
    next_ball_time_ = NextBallTime();

    ++cur_;
    if (cur_ >= drill_length_) {
      if (random_) {
        ShuffleDrill();
      }
      cur_ = 0;
    }

    break;
  case DRILL_STOPPING:
    // Prevent jams by making sure the last ball is played.
    if (millis() >= next_ball_time_) {
      state_ = NONE;
      g_wheel_controller.SetConfiguration(0, 0, 0);
    }
    break;
  case JAMMED:
    if (millis() < next_ball_time_)
      return;

    // Hopefully the jam was resolved. Try playing the ball again.
    state_ = pre_jam_state_;
    PrepareBall(being_played_);
    g_ball_feed_controller.FeedOne();
    next_ball_time_ = NextBallTime();
    break;
  case NONE:
  default:
    break;
  }
}

void DrillController::PrepareBall(const Ball& ball) {
  being_played_ = ball;
  g_head_controller.SetOrientation(ball.position(), ball.trajectory());
  g_wheel_controller.SetConfiguration(
      ball.spin_angle(), ball.spin_strength(), ball.speed());
}

unsigned long DrillController::NextBallTime() {
  switch (state_) {
  case DRILL:
    return millis() + 60000 / balls_per_min_;
  case SAMPLE_FED:
    return millis() + kSampleWheelSpinTime;
  default:
    // Unreached.
    abort();
    break;
  }
}

void DrillController::HandleJam() {
  ++jam_count_;
  if (jam_count_ >= kMaxJamCount) {
    // Jammed too many times, bail out.
    state_ = NONE;
    g_wheel_controller.Stop();
    g_ball_feed_controller.Stop();
    return;
  }

  pre_jam_state_ = state_;
  state_ = JAMMED;

  // Reverse motor wheels and ball feed to try to unjam.
  g_wheel_controller.ReverseJam();
  g_ball_feed_controller.ReverseJam();

  next_ball_time_ = millis() + kJamReverseTime;
}

Ball& DrillController::CurBall() {
  return drill_[indexes_[cur_]];
}
