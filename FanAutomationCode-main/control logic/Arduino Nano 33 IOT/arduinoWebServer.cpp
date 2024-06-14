#include <WiFiNINA.h>
#include <WiFiServer.h>

char ssid[] = "your-ssid";
char pass[] = "your-password";

WiFiServer server(80);
unsigned long startTime;
const unsigned long runTime = 5 * 60 * 1000; // 5 minutes in milliseconds

float dummyTemps[] = {23.7, 25.8, 27.3, 22.1}; // Dummy values for current temperatures

void setup() {
  Serial.begin(9600);
  while (!Serial);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.begin();
  Serial.println("HTTP server started");

  startTime = millis(); // Start the timer
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        client.flush();

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();

        if (request.indexOf("/result") != -1) {
          String preferredTempStr = request.substring(request.indexOf('?') + 1);
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
          client.print(result);
        } else {
          String html = "<html><body>";
          html += "<h1>Temperature Comparison</h1>";
          html += "<form action='/result' method='GET'>";
          html += "Preferred Temperature: <input type='text' name='preferredTemp'><br><br>";
          html += "<input type='submit' value='Submit'>";
          html += "</form></body></html>";
          client.print(html);
        }
        client.stop();
        server.stop(); // Stop the server after handling the request
        WiFi.end(); // Disconnect Wi-Fi
        
        unsigned long currentTime = millis();
        if (currentTime - startTime >= runTime) {
          while (true) {
            // Enter a low-power state or perform any necessary power-saving actions here
          }
        }
      }
    }
  }
}
