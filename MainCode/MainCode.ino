#include <Wire.h>
#include "SFE_ISL29125.h"
#include <math.h>

// Declare sensor object
SFE_ISL29125 RGB_sensor;

//Pins for specifying the digit (this can be condensed down to 2 pins with proper aoi logic)
int digit1 = 11; //PWM Display pin 1
int digit2 = 10; //PWM Display pin 2
int digit3 = 9; //PWM Display pin 6
int digit4 = 6; //PWM Display pin 8

//These output to A-D on SSD Driver (74LS47)
int onesbit = 3;
int twosbit = 4;
int foursbit = 5;
int eightsbit = 7;

unsigned int currentReading;
volatile unsigned int baseline;
int absorbance;
int colorSetting; //0=red, 1=green, 2=blue

boolean calibrate = false;

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

  pinMode(8,OUTPUT);//use pin 8 to control the decimal point
  
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0,pin_ISR,FALLING);

  pinMode(12,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);

  
  RGB_sensor.init();
  RGB_sensor.config(CFG1_MODE_RGB | CFG1_10KLUX, CFG2_IR_ADJUST_HIGH, CFG_DEFAULT);
  delay(2000);
  baseline = RGB_sensor.readRed();
  delay(2000);

  Serial.begin(115200);
}

void loop() {

  //Determine color setting (position of the sp3t switch)
  if(digitalRead(12) == LOW){
    colorSetting = 2; //blue
  } else if(digitalRead(13) == LOW){
    colorSetting = 1; //green
  } else {
    colorSetting = 0; //red
  }

  //Calibrate button detection
  if(calibrate){
    switch(colorSetting){
      case 0:
        baseline = RGB_sensor.readRed();
        break;
      case 1:
        baseline = RGB_sensor.readGreen();
        break;
      case 2:
        baseline = RGB_sensor.readBlue();
        break;
    }
    calibrate = false;
  }

  //take a current reading
  switch(colorSetting){
    case 0:
      currentReading = RGB_sensor.readRed();
      break;
    case 1:
      currentReading = RGB_sensor.readGreen();
      break;
    case 2:
     currentReading = RGB_sensor.readBlue();
     break;
  }

  absorbance = (int)(log10(((float)baseline) / currentReading)*1000);
  displayNumber(absorbance);
}

void displayNumber(int toDisplay) {
#define DISPLAY_BRIGHTNESS  700

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  long beginTime = millis();

  for(int digit = 4 ; digit > 0 ; digit--) {
    //Turn on the right segments for this digit
    lightNumber(toDisplay % 10);
    if(digit == 1){
      digitalWrite(8,LOW);
    } else {
      digitalWrite(8,HIGH);
    }
    toDisplay /= 10;

    //Turn on a digit for a short amount of time
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
    
    delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second (between 1us and 5000us, 500 is pretty good)

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);

  }

  while( (millis() - beginTime) < 15) ; //Wait for 20ms to pass before we paint the display again
  
}

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

#define ONE  LOW
#define ZERO HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(onesbit, ONE);
    digitalWrite(twosbit, ONE);
    digitalWrite(foursbit, ONE);
    digitalWrite(eightsbit, ONE);
    break;

  case 1:
    digitalWrite(onesbit, ZERO);
    digitalWrite(twosbit, ONE);
    digitalWrite(foursbit, ONE);
    digitalWrite(eightsbit, ONE);
    break;

  case 2:
    digitalWrite(onesbit, ONE);
    digitalWrite(twosbit, ZERO);
    digitalWrite(foursbit, ONE);
    digitalWrite(eightsbit, ONE);
    break;

  case 3:
    digitalWrite(onesbit, ZERO);
    digitalWrite(twosbit, ZERO);
    digitalWrite(foursbit, ONE);
    digitalWrite(eightsbit, ONE);
    break;

  case 4:
    digitalWrite(onesbit, ONE);
    digitalWrite(twosbit, ONE);
    digitalWrite(foursbit, ZERO);
    digitalWrite(eightsbit, ONE);
    break;

  case 5:
    digitalWrite(onesbit, ZERO);
    digitalWrite(twosbit, ONE);
    digitalWrite(foursbit, ZERO);
    digitalWrite(eightsbit, ONE);
    break;

  case 6:
    digitalWrite(onesbit, ONE);
    digitalWrite(twosbit, ZERO);
    digitalWrite(foursbit, ZERO);
    digitalWrite(eightsbit, ONE);
    break;

  case 7:
    digitalWrite(onesbit, ZERO);
    digitalWrite(twosbit, ZERO);
    digitalWrite(foursbit, ZERO);
    digitalWrite(eightsbit, ONE);
    break;

  case 8:
    digitalWrite(onesbit, ONE);
    digitalWrite(twosbit, ONE);
    digitalWrite(foursbit, ONE);
    digitalWrite(eightsbit, ZERO);
    break;

  case 9:
    digitalWrite(onesbit, ZERO);
    digitalWrite(twosbit, ONE);
    digitalWrite(foursbit, ONE);
    digitalWrite(eightsbit, ZERO);
    break;

  case 10: //turns off all segments
    digitalWrite(onesbit, ZERO);
    digitalWrite(twosbit, ZERO);
    digitalWrite(foursbit, ZERO);
    digitalWrite(eightsbit, ZERO);
    break;
  }
}

void pin_ISR(){
  //This sets the boolean for calibrate to true which indicates on the next interation of loop() that a calibration needs to occur
  calibrate = true;
}
