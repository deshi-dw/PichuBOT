/*
  Robot.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

// include this library's description file as well as Arduino so we can set port modes.
#include "Robot.h"
#include <Arduino.h>

#include "../Motor/Motor.h"

Robot::Robot() {}

void Robot::init() {
    // When initializing, the robot will set it's state to 'INIT_ROBOT'.
    // When it is in this state, the robot will not be to move or do anything else.
    state = INIT_ROBOT;
}

// Initializes robot variables and sets the starting state.
void Robot::ready() {
    state = INIT_IDLE;
}

// Runs on loop throughout the robots lifetime.
void Robot::update() {
    if (isDisabled) return;

    time = millis() - timeDifference;

    // TODO: Get bluetooth input via ControllerReciever.

    // If the robot is stopped, send nothing. (Better then just sending 0 because reasons...?)
    if (!stopped) {
        motorRight.setSpeed(speedRight);
        motorLeft.setSpeed(speedLeft);
    }

    // Set time difference to time on initialization.
    // timeDifference acts as resetting the timer to 0 but since that isn't possible we use this.
    if (state == INIT_AUTONOMOUS || state == INIT_TELEOP) timeDifference = time;

    // If autonomous time has ended, switch to teleop.
    if (state == LOOP_AUTONOMOUS && time > autonomousTime) state = INIT_TELEOP;
    // If teleop state has ended, switch to disabled.
    if (state == LOOP_TELEOP && time > teleopTime) state = DISABLED;

    switch (state) {
        case INIT_ROBOT:
            state = INIT_IDLE;
            break;
        case INIT_IDLE:
            states[INIT_IDLE]();
            state = LOOP_IDLE;
            break;
        case LOOP_IDLE:
            states[LOOP_IDLE]();
            break;
        case INIT_AUTONOMOUS:
            states[INIT_AUTONOMOUS]();
            state = LOOP_AUTONOMOUS;
            break;
        case LOOP_AUTONOMOUS:
            states[LOOP_AUTONOMOUS]();
            break;
        case INIT_TELEOP:
            states[INIT_TELEOP]();
            state = LOOP_TELEOP;
            break;
        case LOOP_TELEOP:
            states[LOOP_TELEOP]();
            break;
        case DISABLED:
            states[DISABLED]();
            //TODO: Add shutdown functionality.
            break;
        default:
            //TODO: Add shutdown functionality.
            break;
    }
}

// Drive the robot in arcade mode. This means x acts as rotation and y acts as speed.
void Robot::drive(int x, int y) {
    if (stopped) stopped = false;

    // NewX is a float that will multiply speed acording to desired rotation direction.
    newX = (x - 128) * 2 / 255 * rotationBias;
    // NewY is the forwards speed of the robot.
    newY = (int)x * speedBias;

    // Set left and right speed to correct values.
    speedRight = newY * -newX;
    speedLeft = newY * newX;
}

// Drive the robot in tank mode. right is right motor speed and left is left motor speed.
void Robot::driveTank(int right, int left) {
    if (stopped) stopped = false;

    speedRight = right;
    speedLeft = left;
}

// Pause the robot. This will only update the robot and should freeze everything else.
void Robot::pause(unsigned long pause) {
    unsigned long pauseTime = time + pause;
    while (pauseTime > time) {
        update();
    }
}