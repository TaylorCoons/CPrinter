#include "Control.h"

Control::Control() {
  Wire.begin();
  // Slow the clock down for less noisy signal
  Wire.setClock(10000);
  pinMode(STEP_PIN, OUTPUT);
}

void Control::Interpret(CMD cmd) {
  xAxisInst.Clear();
  yAxisInst.Clear();
  flags = static_cast<unsigned int>(OPT_FLAG::NONE);
  maxSteps = 0;
  bool drive = false;
  Serial.print("cmd.addr: ");
  Serial.println(cmd.addr);
  Serial.print("cmd.cmdNum: ");
  Serial.println(cmd.cmdNum);
  if (cmd.addr == 'G') {
    switch(cmd.cmdNum) {
      case 0:
      case 1:
        if (cmd.params[2].set) {
          xAxisInst.opt = OPT::MOVE;
          xAxisInst.flags = static_cast<unsigned int>(OPT_FLAG::DRIVE);
          xAxisInst.value = cmd.params[2].value;
          drive = true;
        }
        if (cmd.params[3].set) {
          yAxisInst.opt = OPT::MOVE;
          yAxisInst.flags = static_cast<unsigned int>(OPT_FLAG::DRIVE);
          yAxisInst.value = cmd.params[3].value;
          drive = true;
        }
        if (drive) {
          flags = static_cast<unsigned int>(OPT_FLAG::DRIVE);
          maxSteps = CalcMaxSteps(xAxisInst.value, yAxisInst.value, 0);
          xAxisInst.steps = yAxisInst.steps = maxSteps;
        }
      break;
      case 28:
        if (!cmd.flags[2].set && !cmd.flags[3].set && !cmd.flags[4].set) {
          cmd.flags[2].set = cmd.flags[3].set = cmd.flags[4].set = true;
        }
        xAxisInst.opt = (cmd.flags[2].set ? OPT::HOME : OPT::NOOP);
        yAxisInst.opt = (cmd.flags[3].set ? OPT::HOME : OPT::NOOP);
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
  Serial.print("Cmds Queued: ");
  Serial.println(cmds.Size());
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
  // Take the absolute value since we only need magnitude
  xDist = abs(xDist);
  yDist = abs(yDist);
  zDist = abs(zDist);
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

void Control::Execute() {
  if (flags & OPT_FLAG::DRIVE) {
    for (unsigned int i = 0; i < maxSteps; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delay(1);
      digitalWrite(STEP_PIN, LOW);
      delay(1);
    }
  }
}

Control::~Control() {
}
