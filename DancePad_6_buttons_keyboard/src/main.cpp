#include <Arduino.h>
#include <Keyboard.h>
#include "serialHelpers.h"
#include "blinker.h"
#include "buttonDebounce.h"
#include "setButtonVariants.h"
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
void setButton(int pin, ButtonDebounce state, char c);
void updateBlinker();
void checkInputGuard(); 
void releaseButtons();
void startupBlink();

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

  Keyboard.begin();

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
  Keyboard.releaseAll();
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
  setButton(buttonLeftPin, buttonLeftState, KEY_LEFT_ARROW);
  setButton(buttonRightPin, buttonRightState, KEY_RIGHT_ARROW);
  setButton(buttonDownPin, buttonDownState, KEY_DOWN_ARROW);
  setButton(buttonUpPin, buttonUpState, KEY_UP_ARROW);
  setButton(buttonStartPin, buttonStartState, KEY_RETURN);
  setButton(buttonSelectPin, buttonSelectState, KEY_ESC);
}

void setButton(int pin, ButtonDebounce state, char c) {
  setButtonRegular(pin, state, c);
  //setButtonTapReverce(pin, state, c);
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
