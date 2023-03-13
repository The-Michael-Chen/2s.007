#include <Servo.h>
#include "MPU9250.h"
#include "SensorFusion.h"
#include <Wire.h>

SF fusion;

////IMU
MPU9250 IMU;
float yaw_target = 0;
float gx, gy, gz, ax, ay, az, temp;
float yaw;
float deltat;
int flag = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int stat = IMU.begin();
  if (stat < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(stat);
    while (1) {} 
  }
  Serial.println("IMU Init Complete");
  sensor_stable();  // Waits for the Madgwick filter to stablize
  Serial.println("Madgwick filter stablized");
}

void loop() {
  // put your main code here, to run repeatedly:
  get_yaw();
  Serial.println(yaw);
}

////IMU Stuff.
void get_yaw(){
  IMU.readSensor();
  yaw = fusion.getYaw();
}

void sensor_stable(){
  unsigned long start = millis();
  while (millis() < start + 15000){
    update_heading();
    delay(2);
  }
}
