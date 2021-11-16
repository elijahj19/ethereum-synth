#include <WiFi.h>
#include <MyWifiConfig.h> // My own wifi config library
#include <HTTPClient.h>

// contains URLs to check contract variables
#include "MyContractVariables.h"

void setup() {
  Serial.begin(115200); // Serial.begin value for my ESP32
  // connect to WiFi
  connect();
}


// connect to wifi network defined in MyWifiConfig.h
void connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to ");
  Serial.println(SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(800);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.print("Connected to ");
  Serial.println(SSID);
}

void loop() {
  // put your main code here, to run repeatedly:

}
