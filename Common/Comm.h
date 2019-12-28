#ifndef Comm_h
#define Comm_h

/*
   Packet structures for sending data across mcus
*/
enum OPT {
  NOOP,
  MOVE,
  HOME
};

enum OPT_FLAG {
  NONE
};

struct INST {
  OPT opt;
  unsigned int flags;
  double value;
  unsigned int steps;
  INST& operator=(const INST& other) {
    if (this != &other) {
      this->opt = other.opt;
      this->flags = other.flags;
      this->value = other.value;
      this->steps = other.steps;
    }
  }
};


#endif
