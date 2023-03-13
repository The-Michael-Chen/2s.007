void pushCars() {
  // start with 1 
  // based on left side
  if (numLines < totalLines) {
    proportionalController();
  } else {
    // stop driving
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
    delay(1000);
    if (instr == 1) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 8;
    } 
    else if (instr == 2) {
      turn(90, 1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 3;
    }
    else if (instr == 3) {
      turn(170, -1);
      delay(100);
      instr = instr + 1;
      numLines = 0;
      totalLines = 1;
    }
    else if (instr == 4) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 4;
    }
    else if (instr == 5) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 1;
    }
    else if (instr == 6) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 4;
    }
    // THIRD CAR SEQUENCE
    else if (instr == 7) {
      motorLeft->run(FORWARD);
      motorRight->run(FORWARD);
      delay(100);
      motorLeft->run(BACKWARD);
      motorRight->run(BACKWARD);
      delay(100);
      motorLeft->run(RELEASE);
      motorRight->run(RELEASE);
      turn(175, 1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 2;
    }
    else if (instr == 8) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 7;
    }
    else if (instr == 9) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 1;
    }
    else if (instr == 10) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 3;
    } 
    else if (instr == 11) {
      turn(90, 1);
      delay(500);
      turn(90, -1);
      delay(500);
      instr = instr + 1;
      numLines = 0;
      totalLines = 6;
    }
    else if (instr == 12) {
      turn(90, -1);
      instr = instr + 1;
      numLines = 0;
      totalLines = 6;
    } 
    else if (instr == 13) {
      eventNumber = 0;
      beginEvent = false;
    }
    
  }

}

// button scheme -> override, is left, control autonomous 1, control autonomous 2
