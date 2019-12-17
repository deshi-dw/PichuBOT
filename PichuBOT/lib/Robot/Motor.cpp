/*
  Motor.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "Motor.h"
#include <Arduino.h>

Motor::Motor(byte _pin1, byte _pin2, byte _pinE) {
    this->pin1 = _pin1;
    this->pin2 = _pin2;
    this->pinE = _pinE;

	pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pinE, OUTPUT);
}

void Motor::setSpeed(byte speed) {
    if (speed == 127) {
        stop();
        return;
    }

    // TODO: Possibly add a "direction" to control as some motors might be reversed.
    // TODO: If motors are spinning the wrong way, switch the 1 and 0.
    if (speed > 127) {
		speed = (speed - 127) * 2;
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else {
		speed = (127 - speed) * 2;
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }

    analogWrite(pinE, speed);
}

void Motor::stop() {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
	
    analogWrite(pinE, 0);
}