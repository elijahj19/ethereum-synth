#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "MyWifiConfig.h" // My own wifi config library, hidden to protect credentials
#include "MyContractVariables.h" // contains URLs to check contract variables
#include "EtherScanAPIKeys.h" // contains etherscan api key, hidden to protect credentials

HTTPClient http;

bool live = true; // variable determining whether instrument should run

// array of digital write pins that will indicate id of value being sent to Teensy
#define idPinsAmount 4
int idPins[] = { 27, 14, 12, 13 };

#define RX2 16 // available pin for reading serial data
#define TX2 17 // available pin for writing/transmitting serial data

void setup() {
  // setup Serial
  Serial.begin(115200); // Serial.begin value for my ESP32
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2); // used for writing data to Teensy
  
  // connect to WiFi
  //connect();

  for (int i = 0; i < idPinsAmount; i++) {
    pinMode(idPins[i], OUTPUT);
  }

  live = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  while (live) {
    for (int i = 0; i < 16; i++) {
      transmitData(i, i+100);
      delay(3000);
    }
    // try to reconnect if disconnected
//    if (WiFi.status() != WL_CONNECTED) {
//      Serial.println("Wifi disconnected, attempting to reconnect...");
//      connect();
//    }

//    int a = getContractValue("https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=0x0C3F3f79cf954eE84CA55488479A76cfC910a3e2&data=0xd46300fd&apikey=M2N2X5CFGFMQSBSWGXIAA5ID4XD6136H38");

//    delay(15000); // delay 2 seconds
  }
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

int getContractValue(String url) {
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  } else {
    Serial.println("Error connecting to " + url);
  }

  http.end();
  return httpCode;
}

// this function will send data to Teensy
void transmitData(int id, int value) {
  Serial.println("Attempting to send");
  Serial.print("ID: ");
  Serial.println(id);
  Serial.print("Value: ");
  Serial.println(value);
  // send digital writes to indicate id of value to Teensy
  for (int i = 0; i < idPinsAmount; i++) {
    byte state = bitRead(id, i);
    digitalWrite(idPins[i], state);
  }
  
  // send actual value via serial port to Teensy
  Serial2.write(value);
  Serial.println("-------------------------");
}
