void liftThor() {
   if (numLines < totalLines) {
    proportionalController();
   } else {
    // stop driving
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
    delay(1000);
    if (instr == 1) {
      turn(85, 1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 10;
    } 
    else if (instr == 2) {
      motorLeft->setSpeed(70);
      motorRight->setSpeed(70);
      motorLeft->run(FORWARD);
      motorRight->run(FORWARD);
      delay(1000);
      motorLeft->run(RELEASE);
      motorRight->run(RELEASE);
      delay(100);
      motorLift->run(FORWARD);
      motorLift->setSpeed(250);
      // lifting speed
      delay(6900);
      motorLift->run(BACKWARD);
      motorLift->setSpeed(250);
      // lifting speed
      delay(6200);
      motorLift->run(RELEASE);
      motorLeft->run(BACKWARD);
      motorRight->run(BACKWARD);
      delay(1000);
      motorLeft->run(RELEASE);
      motorRight->run(RELEASE);
      turn(175, 1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 10;
    }
    else if (instr == 3) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 5;
    } 
    else if (instr == 4) {
      motorLeft->run(RELEASE);
      motorRight->run(RELEASE);
      eventNumber = 0;
      beginEvent = false;
    } 
    
  }
}
