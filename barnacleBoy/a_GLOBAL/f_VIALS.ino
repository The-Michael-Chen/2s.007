
void grabVials() {
    // right side
//  topServo.write(topOpen);
//  bottomServo.write(bottomOpen);
//  delay(500);
  if (numLines < totalLines) {
    proportionalController();
  } else {
    // stop driving
    motors.setSpeeds(0, 0);
    delay(1000);
    if (instr == 1) {
      motors.setSpeeds(70, 70);
      delay(700);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      delay(200);
      turn(175, -1);
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
      delay(200);
      turn(170, 1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
    else if (instr == 3) {
      turn(30, -1);
      delay(500);
      motors.setSpeeds(70, 70);
      delay(500);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(-70, -70);
      delay(500);
      turn(30, 1);
      delay(200);
      motors.setSpeeds(-70, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      delay(200);
      turn(170, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
     else if (instr == 4) {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      delay(200);
      motors.setSpeeds(-80, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      delay(200);
      turn(170, 1);
      instr = instr + 1;
      numLines = 7;
      numLines = 0;
    }
    else if (instr == 5) {
      turn(30, 1);
      delay(500);
      motors.setSpeeds(70, 70);
      delay(500);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(-70, -70);
      delay(500);
      turn(30, -1);
      delay(200);
      motors.setSpeeds(-70, -70);
      delay(1000);
      motors.setSpeeds(0, 0);
      delay(200);
      turn(170, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
     else if (instr == 6) {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      delay(200);
      motors.setSpeeds(-80, -70);
      delay(1300);
      motors.setSpeeds(0, 0);
      delay(200);
      turn(170, 1);
      instr = instr + 1;
      numLines = 7;
      numLines = 0;
    }
  }
  
}
