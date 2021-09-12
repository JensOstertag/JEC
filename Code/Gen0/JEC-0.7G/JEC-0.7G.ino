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
 *  CONSTANTS
 */
int safetyDistance = 20; // Safety Distance (cm)

/*
 *  CACHE
 */
boolean enabled = true;
boolean autoRoute = true; // Self Driving Capability Enabled
String route; // Stores Planned Route
char previousRoute[128]; // Stores Previous Route

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

  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();

    // Enable / Disable
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
    if(command.startsWith("route override")) { // Replace Planned Route
      String newRoute = "";
      boolean added = true;
      for(int i = 0; i < command.substring(15).length(); i++) {
        char c = command.substring(15).charAt(i);
        if(c == 'f' || c == 'l' || c == 'r' || c == 'b')
          newRoute += command.substring(15).charAt(i);
        else
          added = false;
      }
      if(added == true) {
        route = newRoute;
        Serial.println("ROUTE SET TO: " + newRoute);
      } else
        Serial.println("ROUTE NOT OVERRIDDEN, BAD SYNTAX");
    } else if(command.startsWith("route add")) { // Add Route Element to Planned Route
      String newRoute = "";
      boolean added = true;
      for(int i = 0; i < command.substring(10).length(); i++) {
        char c = command.substring(10).charAt(i);
        if(c == 'f' || c == 'l' || c == 'r' || c == 'b')
          newRoute += command.substring(10).charAt(i);
        else
          added = false;
      }
      if(added == true) {
        route += newRoute;
        Serial.println("ADDED ROUTE SEGMENT " + newRoute);
      } else
        Serial.println("ROUTE SEGMENT NOT ADDED, BAD SYNTAX");
    } else if(command.equalsIgnoreCase("route removelast")) { // Remove Last Route Element from Planned Route
      String newRoute = "";
      for(int i = 0; i < route.length() - 1; i++)
        newRoute += route.charAt(i);
        
      route = newRoute;
      Serial.println(newRoute);
    } else if(command.equalsIgnoreCase("route reset")) { // Reset Planned Route
      route = "";
      Serial.println("ROUTE RESET");
    } else if(command.equalsIgnoreCase("route show")) { // Show Planned Route
      Serial.println("ROUTE: " + route);
    }
  }
  
  if(enabled) {
    if(autoRoute) {
      if(!(foreward())) {
        // Robot can't move foreward, selecting a Direction to move next
        long distanceLeft = leftDistance();
        long distanceRight = rightDistance();
    
        if(distanceLeft > distanceRight && distanceLeft > safetyDistance) {
          left();
        } else if(distanceLeft < distanceRight && distanceRight > safetyDistance) {
          right();
        } else if(distanceLeft == distanceRight && leftDistance > safetyDistance) {
          // Select a random Direction to move
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
        } else if(distanceLeft < safetyDistance && distanceRight < safetyDistance) {
          // Robot is stuck
          reprocess();
        }
      }
    } else {
      if(route.length() > 0) {
        int currentDirection = route.charAt(0);

        route = route.substring(1);

        if(currentDirection == 'f') {
          foreward();
        } else if(currentDirection == 'l') {
          left();
        } else if(currentDirection == 'r') {
          right();
        } else if(currentDirection == 'b') {
          backward();
        }
      }
    }
  }
}

boolean foreward() {
  if(centerDistance() < safetyDistance)
    return false;

  addPreviousRoute('f');
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(true);
    rm.driveMotor(false);
    delay(2);
  }
  return true;
}

boolean left() {
  if(leftDistance() < safetyDistance)
    return false;

  addPreviousRoute('l');
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(false);
    rm.driveMotor(false);
    delay(2);
  }
  return true;
}

boolean right() {
  if(rightDistance() < safetyDistance)
    return false;

  addPreviousRoute('r');
  for(int i = 0; i < spr; i++) {
    lm.driveMotor(true);
    rm.driveMotor(true);
    delay(2);
  }
  return true;
}

void backward() {
  addPreviousRoute('b');
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

void addPreviousRoute(char previousDirection) {
  // Move all previous Route Elements further into Past and insert the new one
  for(int i = sizeof(previousRoute) / sizeof(previousRoute[0]) - 1; i > -1 ; i--) {
    if(i != 0)
      previousRoute[i] = previousRoute[i-1]; // Move Previous Route Element
    else
      previousRoute[i] = previousDirection; // Add New Previous Route Element
  }

  Serial.println(previousRoute);
}

void reprocess() {
  Serial.println("Reprocessing");
  enabled = false;
}
