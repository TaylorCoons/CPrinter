#ifndef Control_h
#define Control_h

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
  //COMM::PACKET stagedPacket;
  
  /* Methods */
  private:
  //COMM::PACKET Interpret(CMD cmd);
    
  public:
  Control();
  void Queue(CMD cmd);
  void Dispatch();
  ~Control();
};


#endif
