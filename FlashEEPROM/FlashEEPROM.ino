#include <EEPROM.h>

/* 
 *  Write Encoded GCode command templates to EEPROM
 */

#define EXTRACT_BYTE(x, i) static_cast<char>(x >> (8 * i))
#define BYTES_TO_ULINT(a, b, c, d) static_cast<unsigned long int>(a) << 24 | \
                                static_cast<unsigned long int>(b) << 16 | \
                                static_cast<unsigned long int>(c) << 8 | \
                                static_cast<unsigned long int>(d)
#define BYTES_TO_UINT(a, b) static_cast<unsigned int>(a) << 8 | \
                            static_cast<unsigned int>(b)

struct CMD {
  char addr;
  int num;
  static const unsigned int MAX_OPTIONS = 26;
  char params[MAX_OPTIONS];
  char flags[MAX_OPTIONS];
};

CMD gcodes[] = {
  { 'G', 0, "EFXYZ", "" }, // Linear Move (travel)
  { 'G', 1, "EFXYZ", "" }, // Linear Move (extrude)
  { 'G', 2, "EIJPRXYZ", ""}, // Controlled arc (CCW)
  { 'G', 3, "EIJPRXYZ", ""}, // Controlled arc (CW)
  { 'G', 4, "PS", ""}, // Pause
  { 'G', 5, "EFIJPQXY", ""}, // Bezier cubic spline
  { 'G', 10, "", "S" }, // Retract
  { 'G', 11, "", "" }, // Recover (unretract)
  { 'G', 12, "PRST", ""}, // Clean nozzle
  { 'G', 17, "", ""}, // Select workspace plane (XY)
  { 'G', 18, "", ""}, // Select workspace plane (ZX)
  { 'G', 19, "", ""}, // Select workspace plane (YZ)
  { 'G', 20, "", ""}, // Set inch units
  { 'G', 21, "", ""}, // Set millimeter units
  { 'G', 26, "BCDFHKLOPQRSUXY", "" }, // Mesh Validation Pattern
  { 'G', 27, "P", ""}, // Park toolhead
  { 'G', 28, "ORXYZ", ""}, // Home toolhead
  { 'G', 29, "ABCDEFJLPQRSTVWXYZ", "O" }, // Bed leveling (Automatic)
  { 'G', 29, "IJSXYZ", ""}, // Bedleveling (Manual)
  { 'G', 29, "ABCDEFHIJKLPQRSTUVWXY", ""}, // Bed leveling (Unified)
  
};
unsigned int numGCodes = sizeof(gcodes) / sizeof(CMD);
unsigned int stride = 11;

void FlashEEPROM();

CMD ReadCmd(unsigned int index);

void PrintCmd(CMD cmd);

unsigned long int CompressOptions(char* options);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  char options[CMD::MAX_OPTIONS] = { "EFXYZ" };
  unsigned long int x = CompressOptions(options);
  for (unsigned int i = 0; i < CMD::MAX_OPTIONS; i++) {
    if (options[i] == '\0') {
      Serial.print("\\0");
    } else {
      Serial.print(options[i]);
    }
    Serial.print(' ');
  }
  Serial.println();
  Serial.println(x);
  FlashEEPROM();
  delay(3000);
  for (unsigned int i = 0; i < numGCodes; i++) {
    PrintCmd(ReadCmd(i));
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}


void FlashEEPROM() {
  for (unsigned int i = 0; i < numGCodes; i++) {
    unsigned int k = stride * i;
    EEPROM[k + 0] = gcodes[i].addr;
    EEPROM[k + 1] = EXTRACT_BYTE(gcodes[i].num, 1);
    EEPROM[k + 2] = EXTRACT_BYTE(gcodes[i].num, 0);
    unsigned long int cParams = CompressOptions(gcodes[i].params);
    unsigned long int cFlags = CompressOptions(gcodes[i].flags);
    for (unsigned int j = 0; j < 4; j++) {
      EEPROM[k + 3 + j] = EXTRACT_BYTE(cParams, j);
    }
    for (unsigned int j = 0; j < 4; j++) {
      EEPROM[k + 7 + j] = EXTRACT_BYTE(cFlags, j);
    }
  }
}

CMD ReadCmd(unsigned int index) {
  unsigned int k = index * stride;
  CMD cmd;
  if (k < 1024) {
    cmd.addr = EEPROM[k + 0];
    cmd.num = BYTES_TO_UINT(EEPROM[k + 1], EEPROM[k + 2]);
    unsigned long int cParams = BYTES_TO_ULINT(EEPROM[k + 6], EEPROM[k + 5], EEPROM[k + 4], EEPROM[k + 3]);
    DecompressOptions(cParams, cmd.params);
    unsigned long int cFlags = BYTES_TO_ULINT(EEPROM[k + 10], EEPROM[k + 9], EEPROM[k + 8], EEPROM[k + 7]);
    DecompressOptions(cFlags, cmd.flags);
    return cmd;
  }
  return CMD();
}

unsigned long int CompressOptions(char* options) {
  unsigned long int compressed = 0;
  for (unsigned int i = 0; i < CMD::MAX_OPTIONS; i++) {
    char c = options[i];
    if (c == '\0') {
      continue;
    }
    c = c - 'A';
    compressed |= (1UL << c);
  }
  return compressed;
}

void DecompressOptions(unsigned long int compressed, char* options) {
  unsigned int charIndex = 0;
  for (unsigned int i = 0; i < CMD::MAX_OPTIONS; i++) {
    if (compressed & (1UL << i)) {
      options[charIndex] = static_cast<char>(i) + 'A';
      charIndex++;
    }
  }
  for (unsigned int i = charIndex; i < CMD::MAX_OPTIONS; i++) {
    options[i] = '\0';
  }
}

void PrintCmd(CMD cmd) {
  Serial.print(cmd.addr);
  Serial.print(cmd.num);
  Serial.print(" [ ");
  for (unsigned int i = 0; i < CMD::MAX_OPTIONS; i++) {
    if (cmd.params[i] != '\0') {
      Serial.print(cmd.params[i]);
      Serial.print(' ');
    }
  }
  Serial.print("][ ");
  for (unsigned int i = 0; i < CMD::MAX_OPTIONS; i++) {
    if (cmd.flags[i] != '\0') {
      Serial.print(cmd.flags[i]);
      Serial.print(' ');
    }
  }
  Serial.print("]");
  Serial.println();
}
