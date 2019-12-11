// /*
//   UDPMessenger.cpp - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
//   Copyright (c) 2018 Soviet Robotics.  All right reserved.
// */

// #include "UDPMessenger.h"
// #include <Arduino.h>

// #include <ESP8266WiFi.h>
// #include "ESPAsyncUDP.h"

// UDPMessenger::UDPMessenger(byte tx_pin, byte rx_pin, byte ch_pd_pin) { }

// bool UDPMessenger::start(char *port, char *adrress) {
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(ssid, password);

//     if (WiFi.waitForConnectResult() != WL_CONNECTED) {
//         Serial.println("Wifi failed to connect.");
// 		return false;
//     }

// 	if(udp.connect(IPAddress(192,168,1,100), 1234)) {
// 		udp.onPacket([this](AsyncUDPPacket packet) { });
// 	}
// 	else {
// 		Serial.println("UDP failed to connect.");
// 	}

// 	return true;
// }

// void UDPMessenger::update() {}

// void UDPMessenger::close() {
// 	udp.close();
// }