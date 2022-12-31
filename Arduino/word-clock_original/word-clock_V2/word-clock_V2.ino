#include <TimeLib.h>

/* 

  Arduino + Neopixel Word Clock Code
  by: Alex - Super Make Something
  date: August 16, 2015
  license: Public domain.  Please use, reuse, and modify this sketch!
  additional: modified from "simple.ino" NeoPixel example sketch by Shae Erisson of Adafruit Industries.  
  
  NOTE: REQUIRES NEOPIXEL & TIME LIBRARIES TO BE INSTALLED UNDER ...\Arduino\libraries
  NEOPIXEL LIBRARY AVAILABLE AT: https://github.com/adafruit/Adafruit_NeoPixel
  TIME LIBRARY AVAILABLE AT: https://github.com/PaulStoffregen/Time
  
  Explanation: This code lights up Neopixels corresponding to the current time.
  Time is kept using the time library.
  Neopixels are lit using the Adafruit Neopixel library.
  
  Depending on the current time, flags to light corresponding Neopixels are saved in an array
  After parsing the time, Neopixels are turned on/off according to the flags using a for loop

*/

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "Time.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN           3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     98

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Declare integer array with size corresponding to number of Neopixels in chain
// int individualPixels[NUMPIXELS];

//Declare pins for decrementing/incrementing current time by 5 minutes
#define MINUSFIVEMINS 4
#define PLUSFIVEMINS  5

//Declare pins for addition button 5V/GND sources
#define BUTTONLEDMINUS_GND1 8
#define BUTTONLEDMINUS_GND2 9
#define BUTTONLEDPLUS_GND1 10
#define BUTTONLEDPLUS_GND2 11

#define BUTTONLEDMINUS_5V1 A0
#define BUTTONLEDMINUS_5V2 A1
#define BUTTONLEDPLUS_5V1 A2
#define BUTTONLEDPLUS_5V2 A3

// Current and previous states for button pins -- in setup initialize all to HIGH
int minusPrevState=HIGH;
int minusCurrState=HIGH;
int plusPrevState=HIGH;
int plusCurrState=HIGH;

// Time variables
int h;
int m;
int s;

// RGB color variables
int red=55;
int green=55;
int blue=55;

void setup()
{

  pinMode(MINUSFIVEMINS, INPUT_PULLUP); //Define pin as input, enable pull-up resistor
  pinMode(PLUSFIVEMINS, INPUT_PULLUP); //Define pin as input, enable pull-up resistor
  pinMode(BUTTONLEDMINUS_GND1, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDMINUS_GND2, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDPLUS_GND1, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDPLUS_GND2, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDMINUS_5V1, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDMINUS_5V2, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDPLUS_5V1, OUTPUT); //Define pin as output
  pinMode(BUTTONLEDPLUS_5V2, OUTPUT); //Define pin as output
  
  digitalWrite(BUTTONLEDMINUS_GND1, LOW); //Set pin value to LOW
  digitalWrite(BUTTONLEDMINUS_GND2, LOW); //Set pin value to LOW
  digitalWrite(BUTTONLEDPLUS_GND1, LOW); //Set pin value to LOW
  digitalWrite(BUTTONLEDPLUS_GND2, LOW); //Set pin value to LOW
  digitalWrite(BUTTONLEDMINUS_5V1, HIGH); //Set pin value to HIGH
  digitalWrite(BUTTONLEDMINUS_5V2, HIGH); //Set pin value to HIGH
  digitalWrite(BUTTONLEDPLUS_5V1, HIGH); //Set pin value to HIGH
  digitalWrite(BUTTONLEDPLUS_5V2, HIGH); //Set pin value to HIGH
  
  setTime(12,0,0,31,8,2015); //Initialize current time as Midnight/noon 08/31/2015
  pixels.begin(); //Begin Neopixel string
  Serial.begin(9600); //Begin Serial for debugging purposes
  
}

