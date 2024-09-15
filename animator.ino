#define STATE_COUNT 5

#define SPEED_BASE 3
#define SPEED_STEP .5
#define SPEED_DIFF 2

long time = 0;
bool paused = false;
float speed = 2;

class AnimState {
  public:
    virtual String getName() { return "STATE_DEFAULT"; }
    virtual double evaluate(float map, long time) { return 1.0;}
    virtual bool hasColor() { return false; }
    virtual CRGB color(float map, long time) { return CRGB(255,255,255); }
};

class AnimState_Wave : public AnimState {
public:
  virtual String getName() override { return "STATE_WAVE"; }
  double evaluate(float map, long time) override {
    double val = 1.0 - wave(map + time * .0005);
    return val;
  }
};

constexpr int SLIDE_MARCHERS = 2;
constexpr int SLIDE_LED_COUNT = 3;
constexpr float SLIDE_TOTAL_STEP = LED_STEP * SLIDE_LED_COUNT * SLIDE_MARCHERS;
constexpr float SLIDE_LED_CALC = -1 + SLIDE_TOTAL_STEP;
class AnimState_Slide : public AnimState {
public:
  virtual String getName() override { return "STATE_SLIDE"; }
  double evaluate(float map, long time) override {
    map *= SLIDE_MARCHERS;
    double modulo = fmod(map + time * .0001,1);
    double percent = max(modulo + SLIDE_LED_CALC, 0) / SLIDE_TOTAL_STEP ;
    double val = sin(percent * PI);
    return val;
  }
};

class AnimState_Cloudy : public AnimState {
public:
  virtual String getName() override { return "STATE_CLOUDY"; }
  double evaluate(float map, long time) override {
    double distribution = map / LED_STEP * 1000000;
    double noise = renderNoise(distribution, time * 100, distribution) / 255.0;
    double val = sin(noise * PHI);
    return val;
  }
};

class AnimState_Thunder: public AnimState {
public:
  virtual String getName() override { return "STATE_WASH"; }
  double evaluate(float map, long time) override {
    const double muting = .5;
    double distribution = map / LED_STEP * 5000;
    double noise = max(renderNoise(distribution, time * 100, distribution) / 255.0 - muting,0);
    double val = sin(noise * PHI);
    return val;
  }
};

double wave(double time) {
  return abs(sin(time + PHI));
}

CRGB spectrum(float map, long time) {
  return CHSV(time * .05, 255, 255);
}

int stateIndex = 0;

AnimState defaultState;
AnimState_Wave waveState;
AnimState_Slide slideState;
AnimState_Cloudy cloudyState;
AnimState_Thunder thunderState;

AnimState* stateMap[STATE_COUNT] = {
  &defaultState,
  &waveState,
  &slideState,
  &cloudyState,
  &thunderState,
};

void cycleAnimation(int direction) {
  stateIndex = mod(stateIndex + direction, STATE_COUNT);
  printCommand("STATE CHANGED", stateMap[stateIndex]->getName());
  resetState();
}

int mod(int a, int b) {
  return (a % b + b) % b;
}

void initAnimator() {
  resetAnimator();
}

void resetAnimator() {
  speed = SPEED_BASE;
  stateIndex = 0;
  resetState();
}

void resetState() {
  time = 0;
  paused = false;
}

double getBrightnessWeight(float map) {
  double eval = stateMap[stateIndex]->evaluate(map, time);
  return eval;
}

CRGB getColor(float map) {
  CRGB eval = spectrum(map, time);
  return eval;
}

bool stateHasColor() {
  return stateMap[stateIndex]->hasColor();
}

void togglePause() {
  paused = !paused;
  markChange();
}

void changeSpeed(float direction) {
  if (paused)
    return;
  speed = constrain(speed + SPEED_STEP * direction, SPEED_BASE - SPEED_DIFF, SPEED_BASE + SPEED_DIFF);
  printCommand("SPEED CHANGED", speed);
}

void updateAnimator() {
  if (paused)
    return;
  time += UPDATE_MILLIS * speed;
  time %= 1000000;
  markChange();
}