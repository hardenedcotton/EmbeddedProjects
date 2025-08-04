#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define LED_R 4
#define LED_G 5
#define LED_B 18
#define LED_C 19
#define BUZZER 17
#define POWER 23

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

float const GRAVITY = 9.81;

float const TOLERANCE_LOW = .5;
float const TOLERANCE_MID = 3.0;
float const TOLERANCE_HIGH = 12.0;

float const OFFSET_X = .51;
float const OFFSET_Y = 0;
float const OFFSET_Z = 2.65;

float const NORMALIZE_X = GRAVITY / 10.28;
float const NORMALIZE_Y = GRAVITY / 10.24;
float const NORMALIZE_Z = GRAVITY / 10.06;


float buffer() {
  sensors_event_t event;

  float a = 0;

  float acc_x;
  float acc_y;
  float acc_z;

  for (int i = 0; i < 5; i++) {
    accel.getEvent(&event);
    acc_x = (event.acceleration.x - OFFSET_X) * NORMALIZE_X;
    acc_y = (event.acceleration.y - OFFSET_Y) * NORMALIZE_Y;
    acc_z = (event.acceleration.z - OFFSET_Z) * NORMALIZE_Z;

    a += abs(acc_x) + abs(acc_y) + abs(acc_z);

    delay(10);
  }
  a /= 5;
  return a;
}


void detectAccel(float a0, float a1) {
  if (a0 > a1 + TOLERANCE_HIGH || a0 < a1 - TOLERANCE_HIGH) {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
    analogWrite(BUZZER, 127);
  } else if (a0 > a1 + TOLERANCE_MID || a0 < a1 - TOLERANCE_MID) {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, HIGH);
  } else  if (a0 > a1 + TOLERANCE_LOW || a0 < a1 - TOLERANCE_LOW) {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
  } else {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
    analogWrite(BUZZER, 0);
  }
}




void setup() {
  Serial.begin(115200);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);
  digitalWrite(LED_C, HIGH);

  if (!accel.begin()) {
    Serial.println("No ADXL345 detected!");
    while (1);
  }

  accel.setDataRate(ADXL345_DATARATE_100_HZ);
  accel.setRange(ADXL345_RANGE_8_G);

  Serial.println("Setup Complete!");
}


void loop() {
  float a0 = buffer();
  delay(1);
  float a1 = buffer();

  Serial.print(a0); Serial.print("\t");
  Serial.print(a1); Serial.println();

  detectAccel(a0, a1);
}
