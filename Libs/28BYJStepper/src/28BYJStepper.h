#ifndef BYJSTEPPER_H
#define BYJSTEPPER_H

#include <Arduino.h>

class Stepper {
	private: 
		int a;
		int b;
		int c;
		int d;
		int step_number;
	
	public:
		Stepper(int a, int b, int c, int d);
		void begin();
		
		void driveMotor(bool invert);	
};

#endif