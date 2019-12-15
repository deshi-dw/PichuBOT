#include <Arduino.h>

#include <Motor.h>
#include <Servo.h>
#include <RobotDrive.h>
#include <RobotClaw.h>

#include <RobotClient.h>
#include <MsgIds.h>


// Pins for the right motor.
const byte pin_motorR1 = 1;
const byte pin_motorR2 = 1;
const byte pin_motorRE = 1;

// Pins for the left motor.
const byte pin_motorL1 = 1;
const byte pin_motorL2 = 1;
const byte pin_motorLE = 1;

// Pins for the right and left servo.
const byte pin_servoR = 1;
const byte pin_servoL = 1;

const byte pin_radioen = 7;
const byte pin_radiose = 8;

// Initialize motors right and left.
Motor mright = Motor(pin_motorR1, pin_motorR2, pin_motorRE);
Motor mleft = Motor(pin_motorL1, pin_motorL2, pin_motorLE);

// Initialize the drivetrain with pointers to motors right and left.
RobotDrive drive = RobotDrive(&mright, &mleft);

// Initialize the claw with servo pins right and left.
RobotClaw claw = RobotClaw(pin_servoR, pin_servoL);

// Initialize the robots radio client.
RobotClient client = RobotClient(pin_radioen, pin_radiose);

void setup() {
	Serial.begin(9600);
	client.start(115);
}

void loop() {
	if(client.readMsg()) {
		switch(client.msgid) {
		case msgid_blank:
		break;

		case msgid_help:
		break;

		case msgid_health:
		break;

		case msgid_print:
			client.writeMsg(msgid_ping);
			char *strbuff;
			client.readString(strbuff);
			client.writeString(strbuff);
		break;

		case msgid_ping:
		client.writeMsg(msgid_ping);
		break;

		case msgid_drive:
			drive.arcadeDrive(client.readByte(), client.readByte());
		break;

		case msgid_tdrive:
			drive.tankDrive(client.readByte(), client.readByte());
		break;

		case msgid_claw:
			claw.setAngle(client.readByte());
		break;

		default:
		break;
	}
	}
}