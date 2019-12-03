/*
  RobotClient.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef RobotClient_h
#define RobotClient_h

#include <Arduino.h>

#include <SPI.h>
#include "RF24.h"

// TODO: Make another class for interpreting / parsing the message.

class RobotClient {
    public:
        RobotClient(byte enable_pin, byte select_pin);
        void begin();
        void start(byte address[]);
        void stop();
        void update();

        struct msg_ping {
            char message[256];
        };
        typedef msg_ping Msg_ping;

    private:
      byte enable_pin = 0;
      byte select_pin = 0;

      RF24 radio = RF24(0, 0);

      byte port;
      byte address[];
};

#endif