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

void RobotClient::start(uint8_t channel) {
    // setup radio's configuration.
    radio.begin();
    radio.setChannel(channel);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
}

void RobotClient::stop() {
    radio.stopListening();
    radio.closeReadingPipe(1);
    radio.flush_rx();
    radio.flush_tx();
}

void RobotClient::connect() {
    // open radio to both reading and writing.
    radio.openReadingPipe(1, addresses[0]);
    radio.openWritingPipe(addresses[0]);

    // start listening for transmissions.
    radio.startListening();
}

void RobotClient::disconnect() {
    stop();
}

bool RobotClient::readMsg() {
    if (radio.available()) {
        radio.read(&msgid, sizeof(uint8_t));
        return true;
    }

    return false;
}

uint8_t RobotClient::readByte() {
    uint8_t data;
    radio.read(&data, sizeof(uint8_t));
    return data;
}

void RobotClient::readString(char *data) {
	radio.read(data, 32);
}

void RobotClient::writeMsg(uint8_t id) {
	memcpy(buffer+buffer_index, &id, sizeof(uint8_t));
	buffer_index++;
}

void RobotClient::writeByte(uint8_t data) {
	memcpy(&buffer[buffer_index], &data, sizeof(uint8_t));
	buffer_index++;
}

void RobotClient::writeString(char *buff) {
	memcpy(buffer+buffer_index, buff, 32);
	buffer[buffer_index+31] = '\0';
	buffer_index += 32;
}

void RobotClient::send() {
	radio.stopListening();
	radio.write(buffer, buffer_index);
	radio.startListening();

	buffer_index = 0;
}

void RobotClient::printHealth() {}
void RobotClient::sendHealth() {}