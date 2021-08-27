#include "A4988Stepper.h"

Stepper::Stepper(int direction, int step, int enable, int stepsPerRevolution) {
	this->direction = direction;
	this->step = step;
	this->enable = enable;
	this->stepsPerRevolution = stepsPerRevolution;
}

void Stepper::begin() {
	pinMode(direction, OUTPUT);
	pinMode(step, OUTPUT);
	pinMode(enable, OUTPUT);
}

void Stepper::driveMotor(bool invert, int speed, int steps) {
	unsigned long delay = 60L * 1000L * 1000L / this->stepsPerRevolution / speed;
	
	switch(invert) {
		case true:
			digitalWrite(direction, HIGH);
			
			for(int i = 0; i < steps; i++) {
				digitalWrite(step, HIGH);
				delayMicroseconds(delay);
				digitalWrite(step, LOW);
				delayMicroseconds(delay);
			}
			break;
		case false:
			digitalWrite(direction, LOW);
			
			for(int i = 0; i < steps; i++) {
				digitalWrite(step, HIGH);
				delayMicroseconds(delay);
				digitalWrite(step, LOW);
				delayMicroseconds(delay);
			}
			break;
		default:
			digitalWrite(direction, HIGH);
			
			for(int i = 0; i < steps; i++) {
				digitalWrite(step, HIGH);
				delayMicroseconds(delay);
				digitalWrite(step, LOW);
				delayMicroseconds(delay);
			}
			break;
	}
}

void Stepper::enableMotor() {
	digitalWrite(enable, LOW);
}

void Stepper::disableMotor() {
	digitalWrite(enable, HIGH);
}