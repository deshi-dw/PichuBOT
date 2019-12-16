/*
  RobotClient.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef RobotClient_h
#define RobotClient_h

#include <Arduino.h>

#include <SPI.h>
#include "RF24.h"

class RobotClient {
    public:
        RobotClient(byte enable_pin, byte select_pin);
		void start(uint8_t channel);
		void stop();

		void connect();
		void disconnect();
		
		bool readMsg();
		uint8_t readByte();
		void readString(char *buff);

		void writeMsg(uint8_t id);
		void writeByte(uint8_t data);
		void writeString(char buff[32]);

		void send();

		void printHealth();
		void sendHealth();

		RF24 radio = RF24(7, 8);

		int64_t addresses[2] = {0x00000F10, 0x00000F20};
		uint8_t msgid;
		char buffer[64] = {0};
		uint8_t buffer_index = 0;

    private:
      byte enable_pin = 0;
      byte select_pin = 0;
};

#endif