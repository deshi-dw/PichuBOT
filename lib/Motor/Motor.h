/*
  Motor.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef Motor_h
#define Motor_h

class Motor {
    public:
        Motor(int _pin1, int _pin2, int _pinE);
		void init();
        void setSpeed(int speed);
        void stop();
    private:
        int pin1;
        int pin2;
        int pinE;
};

#endif