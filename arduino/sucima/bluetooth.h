#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <stddef.h>

// HM-17 BLE module controller.
class BluetoothModule {
 public:
  void Init();
  void Send(const char* data);
  size_t Receive(size_t n, char* output);
  void ReceiveAll(size_t n, char* output);
  size_t ReceiveUntil(char delimiter, size_t n, char* output);
};

extern BluetoothModule g_bluetooth;

#endif
