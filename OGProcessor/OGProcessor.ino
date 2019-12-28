#include "Parser.h"
#include "Control.h"
#include "Comm.h"

/*
 * 3D Printer OG Processor
 * Responsible for: Interfacing with g-code source, parsing the g-code, sending the commands to appropriate slaves
 */

Parser parser;
Control control;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

double temp = 10;

void loop() {
  while (Serial.available() > 0) {
    CMD cmd = parser.Parse(Serial.readString());
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
    control.Queue(cmd);
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    control.Dispatch(); 
  }
}
