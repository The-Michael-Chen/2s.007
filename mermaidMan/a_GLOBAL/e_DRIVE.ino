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
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);
  motorLeft->setSpeed(leftSpeed);
  motorRight->setSpeed(rightSpeed);
  delay(0.01);
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
  //(IR1Val < 0.5) && (IR2Val < 0.5) && (IR3Val < 0.5) && (IR4Val < 0.5) && (IR5Val < 0.5)
  if ((IR2Val < 0.5) && (IR4Val < 0.5)) {
    if (onLine == false) {
      onLine = true;
      lastLineCrossTime = millis();
      numLines = numLines + 1; 
      //Serial.println("added Line detection");
      digitalWrite(22, HIGH);
      delay(50);
      digitalWrite(22, LOW);
    }
  }else {
    onLine = false;
  }
  
  return xIR;
}

void turn(double deg, int dir) {
  // -1 for left
  // 1 for right
  if (onLeftSide) {
    dir = -1 * dir;
  } 
  if (dir == -1) {
    turnLeft(deg);
  } else if (dir == 1) {
    turnRight(deg);
  }
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
  } while (checkAngleLeft(oldAngle, newAngle, deg) && activeRobot);
  motorLeft->run(RELEASE);
  motorRight->run(RELEASE);
  return;
}

void turnRight(double deg) {
  sensors_event_t event;
  bno.getEvent(&event);
  double oldAngle = event.orientation.x;
  double newAngle = event.orientation.x;
  //Serial.print("old Angle");
  //Serial.println(oldAngle);
  
  do {
    motorLeft->run(FORWARD);
    motorRight->run(BACKWARD);
    motorLeft->setSpeed(70);
    motorRight->setSpeed(70);
    sensors_event_t event;
    bno.getEvent(&event);
    newAngle = event.orientation.x;
  } while (checkAngle(oldAngle, newAngle, deg) && activeRobot);
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
      //Serial.println("weird");
      return false; 
    } else {
      return true;
    }
  } else {
    if ((newAngle - oldAngle) >= (turnAngle)) {
      //Serial.println("regular");
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
      //Serial.println("weird");
      return false; 
    } else {
      return true;
    }
  } else {
    if ((oldAngle - newAngle ) >= (turnAngle)) {
      //Serial.println("regular");
      return false;
    } else {
      return true;
    }
  }
}
