#ifndef A4988STEPPER_H
#define A4988STEPPER_H

#include <Arduino.h>

class Stepper {
	private: 
		int direction;
		int step;
		int enable;
		int stepsPerRevolution;
	
	public:
		Stepper(int direction, int step, int enable, int stepsPerRevolution);
		void begin();
		
		void driveMotor(bool invert, int speed, int steps);
		
		void enableMotor();
		void disableMotor();		
};

#endif