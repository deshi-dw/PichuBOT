// /*
//   UDPMessenger.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
//   Copyright (c) 2018 Soviet Robotics.  All right reserved.
// */

// #ifndef UDPMessenger_h
// #define UDPMessenger_h

// #include <Arduino.h>

// #include <ESP8266WiFi.h>
// #include "ESPAsyncUDP.h"

// // TODO: Make another class for interpreting / parsing the message.

// class UDPMessenger {
//     public:
//         UDPMessenger(byte tx_pin, byte rx_pin, byte ch_pd_pin);
//         bool start(char *port, char *adrress);
//         void update();
//         void close();

//         byte buffer[];
//         // AsyncUDPPacket newestPacket; 
//     private:
//       byte tx_pin = 0;
//       byte rx_pin = 0;
//       byte ch_pd_pin = 0;
//       char *ssid;
//       char *password;
//       AsyncUDP udp;
//       AsyncUDPPacket (*onPacket)();

//       byte port;
//       int adress;
// };

// #endif