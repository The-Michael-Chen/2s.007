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



void turn(double turnDeg, int dir) {
  if (turnDeg > 90.0) {
    turnFinal(90, dir);
    delay(100);
    turnFinal((turnDeg-90), dir);
  } else {
    turnFinal(turnDeg, dir);
  }
}

void turnFinal (double turnDeg, int dir) {
  // -1 left 1 right
  // left is 80*-1, 80*1
  if (onLeftSide) {
    dir = -1*dir; //reverse for the left side
  }
  sensors_event_t event;
  bno.getEvent(&event);
  double startAngle = event.orientation.x;
  double currentAngle = event.orientation.x;
  motors.setSpeeds(80*dir, 80*-dir);
  unsigned long StartTime = millis();
  while ((getDiff(currentAngle, startAngle) <= (turnDeg - overshoot))) {
    bno.getEvent(&event);
    currentAngle = event.orientation.x;
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    if (ElapsedTime > 1150) {
      break;
    }
  }
  motors.setSpeeds(0, 0);
}

double getDiff(double deg1, double deg2) {
  double diff = abs(deg2 - deg1);
  return min(diff, 360-diff);
}

//void turnLeft(double deg) {
//  /* turn for 100
//     check the yaw
//     if the absolute difference between the current n initial yaw is >= 180 stop
//  */
//  // old 0 -> 360 new 0-> 360
//  sensors_event_t event;
//  bno.getEvent(&event);
//  double oldAngle = event.orientation.x;
//  double newAngle = event.orientation.x;
//  do {
//    motors.setSpeeds(-80, 80);
//    //sensors_event_t event;
//    bno.getEvent(&event);
//    newAngle = event.orientation.x;
//    //radio.rxSendTelemetry("Angle Value: " + String(newAngle));
//  } while (checkAngleLeft(oldAngle, newAngle, deg));
//    motors.setSpeeds(0, 0);
//    delay(1000);
//    return;
//}
//
//void turnRight(double deg) {
//  sensors_event_t event;
//  bno.getEvent(&event);
//  double oldAngle = event.orientation.x;
//  double newAngle = event.orientation.x;
//  
//  do {
//    motors.setSpeeds(80, -80);
//    //sensors_event_t event;
//    bno.getEvent(&event);
//    newAngle = event.orientation.x;
//  } while (checkAngle(oldAngle, newAngle, deg));
//    motors.setSpeeds(0, 0);
//    delay(1000);
//    return;
//}

//bool checkAngle(double oldAngle, double newAngle, double turnAngle) {
//  // checks if I've turned all the way right
//  Serial.println("current Angle     " + String(newAngle) + "     start angle:       " + String(oldAngle));
// 
//  double targetAngle = fmod(((360 + oldAngle) + turnAngle), 360.0);
//  radio.rxSendTelemetry("Target Angle: " + String(targetAngle));
//  if ((newAngle <= (targetAngle + 4.0)) && (newAngle >= (targetAngle - 4.0)) ) {
//    return false;
//  } else {
//    return true;
//  }
//}
//
//bool checkAngleLeft(double oldAngle, double newAngle, double turnAngle) {
//  // checks if I've turned all the way left
//  Serial.println("current Angle     " + String(newAngle) + "     start angle:       " + String(oldAngle));
//  double targetAngle = fmod(((360 + oldAngle) - turnAngle), 360.0);
//  //radio.rxSendTelemetry("Angle Value: " + String(newAngle) + "Target Value" + String(targetAngle));
//  radio.rxSendTelemetry("Target Angle: " + String(targetAngle));
//  if ((newAngle <= (targetAngle + 4.0)) && (newAngle >= (targetAngle - 4.0)) ) {
//    return false;
//  } else {
//    return true;
//  }
//}


//
//bool checkAngle(double oldAngle, double newAngle, double turnAngle) {
//  // old angle and new angle, we need to figure out if the diff between old angle and new angle is >= target angle
//  // normally can just do oldAngle - new Angle
//  // but if oldAngle is not big enough we run into issues 
//  //    so we can check to see if it's a small old angle- if so, we can just 
//  if ((oldAngle + turnAngle) > 360) { // old: 200 turn 180 target = 20 old: 10, turn 180 
//    double targetAngle = turnAngle - (360.0 - oldAngle);
//    if ((newAngle <= (targetAngle + 2.0)) && (newAngle >= (targetAngle - 2.0))) {
//      Serial.println("weird");
//      return false; 
//    } else {
//      return true;
//    }
//  } else {
//    if ((newAngle - oldAngle) >= (turnAngle)) {
//      Serial.println("regular");
//      return false;
//    } else {
//      return true;
//    }
//  }
//}
//
//bool checkAngleLeft(double oldAngle, double newAngle, double turnAngle) {
//  // old angle and new angle, we need to figure out if the diff between old angle and new angle is >= target angle
//  // normally can just do oldAngle - new Angle
//  // but if oldAngle is not big enough we run into issues 
//  //    so we can check to see if it's a small old angle- if so, we can just 
//  if ((oldAngle - turnAngle) < 0) {
//    double targetAngle = 360.0 - (turnAngle - oldAngle); // target = 200
//    if ((newAngle <= (targetAngle + 2.0)) && (newAngle >= (targetAngle - 2.0))) {
//      Serial.println("weird");
//      return false; 
//    } else {
//      return true;
//    }
//  } else {
//    if ((oldAngle - newAngle ) >= (turnAngle)) { // old: 10 new: 350 turn 170 
//      Serial.println("regular");
//      return false;
//    } else {
//      return true;
//    }
//  }
//}
