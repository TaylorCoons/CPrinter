#include "Parser.h"
#include "Control.h"

/*
 * 3D Printer OG Processor
 * Responsible for: Interfacing with g-code source, parsing the g-code, sending the commands to appropriate slaves
 */

Parser parser;
Control control;

enum TEST {
  NONE,
  ONE,
  TWO
};

void setup() {
  Serial.begin(9600);
}

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
    delay(5000);
    control.Queue(cmd);
    control.Dispatch();
  }
}
