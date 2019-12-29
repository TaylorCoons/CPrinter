#include "Parser.h"

Parser::Parser() {

}

void Parser::Format(String* line) {
  line->trim();
  for (unsigned int i = 0; i < line->length(); i++) {
    if (line->charAt(i) == ' ' && line->charAt(i - 1) == ' ') {
      line->remove(i - 1, 1);
      i--;
    } else if (line->charAt(i) == '\t') {
      line->setCharAt(i, ' ');
    }
  }
}

unsigned int Parser::TokenCount(String line) {
  unsigned int tokenCount = 1;
  for (unsigned int i = 0; i < line.length(); i++) {
    if (line.charAt(i) == ' ') {
      tokenCount++;
    }
  }
  return tokenCount;
}

String Parser::TokenAt(unsigned int index, String line) {
  int tokenStart = 0;
  for (unsigned int i = 0; i < index; i++) {
    tokenStart = line.indexOf(' ', tokenStart) + 1;
  }
  int tokenEnd = line.indexOf(' ', tokenStart);
  if (tokenEnd == -1) {
    tokenEnd = line.length();
  }
  return line.substring(tokenStart, tokenEnd);
}

CMD Parser::Parse(String line) {
  Format(&line);
  unsigned int tokenCount = TokenCount(line);
  if (tokenCount == 0) {
    return CMD();
  }
  String token = TokenAt(0, line);
  char addr = token.charAt(0);
  Serial.print("token: "); Serial.println(token);
  Serial.print("substring: "); Serial.println(token.substring(1));
  Serial.print("cmdNum: "); Serial.println(token.substring(1).toInt());
  unsigned int cmdNum = token.substring(1).toInt();
  CMD cmd(addr, cmdNum);
  for (unsigned int i = 1; i < tokenCount; i++) {
    token = TokenAt(i, line);
    char c = token.charAt(0);
    for (unsigned int i = 0; i < cmd.numParams; i++) {
      if (c == cmd.params[i].param) {
        cmd.params[i].Set(token.substring(1).toDouble());
      }
    }
    for (unsigned int i = 0; i < cmd.numFlags; i++) {
      if (c == cmd.flags[i].flag) {
        cmd.flags[i].Set();
      }
    }
  }
  return cmd;
}

Parser::~Parser() {

}
