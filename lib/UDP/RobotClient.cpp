/*
  RobotClient.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#include "RobotClient.h"
#include <Arduino.h>

#include <RF24.h>
#include <SPI.h>

RobotClient::RobotClient(byte enable_pin, byte select_pin) {
    radio = RF24(enable_pin, select_pin);

    this->enable_pin = enable_pin;
    this->select_pin = select_pin;
}

void RobotClient::begin() {
    radio.begin();
    radio.setChannel(122);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
}

void RobotClient::start(byte address[]) {
    radio.openReadingPipe(1, address);
    radio.startListening();
}

void RobotClient::stop() {
    radio.closeReadingPipe(1);
    radio.stopListening();
}

void RobotClient::update() {
    if(radio.available() == true) {
        Msg_ping ping;
        radio.read(&ping, INT8_MAX);

        // Serial.println(ping.message);
    }
}