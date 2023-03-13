#include "/Users/michaelchen/Documents/Arduino/2s.007/barnacleBoy/a_GLOBAL/NEET_RF24.h"
#include <Wire.h>
#include <DRV8835MotorShield.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define NRF_CE 38
#define NRF_CSN 39
#define JOYSTICK_DEADZONE 5
#define echoPin 28 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 29 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

// LINE SENSOR
float IR1Val, IR2Val, IR3Val, IR4Val, IR5Val;
float setpoint = 3.0;
int leftSpeed = 10;
int rightSpeed = 100;
int normalSpeed = 100; // 100 for sharp turns
float previousError = 0.0;
float integral = 0.0;
float prevXIR = 3.0;

// Declare the radio and motors
NEET_RF24 radio(NRF_CE, NRF_CSN, 27);

// MOTOR SHIELD
DRV8835MotorShield motors;

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
const int bottomServoPin = 2;
const int bottomOpen = 0;
const int bottomClosed = 50;

// LINE TRACKING
int numLines = 0;
float lastLineCrossTime = 0;
float lineCrossDelay = 150;
int totalLines = 0;
int instr = 0;
bool onLine = false;

//autonomous sequence
int eventNumber = 0;
bool beginEvent = true;
bool onLeftSide = true; 

// IMU
double overshoot = 10.0;
bool activeRobot = true;

bool doneWithScepter = false;
