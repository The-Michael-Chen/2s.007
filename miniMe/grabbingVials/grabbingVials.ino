#include <Servo.h>
#include <DRV8835MotorShield.h>
#include <math.h>

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


#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

DRV8835MotorShield motors;


float IR1Val, IR2Val, IR3Val, IR4Val, IR5Val;

float setpoint = 3.0;
int leftSpeed = 100;
int rightSpeed = 100;
int normalSpeed = 100; // 100 for sharp turns

float previousError = 0.0;
float integral = 0.0;

float prevXIR = 3.0;

int numLines = 0;

float lastLineCrossTime = 0;
float lineCrossDelay = 150;

int totalLines = 7;

Adafruit_BNO055 bno = Adafruit_BNO055(55);


void setup() {
  Serial.begin(9600);

  pinMode(A8, INPUT); 
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  topServo.write(topOpen);
  bottomServo.write(bottomOpen);
  topServo.attach(topServoPin);
  bottomServo.attach(bottomServoPin);

   pinMode(22, OUTPUT); // BLUE LED
   if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
  motors.flipM2(true);
}

int instr = 1;

void loop() {

  // left side
  if (numLines < totalLines) {
    proportionalController();
  } else {
    // stop driving
    motors.setSpeeds(0, 0);
    delay(1000);
    if (instr == 1) {
      motors.setSpeeds(70, 70);
      delay(500);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      turnLeft(180);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    } 
    else if (instr == 2) {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      delay(200);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      turnRight(170);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
    else if (instr == 3) {
      turnLeft(30);
      delay(500);
      motors.setSpeeds(70, 70);
      delay(500);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(-70, -70);
      delay(500);
      turnRight(45);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      turnLeft(180);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
     else if (instr == 4) {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      delay(200);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      turnRight(170);
      instr = instr + 1;
      numLines = 7;
      numLines = 0;
    }
    else if (instr == 5) {
      turnRight(30);
      delay(500);
      motors.setSpeeds(70, 70);
      delay(500);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(-70, -70);
      delay(500);
      turnLeft(45);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      turnLeft(180);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
     else if (instr == 6) {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      delay(200);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      turnRight(170);
      instr = instr + 1;
      numLines = 7;
      numLines = 0;
    }
  }
  
}

void proportionalController() {
  float Kp = 25.0;
  float dt = 0.01;
  float Kd = 0.3;
  float Ki = 0;
  float actualValue = readValue();
  float error = setpoint - actualValue; 
  float derivative = (error - previousError)/dt;
  float integral = integral + error * dt;
  float deltaSpeed = Kp*error + Kd * derivative + Ki*integral;
  previousError = error;
  leftSpeed = normalSpeed - deltaSpeed;
  rightSpeed = normalSpeed + deltaSpeed;
  motors.setSpeeds(leftSpeed, rightSpeed);
  delay(0.01);
}


bool onLine = false;

float readValue() {
  IR1Val = constrain((analogRead(A8) - 296.0)/(979.0-296.0), 0, 1); // minimum 235 - 982
  IR2Val = constrain((analogRead(A9) - 302.0)/(980.0-302.0), 0, 1); // min 235 - 983
  IR3Val = constrain((analogRead(A10) - 490.0)/(981.0-490.0), 0, 1); // 231 - 983
  IR4Val = constrain((analogRead(A11) - 380.0)/(980.0-380.0), 0, 1); // 253 - 983
  IR5Val = constrain((analogRead(A12) - 382.0)/(980.0-382.0), 0, 1); // 300 - 983

  float xIR = ((1-IR1Val) + (1-IR2Val)*2 + (1-IR3Val)*3 + (1-IR4Val)*4 + (1-IR5Val)*5)/((1.000-IR1Val) + (1.000-IR2Val) + (1.000-IR3Val) + (1.000-IR4Val) + (1.000-IR5Val));
  //Serial.println(xIR);
  
  if (isnan(xIR)) { // if it detects all white
    xIR = prevXIR;
  } else {
    prevXIR = xIR;
  }
  //(IR1Val < 0.5) && (IR2Val < 0.5) && (IR3Val < 0.5) && (IR4Val < 0.5) && (IR5Val < 0.5)
  if ((IR2Val < 0.5) && (IR4Val < 0.5)) {
    if (onLine == false) {
      onLine = true;
      lastLineCrossTime = millis();
      numLines = numLines + 1; 
      Serial.println("added Line detection");
      digitalWrite(22, HIGH);
      delay(50);
      digitalWrite(22, LOW);
    }
  }else {
    onLine = false;
  }
  
  return xIR;
}

void turnLeft(double deg) {
  // old 0 -> 360 new 0-> 360
  sensors_event_t event;
  bno.getEvent(&event);
  double oldAngle = event.orientation.x;
  double newAngle = event.orientation.x;
  do {
    motors.setSpeeds(-70, 70);
    sensors_event_t event;
    bno.getEvent(&event);
    newAngle = event.orientation.x;
  } while (checkAngleLeft(oldAngle, newAngle, deg));
    motors.setSpeeds(0, 0);
    delay(1000);
    return;
}

void turnRight(double deg) {
  sensors_event_t event;
  bno.getEvent(&event);
  double oldAngle = event.orientation.x;
  double newAngle = event.orientation.x;
  
  do {
    motors.setSpeeds(70, -70);
    sensors_event_t event;
    bno.getEvent(&event);
    newAngle = event.orientation.x;
  } while (checkAngle(oldAngle, newAngle, deg));
    motors.setSpeeds(0, 0);
    delay(1000);
    return;
}

bool checkAngle(double oldAngle, double newAngle, double turnAngle) {
  Serial.print("current Angle:    ");
  Serial.println(newAngle);
  double targetAngle = fmod(((360 + oldAngle) + turnAngle), 360.0);
  if ((newAngle <= (targetAngle + 4.0)) && (newAngle >= (targetAngle - 4.0)) ) {
    return false;
  } else {
    return true;
  }
}

bool checkAngleLeft(double oldAngle, double newAngle, double turnAngle) {
  Serial.print("current Angle:    ");
  Serial.println(newAngle);
  double targetAngle = fmod(((360 + oldAngle) - turnAngle), 360.0);
  if ((newAngle <= (targetAngle + 4.0)) && (newAngle >= (targetAngle - 4.0)) ) {
    return false;
  } else {
    return true;
  }
}
