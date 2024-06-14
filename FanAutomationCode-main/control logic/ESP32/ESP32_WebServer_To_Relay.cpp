/**
TEST CODE TO CHECK APP <--> FAN RESPONSIVENESS
*/

// Load Wi-Fi library
#include <WiFi.h>
#include <Wire.h>

// Network credentials
const char* ssid = "iPhone";
const char* password = "Rahil870";

// Relay GPIOs
int ch = 4;
int relay[] = {3, 8, 18, 17};

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// WiFi timeout
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

unsigned long lastTrigger = millis();

void setup() {
  Serial.begin(115200);

  // Set up relays
  for (int i = 0; i < ch; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
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
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";              
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected

      currentTime = millis();

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

            // SETTING 1
            if (header.indexOf("GET /One") >= 0) {
              Serial.println("Setting 1 on");
              for (int i = 0; i < ch; i++) {
                digitalWrite(relay[i], HIGH);
              }
            }
            
            
            // SETTING 2
            else if (header.indexOf("GET /Two") >= 0) {
              Serial.println("Setting 2 off");
              for (int i = 0; i < ch; i++) {
                if (i == 0) {
                  digitalWrite(relay[i], LOW);
                }
                else {
                  digitalWrite(relay[i], HIGH);
                }
              }
            } 
            
            // SETTING 3
            else if (header.indexOf("GET /Three") >= 0) {
              Serial.println("Setting 3 on");
              for (int i = 0; i < ch; i++) {
                if (i == 2) {
                  digitalWrite(relay[i], LOW);
                }
                else if (i == 1) {
                  digitalWrite(relay[i], LOW);
                }
                else {
                  digitalWrite(relay[i], HIGH);
                }
              }
            } 
            
            // SETTING 4
            else if (header.indexOf("GET /Four") >= 0) {
              Serial.println("Setting 4 off");
              for (int i = 0; i < ch; i++) {
                if (i == 1) {
                  digitalWrite(relay[i], LOW);
                }
                else {
                  digitalWrite(relay[i], HIGH);
                }
              }
            } 
            
            // ECO = TURN OFF FAN FOR NOW
            else if (header.indexOf("GET /Eco") >= 0) {
              for (int i = 0; i < ch; i++) {
                if (i == 2) {
                  digitalWrite(relay[i], LOW);
                }
                else {
                  digitalWrite(relay[i], HIGH);
                }
              }
            }
            
            // WEB PAGE OPTION FOR NON-APP TESTING
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
