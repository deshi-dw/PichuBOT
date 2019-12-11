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

void RobotClient::start(byte local[], byte remote[]) {
    this->localAddress = local;
    this->remoteAddress = remote;

    radio.openReadingPipe(1, remote);
    radio.openWritingPipe(remote);
    radio.startListening();
}

void RobotClient::stop() {
    radio.closeReadingPipe(1);
    radio.stopListening();
}

void RobotClient::update() {
    if(radio.available() == true) {
        byte size;
        radio.read(&size, 1);

        char *buffer;
        radio.read(buffer, size);

        Serial.print("robot> read: ");
        Serial.println(buffer);

        // radio.closeReadingPipe(1);
        radio.stopListening();
        radio.write(buffer, size);
        radio.startListening();
        // radio.openReadingPipe(1, localAddress);
    }
}