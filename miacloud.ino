#include "definitions.h"
#define UPDATE_MILLIS 10

bool power = false;

void togglePower() {
  setPower(!power);
}

void printCommand(String name, float value) {
  printCommand(name, String(value));
}

void printCommand(String name, String tag) {
  Serial.print(name);
  Serial.print(": ");
  Serial.println(tag);
}

void setPower(bool _power) {
  power = _power;

  if (power) {
    printCommand("POWER", "ON");
  } else {
    printCommand("POWER", "OFF");
    offLED();
    resetAnimator();
  }
}

void setup() {
  initIR();
  initLED();
  initAnimator();
}

void ButtonInput(ButtonID id) {
  if (!power && id != ID_POWER)
    return;

  if (id == ID_POWER)
    togglePower();
  else if (id == ID_UP)
    changeBrightness(1);
  else if (id == ID_DOWN)
    changeBrightness(-1);
  else {
    trySetAnimState(id);
  }
}

void loop() {
  Button button = pollIR();
  if (button.state == PRESSED){
    ButtonInput(button.id);
  }

  if (power) {
    updateAnimator();
    updateLED();
  }

  delay(UPDATE_MILLIS);
}