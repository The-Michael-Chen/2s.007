#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_MotorShield.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);


// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Set port M1 as the left motor and port M2 as the right motor
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  turnLeft(90.0);


}

void turnLeft(double deg) {
  /* turn for 100
     check the yaw
     if the absolute difference between the current n initial yaw is >= 180 stop
  */
  // old 0 -> 360 new 0-> 360
  sensors_event_t event;
  bno.getEvent(&event);
  double oldAngle = event.orientation.x;
  double newAngle = event.orientation.x;
  do {
    motorLeft->run(BACKWARD);
    motorRight->run(FORWARD);
    motorLeft->setSpeed(70);
    motorRight->setSpeed(70);
    sensors_event_t event;
    bno.getEvent(&event);
    newAngle = event.orientation.x;
  } while (checkAngleLeft(oldAngle, newAngle, deg));
  motorLeft->run(RELEASE);
  motorRight->run(RELEASE);
  return;
}

void turnRight(double deg) {
  sensors_event_t event;
  bno.getEvent(&event);
  double oldAngle = event.orientation.x;
  double newAngle = event.orientation.x;
  Serial.print("old Angle");
  Serial.println(oldAngle);
  
  do {
    motorLeft->run(FORWARD);
    motorRight->run(BACKWARD);
    motorLeft->setSpeed(70);
    motorRight->setSpeed(70);
    sensors_event_t event;
    bno.getEvent(&event);
    newAngle = event.orientation.x;
  } while (checkAngle(oldAngle, newAngle, deg));
  motorLeft->setSpeed(0);
  motorRight->setSpeed(0);
  delay(1000);
  
  return;
}

bool checkAngle(double oldAngle, double newAngle, double turnAngle) {
  // old angle and new angle, we need to figure out if the diff between old angle and new angle is >= target angle
  // normally can just do oldAngle - new Angle
  // but if oldAngle is not big enough we run into issues 
  //    so we can check to see if it's a small old angle- if so, we can just 
  if ((oldAngle + turnAngle) > 360) {
    double targetAngle = turnAngle - (360.0 - oldAngle);
    if ((newAngle <= (targetAngle + 2.0)) && (newAngle >= (targetAngle - 2.0))) {
      Serial.println("weird");
      return false; 
    } else {
      return true;
    }
  } else {
    if ((newAngle - oldAngle) >= (turnAngle)) {
      Serial.println("regular");
      return false;
    } else {
      return true;
    }
  }
}

bool checkAngleLeft(double oldAngle, double newAngle, double turnAngle) {
  // old angle and new angle, we need to figure out if the diff between old angle and new angle is >= target angle
  // normally can just do oldAngle - new Angle
  // but if oldAngle is not big enough we run into issues 
  //    so we can check to see if it's a small old angle- if so, we can just 
  if ((oldAngle - turnAngle) < 0) {
    double targetAngle = 360.0 - (turnAngle - oldAngle);
    if ((newAngle <= (targetAngle + 2.0)) && (newAngle >= (targetAngle - 2.0))) {
      Serial.println("weird");
      return false; 
    } else {
      return true;
    }
  } else {
    if ((oldAngle - newAngle ) >= (turnAngle)) {
      Serial.println("regular");
      return false;
    } else {
      return true;
    }
  }
}
