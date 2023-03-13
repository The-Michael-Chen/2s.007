void setup() {
  // DRIVE TRAIN SETUP
  motors.flipM2(true);
  motors.flipM1(false);
  // SERVO ARM SETUP 
  topServo.write(topOpen);
  bottomServo.write(bottomOpen);
  topServo.attach(topServoPin);
  bottomServo.attach(bottomServoPin);


  // ULTRASONIC PIN SETUP
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  // RC CONTROL SETUP
  Serial.begin(115200);
  if (!radio.begin()){
    Serial.println("Radio not started");
  } else {
    Serial.println("Radio started");
  }
  radio.rxSendTelemetry("Hello world! " + String(analogRead(A0)));
   if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  // LINE SENSOR 
  pinMode(A8, INPUT); 
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);

  //RIGHT SIDE
  onLeftSide = false;
  eventNumber = eventNumber + 1;
  // Right Scepter
  totalLines = 3;
  instr = 1;
  beginEvent = true;
  
  //LEFT SIDE
//  onLeftSide = true;
//  eventNumber = eventNumber + 1;
//  totalLines = 3;
//  instr = 1;
//  beginEvent = true;
//  
}
