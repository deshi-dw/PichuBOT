/*
  Joystick.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "Joystick.h"
#include <Arduino.h>

Joystick::Joystick(int _apinX, int _apinY) {
  apinX = _apinX;
  apinY = _apinY;
}

void Joystick::Update() {
  x = analogRead(apinX);
  y = analogRead(apinY);
}