void loop() {
  // put your main code here, to run repeatedly:
  // If radio input is valid
    if (eventNumber == 1 && beginEvent == true && onLeftSide == true) {
      grabScepter();
    } else if (eventNumber == 1 && beginEvent == true && onLeftSide == false) {
      grabScepter();
    } else if (eventNumber == 2 && beginEvent == true && onLeftSide == true) {
      grabVials();
    } else if (eventNumber == 2 && beginEvent == true && onLeftSide == false) {
      grabVials();
    } else { // RC CONTROL 
      if (radio.rxUpdate()){
    //getDistance();
    // Gets value from radio
        input = radio.rxGetInput();
        rcControl();
      } else {
        drive(0, 0);
      }
    }
    
}
