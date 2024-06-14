/**
According to WHO, ideal indoor temperature should be between 18 to 24 degrees Celsius
Therefore, we will be using the median of 21 degrees Celsius in calculations.
Setting 1: 18 C
Setting 2: 19 C
Setting 3: 22 C
Setting 4: 23 C
Auto: 21 C 
*/

/**
Setting 1: 3
Setting 2: 2
Setting 3: 2, 3
Setting 4: 1
*/

// Load Wi-Fi library
#include <WiFi.h>
#include <Wire.h>

// Load Temp and Humidity sensor library
#include "HIH61xx.h"
// #include "SSD1306Wire.h"
HIH61xx<TwoWire> hih(Wire);
#define BYTES2READ (4)
#define HIH8121addr (0x27)
#define EPOCH (5) // 10 seconds between reads
#define RELAY_WAIT (10)

void delay_seconds(unsigned int);

// Number of relays
int ch = 4;
int relay[] = {34, 33, 21, 20};

// Pushbutton or manual inputs
// OFF, 1, 2, 3, 4, ECO
int pb_count = 6;
int pbs[] = {4, 5, 6, 7, 15, 16};

// FOR HEAT INDEX CALCULATION
const float c1 = -42.379;
const float c2 = 2.04901523;
const float c3 = 10.14333127;
const float c4 = .22475541;
const float c5 = .00683783;
const float c6 = .05481717;
const float c7 = .00122874;
const float c8 = .00085282;
const float c9 = .00000199;

// NETWORK CREDENTIALS
const char* ssid = "iPhone";
const char* password = "Rahil870";

WiFiServer server(80);

// http request
String header;

// Debug LED states
String led1State = "off";
String led2State = "off";

// int led1 = 21;
// int led2 = 20;

// PIR Sensor
// const int outputPIR = 5;
const int sensorPin = 19;

// mmWave sensor
int S1_Pin = 1; // Occupied = HIGH

int SDA_pin = 38;
int SCL_pin = 39;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

unsigned long lastTrigger = millis();
const long PIRTimeout = 60 * 1000; // 60 second timeout for presence detection
int overridePIR = 0;

