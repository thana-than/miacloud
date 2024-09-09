#include <FastLED.h>
#define NUM_LEDS 30
#define LED_PIN 9
#define BRIGHTNESS_STEP .2
#define LED_MAX 255

bool updateLEDs = false;
float brightness = 1;
CRGB leds[NUM_LEDS];

void initLED() {
  // initialize digital pin LED_BUILTIN as an output.
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500); // Limit power to prevent overload
  FastLED.setDither(0); // Disable dithering to reduce CPU load
  startupLEDsTest();
}

void startupLEDsTest() {
  Serial.println("INITIALIZING LEDs");
  // startup blink test to confirm LEDs are working.
  FastLED.setBrightness(32);
  fill_solid(leds, NUM_LEDS, CRGB(255,0,0));  // fill red
  FastLED.show();
  delay(300);
  fill_solid(leds, NUM_LEDS, CRGB(0,255,0));  // fill green
  FastLED.show();
  delay(300);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));  // fill blue
  FastLED.show();
  delay(300);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(brightness * LED_MAX);
  Serial.println("LED INITIALIZING COMPLETE");

}

void onLED() {
  markChange();
}

void offLED() {
  FastLED.clear();
  FastLED.show();
}

void markChange() {
  updateLEDs = true;
}

void changeBrightness(float direction) {
  brightness += direction * BRIGHTNESS_STEP;
  brightness = constrain(brightness, 0.05, 1);
  printCommand("BRIGHTNESS", brightness);

  FastLED.setBrightness(brightness * LED_MAX);
  markChange();
}

void updateLED() { 
  if (!updateLEDs)
    return;

  refresh();
}

void refresh() {
  for (int i = 0; i < NUM_LEDS; i++) {
    float map = i / (float)NUM_LEDS;
    float value = getBrightnessWeight(map);
    value = constrain(value * LED_MAX, 0, LED_MAX);
    leds[i] = CRGB(value, value, value);
  }

  FastLED.show();
  updateLEDs = false;
}