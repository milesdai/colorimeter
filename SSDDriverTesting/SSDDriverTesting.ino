//Pins for specifying the digit (this can be condensed down to 2 pins with proper aoi logic)
int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9; //PWM Display pin 6
int digit4 = 6; //PWM Display pin 8

//These output to A-D on SSD Driver (74LS47)
int onesbit = 3; //pin 7
int twosbit = 4; //pin 1
int foursbit = 5; //pin 2
int eightsbit = 7; //pin 6

void setup() {

  
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
  pinMode(13, OUTPUT);

  pinMode(onesbit, OUTPUT);
  pinMode(twosbit, OUTPUT);
  pinMode(foursbit, OUTPUT);
  pinMode(eightsbit, OUTPUT);

  pinMode(8, OUTPUT);//control the decimal point

}

void loop() {
  // put your main code here, to run repeatedly:

  for(int i = 0; i < 10000; i++){
    displayNumber(i);
    
  }
}

void displayNumber(int toDisplay) {
#define DISPLAY_BRIGHTNESS  200

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  long beginTime = millis();

  for(int digit = 4 ; digit > 0 ; digit--) {

    lightNumber(toDisplay % 10);
    //Turn on a digit for a short amount of time
    
    if(digit == 1){
      digitalWrite(8,LOW);
    } else {
      digitalWrite(8,HIGH);
    }
    switch(digit) {
      case 1:
        digitalWrite(digit1, DIGIT_ON);
        break;
      case 2:
        digitalWrite(digit2, DIGIT_ON);
        break;
      case 3:
        digitalWrite(digit3, DIGIT_ON);
        break;
      case 4:
        digitalWrite(digit4, DIGIT_ON);
        break;
    }

    //Turn on the right segments for this digit

    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    //Turn off all segments
    //lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
  }

  while((millis() - beginTime) < 15) ; //Wait for 20ms to pass before we paint the display again
}

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {
  #define ONE  LOW
  #define ZERO HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(onesbit, LOW);
    digitalWrite(twosbit, LOW);
    digitalWrite(foursbit, LOW);
    digitalWrite(eightsbit, LOW);
    break;

  case 1:
    digitalWrite(onesbit, HIGH);
    digitalWrite(twosbit, LOW);
    digitalWrite(foursbit, LOW);
    digitalWrite(eightsbit, LOW);
    break;

  case 2:
    digitalWrite(onesbit, LOW);
    digitalWrite(twosbit, HIGH);
    digitalWrite(foursbit, LOW);
    digitalWrite(eightsbit, LOW);
    break;

  case 3:
    digitalWrite(onesbit, HIGH);
    digitalWrite(twosbit, HIGH);
    digitalWrite(foursbit, LOW);
    digitalWrite(eightsbit, LOW);
    break;

  case 4:
    digitalWrite(onesbit, LOW);
    digitalWrite(twosbit, LOW);
    digitalWrite(foursbit, HIGH);
    digitalWrite(eightsbit, LOW);
    break;

  case 5:
    digitalWrite(onesbit, HIGH);
    digitalWrite(twosbit, LOW);
    digitalWrite(foursbit, HIGH);
    digitalWrite(eightsbit, LOW);
    break;

  case 6:
    digitalWrite(onesbit, LOW);
    digitalWrite(twosbit, HIGH);
    digitalWrite(foursbit, HIGH);
    digitalWrite(eightsbit, LOW);
    break;

  case 7:
    digitalWrite(onesbit, HIGH);
    digitalWrite(twosbit, HIGH);
    digitalWrite(foursbit, HIGH);
    digitalWrite(eightsbit, LOW);
    break;

  case 8:
    digitalWrite(onesbit, LOW);
    digitalWrite(twosbit, LOW);
    digitalWrite(foursbit, LOW);
    digitalWrite(eightsbit, HIGH);
    break;

  case 9:
    digitalWrite(onesbit, HIGH);
    digitalWrite(twosbit, LOW);
    digitalWrite(foursbit, LOW);
    digitalWrite(eightsbit, HIGH);
    break;

  case 10: //turns off all segments
    digitalWrite(onesbit, HIGH);
    digitalWrite(twosbit, HIGH);
    digitalWrite(foursbit, HIGH);
    digitalWrite(eightsbit, HIGH);
    break;
  }
}
