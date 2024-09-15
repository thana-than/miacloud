#define NUM_LEDS 14
#define LED_PIN 9
#define BRIGHTNESS_STEP .2
#define LED_MAX 255
constexpr float LED_STEP = 1.0 / NUM_LEDS;
constexpr double PHI = 1.618033988749895;

enum ButtonID {
  ID_NONE = 0,
  ID_POWER = 0x45,
  ID_FUNCSTOP = 0x47,
  ID_BACK = 0x44,
  ID_FORWARD = 0x43,
  ID_VOLUP = 0x46,
  ID_VOLDOWN = 0x15,
  ID_PLAY = 0x40,
  ID_DOWN = 0x7,
  ID_UP = 0x9,
  ID_ZERO = 0x16,
  ID_ONE = 0xC,
  ID_TWO = 0x18,
  ID_THREE = 0x5E,
  ID_FOUR = 0x8,
  ID_FIVE = 0x1C,
  ID_SIX = 0x5A,
  ID_SEVEN = 0x42,
  ID_EIGHT = 0x52,
  ID_NINE = 0x4A,
  ID_EQ = 0x19,
  ID_STREPT = 0xD,
};

enum ButtonState {
  NONE, PRESSED, HELD, RELEASED
};


class Button {
  public:
    ButtonID id;
    ButtonState state;

  Button() {
    id = ID_NONE;
    state = NONE;
  }
};