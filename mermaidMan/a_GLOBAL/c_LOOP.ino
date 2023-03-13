void loop() {
  // put your main code here, to run repeatedly:
    checkStop();
    if (activeRobot) {
      if (eventNumber == 1 && beginEvent == true && onLeftSide == true) {
        liftThor();
      } else if (eventNumber == 1 && beginEvent == true && onLeftSide == false) {
        liftThor();
      } else if (eventNumber == 2 && beginEvent == true && onLeftSide == true) {
        pushCars();
      } else if (eventNumber == 2 && beginEvent == true && onLeftSide == false) {
        pushCars();
      } else { // RC CONTROL 
        rcControl();
      }
    } else {
      rcControl();
    }
    
    //Serial.println(eventNumber);
}
