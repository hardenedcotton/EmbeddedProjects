#include "arduino_secrets.h"


#include "thingProperties.h"

#include "pins.h"


#include <SPI.h>
#include <MFRC522.h>


MFRC522 rfid(SS_PIN, RST_PIN); 

MFRC522::MIFARE_Key key; 

byte userTag1[] = {0x1C, 0xC3, 0xAF, 0x5E};
byte userTag2[] = {0x22, 0x42, 0x8F, 0xA9};

#include <DHT.h>
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(1);
  ArduinoCloud.printDebugInfo();


  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  
  SPI.begin();
  rfid.PCD_Init();

  dht.begin();
}

void loop() {
  ArduinoCloud.update();
  compareKeycards();
  readTH();
  delay(100);
}



void compareKeycards() {
 if ( ! rfid.PICC_IsNewCardPresent())
    return;
 if ( ! rfid.PICC_ReadCardSerial())
    return;

  if(memcmp(rfid.uid.uidByte, userTag1, 4) == 0) {
    authUser = "Onur Ümit Şener";
  } else if (memcmp(rfid.uid.uidByte, userTag2, 4) == 0) {
    authUser = "Master Key";
  } else {
    authUser = "Undefined Card";
  }
  Serial.println(authUser);
}


void readTH() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    temperatureHumidity = "Failed to read from DHT sensor!";
    return;
  }
  temperatureHumidity = "Temperature: " + String(t) + " \nHumidity: " + String(h);
}

void onMotorSpeedChange()  {
  int received = motorSpeed;
  bool direction = (received < 0) ? true : false;
  int pwm = abs(received)*1.94+62;

  if (direction) {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
  } else {
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
  }
  analogWrite(EN_PIN, pwm);
}


void onLightSwitch1Change()  {
  int light1brightness = lightSwitch1.getValue().bri * 2.55;
  if (lightSwitch1.getSwitch()) {
    analogWrite(LED_1, light1brightness);
  } else {
    analogWrite(LED_1, 0);
  }
}

void onLightSwitch2Change()  {
  int light2brightness = lightSwitch2.getValue().bri * 2.55;
  if (lightSwitch2.getSwitch()) {
    analogWrite(LED_2, light2brightness);
  } else {
    analogWrite(LED_2, 0);
  }
}



