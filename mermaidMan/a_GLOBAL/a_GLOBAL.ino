#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "NEET_RF24.h"

#define NRF_CE 38
#define NRF_CSN 39
#define JOYSTICK_DEADZONE 5
#define echoPin 28 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 29 //attach pin D3 Arduino to pin Trig of HC-SR04

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Set port M1 as the left motor and port M2 as the right motor
Adafruit_DCMotor *motorLeft = AFMS.getMotor(4);
Adafruit_DCMotor *motorRight = AFMS.getMotor(3);
Adafruit_DCMotor *motorLift = AFMS.getMotor(1);

float IR1Val, IR2Val, IR3Val, IR4Val, IR5Val;

float setpoint = 3.0;
int leftSpeed = 80;
int rightSpeed = 80;
int normalSpeed = 80; // 100 for sharp turns

float previousError = 0.0;
float integral = 0.0;

float prevXIR = 3.0;

int numLines = 0;

float lastLineCrossTime = 0;
float lineCrossDelay = 150;

int totalLines = 4;
int instr = 1;

Adafruit_BNO055 bno = Adafruit_BNO055(55);


// Declare the radio and motors
NEET_RF24 radio(NRF_CE, NRF_CSN, 27);
// Holds the input from controller
ControlInput input;


bool pressedRecently = false;
bool onLine = false;

bool activeRobot = true;

bool onLeftSide = false;
//autonomous sequence
int eventNumber = 0;
bool beginEvent = true;
