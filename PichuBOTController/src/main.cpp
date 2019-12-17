#include <Arduino.h>

#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>

#include <MsgIds.h>

typedef struct msg_drive {
    byte id;
    byte speed;
    byte turn;
} msg_drive;

typedef struct msg_drivetimed {
    byte id;
    byte speed;
    byte turn;
    uint16_t time;
} msg_drivetimed;

typedef struct msg_claw {
    byte id;
    byte angle;
} msg_claw;

// radio variables.
RF24 radio = RF24(9, 10);
const int64_t addresses[2] = {0x00000F10, 0x00000F20};

// send buffer variables.
char *send_buffer;

uint16_t send_count;
uint16_t failed_send_count;

// read buffer variables.
void *read_buffer;
uint8_t read_buffer_len;

// localcmd functions.
void cmdfunc_help();
void cmdfunc_health();
void cmdfunc_setchnnl();
void cmdfunc_start();
void cmdfunc_stop();

void cmdfunc_print();
void cmdfunc_ping();
void cmdfunc_drive();
void cmdfunc_drivetimed();
void cmdfunc_tdrive();
void cmdfunc_tdrivetimed();
void cmdfunc_claw();

// parsing functions.

// radio functions.
void send();
void send_msgid(uint8_t id);

/* -------------------------------------------------------------------------- */
/*                                    setup                                   */
/* -------------------------------------------------------------------------- */

void setup() {
    // Start serial communication.
    Serial.begin(9600);

    // start and configure radio communication.
    radio.begin();
    radio.setChannel(12);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);

    // open radio to both reading and writing.
    radio.openReadingPipe(1, addresses[0]);
    radio.openWritingPipe(addresses[0]);

    // start listening for transmissions.
    radio.startListening();
	send_buffer = (char*)malloc(64);

    Serial.println("Robot transmitter has started. Waiting for pingback...");
}

/* -------------------------------------------------------------------------- */
/*                                    loop                                    */
/* -------------------------------------------------------------------------- */

