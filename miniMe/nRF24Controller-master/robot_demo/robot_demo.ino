#define NRF_CE 38
#define NRF_CSN 39
#define JOYSTICK_DEADZONE 5
#define echoPin 28 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 29 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

#include "NEET_RF24.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>



float IR1Val, IR2Val, IR3Val, IR4Val, IR5Val;

// Declare the radio and motors
NEET_RF24 radio(NRF_CE, NRF_CSN, 27);

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor_left = AFMS.getMotor(4);
Adafruit_DCMotor *motor_right = AFMS.getMotor(3);

Servo s;

// Holds the input from controller
ControlInput input;

bool pressedRecently = false;

int8_t last_servo = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55);

Servo topServo; // Standard 180° servo
// Define servo signal pin
const int topServoPin = 24;
const int topOpen = 40;
const int topClosed = 93;

Servo bottomServo; // Standard 180° servo
// Define servo signal pin
const int bottomServoPin = 9;
const int bottomOpen = 0;
const int bottomClosed = 50;

void setup(){
  topServo.write(topClosed);
  bottomServo.write(bottomClosed);
  topServo.attach(topServoPin);
  bottomServo.attach(bottomServoPin);

  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(115200);
  if (!radio.begin()){
    Serial.println("Radio not started");
  } else {
    Serial.println("Radio started");
  }
  AFMS.begin();
  
  s.attach(27);
  radio.rxSendTelemetry("Hello world! " + String(analogRead(A0)));
   if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);

  bno.setExtCrystalUse(true);
}



void loop(){
  // If radio input is valid
  if (radio.rxUpdate()){
    Serial.println("getting updates");
    //getDistance();
    //radio.rxSendTelemetry("Yaw Value: " + String(getYaw()) + " Weighted Average: " + String(readValue()) + " Distance: " + String(distance));
    
    // Gets value from radio
    input = radio.rxGetInput();
    int left = 0;
    int right = 0;
    if (input.tSwitch == true) {
      Serial.println("Switch on");
      //digitalWrite(22, HIGH);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
    } else {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      //digitalWrite(22, LOW);
    }
    if (input.button1 == true && !pressedRecently) {
      motor_left->setSpeed(0);
      motor_right->setSpeed(0);
    }
    if (input.button1 == false) {
      pressedRecently = false;
    } else {
      pressedRecently = true;
    }
    pinMode(22, OUTPUT);
    // Don't process unless the joysticks are more than 5 away from center
    if (abs(input.j1PotY) > JOYSTICK_DEADZONE){
      Serial.println("getting joystick vals");
      left = input.j1PotY;
    }
    if (abs(input.j2PotX) > JOYSTICK_DEADZONE){
      right = input.j2PotX;
    }
    // Arcade drive
    drive(left + right * 0.4, left - right * 0.4);

    if (abs(input.pot - last_servo) > 5){
      s.write(map(input.pot, -127, 127, 0, 180));
      last_servo = input.pot;
    }
  } else {
    drive(0, 0);
  }
}

float getYaw() {
  sensors_event_t event;
  bno.getEvent(&event);
  double newAngle = event.orientation.x;
  return newAngle;
}

float readValue() {
  IR1Val = constrain((analogRead(A8) - 296.0)/(979.0-296.0), 0, 1); // minimum 235 - 982
  IR2Val = constrain((analogRead(A9) - 302.0)/(980.0-302.0), 0, 1); // min 235 - 983
  IR3Val = constrain((analogRead(A10) - 490.0)/(981.0-490.0), 0, 1); // 231 - 983
  IR4Val = constrain((analogRead(A11) - 380.0)/(980.0-380.0), 0, 1); // 253 - 983
  IR5Val = constrain((analogRead(A12) - 382.0)/(980.0-382.0), 0, 1); // 300 - 983

  float xIR = ((1-IR1Val) + (1-IR2Val)*2 + (1-IR3Val)*3 + (1-IR4Val)*4 + (1-IR5Val)*5)/((1.000-IR1Val) + (1.000-IR2Val) + (1.000-IR3Val) + (1.000-IR4Val) + (1.000-IR5Val));
  return xIR;
}

void getDistance() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

/**
 * @brief Drives the robot by interfacing with the motor controller shield
 * 
 * @param left integer value between -255 and 255 for left motor
 * @param right integer value between -255 and 255 for right motor
 */
void drive(int left, int right){
  right = -right;
  motor_left->setSpeed(abs(left));
  motor_right->setSpeed(abs(right));

  if (left > 0){
    motor_left->run(FORWARD);
  } else if (left < 0){
    motor_left->run(BACKWARD);
  } else {
    motor_left->run(RELEASE);
  }

  if (right > 0){
    motor_right->run(BACKWARD);
  } else if (right < 0){
    motor_right->run(FORWARD);
  } else {
    motor_right->run(RELEASE);
  }
}
