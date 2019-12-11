/*
  Robot.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef RobotState_h
#define RobotState_h

enum RobotState {
  INIT_ROBOT,
  INIT_IDLE,
  LOOP_IDLE,
  INIT_AUTONOMOUS,
  LOOP_AUTONOMOUS,
  INIT_TELEOP,
  LOOP_TELEOP,
  DISABLED,
  TESTING
};

#endif