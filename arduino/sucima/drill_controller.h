#ifndef _DRILL_CONTROLLER_H_
#define _DRILL_CONTROLLER_H_

#include <stdint.h>

#include "Ball.h"

class DrillController {
 public:
  void SetDrillLength(size_t length);
  void SetDrill(size_t i, const Ball& ball);
  void Randomise();
  void SetBallsPerMinute(int balls_per_min);
  void SampleBall(const Ball& ball);
  void Loop();

 private:
  static constexpr int kMaxDrillLength = 16;
  static constexpr int kMaxJamCount = 4;

  enum State {
    NONE,
    SAMPLE,
    SAMPLE_FED,
    DRILL,
    DRILL_STOPPING,
    JAMMED,
  };

  void PrepareBall(const Ball& ball);
  unsigned long NextBallTime();
  void HandleJam();

  Ball being_played_;
  Ball drill_[kMaxDrillLength];
  size_t drill_length_ = 0;
  size_t cur_ = 0;
  unsigned long next_ball_time_ = 0;
  bool random_ = false;
  int balls_per_min_ = 0;
  State state_ = NONE;
  State pre_jam_state_ = NONE;
  int jam_count_ = 0;
};

extern DrillController g_drill_controller;

#endif
