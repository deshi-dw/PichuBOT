/*
  Scheduler.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef Scheduler_h
#define Scheduler_h

#include "Task.h"

#define MAX_TASKS 32

class Scheduler {
    public:
        Scheduler();
        void schedule(Task task);
        void update(long time);
    private:
        Task tasks[MAX_TASKS];
        int currentTaskIndex = 0;
};

#endif