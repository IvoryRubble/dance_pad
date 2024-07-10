#include <Arduino.h>

bool isSerialPortOpened() {
  return Serial.availableForWrite() >= 32;
}

