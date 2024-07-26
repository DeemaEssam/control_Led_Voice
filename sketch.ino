
#include <WiFi.h>
#include <HTTPClient.h>

const int relayPin = 23;
const int ledPin = 22;  

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverURL = "https://s-m.com.sa/r2/test/i.html";

void setup() {
  pinMode(relayPin, OUTPUT); // Set relay pin as output
  pinMode(ledPin, OUTPUT);   // Set LED pin as output
  digitalWrite(relayPin, LOW); // Ensure relay is off at startup
  digitalWrite(ledPin, LOW);   // Ensure LED is off at startup

  Serial.begin(115200); // Start serial communication for debugging

  WiFi.begin(ssid, password); // Connect to WiFi

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL); // Specify the URL
    int httpCode = http.GET(); 

    if (httpCode > 0) {
      String payload = http.getString();

      payload.trim();

      // Check if the response contains specific keywords
      if (payload.indexOf("off") >= 0) {
        // Turn the relay and LED off
        digitalWrite(relayPin, LOW);
        digitalWrite(ledPin, LOW);
      } else if (payload.indexOf("on") >= 0) {
        // Turn the relay and LED on
        digitalWrite(relayPin, HIGH);
        digitalWrite(ledPin, HIGH);
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end(); // Free resources
  } else {
    Serial.println("WiFi not connected");
  }

  delay(5000); // Wait for 5 seconds before checking again
}
