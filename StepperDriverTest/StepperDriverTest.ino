
const unsigned int DIR_PIN = 2;
const unsigned int STEP_PIN = 3;

void Step();

void Direction(bool);

void setup() {
  // put your setup code here, to run once:
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN, LOW);
}

bool clockwise = false;
void loop() {
  // put your main code here, to run repeatedly:
  for (unsigned int i = 0; i < 100; i++) {
    Step();
    delay(10);
  }
  delay(1000);
  clockwise = !clockwise;
  Direction(clockwise);
}

void Step() {
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(STEP_PIN, LOW);
}

void Direction(bool clockwise) {
  digitalWrite(DIR_PIN, clockwise);
}
