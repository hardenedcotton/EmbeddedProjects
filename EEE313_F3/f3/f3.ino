/*
7SEG:   10 countdown
Button: Initianlize countdown
RGBLED: Turn green to red as countdown
Buzzer: Beep per second
Motor:  Chronometer
Pot:    Pace of the countdown
LDR:    Frequency of beep
*/

#define buzzer 11

#define a 2
#define b 3
#define c 4
#define d 7
#define e 8
#define f 12
#define g 13

#define pot A0
#define ldr A1
#define button A5

#include <SevSeg.h>  //https://github.com/DeanIsMe/SevSeg
#include <Servo.h>
#include "hsv2rgb.h"

Servo myservo;
SevSeg sevseg;

const int SECONDS = 10;
const int ADC_MAX = 1023;

float rgb[3];
int rgbPins[] = { 5, 6, 9 };

void setColors(int sec) {
  float hue = ((float)sec / SECONDS);
  hsv2rgb(hue, 1, 1, rgb);
  for (int color = 0; color <= 2; color++) {
    int colorPwm = rgb[color] * 256;
    analogWrite(rgbPins[color], colorPwm);
  }
}

void beep() {
  int frequency = analogReadAverage(ldr)*3 + 500;
  Serial.println(frequency);

  tone(buzzer, frequency);
  delay(getSpeed() * 0.2);
  noTone(buzzer);
}

float getSpeed() {
  float secs = 10 + (1000 * ((float)analogReadAverage(pot) / ADC_MAX));
  delay(10);
  return (int)secs;
}

int analogReadAverage(int value) {
  int readingCount = 5;
  int sum = 0;
  int measures[readingCount];
  for (int i = readingCount; i >= 0; i--) {
    sum += analogRead(value);
    delay(1);  
  }
  return (int)sum / readingCount;
}


void setup() {
  myservo.attach(10);

  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = { a, b, c, d, e, f, g};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  int servoPos = 0;
  int servoStep = (180 / (SECONDS-1));
  myservo.write(0);
  //button
  if (!digitalRead(button)) {
    for (int sec = 0; sec < 10; sec++) {     
      //7seg-C
      sevseg.setNumber(sec);
      delay(1);
      sevseg.refreshDisplay();
      //servo

      if (servoPos <= 180 && servoPos >= 0) {
        myservo.write(servoPos);
        servoPos += servoStep;
      }
      //rgb-a
      setColors(sec);
      //buzzer & ldr
      beep();
      //pot
      delay(getSpeed() * 0.8);
    }
  }
}