#include <Arduino.h>

#include "command_processor.h"
#include "ball.h"
#include "bluetooth.h"
#include "connections.h"

bool CommandProcessor::Loop(Command* out) {
  switch (state_) {
  case WAITING:
    return HandleWaiting(out);
  case IN_COMMAND:
    return HandleInCommand(out);
  default:
    return false;
  }
}

bool CommandProcessor::HandleWaiting(Command* out) {
  cur_ += g_bluetooth.Receive(kCommandSize - cur_, buf_ + cur_);
  if (cur_ < kCommandSize)
    return false;

  state_ = IN_COMMAND;
  cur_ = 0;

  if (memcmp(buf_, "SET", 3) == 0) {
    expecting_ = sizeof(Ball) * buf_[3];
    out->type = SET;
  } else if (memcmp(buf_, "SMPL", kCommandSize) == 0) {
    expecting_ = sizeof(Ball);
    out->type = SMPL;
  } else if (memcmp(buf_, "SBPM", kCommandSize) == 0) {
    expecting_ = 1;
    out->type = SBPM;
  } else {
    for (int i = 0; i < kCommandSize; i++) {
      DebugLog(buf_[i], HEX);
      DebugLog(' ');
    }
    DebugLog("\n");

    state_ = WAITING;
  }

  return false;
}

bool CommandProcessor::HandleInCommand(Command* out) {
  cur_ += g_bluetooth.Receive(expecting_ - cur_, buf_ + cur_);
  if (cur_ < expecting_)
    return false;

  out->payload = buf_;
  out->size = cur_;

  state_ = WAITING;
  cur_ = 0;
  return true;
}
