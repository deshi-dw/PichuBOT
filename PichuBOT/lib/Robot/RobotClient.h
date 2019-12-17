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
		
		bool avaliable();
		void read(uint8_t size);
		void write(uint8_t size);

		void printHealth();
		void sendHealth();

		RF24 radio = RF24(7, 8);

		int64_t addresses[2] = {0x00000F10, 0x00000F20};
		char *writebuff;
		char *readbuff;

    private:
      byte enable_pin = 0;
      byte select_pin = 0;

	  uint16_t send_count = 0;
	  uint16_t failed_send_count = 0;
};

#endif