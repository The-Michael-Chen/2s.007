
void rcControl() {
  if (radio.rxUpdate()){
    input = radio.rxGetInput();
    int left = 0;
    int right = 0;
    // Don't process unless the joysticks are more than 5 away from center
    //Serial.println(input.pot);
    if (input.button1 == true && !pressedRecently) {
      
      onLeftSide = true;
      eventNumber = eventNumber + 1;
      totalLines = 4;
      instr = 1;
      beginEvent = true;
    }
    if (input.button1 == false) {
      pressedRecently = false;
    } else {
      pressedRecently = true;
    }
    if (input.button2 == true && !pressedRecently) {
      onLeftSide = true;
      eventNumber = eventNumber + 2;
      // Right Scepter
      totalLines = 1;
      instr = 1;
      beginEvent = true;
    }
    if (input.button2 == false) {
      pressedRecently = false;
    } else {
      pressedRecently = true;
    }
    if (input.button3 == true && !pressedRecently) {
        onLeftSide = false;
        eventNumber = eventNumber + 1;
        // Right Scepter
        totalLines = 4;
        instr = 1;
        beginEvent = true;
      }
      if (input.button3 == false) {
        pressedRecently = false;
      } else {
        pressedRecently = true;
      }
    if (input.button4 == true && !pressedRecently) {
      onLeftSide = false;
      eventNumber = eventNumber + 2;
      totalLines = 1;
      instr = 1;
      beginEvent = true;
    }
    if (input.button4 == false) {
      pressedRecently = false;
    } else {
      pressedRecently = true;
    }
    if (input.pot > 40) {
      motorLift->run(FORWARD);
      int liftSpeed = map(input.pot, 0, 127, 0, 250);
      motorLift->setSpeed(liftSpeed);
    } else if (input.pot < -40){
      motorLift->run(BACKWARD);
      int liftSpeed = map(abs(input.pot), 0, 127, 0, 250);
      motorLift->setSpeed(liftSpeed);
    } else {
      motorLift->run(RELEASE);
    }
//    if (input.tSwitch == true) {
//      // DOWN MERMAID MAN
//      activeRobot = true;
//      
//    } else {
//      activeRobot = false;
//    }
    if (activeRobot) {
      if (abs(input.j1PotY) > JOYSTICK_DEADZONE){
        left = input.j1PotY;
       }
       if (abs(input.j1PotX) > JOYSTICK_DEADZONE){
         right = input.j2PotX;
       }
      // Arcade drive
      drive(left + right * 0.4, left - right * 0.4);
    }
  } else {
    drive(0, 0);
  }
}

void checkStop() {
  if (radio.rxUpdate()){
    Serial.println("update");
    input = radio.rxGetInput();
    delay(10);
    if (input.tSwitch == false) {
      Serial.println("flipped switch");
      eventNumber = 0;
      activeRobot = false;
      rcControl();
    } else {
      activeRobot = true;
    }
  }
}

void drive(int left, int right){
  right = -right;
  motorLeft->setSpeed(abs(left));
  motorRight->setSpeed(abs(right));

  if (left > 0){
    motorLeft->run(FORWARD);
  } else if (left < 0){
    motorLeft->run(BACKWARD);
  } else {
    motorLeft->run(RELEASE);
  }

  if (right > 0){
    motorRight->run(BACKWARD);
  } else if (right < 0){
    motorRight->run(FORWARD);
  } else {
    motorRight->run(RELEASE);
  }
}


float getYaw() {
  sensors_event_t event;
  bno.getEvent(&event);
  double newAngle = event.orientation.x;
  return newAngle;
}
