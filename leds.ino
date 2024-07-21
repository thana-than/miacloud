#define LED_PIN A0
#define BRIGHTNESS_STEP .2
#define LED_MAX 255

float brightness = 1;

void initLED() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
}

void offLED() {
  analogWrite(LED_PIN, 0);
}

void changeBrightness(float direction) {
  brightness += direction * BRIGHTNESS_STEP;
  brightness = constrain(brightness, 0, 1);
  printCommand("BRIGHTNESS", brightness);
}

void updateLED() { 
  float value = getBrightnessWeight(0) * brightness;
  value = constrain(value * LED_MAX, 0, LED_MAX);
  analogWrite(LED_PIN, value);
}