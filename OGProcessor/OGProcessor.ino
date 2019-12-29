#include "Parser.h"
#include "Control.h"
#include "INST.h"

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

void loop() {
  while (Serial.available() > 0) {
    String buff = Serial.readString();
    if (buff == "exec\n") {
      control.Dispatch();
      delay(1000);
      control.Execute();
      break;
    }
    CMD cmd = parser.Parse(buff);
    if (cmd.addr == '\0') {
      Serial.println("Invalid");
      continue;
    }
    control.Queue(cmd);
    for (unsigned int i = 0; i < cmd.numParams; i++) {
      Serial.print(cmd.params[i].param);
      Serial.print(" : ");
      Serial.print((cmd.params[i].set ? "set" : "unset"));
      Serial.print(" : ");
      Serial.println(cmd.params[i].value);
    }
    for (unsigned int i = 0; i < cmd.numFlags; i++) {
      Serial.print(cmd.flags[i].flag);
      Serial.print(" : ");
      Serial.println((cmd.flags[i].set ? "set" : "unset"));
    }
  }
  /*
  CMD cmd('G', 0);
  cmd.params[2].Set(50.0);
  cmd.params[3].Set(10.0);
  control.Queue(cmd);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  control.Dispatch(); 
  delay(5000);
  control.Execute();
  delay(5000);
  */
  
}
