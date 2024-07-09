#include <Arduino.h>
#include <Joystick.h>
#include "serialHelpers.h"
#include "blinker.h"
#include "buttonDebounce.h"
#define LED 13
#define LED_GND A0


Blinker blinker;

const int buttonDownPin = A0;
const int buttonUpPin = A1;
const int buttonRightPin = A2;
const int buttonLeftPin = A3;
const int buttonStartPin = A4;
const int buttonSelectPin = A5;

const int inputGuardPin = 7;
bool isInputEnabled = true;
bool isButtonsReleased = false;

ButtonDebounce buttonLeftState;
ButtonDebounce buttonRightState;
ButtonDebounce buttonDownState;
ButtonDebounce buttonUpState;
ButtonDebounce buttonStartState;
ButtonDebounce buttonSelectState;

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
  setButton(0, buttonLeftState);
  setButton(1, buttonRightState);
  setButton(2, buttonDownState);
  setButton(3, buttonUpState);
  setButton(4, buttonStartState);
  setButton(5, buttonSelectState);
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
