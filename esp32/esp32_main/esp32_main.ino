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

/*
    1 = f0
    2 = noteFadeInTime
    3 = noteSustainTime
    4 = noteFadeOutTime
    5 = chordProgressionStyle
    6 = chordPlayStyle
    7 = chordFamily
  */
String CONTRACT_HASH = "0xc82f07b958Fd66f82eCa7e7691db0D0a176B2A15";
String paramURLs[7] = {
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0xd6d7d066&tag=latest&apikey=MY_API_KEY", // 1 = f0
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0x9bdb6c38&tag=latest&apikey=MY_API_KEY", // 2 = noteFadeInTime
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0x7e43b581&tag=latest&apikey=MY_API_KEY", // 3 = noteSustainTime
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0x0eb64081&tag=latest&apikey=MY_API_KEY", // 4 = noteFadeOutTime
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0x2a13ccf6&tag=latest&apikey=MY_API_KEY", // 5 = chordProgressionStyle
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0xdfd1b7d7&tag=latest&apikey=MY_API_KEY", // 6 = chordPlayStyle
  "https://api-ropsten.etherscan.io/api?module=proxy&action=eth_call&to=CONTRACT_HASH&data=0xfc95ebad&tag=latest&apikey=MY_API_KEY" // 7 = chordFamily
};
int idValues[7] = { -1, -1, -1, -1, -1, -1, -1 }; // will hold previous values to check for change

DynamicJsonDocument doc(2048);

void setup() {
  // setup Serial
  Serial.begin(115200); // Serial.begin value for my ESP32
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2); // used for writing data to Teensy
  
  // connect to WiFi
  connect();

  for (int i = 0; i < idPinsAmount; i++) {
    pinMode(idPins[i], OUTPUT);
  }

  for (int i = 0; i < 7; i++) {
    paramURLs[i].replace("CONTRACT_HASH", CONTRACT_HASH);
    paramURLs[i].replace("MY_API_KEY", apiKey);
  }

  live = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    getAllContractValues();
  } else {
    Serial.println("Wifi disconnected, attempting to reconnect...");
    connect();
   }
  delay(2000);
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

/*
    1 = f0
    2 = noteFadeInTime
    3 = noteSustainTime
    4 = noteFadeOutTime
    5 = chordProgressionStyle
    6 = chordPlayStyle
    7 = chordFamily
  */
void getAllContractValues() {
  for (int i = 0; i < 7; i++) {
    // get value from web
    Serial.printf("Retrieving value for ID %i\n", i);
    int value = getContractValue(paramURLs[i]);
    delay(1500);
    if (value == -1) continue; // skip if invalid value

    // send value to Teensy if there is a difference
    if (value != idValues[i]) {
      Serial.printf("Prev value for ID %i was %i, new value is %i, send change to Teensy\n", i, idValues[i], value);
      idValues[i] = value;
      transmitData(i, value);
    }

    delay(500);
  }
}

int getContractValue(String url) {
  char buffer[200];
  url.toCharArray(buffer, 200);
  Serial.printf("Getting value from url %s\n", buffer);
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.println("Error connecting to " + url);
    return -1;
  }

  // get the value from the string payload using JSON and stuff
  deserializeJson(doc, http.getStream());
  String valString = doc["result"].as<String>();
  char buffer2[valString.length()+1];
  valString.toCharArray(buffer2, valString.length()+1);
  char *ptr;
  int value = strtol(buffer2, &ptr, 16); // convert the hex string to int
  Serial.printf("Raw Received Value: %s\n", buffer2);
  Serial.printf("Converted Received Value: %i\n", value);


  http.end();
  return value;
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
  Serial2.write(value & 0xFF);
  Serial2.write((value >> 8) & 0xFF);
  Serial2.write((value >> 16) & 0xFF);
  Serial2.write((value >> 24) & 0xFF);
  Serial.println("-------------------------");
}
