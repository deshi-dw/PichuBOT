#include <Arduino.h>

#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>

// arcade driving commands.
struct cmd_drive {
    byte speed;
    byte turn;
};
typedef struct cmd_drive cmd_drive;
const byte cmdid_drive = 0b00100011;

struct cmd_drivetimed {
    byte speed;
    byte turn;
};
typedef struct cmd_drivetimed cmd_drivetimed;
const byte cmdid_drivetimed = 0b00100100;

// tank driving commands.
struct cmd_tdrive {
    byte speed;
    byte turn;
};
typedef struct cmd_tdrive cmd_tdrive;
const byte cmdid_tdrive = 0b00100101;

struct cmd_tdrivetimed {
    byte speed;
    byte turn;
};
typedef struct cmd_tdrivetimed cmd_tdrivetimed;
const byte cmdid_tdrivetimed = 0b00100110;

// Servo commands.
struct cmd_claw {
    byte angle;
};
typedef struct cmd_claw cmd_claw;
const byte cmdid_claw = 0b00100111;

// Utility commands.
struct cmd_print {
    char message[31];
};
typedef struct cmd_print cmd_print;
const byte cmdid_print = 0b00100000;

struct cmd_ping {
    bool result;
};
typedef struct cmd_ping cmd_ping;
const byte cmdid_ping = 0b00100001;

struct cmd_setbchnnl {
    uint8_t channel;
};
typedef struct cmd_setbchnnl cmd_setbchnnl;
const byte cmdid_setbchnnl = 0b00100010;

// local command ids.
const byte cmdid_blank = 0b00000000;
const byte cmdid_help = 0b00000001;
const byte cmdid_health = 0b00000010;
const byte cmdid_setchnnl = 0b00000100;
const byte cmdid_start = 0b00000101;
const byte cmdid_stop = 0b00000110;

// radio variables.
RF24 radio = RF24(7, 8);
const int64_t addresses[2] = {0x00000F10, 0x00000F20};

// send buffer variables.
void *send_buffer;
uint8_t send_buffer_len;

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
void read();
void write();

/* -------------------------------------------------------------------------- */
/*                                    setup                                   */
/* -------------------------------------------------------------------------- */

void setup() {
    // Start serial communication.
    Serial.begin(9600);

    // start and configure radio communication.
    radio.begin();
    radio.setChannel(115);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);

    // open radio to both reading and writing.
    radio.openReadingPipe(1, addresses[0]);
    radio.openWritingPipe(addresses[0]);

    // start listening for transmissions.
    radio.startListening();

    Serial.println("Robot transmitter has started. Waiting for pingback...");
}

/* -------------------------------------------------------------------------- */
/*                                    loop                                    */
/* -------------------------------------------------------------------------- */

void loop() {
    if (Serial.available() > 0) {
        byte serial_id = Serial.read();

        switch (serial_id) {
            case cmdid_blank:
				Serial.println(Serial.readStringUntil('\n'));
                break;

            case cmdid_help:
                cmdfunc_help();
                break;

            case cmdid_health:
                cmdfunc_health();
                break;

            case cmdid_setchnnl:
                cmdfunc_setchnnl();
                break;

            case cmdid_start:
                cmdfunc_start();
                break;

            case cmdid_stop:
                cmdfunc_stop();
                break;


            case cmdid_print:
                cmdfunc_print();
                break;

            case cmdid_ping:
                cmdfunc_ping();
                break;

            case cmdid_drive:
                cmdfunc_drive();
                break;

            case cmdid_drivetimed:
                cmdfunc_drivetimed();
                break;

            case cmdid_tdrive:
                cmdfunc_tdrive();
                break;

            case cmdid_tdrivetimed:
                cmdfunc_tdrivetimed();
                break;

            case cmdid_claw:
                cmdfunc_claw();
                break;

            default:
                break;
        }
    }

    while (radio.available() == true) {
        byte cmdid;
        radio.read(&cmdid, sizeof(byte));

        if (cmdid != 0) {
            switch (cmdid) {
                case cmdid_print:
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

void send() {
    radio.stopListening();

    if (radio.write(&send_buffer, send_buffer_len) == true) {
        send_count++;
    } else {
        failed_send_count++;
    }

    radio.startListening();
}
void read() {
    radio.read(&read_buffer, read_buffer_len);
}

/* -------------------------------------------------------------------------- */
/*                             localcmd functions                             */
/* -------------------------------------------------------------------------- */

void cmdfunc_print() {
	cmd_print print;
	size_t len = Serial.readBytesUntil('\n', (char*)&print, 32);
	print.message[len] = '\0';

	send_buffer = &print;
	send_buffer_len = 32;
	send();

	Serial.print("sent: ");
	Serial.println(print.message);
}

void cmdfunc_drive() {
	cmd_drive drive;
	// drive.speed = 
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