/*
  // Robot.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef Robot_h
#define Robot_h

#include "RobotState.h"
#include "../Motor/Motor.h"

class Robot {
  public:
    Robot();
	void init();
    void update();
    void ready();

    void drive(int x, int y);
    void driveTank(int right, int left);
    void stop();

	void pause(unsigned long time);

    Motor motorRight = Motor(0, 0, 0);
    Motor motorLeft = Motor(0, 0, 0);

    RobotState state;
	void (*states[])();

    float rotationBias = 1.0f;
    float speedBias = 1.0f;

    unsigned long time;

    unsigned long autonomousTime = 15000;
    unsigned long teleopTime = 45000;

	bool isDisabled = false;

  private:
    int speedRight = 0;
    int speedLeft = 0;
    bool stopped = false;

    float newX = 0;
    float newY = 0;
    
    unsigned long timeDifference;
};

#endif
