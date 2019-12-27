#ifndef Parser_h
#define Parser_h

#include <Arduino.h>
#include "Cmds.h"

/*
 * GCode parser for the OGProcessor
 */

class Parser {
  /* Variables */
  private:

  /* Methods */
  private:
  
  void Format(String* line);

  unsigned int TokenCount(String line);
  
  String TokenAt(unsigned int index, String line);
  
  public:
  
  Parser();
  
  CMD Parse(String line);
  
  ~Parser();
};

#endif
