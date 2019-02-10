#include "bluetooth.h"
#include <Arduino.h>

#include "connections.h"

constexpr int kMaxProcessBytes = 16;

BluetoothModule g_bluetooth;

void BluetoothModule::Init() {
  BluetoothSerial.begin(9600);
}

void BluetoothModule::Send(const char* data) {
  BluetoothSerial.print(data);
}

size_t BluetoothModule::Receive(size_t n, char* data) {
  for (size_t i = 0; i < n; ++i) {
    if (!BluetoothSerial.available()) {
      return i;
    }

    data[i] = BluetoothSerial.read();
  }

  return n;
}

void BluetoothModule::ReceiveAll(size_t n, char* data) {
  size_t i = 0;
  while (i < n) {
    i += Receive(n - i, data + i);
  }
}

size_t BluetoothModule::ReceiveUntil(char delimiter, size_t n, char* data) {
  size_t i = 0;
  while (i < n) {
    i += Receive(1, data + i);
    if (i && data[i - 1] == delimiter)
      return i;
  }

  return n;
}
