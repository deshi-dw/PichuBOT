/*
  // Robot.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef RobotClaw_h
#define RobotClaw_h

#include "Arduino.h"
#include "Servo.h"

class RobotClaw {
	public:
	RobotClaw(byte pin_right, byte pin_left);
	void setAngle(byte angle);
	byte getAngle();
	void reset();

	private:
	Servo sright;
	Servo sleft;
};

#endif