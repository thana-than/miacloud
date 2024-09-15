#include "definitions.h"
#include "debug.h"

#include <FastLED.h>

#define POLLING_MILLIS 10
#define UPDATE_MILLIS POLLING_MILLIS

bool power = false;
static unsigned long lastUpdate = 0;

void togglePower() {
  setPower(!power);
}

void setPower(bool _power) {
  power = _power;

  if (power) {
    printCommand("POWER", "ON");
    onLED();
    resetState();
  } else {
    printCommand("POWER", "OFF");
    offLED();
    resetState();
  }
}

void setup() {
  initIR();
  initLED();
  initAnimator();
}

void reset() {
  resetLED();
  resetAnimator();
  Serial.println("RESET");
}

void ButtonInput(ButtonID id) {
  if (!power && id != ID_POWER)
    return;

  if (id == ID_POWER)
    togglePower();
  else if (id == ID_VOLUP)
    changeBrightness(1);
  else if (id == ID_VOLDOWN)
    changeBrightness(-1);
  else if (id == ID_PLAY)
    togglePause();
  else if (id == ID_UP)
    changeSpeed(1);
  else if (id == ID_DOWN)
    changeSpeed(-1);
  else if (id == ID_FORWARD)
    cycleAnimation(1);
  else if (id == ID_BACK)
    cycleAnimation(-1);
  else if (id == ID_FUNCSTOP)
    reset();
  else {
    trySetColor(getNumberedButton(id));
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