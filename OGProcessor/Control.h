#ifndef Control_h
#define Control_h

#include <Arduino.h>
#include <Wire.h>

#include "Cmds.h"
#include "INST.h"
#include "SlaveAddr.h"
#include "QBuffer.h"
#include "AxisDefines.h"

/*
 * Class to provide logical control from g-code command
 */
class Control {

  /* Variables */
  private:
  INSTSET instruction;
  //QBuffer<INSTSET> instructions;
  const unsigned int STEP_PIN = 5;
  double xPos = 0.0;
  double yPos = 0.0;
  double zPos = 0.0;
  bool xKnown = false;
  bool yKnown = false;
  bool zKnown = false;
  
  /* Methods */
  private:
  INSTSET Interpret(CMD& cmd);

  void Write(unsigned int x);
  void Write(unsigned long int x);
  void Write(double x);

  void SendInst(uint8_t addr, const INST& inst);

  unsigned int CalcMaxSteps(double xDist, double yDist, double zDist);

  bool PosKnown();

  void BlockOnAxis(unsigned int axisAddr, unsigned int pollTime = 200);
  
  // All the GCode Ops
  INSTSET G0(CMD& cmd);
  INSTSET G1(CMD& cmd);
  INSTSET G28(CMD& cmd);
  
  public:
  Control();
  void Queue(CMD& cmd);
  void Dispatch();
  ~Control();
};


#endif
