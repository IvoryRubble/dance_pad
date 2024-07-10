// pinout:
// 4 --   start
// 5 --   select
// 6 --   right
// 7 --   up
// 8 --   down
// 9 --   left
// A1 --  led
// A0 -   led gnd
// A3 -   switch

// two blinks on startup: gamepad mode
// three blinks on startup: keyboard mode

#include <Arduino.h>
#include <Joystick.h>
#include <Keyboard.h>

#include "blinker.h"
#include "buttonDebounce.h"
#include "serialHelpers.h"
#define LED A1
#define LED_GND A0

Blinker blinker;

const int buttonStartPin = 4;
const int buttonSelectPin = 5;
const int buttonRightPin = 6;
const int buttonUpPin = 7;
const int buttonDownPin = 8;
const int buttonLeftPin = 9;

const int inputGuardPin = 10;
bool isInputEnabled = true;
bool isButtonsReleased = false;

const int switchPin = A3;
bool switchState = false;

ButtonDebounce buttonStartState;
ButtonDebounce buttonSelectState;
ButtonDebounce buttonRightState;
ButtonDebounce buttonUpState;
ButtonDebounce buttonDownState;
ButtonDebounce buttonLeftState;

void updateStates();
void setButtons();
void setButton(int, ButtonDebounce, char);
void updateBlinker();
void checkInputGuard();
void releaseButtons();
void startupBlink();
void switchBlink();

// Set up joystick with 6 buttons, and disable hat switch & all axes
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 6, 0,
                   false, false, false, false, false, false, false, false,
                   false, false, false);

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(LED_GND, OUTPUT);

  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonStartPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(inputGuardPin, INPUT_PULLUP);

  blinker.setPeriod(1000, 100);

  startupBlink();
  delay(500);

  switchState = digitalRead(switchPin);
  switchBlink();
  delay(1000);
  if (switchState) {
    Joystick.begin();
  } else {
    Keyboard.begin();
  }
}

void loop() {
  updateStates();
  checkInputGuard();
  setButtons();

  updateBlinker();
  digitalWrite(LED, blinker.state);
}

void checkInputGuard() {
  isInputEnabled = true;  // !digitalRead(inputGuardPin);
  if (isInputEnabled) {
    isButtonsReleased = false;
  } else {
    releaseButtons();
    isButtonsReleased = true;
  }
}

void releaseButtons() {
  if (isButtonsReleased) return;
  if (switchState) {
    for (int i = 0; i < 6; i++) {
      Joystick.setButton(i, false);
    }
  } else {
    Keyboard.releaseAll();
  }
}

void updateStates() {
  buttonLeftState.updateState(digitalRead(buttonLeftPin));
  buttonRightState.updateState(digitalRead(buttonRightPin));
  buttonDownState.updateState(digitalRead(buttonDownPin));
  buttonUpState.updateState(digitalRead(buttonUpPin));
  buttonStartState.updateState(digitalRead(buttonStartPin));
  buttonSelectState.updateState(digitalRead(buttonSelectPin));
}

void setButtons() {
  if (!isInputEnabled) return;
  setButton(0, buttonStartState, KEY_RETURN);
  setButton(1, buttonSelectState, KEY_ESC);
  setButton(2, buttonUpState, KEY_UP_ARROW);
  setButton(3, buttonDownState, KEY_DOWN_ARROW);
  setButton(4, buttonLeftState, KEY_LEFT_ARROW);
  setButton(5, buttonRightState, KEY_RIGHT_ARROW);
}

void setButton(int index, ButtonDebounce state, char c) {
  if (switchState) {
    if (state.isBtnPressed || state.isBtnReleased) {
      Joystick.setButton(index, state.btnState);
    }
  } else {
    if (state.isBtnPressed) {
      Keyboard.press(c);
    } else if (state.isBtnReleased) {
      Keyboard.release(c);
    }
  }
}

void updateBlinker() {
  if (buttonLeftState.btnState || buttonRightState.btnState ||
      buttonDownState.btnState || buttonUpState.btnState ||
      buttonStartState.btnState || buttonSelectState.btnState) {
    blinker.setPeriod(50, 50);
  } else {
    blinker.setPeriod(1000, 0);
  }
  blinker.update();
}

void startupBlink() {
  for (int i = 0; i < 20; i++) {
    digitalWrite(LED, HIGH);
    delay(10);
    digitalWrite(LED, LOW);
    delay(20);
  }
}

void switchBlink() {
  if (switchState) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(300);
    }
  } else {
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(300);
    }
  }
}
