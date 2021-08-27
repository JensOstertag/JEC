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
 *  CACHE
 */
boolean enabled = true;
boolean autoRoute = true;
String route;

void setup() {
  lm.begin();
  rm.begin();

  servo.attach(smp);
  servo.write(centerPos);
  delay(500);
  servo.write(leftPos);
  delay(500);
  servo.write(rightPos);
  delay(500);
  servo.write(centerPos);
  
  us.begin();

  // Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();

    // Start / Stop
    if(command.equalsIgnoreCase("start")) {
      enabled = true;
      Serial.println("STARTED");
    } else if(command.equalsIgnoreCase("stop")) {
      enabled = false;
      Serial.println("STOPPED");
    }

    // AUTO-ROUTE
    if(command.equalsIgnoreCase("autoroute enable")) {
      autoRoute = true;
      Serial.println("AUTOROUTE ENABLED");
    } else if(command.equalsIgnoreCase("autoroute disable")) {
      autoRoute = false;
      Serial.println("AUTOROUTE DISABLED");
    }

    // ROUTE
    if(command.startsWith("route override")) {
      int l = command.substring(15).length();
      String r = "";
      boolean added = true;
      for(int i = 0; i < l; i++) {
        char c = command.substring(15).charAt(i);
        if(c == 'f' || c == 'l' || c == 'r' || c == 'b') {
          r += command.substring(15).charAt(i);
        } else {
          added = false;
        }
      }
      if(added == true) {
        route = r;
        Serial.println("ROUTE SET TO: " + r);
      } else {
        Serial.println("ROUTE NOT OVERRIDDEN, BAD SYNTAX");
      }
    } else if(command.startsWith("route add")) {
      int l = command.substring(10).length();
      String r = "";
      boolean added = true;
      for(int i = 0; i < l; i++) {
        char c = command.substring(10).charAt(i);
        if(c == 'f' || c == 'l' || c == 'r' || c == 'b') {
          r += command.substring(10).charAt(i);
        } else {
          added = false;
        }
      }
      if(added == true) {
        route += r;
        Serial.println("ADDED ROUTE SEGMENT " + r);
      } else {
        Serial.println("ROUTE SEGMENT NOT ADDED, BAD SYNTAX");
      }
    } else if(command.equalsIgnoreCase("route removelast")) {
      int l = route.length();
      String r = "";
      for(int i = 0; i < l - 1; i++) {
        r += route.charAt(i);
      }
      route = r;
      Serial.println(r);
    } else if(command.equalsIgnoreCase("route reset")) {
      route = "";
      Serial.println("ROUTE RESET");
    } else if(command.equalsIgnoreCase("route show")) {
      Serial.println("ROUTE: " + route);
    }
  }
  
  if(enabled) {
    if(autoRoute == true) {
      long distanceCenter = centerDistance();
      if(distanceCenter < 20) {
        long distanceLeft = leftDistance();
        long distanceRight = rightDistance();
    
        if(distanceLeft > distanceRight && distanceLeft > 20) {
          left();
        } else if(distanceLeft < distanceRight && distanceRight > 20) {
          right();
        } else if(distanceLeft == distanceRight && leftDistance > 20) {
          int rnd = (int) (random(0, 2));
          switch(rnd) {
            case 0:
              left();
              break;
              
            case 1:
              right();
              break;
              
            default:
              left();
              break;
          }
        }
      } else {
        foreward();
      }
    } else {
      int l = route.length();

      if(l > 0) {
        int dir = route.charAt(0);

        String r = "";
        for(int i = 1; i < l; i++) {
          r += route.charAt(i);
        }

        route = r;

        if(dir == 'f') {
          foreward();
        } else if(dir == 'l') {
          left();
        } else if(dir == 'r') {
          right();
        } else if(dir == 'b') {
          backward();
        }
      }
    }
  }
}

void foreward() {
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(true);
    rm.driveMotor(false);
    delay(2);
  }
}

void left() {
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(false);
    rm.driveMotor(false);
    delay(2);
  }
}

void right() {
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(true);
    rm.driveMotor(true);
    delay(2);
  }
}

void backward() {
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(false);
    rm.driveMotor(true);
    delay(2);
  }
}

long centerDistance() {
  servo.write(centerPos);
  delay(1000);
  return us.read();
}

long leftDistance() {
  servo.write(leftPos);
  delay(1000);
  return us.read();
}

long rightDistance() {
  servo.write(rightPos);
  delay(1000);
  return us.read();
}

void resetServo() {
  servo.write(centerPos);
  delay(1000);
}
