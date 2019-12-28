#include "Parser.h"
#include "Control.h"
#include "INST.h"

/*
 * 3D Printer OG Processor
 * Responsible for: Interfacing with g-code source, parsing the g-code, sending the commands to appropriate slaves
 */

const unsigned int STEP_PIN = 5;

Parser parser;
Control control;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  /*
  while (Serial.available() > 0) {
    String buff = Serial.readString();
    if (buff == "step\n") {
      for (unsigned int i = 0; i < 100; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delay(10);
        digitalWrite(STEP_PIN, LOW);
        delay(10);
      }
      break;
    }
    CMD cmd = parser.Parse(buff);
    if (cmd.addr == '\0') {
      Serial.println("Invalid");
      continue;
    }
    for (unsigned int i = 0; i < cmd.numParams; i++) {
      Serial.print(cmd.params[i].param);
      Serial.print(" : ");
      Serial.print((cmd.params[i].set ? "set" : "unset"));
      Serial.print(" : ");
      Serial.println(cmd.params[i].value);
    }
  }
  */
  
  CMD cmd('G', 0);
  cmd.params[2].Set(5.0);
  cmd.params[3].Set(10.0);
  control.Queue(cmd);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  control.Dispatch(); 
  delay(5000);
  for (unsigned int i = 0; i < 20; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delay(2000);
    digitalWrite(STEP_PIN, LOW);
    delay(2000);
  }
  delay(5000);
  
}
