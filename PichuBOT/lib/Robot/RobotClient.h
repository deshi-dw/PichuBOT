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
		void writeString(char buff[32]);

		int64_t addresses[2] = {0x00000F20, 0x00000F10};
		uint8_t msgid;

    private:
      byte enable_pin = 0;
      byte select_pin = 0;

      RF24 radio = RF24(0, 0);
};

#endif