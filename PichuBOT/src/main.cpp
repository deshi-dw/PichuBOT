#include <Arduino.h>

#include <Motor.h>
#include <RobotClaw.h>
#include <RobotDrive.h>
#include <Servo.h>

#include <MsgIds.h>
#include <RobotClient.h>

// Pins for the right motor.
const byte pin_motorR1 = 2;
const byte pin_motorR2 = 4;
const byte pin_motorRE = 3;

// Pins for the left motor.
const byte pin_motorL1 = 6;
const byte pin_motorL2 = 7;
const byte pin_motorLE = 5;

// Pins for the right and left servo.
const byte pin_servoR = 10;
const byte pin_servoL = 1;

const byte pin_radioen = 8;
const byte pin_radiose = 9;

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

void loop() {
    if (client.radio.available() == true) {
		client.read(1);
		byte msgid = client.readbuff[0];
		// Serial.println("message recived.");

		while(client.radio.available() == false);

        switch (msgid) {
            case msgid_blank:
                break;

            case msgid_help:
                break;

            case msgid_health:
				client.printHealth();
                break;

            case msgid_print:
                client.read(32);

                client.writebuff[0] = msgid_print;
                memcpy(client.writebuff+1, client.readbuff, 31);
				client.writebuff[31] = '\0';
                client.write(32);

				Serial.println(client.writebuff);
                break;

            case msgid_ping:
                client.writebuff[0] = msgid_ping;
                client.write(1);
                break;

            case msgid_drive:
				client.read(2);
                drive.arcadeDrive(client.readbuff[0], client.readbuff[1]);
                break;

            case msgid_tdrive:
				client.read(2);
                drive.tankDrive(client.readbuff[0], client.readbuff[1]);
                break;

            case msgid_claw:
				client.read(1);
                claw.setAngle(client.readbuff[0]);
                break;

            default:
                break;
        }
    }
	else {
		// Serial.println("waiting for data...");
	}

	// drive.arcadeDrive(125, 127);
	// delay(1000);
	// drive.arcadeDrive(0, 20);
	// delay(1000);
	// drive.arcadeDrive(0, 147);
	// delay(1000);

	// client.writeByte(msgid_print);
	// client.writeString(teststr);
	// client.send();

	// claw.setAngle(90);
}