#ifndef INST_h
#define INST_h

/*
   Packet structures for sending data across mcus
*/
// Enum to define the operations
enum OPT {
  NOOP,
  MOVE,
  HOME
};

// Enum to define specified flags
enum OPT_FLAG {
  NONE = 0, // No flags
  DRIVE = 2, // Specifies the axis is to be driven by step signal
  RAPID = 4 // Non-Extrude rapid movement
};

// Structure to hold instruction packet to be sent from OGProcessor
struct INST {
  // OPT: operator
  OPT opt;
  // FLAG: instruction flags
  unsigned int flags;
  // VALUE: value associated with instruction (if any)
  double value;
  // STEPS: steps associated with instruction (if any)
  unsigned int steps;

  // Helper to clear structure
  void Clear() {
    opt = NOOP;
    flags = 0;
    value = 0;
    steps = 0;
  }

  // Helper to print structure for debug
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

struct INSTSET {
  INST xAxis;
  INST yAxis;
  INST zAxis;
  unsigned int maxSteps = 0;
  unsigned int flags;
  
  INSTSET() {
    Clear();
  }
  
  void Clear() {
    xAxis.Clear();
    yAxis.Clear();
    zAxis.Clear();
    maxSteps = 0;
    flags = 0;
  }
};


#endif
