#include "Control.h"

Control::Control() {
  Wire.begin();
  // Slow the clock down for less noisy signal
  Wire.setClock(10000);
  pinMode(STEP_PIN, OUTPUT);
}

INSTSET Control::Interpret(CMD& cmd) {
  INSTSET instSet;
  switch (cmd.addr) {
    case 'G':
      switch (cmd.num) {
        case 0:
          instSet = G0(cmd);
        break;
        case 1:
          instSet = G1(cmd);
        break;
        case 28:
          instSet = G28(cmd);
        break;
      }
    break;
    case 'M':

    break;
  }
  return instSet;
}

void Control::Queue(CMD& cmd) {
  instruction = Interpret(cmd);
  // instructions.Push(Interpret(cmd));
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

void Control::SendInst(uint8_t addr, const INST& inst) {
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
  unsigned int zSteps = zDist * Z_AXIS_STEPS_PER_MM;
  return max(max(xSteps, ySteps), zSteps);
}

bool Control::PosKnown() {
  return xKnown && yKnown && zKnown;
}

void Control::Dispatch() {
  if (/*!instructions.Empty()*/ true) {
    INSTSET instSet = instruction;
    //INSTSET instSet = instructions.Pop();
    instSet.xAxis.Print("X Axis");
    instSet.yAxis.Print("Y Axis");
    instSet.zAxis.Print("Z Axis");
    SendInst(X_AXIS_ADDR, instSet.xAxis);
    SendInst(Y_AXIS_ADDR, instSet.yAxis);
    SendInst(Z_AXIS_ADDR, instSet.zAxis);
    delay(1000);
    if (instSet.flags & OPT_FLAG::DRIVE) {
      for (unsigned int i = 0; i < instSet.maxSteps; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delay(1);
        digitalWrite(STEP_PIN, LOW);
        delay(1);
      }
    }
  }
}

INSTSET Control::G0(CMD& cmd) {
  INSTSET instSet = G1(cmd);
  instSet.flags |= OPT_FLAG::RAPID;
  return instSet;
}

INSTSET Control::G1(CMD& cmd) {
  INSTSET instSet;
  instSet.Clear();
  double xDist = 0;
  double yDist = 0;
  double zDist = 0;
  if (!PosKnown()) {
    return instSet;
  }
  if (cmd.ParamAt('X')->set) {
    instSet.xAxis.opt = OPT::MOVE;
    xDist = cmd.ParamAt('X')->value - xPos;
    instSet.xAxis.value = xDist;
    instSet.flags |= OPT_FLAG::DRIVE;
  }
  if (cmd.ParamAt('Y')->set) {
    instSet.yAxis.opt = OPT::MOVE;
    yDist = cmd.ParamAt('Y')->value - yPos;
    instSet.yAxis.value = yDist;
    instSet.flags |= OPT_FLAG::DRIVE;
  }
  if (cmd.ParamAt('Z')->set) {
    instSet.zAxis.opt = OPT::MOVE;
    zDist = cmd.ParamAt('Z')->value - zPos;
    instSet.zAxis.value = zDist;
    instSet.flags |= OPT_FLAG::DRIVE;
  }
  instSet.maxSteps = CalcMaxSteps(xDist, yDist, zDist);
  instSet.xAxis.steps = instSet.yAxis.steps = instSet.zAxis.steps = instSet.maxSteps;
  xPos = cmd.ParamAt('X')->value;
  yPos = cmd.ParamAt('Y')->value;
  zPos = cmd.ParamAt('Z')->value;
  return instSet;
}

INSTSET Control::G28(CMD& cmd) {
  INSTSET instSet;
  instSet.Clear();
  if (cmd.ParamAt('X')->set) {
    instSet.xAxis.opt = OPT::HOME;
    xKnown = true;
    xPos = 0.0;
  }
  if (cmd.ParamAt('Y')->set) {
    instSet.yAxis.opt = OPT::HOME;
    yKnown = true;
    yPos = 0.0;
  }
  if (cmd.ParamAt('Z')->set) {
    instSet.zAxis.opt = OPT::HOME;
    zKnown = true;
    zPos = 0.0;
  }
  if (!cmd.ParamAt('X')->set && !cmd.ParamAt('Y')->set && !cmd.ParamAt('Z')->set) {
    instSet.xAxis.opt = OPT::HOME;
    instSet.yAxis.opt = OPT::HOME;
    instSet.zAxis.opt = OPT::HOME;
    xKnown = yKnown = zKnown = true;
    xPos = yPos = zPos = 0.0;
  }
  return instSet;
}

Control::~Control() {
}
