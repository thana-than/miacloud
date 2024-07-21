enum ButtonID {
  ID_NONE = 0,
  ID_POWER = 0x45,
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