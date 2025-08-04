#include <BluetoothSerial.h>

#define LED_R 15
#define LED_G 2
#define LED_B 0

uint8_t red;
uint8_t green;
uint8_t blue;

BluetoothSerial SerialBT;
String received;

void hexToRGB(String hex, uint8_t& red, uint8_t& green, uint8_t& blue) {
  // Code from "Deneyap_RenkDonusturme.h" library,

  long number;
  if (hex[0] == '#') number = strtol(&hex[1], nullptr, 16);
  else number = strtol(&hex[0], nullptr, 16);
  red = number >> 16;
  green = number >> 8 & 0xFF;
  blue = number & 0xFF;
}

void lightRGB(uint8_t& red, uint8_t& green, uint8_t& blue, String com = "a") {
  uint8_t redIn = red;
  uint8_t greenIn = green;
  uint8_t blueIn = blue;
  if (com == "a") {
    redIn = 255 - red;
    greenIn = 255 - green;
    blueIn = 255 - blue;
  }
  if (received) {
    analogWrite(LED_R, redIn);
    analogWrite(LED_G, greenIn);
    analogWrite(LED_B, blueIn);
    Serial.print(red); Serial.print(", ");
    Serial.print(green); Serial.print(", ");
    Serial.println(blue);
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin();
  Serial.println("Bluetooth Active");
}

void loop() {
  if (SerialBT.available()) {
    received = SerialBT.readString();
    Serial.println(received);
  }
  hexToRGB(received, red, green, blue);
  lightRGB(red, green, blue, "a");
  delay(50);
}
