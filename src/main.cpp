#include <Arduino.h>
#include <Robot.h>

Robot bot = Robot();
Motor rightMotor = Motor(1, 2, 3);
Motor leftMotor = Motor(4, 5, 6);

void init_idle() {
	Serial.println("init idle");
}
void loop_idle() {
	Serial.println("loop idle");
}

void init_autonomous() {
	Serial.println("init autonomous");
}
void loop_autonomous() {
	Serial.println("loop autonomous");
}

void init_teleop() {
	Serial.println("init teleop");
}
void loop_teleop() {
	Serial.println("loop teleop");
}

void setup() {
	/**
	 * Byte array to float example:
	 * byte incoming[4]={0X8A,0X23,0X48,0X42};
	 * float velocity = *( (float*) incoming );
	*/

	bot.motorRight = rightMotor;
	bot.motorLeft = leftMotor;

	Serial.begin(9600);

	bot.states[INIT_IDLE] = &init_idle;
	bot.states[LOOP_IDLE] = &loop_idle;

	bot.states[INIT_AUTONOMOUS] = &init_autonomous;
	bot.states[LOOP_AUTONOMOUS] = &loop_autonomous;

	bot.states[INIT_TELEOP] = &init_teleop;
	bot.states[LOOP_TELEOP] = &loop_teleop;
	bot.ready();
}

void loop() {
	Serial.print("[");
	Serial.print(bot.time);
	Serial.print("] state:");
	Serial.println(bot.state);
	delay(1000);

	bot.update();
}