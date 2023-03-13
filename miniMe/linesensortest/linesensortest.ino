
float IR1Val, IR2Val, IR3Val, IR4Val, IR5Val;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A8, INPUT); 
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readValue();
}

float readValue() {
  Serial.print("Sensor 1: "); // b: 296, w: 979
  Serial.print(analogRead(A8));
  Serial.print("     Sensor 2: "); // b:302, w:980
  Serial.print(analogRead(A9));
  Serial.print("     Sensor 3: "); // b:490, w:981
  Serial.print(analogRead(A10));
  Serial.print("     Sensor 4: "); // b:380, w: 980
  Serial.print(analogRead(A11));
  Serial.print("     Sensor 5: "); // b:382, w: 980
  Serial.println(analogRead(A12));
  IR1Val = constrain((analogRead(A8) - 296.0)/(979.0-296.0), 0, 1); // minimum 235 - 982
  IR2Val = constrain((analogRead(A9) - 302.0)/(980.0-302.0), 0, 1); // min 235 - 983
  IR3Val = constrain((analogRead(A10) - 490.0)/(981.0-490.0), 0, 1); // 231 - 983
  IR4Val = constrain((analogRead(A11) - 380.0)/(980.0-380.0), 0, 1); // 253 - 983
  IR5Val = constrain((analogRead(A12) - 382.0)/(980.0-382.0), 0, 1); // 300 - 983

  float xIR = ((1-IR1Val) + (1-IR2Val)*2 + (1-IR3Val)*3 + (1-IR4Val)*4 + (1-IR5Val)*5)/((1-IR1Val) + (1-IR2Val) + (1-IR3Val) + (1-IR4Val) + (1-IR5Val));
  Serial.println(xIR);
  if (isnan(xIR)) {
    Serial.println("yup");
  }
  delay(100);
//  IR1Val = constrain((analogRead(A1) - 235.0)/(982.0-235.0), 0, 1); // minimum 235 - 982
//  IR2Val = constrain((analogRead(A2) - 235.0)/(982.0-235.0), 0, 1); // min 235 - 983
//  IR3Val = constrain((analogRead(A3) - 231.0)/(983.0-231.0), 0, 1); // 231 - 983
//  IR4Val = constrain((analogRead(A4) - 253.0)/(983.0-253.0), 0, 1); // 253 - 983
//  IR5Val = constrain((analogRead(A5) - 300.0)/(983.0-300.0), 0, 1); // 300 - 983
//
//  float xIR = ((1-IR1Val) + (1-IR2Val)*2 + (1-IR3Val)*3 + (1-IR4Val)*4 + (1-IR5Val)*5)/((1-IR1Val) + (1-IR2Val) + (1-IR3Val) + (1-IR4Val) + (1-IR5Val));
//  Serial.println(xIR);
//  return xIR;
}
