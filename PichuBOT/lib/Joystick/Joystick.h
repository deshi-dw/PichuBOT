/*
  Joystick.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef Joystick_h
#define Joystick_h

class Joystick {
    public:
        Joystick(int _apinX, int _apinY);
        void Update();
        int x;
        int y;
    private:
        int apinX;
        int apinY;
};

#endif