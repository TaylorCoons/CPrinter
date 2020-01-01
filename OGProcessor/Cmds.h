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
  unsigned int num;
  unsigned int subNum;
  static const unsigned int MAX_PARAMS = 26;
  PARAM params[MAX_PARAMS];
  
  CMD() {
    Clear();
  }
  
  CMD(char addr, unsigned int num, unsigned int subNum = 0) {
    Clear();
    this->num = num;
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
    num = 0;
    subNum = 0;
    addr = '\0';
    for (unsigned int i = 0; i < MAX_PARAMS; i++) {
      params[i].Clear();
    }
  }
};

#endif
