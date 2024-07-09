#pragma once
#include <Arduino.h>
#include <Keyboard.h>
#include "buttonDebounce.h"

void setButtonRegular(int pin, ButtonDebounce state, char c) {
  if (state.isBtnPressed) {
    Keyboard.press(c);
  } else if (state.isBtnReleased) {
    Keyboard.release(c);
  }
}

void setButtonReverce(int pin, ButtonDebounce state, char c) {
  if (state.isBtnReleased) {
    Keyboard.press(c);
  } else if (state.isBtnPressed) {
    Keyboard.release(c);
  }
}

void setButtonTap(int pin, ButtonDebounce state, char c) {
  if (state.isBtnPressed) {
    Keyboard.write(c);
  }
}

void setButtonTapReverce(int pin, ButtonDebounce state, char c) {
  if (state.isBtnReleased) {
    Keyboard.write(c);
  }
}

void setButtonTapDouble(int pin, ButtonDebounce state, char c) {
  if (state.isBtnPressed || state.isBtnReleased) {
    Keyboard.write(c);
  }
}
