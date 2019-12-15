/*
  // Robot.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "Arduino.h"
#include "Servo.h"
#include "RobotClaw.h"

RobotClaw::RobotClaw(byte pin_right, byte pin_left) {
	sright.attach(pin_right);
	sleft.attach(pin_left);
}
void RobotClaw::setAngle(byte angle) {
	sright.write(angle);
	sleft.write(angle - 90);
}
byte RobotClaw::getAngle() {
	return (sright.read() + sleft.read()) / 2;
}
void RobotClaw::reset() {
	sright.write(0);
	sleft.write(0);
}