#ifndef _CONNECTIONS_H_
#define _CONNECTIONS_H_

#include <Arduino.h>

// Arduino Mega configuration.
#if defined(__AVR_ATmega2560__)

// Proximity sensor.
constexpr int kSensorPin = A0;

// Motor PWM pins.
constexpr int kPinMotorBottomPwm = 13;
constexpr int kPinMotorLeftPwm = 12;
constexpr int kPinMotorRightPwm = 11;
constexpr int kPinMotorFeedPwm = 10;

// Motor direction pins.
constexpr int kPinMotorBottomDir = 5;
constexpr int kPinMotorLeftDir = 4;
constexpr int kPinMotorRightDir = 3;
constexpr int kPinMotorFeedDir = 2;

// Stepper L/R.
constexpr int kStepperLR0 = 52;
constexpr int kStepperLR1 = 48;
constexpr int kStepperLR2 = 50;
constexpr int kStepperLR3 = 46;

// Stepper U/D.
constexpr int kStepperUD0 = 44;
constexpr int kStepperUD1 = 40;
constexpr int kStepperUD2 = 42;
constexpr int kStepperUD3 = 38;

constexpr int kForwardDir = LOW;
constexpr int kBackwardDir = HIGH;
constexpr int kMotorFeedDir = kForwardDir;
constexpr int kMotorFeedJamDir = kBackwardDir;

#define BluetoothSerial Serial1
#define DebugLog Serial.print
#define InitDebug(x) Serial.begin(9600)

// Arduino Uno configuration.
#elif defined(__AVR_ATmega328P__)

// Proximity sensor.
constexpr int kSensorPin = A0;

// Motor PWM pins.
constexpr int kPinMotorBottomPwm = 6;
constexpr int kPinMotorLeftPwm = 9;
constexpr int kPinMotorRightPwm = 3;
constexpr int kPinMotorFeedPwm = 5;

// Motor direction pins.
constexpr int kPinMotorBottomDir = 7;
constexpr int kPinMotorLeftDir = 8;
constexpr int kPinMotorRightDir = 2;
constexpr int kPinMotorFeedDir = 4;

// Stepper L/R.
constexpr int kStepperLR0 = A4;
constexpr int kStepperLR1 = A2;
constexpr int kStepperLR2 = A3;
constexpr int kStepperLR3 = A1;

// Stepper U/D.
constexpr int kStepperUD0 = 13;
constexpr int kStepperUD1 = 11;
constexpr int kStepperUD2 = 12;
constexpr int kStepperUD3 = 10;

constexpr int kForwardDir = LOW;
constexpr int kBackwardDir = HIGH;
constexpr int kMotorFeedDir = kBackwardDir;
constexpr int kMotorFeedJamDir = kForwardDir;

#define BluetoothSerial Serial
#define DebugLog
#define InitDebug(x)

#else
  static_assert(false, "Unsupported chip");
#endif

#endif
