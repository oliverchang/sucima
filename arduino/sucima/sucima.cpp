#include <Arduino.h>
#include <stdint.h>

#include "sucima.h"
#include "bluetooth.h"
#include "command_processor.h"
#include "connections.h"
#include "drill_controller.h"
#include "head_controller.h"
#include "spin_math.h"

namespace  {

void FirstTimeSetupBluetooth() {
  DebugLog("first time bluetooth setup.\n");
  char buf[16];
  g_bluetooth.Send("AT+RENEW");
  g_bluetooth.ReceiveAll(8, buf);
  if (strncmp(buf, "OK+RENEW", 8) != 0) {
    DebugLog("failed RENEW.\n");
    return;
  }

  delay(1000);

  g_bluetooth.Send("AT+NOTI0");
  g_bluetooth.ReceiveAll(8, buf);
  if (strncmp(buf, "OK+Set:0", 8) != 0) {
    DebugLog("failed NOTI0\n");
    return;
  }

  delay(100);

  g_bluetooth.Send("AT+NAMEsucima");
  g_bluetooth.ReceiveAll(13, buf);
  if (strncmp(buf, "OK+Set:Sucima", 13) != 0) {
    DebugLog("failed NAME\n");
    return;
  }

  delay(100);

  g_bluetooth.Send("AT+MODE0");
  g_bluetooth.ReceiveAll(8, buf);
  if (strncmp(buf, "OK+Set:0", 8) != 0) {
    DebugLog("failed MODE.\n");
    return;
  }

  delay(100);

  g_bluetooth.Send("AT+RESET");
  g_bluetooth.ReceiveAll(8, buf);
  if (strncmp(buf, "OK+RESET", 8) != 0) {
    DebugLog("failed RESET.\n");
    return;
  }

  DebugLog("Initial bluetooth setup complete.\n");
}

void HandleSetDrill(const char* payload, size_t size) {
  if (size < sizeof(Ball))
    return;

  g_drill_controller.SetDrillLength(size / sizeof(Ball));
  for (size_t i = 0; i < size; i += sizeof(Ball)) {
    g_drill_controller.SetDrill(
        i / sizeof(Ball), *reinterpret_cast<const Ball*>(payload + i));
  }
}

void HandleSample(const char* payload, size_t size) {
  if (size < sizeof(Ball))
    return;

  g_drill_controller.SampleBall(*reinterpret_cast<const Ball*>(payload));
}

void HandleBallsPerMin(const char* payload, size_t size) {
  if (size < 2)
    return;

  g_drill_controller.SetRandom(payload[0]);
  g_drill_controller.SetBallsPerMinute(payload[1]);
}

void HandleCommand(const Command& cmd) {
  switch (cmd.type) {
  case SET:
    HandleSetDrill(cmd.payload, cmd.size);
    break;
  case SMPL:
    HandleSample(cmd.payload, cmd.size);
    break;
  case SBPM:
    HandleBallsPerMin(cmd.payload, cmd.size);
    break;
  default:
    break;
  }
}

Command g_cmd;
CommandProcessor g_cmd_processor;

}

void DoSetup() {
  pinMode(kPinMotorBottomPwm, OUTPUT);
  pinMode(kPinMotorLeftPwm, OUTPUT);
  pinMode(kPinMotorRightPwm, OUTPUT);
  pinMode(kPinMotorFeedPwm, OUTPUT);
  pinMode(kPinMotorBottomDir, OUTPUT);
  pinMode(kPinMotorLeftDir, OUTPUT);
  pinMode(kPinMotorRightDir, OUTPUT);
  pinMode(kPinMotorFeedDir, OUTPUT);
  pinMode(A0, INPUT_PULLUP);

  analogWrite(kPinMotorBottomPwm, 0);
  analogWrite(kPinMotorLeftPwm, 0);
  analogWrite(kPinMotorRightPwm, 0);
  analogWrite(kPinMotorFeedPwm, 0);

  digitalWrite(kPinMotorBottomDir, kForwardDir);
  digitalWrite(kPinMotorLeftDir, kForwardDir);
  digitalWrite(kPinMotorRightDir, kForwardDir);
  digitalWrite(kPinMotorFeedDir, kMotorFeedDir);

  InitDebug();
  g_bluetooth.Init();
  g_bluetooth.Send("AT");

  //FirstTimeSetupBluetooth();
  g_head_controller.Calibrate();

}

void DoLoop() {
  if (g_cmd_processor.Loop(&g_cmd)) {
    HandleCommand(g_cmd);
  }
  g_drill_controller.Loop();
}
