#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h> 
#include <RF24.h>

const byte cmdid_iscon = 1;
const byte cmdid_print = 2;

const byte pin_enable = 8;
const byte pin_select = 9;

RF24 radio = RF24(pin_enable, pin_select);

byte local[] = "000010";
byte remote[] = "000100";

int strsrt(const char *a, const char *b);

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

        String msgs = Serial.readStringUntil('\n');
        const char *msg = msgs.c_str();
        Serial.println(msgs);

        /*
            iscon   =   1
            print   =   2
        */

        if (strcmp(msg, "iscon") == 0) {
            Serial.print("Radio connected: ");
            Serial.println(radio.isChipConnected());
        } else if (strsrt(msg, "print") == 0) {
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
        // int8_t cmdid;
        // radio.read(&cmdid, sizeof(int8_t));

        char printmsg[32];
        radio.read(&printmsg, sizeof(char[32]));

        if(printmsg[0] != 0) {
            Serial.println(printmsg);
            radio.flush_rx();
        }
        else {
            radio.flush_rx();
        }

        // if (cmdid != 0) {
        //     switch (cmdid) {
        //         case 1:
        //             char printmsg[32];
        //             radio.read(&printmsg, sizeof(char[32]));
        //             Serial.print("robot: ");
        //             Serial.println(printmsg);
        //             break;
        //         default:
        //             break;
        //     }
        // }
    }
}

int strsrt(const char *a, const char *b) {
    if (strncmp(a, b, strlen(b)) == 0) return 0;
    return 1;
}