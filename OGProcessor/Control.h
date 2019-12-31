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
  // TODO: Remove
  QBuffer<CMD> cmds;
  INST xAxisInst;
  INST yAxisInst;
  INST zAxisInst;
  unsigned int flags;
  unsigned int maxSteps;
  const unsigned int STEP_PIN = 5;
  
  /* Methods */
  private:
  void Interpret(CMD cmd);

  void Write(unsigned int x);
  void Write(unsigned long int x);
  void Write(double x);

  void SendInst(uint8_t addr, INST inst);

  unsigned int CalcMaxSteps(double xDist, double yDist, double zDist);
    
  public:
  Control();
  void Queue(CMD cmd);
  void Dispatch();
  void Execute();
  ~Control();
};


#endif
