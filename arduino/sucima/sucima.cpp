#include "sucima.h"

#include <Arduino.h>

void do_setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.print("AT");
}

void do_loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    char value = Serial1.read();
    if (!value)
      return;

     Serial.print(value);
  }
}
