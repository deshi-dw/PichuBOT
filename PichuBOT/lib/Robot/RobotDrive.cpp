/*
  // Robot.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "RobotDrive.h"
#include "Arduino.h"

RobotDrive::RobotDrive(Motor *mright, Motor *mleft) {
    this->mright = mright;
    this->mleft = mleft;
}

void RobotDrive::arcadeDrive(byte speed, byte turn) {
    float turnmult = (turn - 128) * 2 / 255;

	tankDrive(speed * -turnmult, speed * turnmult);
}

void RobotDrive::tankDrive(byte right, byte left) {
	(*mright).setSpeed(right);
	(*mleft).setSpeed(left);
}

void RobotDrive::stop() {
	(*mright).stop();
	(*mleft).stop();
}
