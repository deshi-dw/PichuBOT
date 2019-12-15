/*
  // Robot.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef RobotDrive_h
#define RobotDrive_h

#include "Arduino.h"
#include "Motor.h"

class RobotDrive {
	public:
	RobotDrive(Motor *mright, Motor *mleft);
	void arcadeDrive(byte speed, byte turn);
	void tankDrive(byte right, byte left);
	void stop();

	private:
	Motor *mright;
	Motor *mleft;
};

#endif