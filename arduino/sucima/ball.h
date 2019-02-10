#ifndef _BALL_H_
#define _BALL_H_

#include <stdint.h>

struct __attribute__((__packed__)) Ball {
  uint8_t spin_angle_;  // angle of the spin (divided by 2).
  uint8_t spin_strength_;  // strength of the spin.
  uint8_t trajectory_;  // trajectory.
  int8_t position_;  // left/right position.
  uint8_t speed_;  // speed.

  int spin_angle() const {
    return spin_angle_ * 2;
  }

  int spin_strength() const {
    return spin_strength_;
  }

  int trajectory() const {
    return trajectory_;
  }
  
  int position() const {
    return position_;
  }

  int speed() const {
    return speed_;
  }
};

#endif
