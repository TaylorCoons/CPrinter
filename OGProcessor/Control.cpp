#include "Control.h"

Control::Control() {
  Wire.begin();
  // Slow the clock down for less noisy signal
  Wire.setClock(10000);
}

void Control::Interpret(CMD cmd) {
  xAxisInst.Clear();
  yAxisInst.Clear();
  if (cmd.addr == 'G') {
    switch(cmd.cmdNum) {
      case 0: // G0 and G1 are mostly the same
      case 1:
        if (cmd.params[2].set) {
          xAxisInst.opt = OPT::MOVE;
          xAxisInst.flags = static_cast<unsigned int>(OPT_FLAG::NONE);
          xAxisInst.value = cmd.params[2].value;
        }
        if (cmd.params[3].set) {
          yAxisInst.opt = OPT::MOVE;
          yAxisInst.flags = static_cast<unsigned int>(OPT_FLAG::NONE);
          yAxisInst.value = cmd.params[3].value;
        }
        xAxisInst.steps = yAxisInst.steps = CalcMaxSteps(xAxisInst.value, yAxisInst.value, 0);
      break;
    }
  }
  else if (cmd.addr == 'M') {
    switch(cmd.cmdNum) {
      
    }
  }
}

void Control::Queue(CMD cmd) {
  cmds.Push(cmd);
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

void Control::SendInst(uint8_t addr, INST inst) {
  Wire.beginTransmission(addr);
  Write(static_cast<unsigned int>(inst.opt));
  Write(static_cast<unsigned int>(inst.flags));
  Write(inst.value);
  Write(static_cast<unsigned int>(inst.steps));
  Wire.endTransmission();
}

unsigned int Control::CalcMaxSteps(double xDist, double yDist, double zDist) {
  // Ignore z since idc about it now
  unsigned int xSteps = xDist * X_AXIS_STEPS_PER_MM;
  unsigned int ySteps = yDist * Y_AXIS_STEPS_PER_MM;
  return (xSteps > ySteps ? xSteps : ySteps);
}

void Control::Dispatch() {
  CMD cmd = cmds.Pop();
  Interpret(cmd);
  xAxisInst.Print("X Axis");
  yAxisInst.Print("Y Axis");
  SendInst(Y_AXIS_ADDR, yAxisInst);
  SendInst(X_AXIS_ADDR, xAxisInst);
}

Control::~Control() {
}
