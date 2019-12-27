#include "Control.h"

Control::Control() {
  Wire.begin();
}

//COMM::PACKET Control::Interpret(CMD cmd) {
//  return COMM::PACKET();
//}

void Control::Queue(CMD cmd) {
  if (cmdIndex < MAX_CMDS - 1) {
    cmdIndex++;
    cmds[cmdIndex] = cmd;
  }
}

void Control::Dispatch() {
  if (cmdIndex >= 0) {
    //COMM::PACKET packet = Interpret(cmds[cmdIndex]);
    cmdIndex--;

    Wire.beginTransmission(44);
    Wire.write('a');
    Wire.endTransmission();
  }
}

Control::~Control() {
}
