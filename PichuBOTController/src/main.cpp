#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h> 
#include <RF24.h>

const byte cmdid_iscon = 1;
const byte cmdid_print = 2;

struct cmd_iscon {
	byte id;
	byte result;
};
typedef struct cmd_iscon CMD_iscon;

RF24 radio = RF24(7, 8);

byte local[] = "000010";
byte remote[] = "000100";

int strsrt(const char *a, const char *b);
void print_radio_health();

void setup() {
    Serial.begin(9600);

    delay(1000);

    radio.begin();
    radio.setChannel(115);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);

    radio.openReadingPipe(1, local);
    radio.openWritingPipe(remote);

    radio.startListening();

    Serial.println("Robot transmitter has started. Waiting for pingback...");
    Serial.print("Is Radio Connected: ");
    Serial.println(radio.isChipConnected());
}

void loop() {
    if (Serial.available() > 0) {
        // Serial.println("controller> sending... ");

		byte serialcmd = Serial.read();
		if(serialcmd == 1) {
			parsecommand();
		}
		else if(serialcmd == 2) {
			parsecontroller();
		}

        String msgs = Serial.readStringUntil('\n');
        const char *msg = msgs.c_str();
        Serial.println(msgs);

        if(strcmp(msg, "health") == 0) {
			print_radio_health();
		}
		else if(strcmp(msg, "help") == 0) {
			print_help();
		}
		else if(strcmp(msg, "start") == 0) {
			radio.stopListening();
		}
		else if(strcmp(msg, "stop") == 0) {
			radio.stopListening();
		}
		
		if (strsrt(msg, "print") == 0) {
            radio.stopListening();
            size_t len = strlen(msg) - 5;
            if (radio.write(&cmdid_print, sizeof(byte)) && radio.write(msg + 6, len)) {
            }
            Serial.print("sent:");
            Serial.println(msg + 6);

            radio.startListening();
        }
    }

    while(radio.available() == true) {
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

int strsrt(const char *a, const char *b) {
    if (strncmp(a, b, strlen(b)) == 0) return 0;
    return 1;
}

void send() {}
void read() {}

void print_radio_health() {
	Serial.println("checking radio health...");
		if(radio.isChipConnected()) {
			Serial.println("	the chip is connected.");
		}
		else {
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


		if(radio.testRPD()) {
			Serial.println("	radio's signal is strong.");
		}
		else {
			Serial.println("	radio's signal is weak.");
		}
		if(radio.rxFifoFull()) {
			Serial.println("	read buffer is full.");
		}
		if(radio.available()) {
			Serial.println("	data is avaliable to read.");
		}
		Serial.println();
}

void print_help() {
	Serial.println("local commands...");
	Serial.println("	help - this menu.");
	Serial.println("	clear - clears the console.");
	Serial.println("	health - displays the radios connection details.");
	Serial.println("	start - starts interfacing with the robot.");
	Serial.println("	stop - stops interfacing with the robot.");

	Serial.println("remote commands...");
	Serial.println("	print [message] - sends a message to the robot.");
	Serial.println("		[message] - the message that gets sent to the robot.");
	Serial.println("	ping - pings the robot and waits for a pingback or timeout.");
	Serial.println("	drive [speed] [turn] [time] - drives the robot in a direction and speed for a set amount of time.");
	Serial.println("		[speed] - a value between -127 and 127 that dictates how fast the robot will drive.");
	Serial.println("		[turn] - a value between -127 and 127 that dictates what direction the robot will be turning while driving.");
	Serial.println("		[time] - the amount of milliseconds to repeat this command for.");
	Serial.println("	tdrive [right] [left] [time] - drives the robot by setting the speed of the right and left motors for a set amount of time.");
	Serial.println("		[right] - a value between -127 and 127 that dictates how fast the right motor will turn.");
	Serial.println("		[left] - a value between -127 and 127 that dictates how fast the left motor will turn.");
	Serial.println("		[time] - the amount of milliseconds to repeat this command for.");
	Serial.println("	claw [angle] - open or closes the claw based an angle.");
	Serial.println("		[angle] - a value between -90 and 90 that tells the serovs what angle the claw should turn to.");
}