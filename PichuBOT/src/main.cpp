#include <Arduino.h>

#include <Robot.h>
#include <RobotClient.h>
#include <Scheduler.h>

Scheduler scheduler;

Robot bot = Robot();
RobotClient client = RobotClient(8, 9);
Motor rightMotor = Motor(2, 4, 3);
Motor leftMotor = Motor(6, 7, 5);

RobotState pre_state = INIT_ROBOT;

void test_task() {
    Serial.print("Task: (test_task) was executed at...");
    Serial.println(bot.time);
}

void init_idle() {
    Serial.println("init idle");
}
void loop_idle() {
    // Serial.println("loop idle");
    if (bot.time >= 5000) {
        bot.state = INIT_AUTONOMOUS;
    }
}

void loop_testing() {
    // Serial.println("Testing");
    if (bot.isFirstTest == true) {
        client.start((byte*) "1122", (byte*) "1125");
    }
    client.update();

    // Serial.println("write 3 as 255");
    // digitalWrite(3, 255);

    // if (bot.time % 2000 < 1000) {
    //     bot.driveTank(0, 255);
    // } else {
    //     bot.driveTank(255, 0);
    // }
}

void init_autonomous() {
    Serial.println("init autonomous");
    // scheduler.schedule(Task(&test_task, 5000));
}
void loop_autonomous() {
    // Serial.println("loop autonomous");
    // scheduler.update(bot.time);
}

void init_teleop() {
    Serial.println("init teleop");
}
void loop_teleop() {
    // Serial.println("loop teleop");
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

    bot.states[TESTING] = &loop_testing;
    bot.isTesting = true;
    bot.ready();

    client.begin();
}

void loop() {
    pre_state = bot.state;
    bot.update();

    if (pre_state != bot.state) {
        Serial.print("[");
        Serial.print(bot.time);
        Serial.print("] state:");
        Serial.print(pre_state);
        Serial.print(" -> ");
        Serial.println(bot.state);
        Serial.println();
    }

    String recived = Serial.readStringUntil('\n');

    if(recived.length() > 0) {
        Serial.write("Relay::");
        Serial.write(recived.c_str());
        Serial.write('\n');
        Serial.flush();

    }
}