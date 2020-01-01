#ifndef Cmds_h
#define Cmds_h

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
  static const unsigned int MAX_PARAMS = 26;
  PARAM params[MAX_PARAMS];
  
  CMD() {
    Clear();
  }
  
  CMD(char addr, unsigned int cmdNum) {
    Clear();
    this->cmdNum = cmdNum;
    this->addr = addr;
    for (unsigned int i = 0; i < MAX_PARAMS; i++) {
      params[i] = PARAM('A' + i);
    }
  }

  PARAM* ParamAt(char addr) {
    if (addr >= 'A' && addr <= 'Z') {
      return &params[addr - 'A'];
    }
    return NULL;
  }
  
  void Clear() {
    cmdNum = 0;
    addr = '\0';
    for (unsigned int i = 0; i < MAX_PARAMS; i++) {
      params[i].Clear();
    }
  }
};

#endif
