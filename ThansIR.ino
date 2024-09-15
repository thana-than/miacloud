#define BUTTON_POLL_BUFFER_MILLIS 100
#define BUTTON_HELD_PULSE_MILLIS 200

void initIR() {
  receiver_setup();
}

Button currentButton;
int heldBufferMillis = 0;
int heldMillis = 0;

int getNumberedButton(ButtonID id) {
  switch (id) {
    case ID_ZERO:
      return 0;
    case ID_ONE:
      return 1;
    case ID_TWO:
      return 2;
    case ID_THREE:
      return 3;
    case ID_FOUR:
      return 4;
    case ID_FIVE:
      return 5;
    case ID_SIX:
      return 6;
    case ID_SEVEN:
      return 7;
    case ID_EIGHT:
      return 8;
    case ID_NINE:
      return 9;
    case ID_EQ:
      return -10;
    //case ID_STREPT:
    //  return -11;
  }

  return -1000;
}

bool allowsPulse(ButtonID id) {
  if (id == ID_DOWN || id == ID_UP)
    return true;
  return false;
}

void printButton(int id, String state) {
  print("___");
  print(state);
  print(" id: ");
  printHex(id);
  println(" ___");
}

void ButtonState_None(ButtonID id_latest) {
  if (id_latest == ID_NONE)
    return;
  currentButton.id = id_latest;
  currentButton.state = PRESSED;

  println("");
  printButton(currentButton.id, "PRESSED");
}

void ButtonState_Pressed(ButtonID id_latest) {
  currentButton.state = HELD;

  heldMillis = 0;
  heldBufferMillis = 0;
}

void ButtonState_Held(ButtonID id_latest) {
  if (id_latest != currentButton.id) {
    if (heldBufferMillis < BUTTON_POLL_BUFFER_MILLIS)
      heldBufferMillis += POLLING_MILLIS;
    else
      currentButton.state = RELEASED;
  }
  else {
    heldBufferMillis = 0;

    if (allowsPulse(currentButton.id) && heldMillis > BUTTON_HELD_PULSE_MILLIS) {
      printButton(currentButton.id, "PULSE");
      currentButton.state = PRESSED;
    }
  }

  heldMillis += POLLING_MILLIS;
}

void ButtonState_Released(ButtonID id_latest) {
  printButton(currentButton.id, "RELEASED");
  currentButton.id = ID_NONE;
  currentButton.state = NONE;
}

Button pollIR() {
  int code = receiver_poll();
  ButtonID id = (ButtonID)code;

  switch (currentButton.state) {
    case NONE:
      ButtonState_None(id);
      break;
    case PRESSED:
      ButtonState_Pressed(id);
      break;
    case HELD:
      ButtonState_Held(id);
      break;
    case RELEASED:
      ButtonState_Released(id);
      break;
  }

  return currentButton;
}

