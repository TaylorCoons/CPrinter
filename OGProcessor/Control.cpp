#include "Control.h"

Control::Control() {
  Wire.begin();
  Wire.setClock(10000);
}

INST Control::Interpret(CMD cmd) {
  INST inst;
  if (cmd.addr == 'G') {
    switch(cmd.cmdNum) {
      case 0:
        inst.opt = OPT::MOVE;
        inst.flags = 0;
        inst.flags |= OPT_FLAG::NONE;
        Serial.print("cmd.params[2].value: ");
        Serial.println(cmd.params[2].value);
        inst.value = cmd.params[2].value;
        inst.steps = 0;
      break;
      case 1:
        inst.opt = OPT::MOVE;
        inst.flags = 0;
        inst.flags |= OPT_FLAG::NONE;
        inst.value = cmd.params[2].value;
        inst.steps = 0;
      break;
    }
  }
  else if (cmd.addr == 'M') {
    switch(cmd.cmdNum) {
      
    }
  }
  return inst;
}

void Control::Queue(CMD cmd) {
  if (cmdIndex < MAX_CMDS - 1) {
    cmdIndex++;
    cmds[cmdIndex] = cmd;
  }
}

void Control::Write(unsigned int x) {
  for (int i = 1; i >= 0; i--) {
    Wire.write((byte)(x >> (8 * i)));
  }
}

void Control::Write(unsigned long int x) {
  for (int i = 3; i >= 0; i--) {
    Wire.write((byte)(x >> (8 * i)));
  }
}

void Control::Write(double x) {
  unsigned long int y = *reinterpret_cast<unsigned long int*>(&x);
  Write(y);
}

void Control::Dispatch() {
  if (cmdIndex >= 0) {
    INST inst = Interpret(cmds[cmdIndex]);
    Serial.print("inst.value: ");
    Serial.println(inst.value);
    Wire.beginTransmission(8);
    Write(static_cast<unsigned int>(inst.opt));
    Write(static_cast<unsigned int>(inst.flags));
    Write(inst.value);
    Write(static_cast<unsigned int>(inst.steps));
    Wire.endTransmission();
    cmdIndex--;
  }
}

Control::~Control() {
}
