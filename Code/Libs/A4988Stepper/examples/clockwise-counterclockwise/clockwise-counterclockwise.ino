/*
 * 
 *     Stepper Motor Library
 *     Developed by Jens Ostertag
 *     
 *     Ability to drive stepper motors with A4988 driver board
 *     
 *     © 2020 Jens Ostertag
 *     
 */

#include <A4988Stepper.h>

#define direction 3
#define step 4
#define enable 5

#define stepsPerRevolution 200

Stepper stepper(direction, step, enable, stepsPerRevolution);   // Define pins of A4988 board

void setup() {
  Serial.begin(9600);
  stepper.begin();                                              // Defines pins as OUTPUTs
}

void loop() {
  int steps = 200;
  
  stepper.enableMotor();                                        // Enables the motor
  stepper.driveMotor(false, 1000, steps);                       // Turns the motor at speed 1000 one revolution clockwise
  stepper.disableMotor();                                       // Disables the motor
  
  delay(1000);                                                  // Wait 1 second
  
  stepper.enableMotor();                                        // Enables the motor
  stepper.driveMotor(true, 1000, steps);                        // Turns the motor at speed 1000 one revolution counter-clockwise
  stepper.disableMotor();                                       // Disables the motor
  
  delay(1000);                                                  // Wait 1 second
}
