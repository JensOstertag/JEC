#include <28BYJStepper.h>
#include <Servo.h>
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
 *  SERVO MOTOR
 */
Servo servo;
#define smp 52
#define centerPos 85
#define leftPos 200
#define rightPos 0

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

  servo.attach(smp);
  servo.write(rightPos);
  
  us.begin();

  Serial.begin(9600);
}

void loop() {
  if(enabled) {
    long distance = getDistance();
    long smallestDistance = getSmallestDistance();

    if(smallestDistance > wallDistance + hysterisis) {
      right((distance - wallDistance) * 10);
      shortForeward();
    } else if(smallestDistance < wallDistance - hysterisis) {
      left((wallDistance - smallestDistance) * 10);
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
  servo.write(rightPos);
  delay(200);
  
  return us.read();
}

long getSmallestDistance() {
  int maxDeflection = centerPos;
  int minDeflection = rightPos;
  int deflection = minDeflection;
  
  long distances[maxDeflection - minDeflection];

  servo.write(minDeflection);
  delay(200);

  long smallestDistance = 0;

  for(int i = 0; i < maxDeflection - minDeflection; i++) {
    servo.write(deflection);
    delay(10);
    long distance = us.read();
    distances[i] = distance;

    int smallerDistances = 0;

    for(int j = 1; j < 20; j++) {
      if(distance >= distances[i - j]) {
        smallerDistances++;
      }
    }

    if(smallerDistances >= 10) {
      smallestDistance = distance;
      break;
    }
  }

  if(smallestDistance == 0) {
    smallestDistance = distances[maxDeflection - minDeflection - 1];
  }

  return smallestDistance;
}
