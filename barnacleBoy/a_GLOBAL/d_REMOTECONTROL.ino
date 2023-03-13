
void rcControl() {
  int left = 0;
  int right = 0;
  if (input.tSwitch == true) {
    activeRobot = false;
//    Serial.println("Switch on");
//    //digitalWrite(22, HIGH);
//    topServo.write(topClosed);
//    bottomServo.write(bottomClosed);
  } else {
    activeRobot = true;
//    topServo.write(topOpen);
//    bottomServo.write(bottomOpen);
    //digitalWrite(22, LOW);
  }
  if (activeRobot) {
        if (input.button1 == true && !pressedRecently) {
        onLeftSide = true;
        eventNumber = eventNumber + 1;
        // Right Scepter
        totalLines = 3;
        instr = 1;
        beginEvent = true;
      }
      if (input.button1 == false) {
        pressedRecently = false;
      } else {
        pressedRecently = true;
      }
      if (input.button2 == true && !pressedRecently) {
        radio.rxSendTelemetry("pressed button");
        onLeftSide = true;
        eventNumber = eventNumber + 2;
        // Right Scepter
        totalLines = 7;
        numLines = 0;
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
        totalLines = 3;
        instr = 1;
        beginEvent = true;
      }
      if (input.button3 == false) {
        pressedRecently = false;
      } else {
        pressedRecently = true;
      }
      if (input.button4 == true && !pressedRecently) {
        radio.rxSendTelemetry("pressed button");
        onLeftSide = false;
        eventNumber = eventNumber + 2;
        // Right Scepter
        totalLines = 7;
        numLines = 0;
        instr = 1;
        beginEvent = true;
      }
      if (input.button4 == false) {
        pressedRecently = false;
      } else {
        pressedRecently = true;
      }
      pinMode(22, OUTPUT);
      // Don't process unless the joysticks are more than 5 away from center
      if (abs(input.j1PotY) > JOYSTICK_DEADZONE){
        left = input.j1PotY;
      }
      if (abs(input.j2PotX) > JOYSTICK_DEADZONE){
        right = input.j2PotX;
      }
      // Arcade drive
      drive(left + right * 0.4, left - right * 0.4);
  }
  
}

void drive(int left, int right){
  int leftVal = left;
  int rightVal = right;
  left = abs(left);
  right = abs(right);
  
  if (leftVal > 0){
    motors.setM1Speed(left);
  } else if (leftVal < 0){
    motors.setM1Speed(-left);
  } else {
    motors.setM1Speed(0);
  }

  if (rightVal > 0){
   // motor_right->run(BACKWARD);
    motors.setM2Speed(right);
  } else if (rightVal < 0){
    //motor_right->run(FORWARD);
    motors.setM2Speed(-right);
  } else {
    motors.setM2Speed(0);
    //motor_right->run(RELEASE);
  }
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
void getDistance() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

float getYaw() {
  sensors_event_t event;
  bno.getEvent(&event);
  double newAngle = event.orientation.x;
  return newAngle;
}
