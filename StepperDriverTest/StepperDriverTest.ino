d#include <Wire.h>

#include "Comm.h"

const unsigned int DIR_PIN = 2;
const unsigned int STEP_PIN = 3;

void Step();

void Direction(bool);

void onRecv(int numBytes);

void setup() {
  // put your setup code here, to run once:
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onReceive(onRecv);
}

bool clockwise = false;
const int MAX_INST = 10;
int instIndex = -1;
INST instQueue[MAX_INST];

void loop() {
  if (instIndex > -1) {
    INST inst = instQueue[instIndex];
    Direction(true);
    if (inst.value < 0) {
      Direction(false);
      inst.value *= -1;
    }
    for (unsigned int i = 0; i < (unsigned int)floor(inst.value); i++) {
      Step();
      delay(10);
    }
    instIndex--;
  }
}

void Step() {
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(STEP_PIN, LOW);
}

void Direction(bool clockwise) {
  digitalWrite(DIR_PIN, clockwise);
}

void onRecv(int numBytes) {
  INST inst;
  if (numBytes == 10) {
    byte c;
    unsigned int x;
    unsigned long int y;
    x = static_cast<unsigned int>(Wire.read()) << 8;
    x |= static_cast<unsigned int>(Wire.read());
    inst.opt = static_cast<OPT>(x);
    x = static_cast<unsigned int>(Wire.read()) << 8;
    x |= static_cast<unsigned int>(Wire.read());
    inst.flags = x;
    y = static_cast<unsigned long int>(Wire.read()) << 24;
    y |= static_cast<unsigned long int>(Wire.read()) << 16;
    y |= static_cast<unsigned long int>(Wire.read()) << 8;
    y |= static_cast<unsigned long int>(Wire.read());
    inst.value = *reinterpret_cast<double*>(&y);
    x = static_cast<unsigned int>(Wire.read()) << 8;
    x |= static_cast<unsigned int>(Wire.read());
    inst.steps = x;
    if (instIndex < MAX_INST - 1) {
      instIndex++;
      instQueue[instIndex] = inst;
    }
  }
}
