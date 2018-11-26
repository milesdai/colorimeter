void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);
  Serial.begin(9600);
  //int switchReading = LOW;
}

void loop() {
  // put your main code here, to run repeatedly:
  //switchReading = digitalRead(12)
  if(digitalRead(12) == LOW){
    Serial.println("Blue");
  } else if(digitalRead(13) == LOW){
    Serial.println("Green");
  } else {
    Serial.println("Red");
  }
}
