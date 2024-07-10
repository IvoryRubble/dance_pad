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

#include <Arduino.h>
#include <Joystick.h>
#include "serialHelpers.h"
#include "blinker.h"
#include "buttonDebounce.h"
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

ButtonDebounce buttonStartState;
ButtonDebounce buttonSelectState;
ButtonDebounce buttonRightState;
ButtonDebounce buttonUpState;
ButtonDebounce buttonDownState;
ButtonDebounce buttonLeftState;

void updateStates();
void setButtons();
void setButton(int, ButtonDebounce);
void updateBlinker();
void checkInputGuard(); 
void releaseButtons();
void startupBlink();

// Set up joystick with 6 buttons, and disable hat switch & all axes
Joystick_ Joystick( JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                    6, 0, false, false, false, false, false,
                    false, false, false, false, false, false );

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

  pinMode(inputGuardPin, INPUT_PULLUP);

  Joystick.begin();

  blinker.setPeriod(1000, 100);

  startupBlink();
}

void loop() {
  updateStates();
  checkInputGuard();
  setButtons();

  updateBlinker();
  digitalWrite(LED, blinker.state);
}

void checkInputGuard() {
  isInputEnabled = true;// !digitalRead(inputGuardPin);
  if (isInputEnabled) {
    
    isButtonsReleased = false;
  } else {
    releaseButtons();
    isButtonsReleased = true;
  }
}

void releaseButtons() {
  if (isButtonsReleased) return;
  for (int i = 0; i < 6; i++) {
    Joystick.setButton(i, false);
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
  setButton(0, buttonStartState);
  setButton(1, buttonSelectState);
  setButton(2, buttonUpState);
  setButton(3, buttonDownState);
  setButton(4, buttonLeftState);
  setButton(5, buttonRightState);
}

void setButton(int index, ButtonDebounce state) {
  if (state.isBtnPressed || state.isBtnReleased) {
    Joystick.setButton(index, state.btnState);
  }
}

void updateBlinker() {
  if (
    buttonLeftState.btnState || 
    buttonRightState.btnState || 
    buttonDownState.btnState || 
    buttonUpState.btnState || 
    buttonStartState.btnState || 
    buttonSelectState.btnState) {
    blinker.setPeriod(50, 50);
  } else {
    blinker.setPeriod(1000, 0);
  }
  blinker.update();
}

void startupBlink() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}
