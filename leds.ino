bool updateLEDs = false;
float brightness = 1;
CRGB leds[NUM_LEDS];

int colorIndex = 0;
CRGB colorMap[12] = {
  CRGB(0x7d3004), // 0 Warm

  CRGB(0x008080), // 1 Mia's Aqua
  CRGB(0x0000FF), // 2 Blue
  CRGB(0x8000FF), // 3 Violet
  CRGB(0x800080), // 4 Purple
  CRGB(0xFF00FF), // 5 Magenta
  CRGB(0xFF0000), // 6 Red
  CRGB(0xFFFF00), // 7 Yellow
  CRGB(0x00FF00), // 8 Green
  CRGB(0x00ff91), // 9 Mint

  // Mapped from -10 and -11
  CRGB(0xe47025), // 10 Incandescent
  CRGB(0xFDFBD3), // 11 Sunlight
};

void initLED() {
  // initialize digital pin LED_BUILTIN as an output.
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500); // Limit power to prevent overload
  FastLED.setDither(0); // Disable dithering to reduce CPU load
  if (DEBUG_MODE)
    startupLEDsTest();
  resetLED();
}

void startupLEDsTest() {
  println("INITIALIZING LEDs");
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
  println("LED INITIALIZING COMPLETE");
}


void resetLED() {
  FastLED.clear();
  FastLED.show();
  colorIndex = 0;
  brightness = 1;
  FastLED.setBrightness(brightness * LED_MAX);
  markChange();
}

bool trySetColor(int index) {
  if (index < -100)
    colorIndex = -1;
  else
    colorIndex = abs(index);

  
  markChange();

  if (DEBUG_MODE) {
    char* hexString;
    if (colorIndex < 0)
      hexString = "MULTI";
    else
      hexString = getHex(colorMap[colorIndex]);

    printCommand("COLOR CHANGED", hexString);
  }

  return true;
}

char* getHex(CRGB color) {
  // Convert to hexadecimal
  static char hexString[8];
  snprintf(hexString, sizeof(hexString), "#%02X%02X%02X", color.r, color.g, color.b);
  return hexString;
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
  brightness = constrain(brightness, 0.1, 1);
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
  bool useStateColor = colorIndex < 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    float map = i / (float)NUM_LEDS;
    double value = getBrightnessWeight(map);
    value = constrain(value, 0, 1);
    CRGB c = useStateColor ? getColor(map) : colorMap[colorIndex];
    leds[i] = CRGB(c.r * value, c.g * value, c.b * value);
  }

  FastLED.show();
  updateLEDs = false;
}