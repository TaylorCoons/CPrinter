#ifndef Control_h
#define Control_h

#include <Arduino.h>
#include <Wire.h>

#include "Cmds.h"
#include "Comm.h"

/*
 * Class to provide logical control from g-code command
 */

class Control {

  /* Variables */
  private:
  static const unsigned int MAX_CMDS = 5;
  CMD cmds[MAX_CMDS];
  int cmdIndex = 0;
  INST stagedPacket;
  
  /* Methods */
  private:
  INST Interpret(CMD cmd);

  void Write(unsigned int x);
  void Write(unsigned long int x);
  void Write(double x);
    
  public:
  Control();
  void Queue(CMD cmd);
  void Dispatch();
  ~Control();
};


#endif
