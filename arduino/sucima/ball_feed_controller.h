#ifndef _BALL_FEED_CONTROLLER_H_
#define _BALL_FEED_CONTROLLER_H_

class BallFeedController {
 public:
  void FeedOne();
  void Stop();
  void Loop();
  bool BallWasFed();
  bool IsJammed();

 private:
  enum State {
    IDLE,
    FEEDING,
  };
  unsigned long last_millis_ = 0;
  bool ball_was_fed_ = false;
  bool jammed_ = false;
  State state_ = IDLE;
};

extern BallFeedController g_ball_feed_controller;

#endif