void loop()
{
  
  //Declare integer array with size corresponding to number of Neopixels in chain
  int individualPixels[NUMPIXELS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  /* Check for button presses & reset time if necessary */
  minusCurrState=digitalRead(MINUSFIVEMINS); //Get current state of MINUSFIVEMINS button
  /* If current state is different from previous state and value is now LOW, subtract five minutes from current time */
  if ((minusCurrState!=minusPrevState) && (minusCurrState==LOW)){
    adjustTime(-5*60); //Shift time five minutes backwards
    minusPrevState=minusCurrState;
  }
  else{
   minusPrevState=minusCurrState; 
  }
  
  plusCurrState=digitalRead(PLUSFIVEMINS); //Get current state of PLUSFIVEMINS button
  /* If current state is different from previous state and value is now LOW, add five minutes from current time */
  if ((plusCurrState!=plusPrevState) && (plusCurrState==LOW)){
    adjustTime(5*60); //Shift time five minutes forwards
    plusPrevState=plusCurrState;
  }
    else{
   plusPrevState=plusCurrState; 
  }
    
  /* Get current time */
  h=hourFormat12();    // Returns the hour of current time between 1-12
  m=minute();        // Returns the minute of current time
  s=second();        // Returns the second of current time (not used, included for completeness)
  
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.println(s);

  /* Parse time values to light corresponding pixels */
  individualPixels[0]=1; //Light "IT"
  individualPixels[1]=1; 
   
  individualPixels[2]=1; //Light "IS" 
  individualPixels[3]=1;
  
  /* Minutes between 0-5 - Light "O CLOCK" */
  if ((m>=0 && m<5)){
    individualPixels[86]=1;
    individualPixels[87]=1;
    individualPixels[88]=1;
    individualPixels[89]=1;
    individualPixels[90]=1;
    individualPixels[91]=1;
    
  }
  
  /* Minutes between 5-10 or 55-60 - Light "FIVE," "MINUTES" */
  if ((m>=5 && m<10) || (m>=55 && m<60)){
    individualPixels[24]=1; //FIVE
    individualPixels[25]=1;
    individualPixels[26]=1;
    individualPixels[27]=1;

    individualPixels[28]=1; //MINUTES
    individualPixels[29]=1;
    individualPixels[30]=1;
    individualPixels[31]=1;
    individualPixels[32]=1;
    individualPixels[33]=1;
    individualPixels[34]=1;
  }
  
  /* Minutes between 10-15 or 50-55 - Light "TEN," "MINUTES" */
  if ((m>=10 && m<15) || (m>=50 && m<55)){
    individualPixels[4]=1;//TEN
    individualPixels[5]=1;
    individualPixels[6]=1;

    individualPixels[28]=1; //MINUTES
    individualPixels[29]=1;
    individualPixels[30]=1;
    individualPixels[31]=1;
    individualPixels[32]=1;
    individualPixels[33]=1;
    individualPixels[34]=1;
  }
  
  /* Minutes between 15-20 or 45-50 - Light "QUARTER" */
  if ((m>=15 && m<20) || (m>=45 && m<50)){
    individualPixels[17]=1;
    individualPixels[18]=1;
    individualPixels[19]=1;
    individualPixels[20]=1;
    individualPixels[21]=1;
    individualPixels[22]=1;
    individualPixels[23]=1;
  }
  
  /* Minutes between 20-25 or 40-45 - Light "TWENTY," "MINUTES" */
  if ((m>=20 && m<25) || (m>=40 && m<45)){
    individualPixels[11]=1;//TWENTY
    individualPixels[12]=1;
    individualPixels[13]=1;
    individualPixels[14]=1;
    individualPixels[15]=1;
    individualPixels[16]=1;
    
    individualPixels[28]=1; //MINUTES
    individualPixels[29]=1;
    individualPixels[30]=1;
    individualPixels[31]=1;
    individualPixels[32]=1;
    individualPixels[33]=1;
    individualPixels[34]=1;
  }  

  /* Minutes between 25-30 or 35-40 - Light "TWENTY," "FIVE," "MINUTES" */
  if ((m>=25 && m<30) || (m>=35 && m<40)){
    individualPixels[11]=1;//TWENTY
    individualPixels[12]=1;
    individualPixels[13]=1;
    individualPixels[14]=1;
    individualPixels[15]=1;
    individualPixels[16]=1;

    individualPixels[24]=1;//FIVE
    individualPixels[25]=1;
    individualPixels[26]=1;
    individualPixels[27]=1;

    individualPixels[28]=1;//MINUTES
    individualPixels[29]=1;
    individualPixels[30]=1;
    individualPixels[31]=1;
    individualPixels[32]=1;
    individualPixels[33]=1;
    individualPixels[34]=1;


  }

  /* Minutes between 30-35 - Light "HALF" */
  if ((m>=30 && m<35)){
    individualPixels[7]=1;
    individualPixels[8]=1;
    individualPixels[9]=1;
    individualPixels[10]=1;
  }
  
  /* Minutes between 5-35 - Light "PAST" */
  if ((m>=5) && (m<35)){
    individualPixels[43]=1;
    individualPixels[44]=1;
    individualPixels[45]=1;
    individualPixels[46]=1;
  }
  
  /* Minutes between 35-60 - Light "TO" & MODIFY CURRENT HOUR VALUE */
  if (m>=35){
    individualPixels[41]=1;
    individualPixels[42]=1;
    h++; //Add 1 from current hour
    /*Set time to twelve for hour around midnight, noon */
    if (h==0){
      h=12; 
    }
    /*Corner case for 12:35-12:59 */
    if (h==13){
      h=1;
    }
  }

  /* Hour=1 - Light "ONE" */
  if (h==1){
    individualPixels[38]=1;
    individualPixels[39]=1;
    individualPixels[40]=1;
  }
  
  /* Hour=2 - Light "TWO" */
  if (h==2){
    individualPixels[35]=1;
    individualPixels[36]=1;
    individualPixels[37]=1;
  }
  
  /* Hour=3 - Light "THREE" */
  if (h==3){
    individualPixels[47]=1;
    individualPixels[48]=1;
    individualPixels[49]=1; 
    individualPixels[50]=1; 
    individualPixels[51]=1;     
  }
  
  /* Hour=4 - Light "FOUR" */
  if (h==4){
    individualPixels[52]=1;
    individualPixels[53]=1;
    individualPixels[54]=1;
    individualPixels[55]=1;
  }
  
  /* Hour=5 - Light "FIVE" */
  if (h==5){
    individualPixels[56]=1;
    individualPixels[57]=1;
    individualPixels[58]=1;
    individualPixels[59]=1;
  }
  
  /* Hour=6 - Light "SIX" */
  if (h==6){
    individualPixels[70]=1;
    individualPixels[71]=1;
    individualPixels[72]=1;
  }
  
  /* Hour=7 - Light "SEVEN" */
  if (h==7){
    individualPixels[65]=1;
    individualPixels[66]=1;
    individualPixels[67]=1;
    individualPixels[68]=1;
    individualPixels[69]=1;
  }
  
  /* Hour=8 - Light "EIGHT" */
  if (h==8){
    individualPixels[60]=1;
    individualPixels[61]=1;
    individualPixels[62]=1;
    individualPixels[63]=1;
    individualPixels[64]=1;
  }
  
  /* Hour=9 - Light "NINE" */
  if (h==9){
    individualPixels[73]=1;
    individualPixels[74]=1;
    individualPixels[75]=1;
    individualPixels[76]=1;
  }
  
  /* Hour=10 - Light "TEN" */
  if (h==10){
    individualPixels[77]=1;
    individualPixels[78]=1;
    individualPixels[79]=1;
  }
  
  /* Hour=11 - Light "ELEVEN" */
  if (h==11){
    individualPixels[80]=1;
    individualPixels[81]=1;
    individualPixels[82]=1;
    individualPixels[83]=1;
    individualPixels[84]=1;
    individualPixels[85]=1;
  }
  
  /* Hour=12 - Light "TWELVE" */
  if (h==12){
    individualPixels[92]=1;
    individualPixels[93]=1;
    individualPixels[94]=1;
    individualPixels[95]=1;
    individualPixels[96]=1;
    individualPixels[97]=1;
  }
  
  /* Light pixels corresponding to current time */
  for (int i=0; i<sizeof(individualPixels); i++){
    if (individualPixels[i]==1){
      pixels.setPixelColor(i, pixels.Color(red,green,blue)); //Set Neopixel color
    }
    else{
      pixels.setPixelColor(i,pixels.Color(0,0,0));
    }
  }
  
  pixels.show(); //Display Neopixel color
  
//  /* Clear pixel values for re-assignment during next iteration */
//  for (int j=0; j<sizeof(individualPixels); j++){
//      individualPixels[j]=0; //Set array values to 0
//      pixels.setPixelColor(j, pixels.Color(0,0,0)); //Set Neopixel color to 0 brightness, i.e. off
//  }
  
}