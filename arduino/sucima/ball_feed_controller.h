#ifndef _BALL_FEED_CONTROLLER_H_
#define _BALL_FEED_CONTROLLER_H_

class BallFeedController {
 public:
  void FeedOne();
  void Loop();
  bool BallWasFed();

 private:
  enum State {
    kIdle,
    kFeeding,
    kJammed,
  };
  unsigned long last_millis_ = 0;
  bool ball_was_fed_ = false;
  State state_ = kIdle;
};

extern BallFeedController g_ball_feed_controller;

#endif
