#include "definitions.h"
#define POLLING_MILLIS 10
#define UPDATE_MILLIS POLLING_MILLIS * 4

bool power = false;
static unsigned long lastUpdate = 0;

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
    onLED();
    resetAnimator();
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
  unsigned long currentTime = millis();

  Button button = pollIR();
  if (button.state == PRESSED){
    Serial.println("PRESSED");
    ButtonInput(button.id);
  }

  if (power && currentTime - lastUpdate >= UPDATE_MILLIS) {
    updateAnimator();
    updateLED();
    lastUpdate = currentTime;
  }

  delay(POLLING_MILLIS);
}