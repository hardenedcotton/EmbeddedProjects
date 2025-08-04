#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <BluetoothSerial.h>

#define EN_PIN 23
#define IN_1 19
#define IN_2 18
#define LED_1 15
#define LED_2 2
#define LED_3 0
#define LED_4 4

int received;
int val;
float pwm;

float const GRAVITY = 9.81;
float const OFFSET_Z = 2.65;
float const NORMALIZE_Z = GRAVITY / 10.06;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
BluetoothSerial SerialBT;


void lightLED(int received) {
  int val = abs(received);
  if (val >= 90) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
  } else if (val >= 75) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);
  } else if (val >= 50) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else if (val >= 25) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  }
}

void rotateMotor(int pwm, bool direction) {
  if (direction) {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
  } else {
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
  }
  analogWrite(EN_PIN, pwm);
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin();

  if (!accel.begin()) {
    Serial.println("No ADXL345 detected!");
    while (1);
  }

  accel.setDataRate(ADXL345_DATARATE_25_HZ);
  accel.setRange(ADXL345_RANGE_2_G);
  Serial.println("Setup Complete");

  pinMode(EN_PIN, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);
  float acc_z = (event.acceleration.z - OFFSET_Z) * NORMALIZE_Z;

  if (SerialBT.available()) {
    received = SerialBT.readString().toInt();
    Serial.println(received);
  }


  bool direction = false;
  if (received < 0) direction = true;

  val = abs(received);
  pwm = val*1.94+62;

  
  rotateMotor(pwm, direction);
  lightLED(val);

  if (acc_z < 9) rotateMotor(0, direction);
  
  delay(40);
}
