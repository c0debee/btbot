#include <Servo.h>
#include <Wire.h>

const int MOTOR_A_CW  = 4;
const int MOTOR_A_CCW = 5;
const int MOTOR_B_CW  = 6;
const int MOTOR_B_CCW = 7;

const int SONAR_TRIG = 8;
const int SONAR_ECHO = 9;

const int IR_LEFT  = 3;
const int IR_MID   = 11;
const int IR_RIGHT = 10;

const int TELEMETRY_TX_INTERVAL = 500;

long telemetry_tx_next_ms = 0;

void setup() {
  pinMode(MOTOR_A_CW, OUTPUT);
  pinMode(MOTOR_A_CCW, OUTPUT);
  pinMode(MOTOR_B_CW, OUTPUT);
  pinMode(MOTOR_B_CCW, OUTPUT);

  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_MID, INPUT);
  pinMode(IR_RIGHT, INPUT);

  halt();
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    switch(Serial.read()) {
    case 'w':
      halt();
      forward();
      break;
    case 's':
      halt();
      backward();
      break;
    case 'a':
      halt();
      turnLeft();
      break;
    case 'd':
      halt();
      turnRight();
      break;
    case ' ':
      halt();
      break;
    }
  }
  transmitTelemetry();
}

void halt() {
  digitalWrite(MOTOR_A_CW, HIGH);
  digitalWrite(MOTOR_A_CCW, HIGH);
  digitalWrite(MOTOR_B_CW, HIGH);
  digitalWrite(MOTOR_B_CCW, HIGH);
}

void forward() {
  digitalWrite(MOTOR_A_CW, LOW);
  digitalWrite(MOTOR_B_CW, LOW);
}

void backward() {
  digitalWrite(MOTOR_A_CCW, LOW);
  digitalWrite(MOTOR_B_CCW, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_A_CCW, LOW);
  digitalWrite(MOTOR_B_CW, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_A_CW, LOW);
  digitalWrite(MOTOR_B_CCW, LOW);
}

long measureDistance() {
  long duration;

  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG, LOW);

  duration = pulseIn(SONAR_ECHO, HIGH);
  return duration / 29 / 2;
}

void transmitTelemetry() {
  if(millis() > telemetry_tx_next_ms) {
    Serial.print("\033[2J\033[;H");
    Serial.print("Distance: ");
    Serial.println(measureDistance(), DEC);

    telemetry_tx_next_ms = millis() + TELEMETRY_TX_INTERVAL;
  }
}

