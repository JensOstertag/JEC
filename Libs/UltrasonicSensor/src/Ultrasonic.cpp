#include "Ultrasonic.h"

UltrasonicSensor::UltrasonicSensor(int trig, int echo) {
	this->trig = trig;
	this->echo = echo;
}

void UltrasonicSensor::begin() {
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
}

long UltrasonicSensor::read() {	
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	long duration  = pulseIn(echo, HIGH);
	return duration*0.034/2;
}