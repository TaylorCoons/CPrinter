#include "Parser.h"
/*
 * 3D Printer OG Processor
 * Responsible for: Interfacing with g-code source, parsing the g-code, sending the commands to appropriate slaves
 */

Parser parser;

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    CMD cmd = parser.Parse(Serial.readString());
    if (cmd.addr == '\0') {
      Serial.println("Invalid");
    }
    for (unsigned int i = 0; i < cmd.numParams; i++) {
      Serial.print(cmd.params[i].param);
      Serial.print(" : ");
      Serial.print((cmd.params[i].set ? "set" : "unset"));
      Serial.print(" : ");
      Serial.println(cmd.params[i].value);
    }
  }
}
