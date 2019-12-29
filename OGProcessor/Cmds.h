#ifndef Cmds_h
#define Cmds_h

struct FLAG {
  char flag;
  bool set;
  FLAG() {
    Clear();
  }
  FLAG(char flag) {
    Clear();
    this->flag = flag;
  }
  void Set() {
    set = true;
  }
  void Clear() {
    flag = '\0';
    set = false;
  }
};

struct PARAM {
  char param;
  bool set;
  double value;
  PARAM() {
    Clear();
  }
  PARAM(char param) {
    Clear();
    this->param = param;
  }
  void Set(double value) {
    this->value = value;
    set = true;
  }
  void Clear() {
    param = '\0';
    this->value = 0.0;
    set = false;
  }
};

struct CMD {
  char addr;
  unsigned int cmdNum;
  static const unsigned int MAX_PARAMS = 5;
  static const unsigned int MAX_FLAGS = 5;
  unsigned int numParams;
  unsigned int numFlags;
  PARAM params[MAX_PARAMS];
  FLAG flags[MAX_FLAGS];
  CMD() {
    Clear();
  }
  CMD(char addr, unsigned int cmdNum) {
    Clear();
    this->cmdNum = cmdNum;
    if (addr == 'M') {
      this->addr = addr;
      switch(cmdNum) {
        
      }
    } else if (addr == 'G') {
      this->addr = addr;
      switch(cmdNum) {
        case 0: // Linear move (Non-extrusion)
          params[0].param = 'F'; // Maximum feedrate
          params[1].param = 'E'; // Length of filament to feed between start and end
          params[2].param = 'X'; // X coordinate
          params[3].param = 'Y'; // Y coordinate
          params[4].param = 'Z'; // Z coordinate
          numParams = 5;
          numFlags = 0;
          break;
        case 1: // Linear move (Extrusion)
          params[0].param = 'F'; // Maximum feedrate
          params[1].param = 'E'; // Length of filament to feed between start and end
          params[2].param = 'X'; // X coordinate
          params[3].param = 'Y'; // Y coordinate
          params[4].param = 'Z'; // Z coordinate
          numParams = 5;
          numFlags = 0;
          break;
        case 28: // Auto home
          flags[0].flag = 'O'; // Optional if position is known
          flags[1].flag = 'R'; // Raise before homing distance
          flags[2].flag = 'X'; // X axis
          flags[3].flag = 'Y'; // Y axis
          flags[4].flag = 'Z'; // Z axis
          numParams = 0;
          numFlags = 5;
        break;
      }
    }
  }
  
  void Clear() {
    cmdNum = 0;
    addr = '\0';
    numParams = 0;
    numFlags = 0;
    for (unsigned int i = 0; i < MAX_PARAMS; i++) {
      params[i].Clear();
    }
    for (unsigned int i = 0; i < MAX_FLAGS; i++) {
      flags[i].Clear();
    }
  }
};

#endif
