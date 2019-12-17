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

	readbuff = (char*)malloc(64);
	writebuff = (char*)malloc(64);
}

void RobotClient::start(uint8_t channel) {
    // setup radio's configuration.
    radio.begin();
    radio.setChannel(channel);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);

	radio.flush_rx();
	radio.flush_tx();
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

void RobotClient::write(uint8_t size) {
	radio.stopListening();
	if(radio.write(writebuff, size) == true) {
		send_count++;
	}
	else {
		failed_send_count++;
	}
	radio.startListening();
}

void RobotClient::read(uint8_t size) {
	radio.read(readbuff, size);
}

void RobotClient::printHealth() {
	Serial.println("checking radio health...");
    if (radio.isChipConnected()) {
        Serial.println("	the chip is connected.");
    } else {
        Serial.println("	the chip is disconnected.");
    }
    Serial.print("	channel:");
    Serial.println(radio.getChannel());
    Serial.print("	data rate:");
    Serial.println(radio.getDataRate());

    Serial.print("	send count:");
    Serial.println(send_count);
    Serial.print("	failed send count:");
    Serial.println(failed_send_count);
    send_count = 0;
    failed_send_count = 0;

    if (radio.testRPD()) {
        Serial.println("	radio's signal is strong.");
    } else {
        Serial.println("	radio's signal is weak.");
    }
    if (radio.rxFifoFull()) {
        Serial.println("	read buffer is full.");
    }
    if (radio.available()) {
        Serial.println("	data is avaliable to read.");
    }
    Serial.println();
}
void RobotClient::sendHealth() {}