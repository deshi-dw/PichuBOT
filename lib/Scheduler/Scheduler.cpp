/*
  Scheduler.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "Scheduler.h"
#include <Arduino.h>
#include <stdio.h>

Scheduler::Scheduler() { }

void Scheduler::update(long globalTime) {
    for(int i = currentTaskIndex-1; i >= 0; i--) {
        if(tasks[i].triggered == false) {
            tasks[i].update(globalTime);
        }
    }
}

void Scheduler::schedule(Task task) {
    tasks[currentTaskIndex] = task;
    currentTaskIndex++;
}