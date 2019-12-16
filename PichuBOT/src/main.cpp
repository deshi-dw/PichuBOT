#include <Arduino.h>

#include <Motor.h>
#include <RobotClaw.h>
#include <RobotDrive.h>
#include <Servo.h>

#include <MsgIds.h>
#include <RobotClient.h>

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

    client.start(12);
    client.connect();

	Serial.println("robot has started.");
}

char teststr[32] = "Hello World!";

void loop() {
    if (client.radio.available() == true) {
		byte msgid = client.readByte();
		Serial.println("message recived.");
		delay(100);

        switch (msgid) {
            case msgid_blank:
                break;

            case msgid_help:
                break;

            case msgid_health:
                break;

            case msgid_print:
                char *strbuff;
                client.readString(strbuff);

                client.writeByte(msgid_print);
                client.writeString(strbuff);

				Serial.println(strbuff);
                client.send();
                break;

            case msgid_ping:
                client.writeMsg(msgid_ping);
                client.send();
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
	else {
		// Serial.println("waiting for data...");
	}

	// client.writeByte(msgid_print);
	// client.writeString(teststr);
	// client.send();
}