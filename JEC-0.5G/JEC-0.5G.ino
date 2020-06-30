#include <28BYJStepper.h>
#include <Ultrasonic.h>

/*
 *  STEPPER MOTOR PINS
 */
#define spr 1024

#define lm1 22
#define lm2 24
#define lm3 26
#define lm4 28

#define rm1 23
#define rm2 25
#define rm3 27
#define rm4 29

Stepper lm(lm1, lm2, lm3, lm4);
Stepper rm(rm1, rm2, rm3, rm4);

/*
 *  ULTRASONIC SENSOR
 */
#define trig 30
#define echo 31

UltrasonicSensor us(trig, echo);

/*
 *  SETTINGS
 */
#define wallDistance 20
#define hysterisis 3

/*
 *  CACHE
 */
boolean enabled = true;
boolean autoRoute = true;
String route;

void setup() {
  lm.begin();
  rm.begin();
  
  us.begin();

  Serial.begin(9600);
}

void loop() {
  if(enabled) {
    long distance = getDistance();
    Serial.println(distance);

    if(distance > wallDistance + hysterisis) {
      right((distance - wallDistance) * 10);
      shortForeward();
    } else if(distance < wallDistance - hysterisis) {
      left((wallDistance - distance) * 10);
      shortForeward();
    } else {
      foreward();
    }
  }
}

void foreward() {
  for(int i = 0; i < 1024; i++) {
    lm.driveMotor(true);
    rm.driveMotor(false);
    delay(2);
  }
}

void shortForeward() {
  for(int i = 0; i < 128; i++) {
    lm.driveMotor(true);
    rm.driveMotor(false);
    delay(2);
  }
}

void left(int steps) {
  for(int i = 0; i < steps; i++) {
    lm.driveMotor(false);
    rm.driveMotor(false);
    delay(2);
  }
}

void right(int steps) {
  for(int i = 0; i < steps; i++) {
    lm.driveMotor(true);
    rm.driveMotor(true);
    delay(2);
  }
}

long getDistance() {
  return us.read();
}
