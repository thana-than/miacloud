#define BUTTON_POLL_BUFFER_MILLIS 100
#define BUTTON_HELD_PULSE_MILLIS 200

void initIR() {
  receiver_setup();
}

Button currentButton;
int heldBufferMillis = 0;
int heldMillis = 0;

bool allowsPulse(ButtonID id) {
  if (id == ID_DOWN || id == ID_UP)
    return true;
  return false;
}

void printButton(int id, String state) {
  Serial.print("___");
  Serial.print(state);
  Serial.print(" id: ");
  Serial.print(id, HEX);
  Serial.println(" ___");
}

void ButtonState_None(ButtonID id_latest) {
  if (id_latest == ID_NONE)
    return;
  currentButton.id = id_latest;
  currentButton.state = PRESSED;

  Serial.println("");
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

