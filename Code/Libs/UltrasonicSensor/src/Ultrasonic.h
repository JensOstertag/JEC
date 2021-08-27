#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class UltrasonicSensor {
	private: 
		int trig;
		int echo;
	
	public:
		UltrasonicSensor(int trig, int echo);
		void begin();
		
		long read();	
};

#endif