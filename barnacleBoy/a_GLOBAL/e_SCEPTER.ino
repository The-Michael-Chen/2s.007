void grabScepter() {
//  topServo.write(topOpen);
//  bottomServo.write(bottomOpen);
//  delay(500);
  if (numLines < totalLines) {
    proportionalController();
  } else {
    // stop driving
    delay(300);
    motors.setSpeeds(0, 0);
    delay(1000);
    if (instr == 1) {
      //turnRight(85.0);
      turn(90.0, 1);
      delay(500);
      instr = instr + 1;
      numLines = 0;
      totalLines = 16;
    } 
    else if (instr == 2) {
      motors.setSpeeds(70, 70);
      delay(500);
      topServo.write(topClosed);
      bottomServo.write(bottomClosed);
      delay(500);
      motors.setSpeeds(0, 0);
      delay(100);
      motors.setSpeeds(-70, -70);
      delay(1000);
      motors.setSpeeds(0, 0);
      turn(170.0, 1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 17;
    }
    else if (instr == 3) {
      turn(90.0, -1);
      delay(500);
      totalLines = 3;
      instr = instr + 1;
      numLines = 0;
    }
    else if (instr == 4) {
      topServo.write(topOpen);
      bottomServo.write(bottomOpen);
      motors.setSpeeds(0, 0);
      beginEvent = false;
      delay(300);
      turn(170, 1);
      eventNumber = eventNumber + 1;
      // Right Scepter
      totalLines = 7;
      numLines = 0;
      instr = 1;
      beginEvent = true;
    }
  }
}

//void grabLeftScepter() {
//  if (numLines < totalLines) {
//    proportionalController();
//  } else {
//    // stop driving
//    motors.setSpeeds(0, 0);
//    delay(1000);
//    if (instr == 1) {
//      turnLeft(90.0);
//      delay(500);
//      instr = instr + 1;
//      numLines = 0;
//      totalLines = 16;
//    } 
//    else if (instr == 2) {
//      motors.setSpeeds(70, 70);
//      delay(500);
//      topServo.write(topClosed);
//      bottomServo.write(bottomClosed);
//      delay(500);
//      motors.setSpeeds(0, 0);
//      delay(100);
//      motors.setSpeeds(-70, -70);
//      delay(800);
//      motors.setSpeeds(0, 0);
//      turnRight(160.0);
//      instr = instr + 1;
//      numLines = 0;
//    }
//    else if (instr == 3) {
//      turnRight(90.0);
//      delay(500);
//      totalLines = 3;
//      instr = instr + 1;
//      numLines = 0;
//    }
//    else if (instr == 4) {
//      topServo.write(topOpen);
//      bottomServo.write(bottomOpen);
//      motors.setSpeeds(0, 0);
//      beginEvent = false;
//    }
//  }
//}
