// Number of relays
int ch = 2;
int relay[] = {9, 3, 18}; // change as required, GPIOs
int a = 0;


// leave as is
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for (int i = 0; i < ch; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < ch; i++) { // pull low the relays you want to turn on
    if (i == 0) {
      digitalWrite(relay[i], LOW);
    } else if (i == 1) {
      digitalWrite(relay[i], LOW);
    } 
    else { // leave as is to turn off all other relays
      digitalWrite(relay[i], HIGH);
    }
  }
}
