#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "your-ssid";
const char *password = "your-password";

ESP8266WebServer server(80);
unsigned long startTime;
const unsigned long runTime = 5 * 60 * 1000;  // 5 minutes in milliseconds

float dummyTemps[] = {23.7, 25.8, 27.3, 22.1};  // Dummy values for current temperatures

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Temperature Comparison</h1>";
  html += "<form action='/result' method='GET'>";
  html += "Preferred Temperature: <input type='text' name='preferredTemp'><br><br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form></body></html>";
  server.send(200, "text/html", html);
}

void handleResult() {
  String preferredTempStr = server.arg("preferredTemp");
  float preferredTemp = preferredTempStr.toFloat();
  
  String result = "<html><body>";
  result += "<h1>Temperature Differences</h1>";
  result += "Preferred Temperature: " + String(preferredTemp) + " &#8451;<br><br>";
  result += "<h2>Differences:</h2>";
  
  for (size_t i = 0; i < sizeof(dummyTemps) / sizeof(dummyTemps[0]); ++i) {
    float temperatureDifference = preferredTemp - dummyTemps[i];
    result += "Simulated Current Temperature " + String(i + 1) + ": " + String(dummyTemps[i]) + " &#8451;<br>";
    result += "Difference: " + String(temperatureDifference) + " &#8451;<br><br>";
  }
  result += "</body></html>";
  server.send(200, "text/html", result);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/result", HTTP_GET, handleResult);

  server.begin();
  Serial.println("HTTP server started");

  startTime = millis();  // Start the timer
}

void loop() {
  server.handleClient();

  unsigned long currentTime = millis();
  if (currentTime - startTime >= runTime) {
    server.stop();  // Stop the server after 5 minutes
    WiFi.disconnect(true);  // Disconnect Wi-Fi
    ESP.deepSleep(runTime * 1000);  // Set ESP8266 to sleep for the remaining time
  }
}
