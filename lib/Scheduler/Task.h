/*
  Task.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef Task_h
#define Task_h

class Task {
    public:
        Task();
        Task(void (*task)(), long time);
        void update(long globalTime);
        bool triggered = false;
    private:
        void (*task)();
        long time;
};

#endif