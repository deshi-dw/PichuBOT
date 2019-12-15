/*
  RobotClient.h - Robot library - A specialized library made by Soviet Robotics to help in their goal of world domination.
  Copyright (c) 2018 Soviet Robotics.  All right reserved.
*/

#ifndef MsgId_h
#define MsgId_h

#include <Arduino.h>

const uint8_t msgid_blank 		= 0b00000000;

const uint8_t msgid_help 		= 0b00000100;
const uint8_t msgid_health 		= 0b00000101;

const uint8_t msgid_print 		= 0b00001001;
const uint8_t msgid_ping 		= 0b00001010;
const uint8_t msgid_start 		= 0b00001011;
const uint8_t msgid_stop 		= 0b00001100;

const uint8_t msgid_setchnnl 	= 0b00010001;
const uint8_t msgid_setbchnnl 	= 0b00010010;

const uint8_t msgid_drive 		= 0b00100001;
const uint8_t msgid_tdrive 		= 0b00100010;
const uint8_t msgid_drivetimed	= 0b00100011;
const uint8_t msgid_tdrivetimed	= 0b00100100;
const uint8_t msgid_claw 		= 0b00100101;

#endif