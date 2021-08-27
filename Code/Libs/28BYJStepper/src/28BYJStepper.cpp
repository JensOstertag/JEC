#include "28BYJStepper.h"

Stepper::Stepper(int a, int b, int c, int d) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

void Stepper::begin() {
	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
	pinMode(c, OUTPUT);
	pinMode(d, OUTPUT);
	
	this->step_number = 0;
}

void Stepper::driveMotor(bool invert) {	
	switch(invert) {
		case true:
			switch(step_number) {
				case 0:
				  digitalWrite(a, HIGH);
				  digitalWrite(b, LOW);
				  digitalWrite(c, LOW);
				  digitalWrite(d, LOW);
				break;
				case 1:
				  digitalWrite(a, LOW);
				  digitalWrite(b, HIGH);
				  digitalWrite(c, LOW);
				  digitalWrite(d, LOW);
				break;
				case 2:
				  digitalWrite(a, LOW);
				  digitalWrite(b, LOW);
				  digitalWrite(c, HIGH);
				  digitalWrite(d, LOW);
				break;
				case 3:
				  digitalWrite(a, LOW);
				  digitalWrite(b, LOW);
				  digitalWrite(c, LOW);
				  digitalWrite(d, HIGH);
				break;
			}
			
			break;
		case false:
			switch(step_number) {
				case 0:
				  digitalWrite(a, LOW);
				  digitalWrite(b, LOW);
				  digitalWrite(c, LOW);
				  digitalWrite(d, HIGH);
				break;
				case 1:
				  digitalWrite(a, LOW);
				  digitalWrite(b, LOW);
				  digitalWrite(c, HIGH);
				  digitalWrite(d, LOW);
				break;
				case 2:
				  digitalWrite(a, LOW);
				  digitalWrite(b, HIGH);
				  digitalWrite(c, LOW);
				  digitalWrite(d, LOW);
				break;
				case 3:
				  digitalWrite(a, HIGH);
				  digitalWrite(b, LOW);
				  digitalWrite(c, LOW);
				  digitalWrite(d, LOW);
				break;
			}
			 
			break;
		default:
			switch(step_number) {
				case 0:
				  digitalWrite(a, LOW);
				  digitalWrite(b, LOW);
				  digitalWrite(c, LOW);
				  digitalWrite(d, HIGH);
				break;
				case 1:
				  digitalWrite(a, LOW);
				  digitalWrite(b, LOW);
				  digitalWrite(c, HIGH);
				  digitalWrite(d, LOW);
				break;
				case 2:
				  digitalWrite(a, LOW);
				  digitalWrite(b, HIGH);
				  digitalWrite(c, LOW);
				  digitalWrite(d, LOW);
				break;
				case 3:
				  digitalWrite(a, HIGH);
				  digitalWrite(b, LOW);
				  digitalWrite(c, LOW);
				  digitalWrite(d, LOW);
				break;
			}
			 
			break;
	}
	
	step_number++;
     
    if(step_number > 3) {
      step_number = 0;
    }
}