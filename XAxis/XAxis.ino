#include <Wire.h>

#include "Comm.h"
#include "SlaveAddr.h"
#include "AxisDefines.h"

const unsigned int DIR_PIN = 4;
const unsigned int STEP_PIN = 5;

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
  Wire.begin(X_AXIS_ADDR);
  Wire.onReceive(onRecv);
  attachInterrupt(digitalPinToInterrupt(2), StepAction, RISING);
}

bool clockwise = false;
const int MAX_INST = 10;
int instIndex = -1;
INST instQueue[MAX_INST];
unsigned int stepIndex = 0;
unsigned int stepCount = 0;

// DEBUG:
int prevInstIndex = instIndex;

void loop() {
  if (prevInstIndex != instIndex) {
    Serial.print("instIndex: ");
    Serial.println(instIndex);
    instQueue[instIndex].Print("INST");
    prevInstIndex = instIndex;
  }
}

void StepAction() {
  // Check to see if theres no instruction
  if (instIndex < 0) {
    return;
  }
  // Grab the current instruction
  INST inst = instQueue[instIndex];
  inst.Print("INST");
  // Increment the step index
  stepIndex++;  
  Serial.print("stepIndex: ");
  Serial.println(stepIndex);
  // Check if it is a move instruction, if not exit
  if (inst.opt != OPT::MOVE) {
    return;
  }
  // Check to see if steps exceeded steps for isnt
  if (stepIndex >= inst.steps) {
    // If so load next instruction
    stepCount = 0;
    stepIndex = 0;
    instIndex--;
    return;
  }
  // Check sign to see which direction
  bool clockwise = true;
  if (inst.value < 0) {
    clockwise = false;
  }
  Direction(clockwise);
  // Do linear interpolation to determine wether to step or not
  unsigned int totalSteps = inst.value * X_AXIS_STEPS_PER_MM;
  Serial.print("totalSteps: ");
  Serial.println(totalSteps);
  unsigned int stepsRequired = map(stepIndex, 0, inst.steps, 0, totalSteps);
  Serial.print("stepsRequired: ");
  Serial.println(stepsRequired);
  Serial.print("stepCount: ");
  Serial.println(stepCount);
  if (stepCount < stepsRequired) {
    digitalWrite(13, HIGH);
    Step();
    stepCount++;
    digitalWrite(13, LOW);
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