void loop() {
    if (Serial.available() > 0) {
		// byte msgid = Serial.peek();
		byte msgid = Serial.read();

        switch (msgid) {
            case msgid_blank:
				Serial.println("invalid command.");
                break;

            case msgid_help:
                cmdfunc_help();
                break;

            case msgid_health:
                cmdfunc_health();
				send_msgid(msgid_health);
                break;

            case msgid_setchnnl:
                cmdfunc_setchnnl();
                break;

            case msgid_start:
                cmdfunc_start();
                break;

            case msgid_stop:
                cmdfunc_stop();
                break;

            case msgid_print:
                cmdfunc_print();
                break;

            case msgid_ping:
                cmdfunc_ping();
                break;

            case msgid_drive:
                cmdfunc_drive();
                break;

            case msgid_drivetimed:
                cmdfunc_drivetimed();
                break;

            case msgid_tdrive:
                cmdfunc_tdrive();
                break;

            case msgid_tdrivetimed:
                cmdfunc_tdrivetimed();
                break;

            case msgid_claw:
                cmdfunc_claw();
                break;

            default:
				// Serial.println("invalid command.");
				// Serial.readStringUntil('\0');
				// Serial.read();
                break;
        }
    }

    if (radio.available() == true) {
        byte cmdid;
        radio.read(&cmdid, sizeof(byte));

        if (cmdid != 0) {
            switch (cmdid) {
                case msgid_print:
                    char printmsg[32];
                    radio.read(&printmsg, sizeof(char[32]));
                    Serial.print("robot: ");
                    Serial.println(printmsg);
                    break;
                default:
                    break;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                               radio functions                              */
/* -------------------------------------------------------------------------- */

void send(uint8_t size) {
    radio.stopListening();
    if (radio.write(send_buffer, size) == true) {
        send_count++;
    } else {
        failed_send_count++;
    }

    radio.startListening();

	// free(send_buffer);
	// send_buffer = (char*)malloc(64);
}

void send_msgid(uint8_t id) {
	send_buffer[0] = id;

	send(1);
}

/* -------------------------------------------------------------------------- */
/*                             localcmd functions                             */
/* -------------------------------------------------------------------------- */

void cmdfunc_print() {
	Serial.println("sending...");
	send_msgid(msgid_print);
	memcpy(send_buffer, Serial.readString().c_str(), 31);
    send(32);

    Serial.print("sent: ");
    Serial.println(send_buffer);
}

void cmdfunc_ping() {
	send_msgid(msgid_ping);
}

void cmdfunc_drive() {
    send_msgid(msgid_drive);

	send_buffer[0] = Serial.read();
	send_buffer[1] = Serial.read();

	send(2);
}

void cmdfunc_tdrive() {
    send_msgid(msgid_tdrive);

	send_buffer[0] = Serial.read();
	send_buffer[1] = Serial.read();

	Serial.print("drive(");
	Serial.print((byte)send_buffer[0]);
	Serial.print(",");
	Serial.print((byte)send_buffer[1]);
	Serial.println(")");

	send(2);
}

void cmdfunc_drivetimed() {}
void cmdfunc_tdrivetimed() {}

void cmdfunc_claw() {
    send_msgid(msgid_claw);

	send_buffer[0] = Serial.read();

	send(1);
}

void cmdfunc_setchnnl() {
    uint8_t channel;
    Serial.readBytes(&channel, sizeof(uint8_t));

    radio.setChannel(channel);

    Serial.print("channel set to ");
    Serial.println(channel);
}

void cmdfunc_start() {}
void cmdfunc_stop() {}

void cmdfunc_health() {
    Serial.println("checking radio health...");
    if (radio.isChipConnected()) {
        Serial.println("	the chip is connected.");
    } else {
        Serial.println("	the chip is disconnected.");
    }
    Serial.print("	channel:");
    Serial.println(radio.getChannel());
    Serial.print("	data rate:");
    Serial.println(radio.getDataRate());

    Serial.print("	send count:");
    Serial.println(send_count);
    Serial.print("	failed send count:");
    Serial.println(failed_send_count);
    send_count = 0;
    failed_send_count = 0;

    if (radio.testRPD()) {
        Serial.println("	radio's signal is strong.");
    } else {
        Serial.println("	radio's signal is weak.");
    }
    if (radio.rxFifoFull()) {
        Serial.println("	read buffer is full.");
    }
    if (radio.available()) {
        Serial.println("	data is avaliable to read.");
    }
    Serial.println();
}

void cmdfunc_help() {
    Serial.println(F("local commands..."));
    Serial.println(F("	help - this menu."));
    Serial.println(F("	clear - clears the console."));
    Serial.println(F("	health - displays the radios connection details."));
    Serial.println(F("	setchnnl [channel] - sets the channel of the current radio to the specifed channel."));
    Serial.println(F("		[channel] - a value between 0 and 125 that is used as the channel number of the radio."));
    Serial.println(F("	start - starts interfacing with the robot."));
    Serial.println(F("	stop - stops interfacing with the robot."));

    Serial.println(F("remote commands..."));
    Serial.println(F("	print [message] - sends a message to the robot."));
    Serial.println(F("		[message] - the message that gets sent to the robot."));
    Serial.println(F("	ping - pings the robot and waits for a pingback or timeout."));
    Serial.println(F("	setbchnnl [channel] - sets the channel of the robots radio to the specifed channel."));
    Serial.println(F("		[channel] - a value between 0 and 125 that is used as the channel number of the bots radio."));
    Serial.println(F("	drive [speed] [turn] [time] - drives the robot in a direction and speed for a set amount of time."));
    Serial.println(F("		[speed] - a value between -127 and 127 that dictates how fast the robot will drive."));
    Serial.println(F("		[turn] - a value between -127 and 127 that dictates what direction the robot will be turning while driving."));
    Serial.println(F("		[time] - the amount of milliseconds to repeat this command for."));
    Serial.println(F("	tdrive [right] [left] [time] - drives the robot by setting the speed of the right and left motors for a set amount of time."));
    Serial.println(F("		[right] - a value between -127 and 127 that dictates how fast the right motor will turn."));
    Serial.println(F("		[left] - a value between -127 and 127 that dictates how fast the left motor will turn."));
    Serial.println(F("		[time] - the amount of milliseconds to repeat this command for."));
    Serial.println(F("	claw [angle] - open or closes the claw based an angle."));
    Serial.println(F("		[angle] - a value between -90 and 90 that tells the serovs what angle the claw should turn to."));
}