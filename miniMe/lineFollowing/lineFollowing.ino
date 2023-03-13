#include <Adafruit_MotorShield.h>

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Set port M1 as the left motor and port M2 as the right motor
Adafruit_DCMotor *motorLeft = AFMS.getMotor(4);
Adafruit_DCMotor *motorRight = AFMS.getMotor(3);

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

int totalLines = 17;


void setup() {
  Serial.begin(9600);

  pinMode(A8, INPUT); 
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
   Serial.println("Motor Shield found.");
   pinMode(22, OUTPUT); // BLUE LED
}

void loop() {
  // bangbang();
  if (numLines < totalLines) {
    proportionalController();
  } else {
    // stop driving
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
    delay(100);
  }
}

void proportionalController() {
  float Kp = 50.0;
  float dt = 0.01;
  float Kd = 0;
  float Ki = 0;
  float actualValue = readValue();
  float error = setpoint - actualValue; 
  float derivative = (error - previousError)/dt;
  float integral = integral + error * dt;
  float deltaSpeed = Kp*error + Kd * derivative + Ki*integral;
  previousError = error;
  leftSpeed = normalSpeed - deltaSpeed;
  rightSpeed = normalSpeed + deltaSpeed;
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);
  motorLeft->setSpeed(leftSpeed);
  motorRight->setSpeed(rightSpeed);
}

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
  
  if ((IR1Val < 0.5) && (IR2Val < 0.5) && (IR3Val < 0.5) && (IR4Val < 0.5) && (IR5Val < 0.5)) {
    if ((millis() - lastLineCrossTime) > lineCrossDelay) {
      lastLineCrossTime = millis();
      numLines = numLines + 1; 
      Serial.println("added Line detection");
      digitalWrite(22, HIGH);
      delay(50);
      digitalWrite(22, LOW);
    }
  }
  
  return xIR;
}

//void turnLeft(int normalSpeed, int deltaSpeed) {
//  leftSpeed = normalSpeed - deltaSpeed;
//  rightSpeed = normalSpeed + deltaSpeed;
//  motors.setM1Speed(leftSpeed);
//  motors.setM2Speed(rightSpeed);
//}
//
//void turnRight(int normalSpeed, int deltaSpeed) {
//  leftSpeed = normalSpeed + deltaSpeed;
//  rightSpeed = normalSpeed - deltaSpeed;
//  motors.setM1Speed(leftSpeed);
//  motors.setM2Speed(rightSpeed);
//}
