
#define idPinsAmount 4
int idPins[] = { 2, 3, 4, 5 };

void setup() {
  // setup Serial
  Serial.begin(9600);
  Serial1.begin(9600); // for receiving values from ESP32

  
  // setup id pins, to read ID of value to be changed
  for (int i = 0; i < idPinsAmount; i++) {
    pinMode(idPins[i], INPUT);
  }
}

void loop() {
  while(Serial1.available() > 0) {
    // update values
    updateValue();
    
    delay(1000);
  }
}

// get id for data being sent by ESP32
byte getID() {
  byte id = 0;
  for (byte i = 0; i < idPinsAmount; i++) {
    byte nextVal = (digitalRead(idPins[i]) == HIGH) ? 1 : 0;
    id = (id << 1) + nextVal;
  }
  return id;
}

// get value for data being sent by ESP32
void updateValue() {
  // read data being sent by ESP32
  byte id = getID();
  int value = Serial1.read();
  
  Serial.print("ID: ");
  Serial.println(id);
  Serial.print("Value: ");
  Serial.println(value);
  Serial.println(" ");
  
  // actually change values
  // TODO
}
