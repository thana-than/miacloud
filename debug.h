#define DEBUG_MODE false

void print(String str) {
  if (!DEBUG_MODE) return;
  Serial.print(str);
}

void print(float num) {
  if (!DEBUG_MODE) return;
  Serial.print(num);
}

void printHex(int num) {
  if (!DEBUG_MODE) return;
  Serial.print(num, HEX);
}

void println(String str) {
  if (!DEBUG_MODE) return;
  Serial.println(str);
}

void println(float num) {
  if (!DEBUG_MODE) return;
  Serial.println(num);
}

void printCommand(String name, String tag) {   
  if (!DEBUG_MODE) return;
  Serial.print(name);
  Serial.print(": ");
  Serial.println(tag);
}

void printCommand(String name, float value) {
  if (!DEBUG_MODE) return;
  printCommand(name, String(value));
}