void setup() {
  Serial.begin(115200);
  // Initializing hardware
  // pinMode(led1, OUTPUT);
  // pinMode(led2, OUTPUT);
  //pinMode(outputPIR, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(S1_Pin, INPUT);

  // Set outputs to LOW
  // digitalWrite(led1, LOW);
  // digitalWrite(led2, LOW);
  //digitalWrite(outputPIR, LOW);

  // set up relays
  for (int i = 0; i < ch; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
  }

  // set up pushbuttons
  for (int j = 0; j < pb_count; j++) {
    pinMode(pbs[j], INPUT);
  }


  // byte state1 = digitalRead(S1_Pin);
  // Serial.print("Presence Detected: ");
  // while (1){
  //   byte state1 = digitalRead(S1_Pin);
  //   if(state1 == 1) {
  //     Serial.println("Yes");
  //   } else {
  //     Serial.println("No");
  //   }
  //   delay(500);
  // }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Wire.begin(SDA_pin, SCL_pin);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // WIFI client
  WiFiClient client = server.available();

  // mmWave read
  byte occupied = digitalRead(S1_Pin);

  bool presence = digitalRead(sensorPin); // PIR read

  // HIH8121 Temp Humidity reading
  int dummydata = 0xff;
  unsigned int sample = 1;
  unsigned int RHraw, Tempraw; // raw RH (%) and temp (c) values
  float RH, TempC, TempF;  // converted RH (%) and Temp (C)
  int HIHdata[4];  // data read from the sensor
  int HIHstatus;  // sensor status bits

  int th_data[4];
  int th_status;
  unsigned int in_humidity, in_temp;
  float humidity, temp, heat_index;
  float desired_HI = 25.0;
  
  // Manual Mode, do not need to connect to Wi Fi Client
  int manual_mode = 0;

  for (int m = 0; m < pb_count; m++) {
    if (pbs[m] == 1) {
      // overridePIR = 1;
      manual_mode = 1;
      if (m == 0) {
        for (int i = 0; i < ch; i++) {
          digitalWrite(relay[i], HIGH);
        }
        delay_seconds(RELAY_WAIT);
      } else if (m == 1) { // Setting 1
        for (int i = 0; i < ch; i++) {
          if (i == 2) {
            digitalWrite(relay[i], LOW);
          } else {
            digitalWrite(relay[i], HIGH);
          }
        }
        delay_seconds(RELAY_WAIT);
      } else if (m == 2) { // Setting 2
        for (int i = 0; i < ch; i++) {
          if (i == 1) {
            digitalWrite(relay[i], LOW);
          } else {
            digitalWrite(relay[i], HIGH);
          }
        }
        delay_seconds(RELAY_WAIT);
      } else if (m == 3) { // Setting 3
        for (int i = 0; i < ch; i++) {
          if (i == 1 || i == 2) {
            digitalWrite(relay[i], LOW);
          } else {
            digitalWrite(relay[i], HIGH);
          }
        }
        delay_seconds(RELAY_WAIT);
      } else if (m == 4) { // Setting 2
        for (int i = 0; i < ch; i++) {
          if (i == 0) {
            digitalWrite(relay[i], LOW);
          } else {
            digitalWrite(relay[i], HIGH);
          }
        }
        delay_seconds(RELAY_WAIT);
      }
    }
  }

  //delay(500);

  // Occupancy detection by PIR -- with manual mode
  if (overridePIR == 0) {
    if (presence) {
      // Serial.println("Found someone!");
      lastTrigger = millis();
      // digitalWrite(outputPIR, HIGH);
    } else if (!presence) {
      // Serial.println("No one here");
      // digitalWrite(outputPIR, LOW);
    }
  }

  // Occupancy switched off if PIR inactive after timeout
  if (millis() - lastTrigger > PIRTimeout) {
    for (int i = 0; i < ch; i++) {
      digitalWrite(relay[i], HIGH);
    }
  }

  // Web Server connection
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";         
    Serial.println("Also testing temperature and humidity sensor");        
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected

      if (overridePIR == 0) {
        if (presence) {
          // Serial.println("Found someone!");
          lastTrigger = millis();
          // digitalWrite(outputPIR, HIGH);
        } else if (!presence) {
          // Serial.println("No one here");
          // digitalWrite(outputPIR, LOW);
        }
      }

      // Fan switched off if PIR inactive after timeout
      if (millis() - lastTrigger > PIRTimeout) {
        for (int i = 0; i < ch; i++) {
          digitalWrite(relay[i], HIGH);
        }
      }

      for (int m = 0; m < pb_count; m++) {
        if (pbs[m] == 1) {
          // overridePIR = 1;
          manual_mode = 1;
          if (m == 0) {
            for (int i = 0; i < ch; i++) {
              digitalWrite(relay[i], HIGH);
            }
            delay_seconds(RELAY_WAIT);
          } else if (m == 1) { // Setting 1
            for (int i = 0; i < ch; i++) {
              if (i == 2) {
                digitalWrite(relay[i], LOW);
              } else {
                digitalWrite(relay[i], HIGH);
              }
            }
            delay_seconds(RELAY_WAIT);
          } else if (m == 2) { // Setting 2
            for (int i = 0; i < ch; i++) {
              if (i == 1) {
                digitalWrite(relay[i], LOW);
              } else {
                digitalWrite(relay[i], HIGH);
              }
            }
            delay_seconds(RELAY_WAIT);
          } else if (m == 3) { // Setting 3
            for (int i = 0; i < ch; i++) {
              if (i == 1 || i == 2) {
                digitalWrite(relay[i], LOW);
              } else {
                digitalWrite(relay[i], HIGH);
              }
            }
            delay_seconds(RELAY_WAIT);
          } else if (m == 4) { // Setting 4
            for (int i = 0; i < ch; i++) {
              if (i == 0) {
                digitalWrite(relay[i], LOW);
              } else {
                digitalWrite(relay[i], HIGH);
              }
            }
            delay_seconds(RELAY_WAIT);
          }
        }
      }
      currentTime = millis();

      // HEAT INDEX CALCULATION
      Wire.beginTransmission(HIH8121addr);
      Wire.write(dummydata);
      Wire.endTransmission();
      delay(50);

      Wire.requestFrom(HIH8121addr, BYTES2READ);
      // With sensor connection:
      HIHdata[0] = Wire.read();
      HIHdata[1] = Wire.read();
      HIHdata[2] = Wire.read();
      HIHdata[3] = Wire.read();

      // Without sensor connection:
      // HIHdata[0] = 1;
      // HIHdata[1] = 1;
      // HIHdata[2] = 1;
      // HIHdata[3] = 1;

      HIHstatus = HIHdata[0] & 0b11000000;  // get status bits
      if (HIHstatus != 0) {
        Serial.println("Status Bits Error!");
      }
      HIHdata[0] = HIHdata[0] & 0b00111111;  //mask status
      Tempraw = (HIHdata[2] * 256) + HIHdata[3];
      Tempraw = Tempraw / 4;  //shift temperature
      RHraw = (HIHdata[0] * 256) + HIHdata[1];
      RH = ((float)RHraw / 16382.0) * 100.0;
      TempC = (((float)Tempraw / 16382.0) * 165.0) - 40;
      TempF = (TempC * 9/5) + 32;
      Serial.print(sample);
      Serial.print("  ");
      // Serial.print(RH);
      // Serial.print("  ");
      // Serial.print(TempC);
      // Serial.println();
      heat_index = c1 + c2*TempF + c3*RH - c4*TempF*RH - c5*TempF*TempF - c6*RH*RH + c7*TempF*TempF*RH + c8*TempF*RH*RH - c9*TempF*TempF*RH*RH; 
      heat_index = (heat_index - 32) * 5/9;
      Serial.print("Heat index: ");
      Serial.println(heat_index);
      sample++;

      // Controlling the Relay according to Heat Index
      float deltaT = heat_index - desired_HI;

      // Only change fan settings if room is occupied, switch off the fan otherwise
      // if (occupied) {
        // Serial.println("Presence detected!");
        // Condition 1: OFF
        if (deltaT < -1.0) {
          Serial.println("Switching off all relays, deltaT < -1.0");
          for (int i = 0; i < ch; i++) {
            digitalWrite(relay[i], HIGH);
          }
          delay_seconds(RELAY_WAIT);
        }

        // Condition 2: Slowest speed
        if (-1.0 <= deltaT < 1.0) {
          Serial.println("Switching on setting 1, -1.0 <= deltaT < 1.0");
          for (int i = 0; i < ch; i++) {
            if (i == 2) {
              digitalWrite(relay[i], LOW);
            } else {
              digitalWrite(relay[i], HIGH);
            }
          }
          delay_seconds(RELAY_WAIT);
        }

        // Condition 3: Mild speed
        if (1.0 <= deltaT < 2.0) {
          Serial.println("Switching on relay 2, 1.0 <= deltaT < 2.0");
          for (int i = 0; i < ch; i++) {
            if (i == 1) {
              digitalWrite(relay[i], LOW);
            } else {
              digitalWrite(relay[i], HIGH);
            }
          }
          delay_seconds(RELAY_WAIT);
        }

        // Condition 4: Medium speed
        if (2.0 <= deltaT < 3.0) {
          Serial.println("Switching on relay 3, 2.0 <= deltaT < 3.0");
          for (int i = 0; i < ch; i++) {
            if (i == 1 || i == 2) {
              digitalWrite(relay[i], LOW);
            } else {
              digitalWrite(relay[i], HIGH);
            }
          }
          delay_seconds(RELAY_WAIT);
        }

        // Condition 5: Full speed
        if (deltaT >= 3.0) {
          Serial.println("Switching on relay 4, 3.0 <= deltaT");
          for (int i = 0; i < ch; i++) {
            if (i == 0) {
              digitalWrite(relay[i], LOW);
            } else {
              digitalWrite(relay[i], HIGH);
            }
          }
          delay_seconds(RELAY_WAIT);
        }
      // } else {
      //   Serial.println("No one's home, switch off fan");
      //   for (int i = 0; i < ch; i++) {
      //       digitalWrite(relay[i], HIGH);
      //   }
      //   delay_seconds(RELAY_WAIT);
      // }

      delay_seconds(EPOCH);


      // Communication with the app
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {                    // check for end of HTTP request
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /One") >= 0) {
              Serial.println("Setting 1 on");
              desired_HI = 23.0;
              led1State = "off";
              led2State = "off";
              // digitalWrite(led1, LOW);
              // digitalWrite(led2, LOW);
            } else if (header.indexOf("GET /Two") >= 0) {
              Serial.println("Setting 2 off");
              desired_HI = 24.0;
              led1State = "on";
              led2State = "off";
              // digitalWrite(led1, HIGH);
              // digitalWrite(led2, LOW);
            } else if (header.indexOf("GET /Three") >= 0) {
              Serial.println("Setting 3 on");
              desired_HI = 26.0;
              led1State = "off";
              led2State = "on";
              // digitalWrite(led1, LOW);
              // digitalWrite(led2, HIGH);
            } else if (header.indexOf("GET /Four") >= 0) {
              Serial.println("Setting 4 off");
              desired_HI = 27.0;
              led1State = "on";
              led2State = "on";
              // digitalWrite(led1, HIGH);
              // digitalWrite(led2, HIGH);
            } else if (header.indexOf("GET /Eco") >= 0) { // Change to Auto
              // overridePIR = 1;
              // digitalWrite(outputPIR, HIGH);
              desired_HI = 25.0;
            } else if (header.indexOf("GET /InHome") < 0) {
              // overridePIR = 0;
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            // Display buttons for each variable
            client.println("<p><a href=\"/One\"><button class=\"button\">Off Both LEDs</button></a></p>");
            client.println("<p><a href=\"/Two\"><button class=\"button\">On 1, Off 2</button></a></p>");
            client.println("<p><a href=\"/Three\"><button class=\"button\">On 2, Off 1</button></a></p>");
            client.println("<p><a href=\"/Four\"><button class=\"button\">On Both LEDs</button></a></p>");
            client.println("<p><a href=\"/InHome\"><button class=\"button\">I'm Home!</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;     
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void delay_seconds(unsigned int mins) {
  unsigned int count;

  for (count = 0; count < mins; count++) {
  //delay(60000);  // delay 60 seconds
  delay(1000); // use this if you want seconds epoch
  }
}
