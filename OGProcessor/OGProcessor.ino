#include "Parser.h"
#include "Control.h"
#include "INST.h"

/*
   3D Printer OG Processor
   Responsible for: Interfacing with g-code source, parsing the g-code, sending the commands to appropriate slaves
*/



Parser parser;
Control control;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  while (Serial.available() > 0) {
    String buff = Serial.readString();
    if (buff == "exec\n") {
      control.Dispatch();
      break;
    }

    CMD cmd = parser.Parse(buff);
    if (cmd.addr == '\0') {
      Serial.println("Invalid");
      continue;
    }
    control.Queue(cmd);
    Serial.println("Loaded command");
  }

}
