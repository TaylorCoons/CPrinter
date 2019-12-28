#include <Wire.h>

#include "INST.h"
#include "SlaveAddr.h"
#include "AxisDefines.h"
#include "QBuffer.h"

// Stepper driver pins
const unsigned int DIR_PIN = 4;
const unsigned int STEP_PIN = 5;

// Step the stepper motor once
void Step();

// Set the direction of the stepper motor
void Direction(bool);

// Callback for I2C data
void onRecv(int numBytes);

// Instruction buffer
INST inst;
// Step index (incremented every step signal from OGProcessor)
unsigned int stepIndex = 0;
// Step count (incremented only when axis actually steps the motor)
unsigned int stepCount = 0;

void setup() {
  // put your setup code here, to run once:
  // Set the stepper driver pins
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  // Just write no action to pins
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
  // Initialize the serial for debug
  Serial.begin(9600);
  // Initialize I2C to receive data from OGProcessor
  Wire.begin(X_AXIS_ADDR);
  // Set I2C callback
  Wire.onReceive(onRecv);
  // Attach interrupt on the step pin driven by OGProcessor
  attachInterrupt(digitalPinToInterrupt(2), StepAction, RISING);
  // Debug LED PIN
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, inst.opt != OPT::NOOP);
}

void StepAction() {
  // Debug flag
  bool debugPrint = false;
  // Check to see if theres no instruction
  if (inst.opt == OPT::NOOP) {
    return;
  }
  // Increment the step index
  stepIndex++; 
  if (debugPrint) {
    Serial.print("stepIndex: ");
    Serial.println(stepIndex);
    inst.Print("INST");
  }
  // Check if it is a move instruction, if not exit
  if (inst.opt != OPT::MOVE) {
    return;
  }
  // Check to see if steps exceeded steps for isnt
  if (stepIndex >= inst.steps) {
    // Reset counters
    stepCount = 0;
    stepIndex = 0;
    // Clear instruction
    inst.Clear();
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
  unsigned int stepsRequired = map(stepIndex, 0, inst.steps, 0, totalSteps);
  if (debugPrint) {
    Serial.print("totalSteps: ");
    Serial.println(totalSteps);
    Serial.print("stepsRequired: ");
    Serial.println(stepsRequired);
    Serial.print("stepCount: ");
    Serial.println(stepCount);
  }
  // Check if we need to step based on the linear interpolation
  if (stepCount < stepsRequired) {
    // If so step
    Step();
    stepCount++;
  } 
}

// Step the stepper motor once
void Step() {
  // Just toggle pin to trigger driver interrupt
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(STEP_PIN, LOW);
}


// Set the direction of the stepper motor
void Direction(bool clockwise) {
  digitalWrite(DIR_PIN, clockwise);
}

// Callback for I2C data
void onRecv(int numBytes) {
  // Check that the data recieved is of the right size
  if (numBytes == 10) {
    // Cast all of the bytes to the correct type
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
  }
}
