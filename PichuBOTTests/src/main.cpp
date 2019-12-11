#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7, 8);

#define RADIO_NUMBER 1

const int64_t addresses[2] = {0x00000F10, 0x00000F20};

bool tx_ok;
bool tx_fail;
bool rx_ready;

uint16_t tick = 0;

#if RADIO_NUMBER == 0
    char buffer[16] = {0};
#elif RADIO_NUMBER == 1
    const char buffer[16] = "Hello World!\0\0\0";
	int send_count = 0;
	int failed_send_count = 0;
#endif

void setup() {
    Serial.begin(9600);
	delay(50);

    radio.begin();

    radio.setPALevel(RF24_PA_HIGH);
    radio.setChannel(12);
    radio.setDataRate(RF24_250KBPS);

#if RADIO_NUMBER == 0
    radio.openReadingPipe(1, addresses[0]);
    radio.startListening();
#elif RADIO_NUMBER == 1
    radio.openWritingPipe(addresses[0]);
#endif

	Serial.print("connnected:");
	Serial.println(radio.isChipConnected());
}

void loop() {
#if RADIO_NUMBER == 0
    if (radio.available() == true) {
        radio.read(&buffer, 16);
        Serial.print("recived:");
        Serial.println(buffer);
    }
	delay(50);
#elif RADIO_NUMBER == 1
	if(radio.write(&buffer, sizeof(buffer)) == false) {
		failed_send_count++;
	}
	send_count++;
	delay(50);
#endif

	if(tick % 300 == 0) {
		Serial.println("checking radio health...");
		if(radio.isChipConnected()) {
			Serial.println("	the chip is connected.");
		}
		else {
			Serial.println("the chip is disconnected.");
		}
		Serial.print("	channel:");
		Serial.println(radio.getChannel());
		Serial.print("	data rate:");
		Serial.println(radio.getDataRate());

		#if RADIO_NUMBER == 1
			Serial.print("	send count:");
			Serial.println(send_count);
			Serial.print("	failed send count:");
			Serial.println(failed_send_count);
			send_count = 0;
			failed_send_count = 0;
		#endif

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
		tick = 0;
	}

	tick++;
}