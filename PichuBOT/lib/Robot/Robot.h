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

    Motor motorRight = Motor(0, 0, 0);
    Motor motorLeft = Motor(0, 0, 0);

    RobotState state;
	void (*states[9])();

    float rotationBias = 1.0f;
    float speedBias = 1.0f;

    unsigned long time;

    unsigned long autonomousTime = 5000;
    unsigned long teleopTime = 5000;

	bool isDisabled = false;
	bool isTesting = false;
  bool isFirstTest = true;

  private:
    int speedRight = 0;
    int speedLeft = 0;
    bool stopped = false;

    float newX = 0;
    float newY = 0;
    
    unsigned long elapsedTime;
};

#endif
