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
    if (addr == 'M') {
      this->addr = addr;
      switch(cmdNum) {
        
      }
    } else if (addr == 'G') {
      this->addr = addr;
      switch(cmdNum) {
        case 0:
          params[0].param = 'F';
          params[1].param = 'E';
          params[2].param = 'X';
          params[3].param = 'Y';
          params[4].param = 'Z';
          numParams = 5;
          numFlags = 0;
          break;
        case 1:
          params[0].param = 'F';
          params[1].param = 'E';
          params[2].param = 'X';
          params[3].param = 'Y';
          params[4].param = 'Z';
          numParams = 5;
          numFlags = 0;
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
