#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;

void setup()
{
}

void loop()
{
  motors.setM1Speed(0);
  motors.setM2Speed(100);
}
