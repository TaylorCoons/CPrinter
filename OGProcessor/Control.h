#ifndef Control_h
#define Control_h

#include <Arduino.h>
#include <Wire.h>

#include "Cmds.h"
#include "Comm.h"
#include "SlaveAddr.h"
#include "AxisDefines.h"

/*
 * Class to provide logical control from g-code command
 */

class Control {

  /* Variables */
  private:
  static const unsigned int MAX_CMDS = 5;
  CMD cmds[MAX_CMDS];
  int cmdIndex = 0;
  INST xAxisInst;
  INST yAxisInst;
  
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
  ~Control();
};


#endif
