/*
  Motor.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor {
    public:
        Motor(byte _pin1, byte _pin2, byte _pinE);
        void setSpeed(byte speed);
        void stop();
    private:
        byte pin1;
        byte pin2;
        byte pinE;
};

#endif