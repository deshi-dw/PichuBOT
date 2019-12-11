/*
  Task.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "Task.h"
#include <Arduino.h>

Task::Task() { }

Task::Task(void (*task)(), long time) {
    this->task = task;
    this->time = time;
}

void Task::update(long globalTime) {
    if(time >= globalTime && triggered == false) {
        task();
        triggered = true;
    }
}