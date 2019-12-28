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
  
  void Clear() {
    opt = NOOP;
    flags = 0;
    value = 0;
    steps = 0;
  }

  void Print(String instName) {
    Serial.print("<");
    Serial.print(instName);
    Serial.println(">");
    Serial.print("opt: ");
    switch (opt) {
      case NOOP:
        Serial.println("NOOP");
      break;
      case MOVE:
        Serial.println("MOVE");
      break;
      case HOME:
        Serial.println("HOME");
      break;
    }
    Serial.print("flags: ");
    Serial.println(flags);
    Serial.print("value: ");
    Serial.println(value);
    Serial.print("steps: ");
    Serial.println(steps);
  }
};


#endif
