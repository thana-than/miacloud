long time = 0;

class AnimState {
  public:
    virtual String getName() { return "STATE_DEFAULT"; }
    virtual float evaluate(float map, long time) { return 1.0;}
};

class AnimState_Wave : public AnimState {
public:
  virtual String getName() override { return "STATE_WAVE"; }
  float evaluate(float map, long time) override {
    float val = 1.0 - wave(time);
    return val;
  }
};

class AnimState_Slide : public AnimState {
public:
  virtual String getName() override { return "STATE_SLIDE"; }
  float evaluate(float map, long time) override {
    float modulo = fmod(map + time / 1000.0,1);
    float val = round(modulo);
    return val;
  }
};

float wave(long time) {
  return abs(sin(time / 1000.0));
}

AnimState* currentState;

AnimState defaultState;
AnimState_Wave waveState;
AnimState_Slide slideState;

bool trySetAnimState(ButtonID id) {
  switch (id) {
    case ID_ZERO:
      currentState = &defaultState;
      break;
    case ID_ONE:
      currentState = &waveState;
      break;
    case ID_TWO:
      currentState = &slideState;
      break;

    default:
      return false;
  }

  printCommand("STATE CHANGED", currentState->getName());
  resetAnimator();
  return true;
}

void initAnimator() {
  resetAnimator();
  currentState = &defaultState;
}

void resetAnimator() {
  time = 0;
}

float getBrightnessWeight(float map) {
  float eval = currentState->evaluate(map, time);
  return eval;
}

void updateAnimator() {
  time += UPDATE_MILLIS;
  markChange();
}