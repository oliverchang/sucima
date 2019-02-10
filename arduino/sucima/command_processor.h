#ifndef _COMMAND_PROCESSOR_H_
#define _COMMAND_PROCESSOR_H_

enum Type {
  SET,
  SMPL,
  SBPM,
};

struct Command {
  Type type;
  const char* payload;
  size_t size;
};

class CommandProcessor {
 public:
  // Note: The command payload from a Loop call is invalidated after subsequent
  // Loop calls.
  bool Loop(Command* out);

 private:
  static constexpr size_t kCommandSize = 4;
  static constexpr size_t kBufSize = 128;

  enum State {
    WAITING,
    IN_COMMAND,
  };

  bool HandleWaiting(Command* out);
  bool HandleInCommand(Command* out);

  char buf_[kBufSize];
  size_t cur_ = 0;
  size_t expecting_ = 0;
  State state_ = WAITING;
};

#endif
