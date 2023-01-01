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

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN           3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     98

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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

/*
13 x 8
IT~IS~TENHALF
QUARTERTWENTY
FIVE~MINUTES~
PASTTO~ONETWO
THREEFOURFIVE
SIXSEVENEIGHT
NINETENELEVEN
TWELVE~OCLOCK
*/

// Matrix variables
const short
  gridCols = 13, // How many letters across
  gridRows = 8, // How many letters tall
  millisTransition = 5000, // How long should we do the matrix for? (in milliseconds)
  millisWait = 100, // How long should we wait, before changing to the next frame of the matrix animation? (in milliseconds)
  matrixDensity = 5, // How often should we start a new thread in the matrix? Higher = less frequent, Lower = more frequent
  matrixIterations = millisTransition / millisWait;

// List of colours used in the transition for faded matrix effect
const uint32_t colours[5] = {
  pixels.Color(0, 0, 0),
  pixels.Color(63, 63, 63),
  pixels.Color(127, 127, 127),
  pixels.Color(191, 191, 191),
  pixels.Color(255, 255, 255)
};

// Index grid to keep track of the matrix colour for each pixel
short pixelColours[NUMPIXELS];

// Pixel mapping into grid, so matrix can flow
const short pixelGrid[gridRows][gridCols] = {
  {0,1,-1,2,3,-1,4,5,6,7,8,9,10},
  {23,22,21,20,19,18,17,16,15,14,13,12,11},
  {24,25,26,27,-1,28,29,30,31,32,33,34,34},
  {46,45,44,43,42,41,-1,40,39,38,37,36,35},
  {47,48,49,50,51,52,53,54,55,56,57,58,59},
  {72,71,70,69,68,67,66,65,64,63,62,61,60},
  {73,74,75,76,77,78,79,80,81,82,83,84,85},
  {97,96,95,94,93,92,-1,91,90,89,88,87,86}
};

//Declare boolean array with size corresponding to number of Neopixels in chain
bool pixelOnOff[NUMPIXELS];

/* Light pixels corresponding to current time */
void colourPixels(unsigned long wait) {
  for (short i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixelColours[i]); //Set Neopixel color
  }
  
  pixels.show(); //Display Neopixel color

  delay(wait);
}

void matrixFade(const short& r, const short& c, const short& currIteration) {
    // Transition the current letter's colour
    short currColour = pixelColours[pixelGrid[r][c]];
    short nextColour = currColour > 0 ? currColour - 1 : currColour;

    pixelColours[pixelGrid[r][c]] = nextColour;

    // If we're stopping the animation
    if (currIteration >= matrixIterations) {
        // If there's another letter under the current one AND 
        // we're not overwriting a letter that is supposed to be lit
        if (
            r + 1 < gridRows
            && !(
                pixelOnOff[pixelGrid[r + 1][c]] &&
                pixelColours[pixelGrid[r + 1][c]] == colours[sizeof(colours) - 1]
                )
            ) {
            // Pass the colour along
            pixelColours[pixelGrid[r + 1][c]] = currColour;
        }
    }
    // If there's another letter under the current one
    else if (r + 1 < gridRows) {
        // Pass the coloru along
        pixelColours[pixelGrid[r + 1][c]] = currColour;
    }
}

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
  
  // Clear the clock
  for (int i = 0; i < NUMPIXELS; i++) {
    pixelOnOff[i] = false;
  }
  
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
  pixelOnOff[0]=true; //Light "IT"
  pixelOnOff[1]=true; 
   
  pixelOnOff[2]=true; //Light "IS" 
  pixelOnOff[3]=true;
  
  /* Minutes between 0-5 - Light "O CLOCK" */
  if ((m>=0 && m<5)){
    pixelOnOff[86]=true;
    pixelOnOff[87]=true;
    pixelOnOff[88]=true;
    pixelOnOff[89]=true;
    pixelOnOff[90]=true;
    pixelOnOff[91]=true;
    
  }
  
  /* Minutes between 5-10 or 55-60 - Light "FIVE," "MINUTES" */
  if ((m>=5 && m<10) || (m>=55 && m<60)){
    pixelOnOff[24]=true; //FIVE
    pixelOnOff[25]=true;
    pixelOnOff[26]=true;
    pixelOnOff[27]=true;

    pixelOnOff[28]=true; //MINUTES
    pixelOnOff[29]=true;
    pixelOnOff[30]=true;
    pixelOnOff[31]=true;
    pixelOnOff[32]=true;
    pixelOnOff[33]=true;
    pixelOnOff[34]=true;
  }
  
  /* Minutes between 10-15 or 50-55 - Light "TEN," "MINUTES" */
  if ((m>=10 && m<15) || (m>=50 && m<55)){
    pixelOnOff[4]=true;//TEN
    pixelOnOff[5]=true;
    pixelOnOff[6]=true;

    pixelOnOff[28]=true; //MINUTES
    pixelOnOff[29]=true;
    pixelOnOff[30]=true;
    pixelOnOff[31]=true;
    pixelOnOff[32]=true;
    pixelOnOff[33]=true;
    pixelOnOff[34]=true;
  }
  
  /* Minutes between 15-20 or 45-50 - Light "QUARTER" */
  if ((m>=15 && m<20) || (m>=45 && m<50)){
    pixelOnOff[17]=true;
    pixelOnOff[18]=true;
    pixelOnOff[19]=true;
    pixelOnOff[20]=true;
    pixelOnOff[21]=true;
    pixelOnOff[22]=true;
    pixelOnOff[23]=true;
  }
  
  /* Minutes between 20-25 or 40-45 - Light "TWENTY," "MINUTES" */
  if ((m>=20 && m<25) || (m>=40 && m<45)){
    pixelOnOff[11]=true;//TWENTY
    pixelOnOff[12]=true;
    pixelOnOff[13]=true;
    pixelOnOff[14]=true;
    pixelOnOff[15]=true;
    pixelOnOff[16]=true;
    
    pixelOnOff[28]=true; //MINUTES
    pixelOnOff[29]=true;
    pixelOnOff[30]=true;
    pixelOnOff[31]=true;
    pixelOnOff[32]=true;
    pixelOnOff[33]=true;
    pixelOnOff[34]=true;
  }  

  /* Minutes between 25-30 or 35-40 - Light "TWENTY," "FIVE," "MINUTES" */
  if ((m>=25 && m<30) || (m>=35 && m<40)){
    pixelOnOff[11]=true;//TWENTY
    pixelOnOff[12]=true;
    pixelOnOff[13]=true;
    pixelOnOff[14]=true;
    pixelOnOff[15]=true;
    pixelOnOff[16]=true;

    pixelOnOff[24]=true;//FIVE
    pixelOnOff[25]=true;
    pixelOnOff[26]=true;
    pixelOnOff[27]=true;

    pixelOnOff[28]=true;//MINUTES
    pixelOnOff[29]=true;
    pixelOnOff[30]=true;
    pixelOnOff[31]=true;
    pixelOnOff[32]=true;
    pixelOnOff[33]=true;
    pixelOnOff[34]=true;


  }

  /* Minutes between 30-35 - Light "HALF" */
  if ((m>=30 && m<35)){
    pixelOnOff[7]=true;
    pixelOnOff[8]=true;
    pixelOnOff[9]=true;
    pixelOnOff[10]=true;
  }
  
  /* Minutes between 5-35 - Light "PAST" */
  if ((m>=5) && (m<35)){
    pixelOnOff[43]=true;
    pixelOnOff[44]=true;
    pixelOnOff[45]=true;
    pixelOnOff[46]=true;
  }
  
  /* Minutes between 35-60 - Light "TO" & MODIFY CURRENT HOUR VALUE */
  if (m>=35){
    pixelOnOff[41]=true;
    pixelOnOff[42]=true;
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
    pixelOnOff[38]=true;
    pixelOnOff[39]=true;
    pixelOnOff[40]=true;
  }
  
  /* Hour=2 - Light "TWO" */
  if (h==2){
    pixelOnOff[35]=true;
    pixelOnOff[36]=true;
    pixelOnOff[37]=true;
  }
  
  /* Hour=3 - Light "THREE" */
  if (h==3){
    pixelOnOff[47]=true;
    pixelOnOff[48]=true;
    pixelOnOff[49]=true; 
    pixelOnOff[50]=true; 
    pixelOnOff[51]=true;     
  }
  
  /* Hour=4 - Light "FOUR" */
  if (h==4){
    pixelOnOff[52]=true;
    pixelOnOff[53]=true;
    pixelOnOff[54]=true;
    pixelOnOff[55]=true;
  }
  
  /* Hour=5 - Light "FIVE" */
  if (h==5){
    pixelOnOff[56]=true;
    pixelOnOff[57]=true;
    pixelOnOff[58]=true;
    pixelOnOff[59]=true;
  }
  
  /* Hour=6 - Light "SIX" */
  if (h==6){
    pixelOnOff[70]=true;
    pixelOnOff[71]=true;
    pixelOnOff[72]=true;
  }
  
  /* Hour=7 - Light "SEVEN" */
  if (h==7){
    pixelOnOff[65]=true;
    pixelOnOff[66]=true;
    pixelOnOff[67]=true;
    pixelOnOff[68]=true;
    pixelOnOff[69]=true;
  }
  
  /* Hour=8 - Light "EIGHT" */
  if (h==8){
    pixelOnOff[60]=true;
    pixelOnOff[61]=true;
    pixelOnOff[62]=true;
    pixelOnOff[63]=true;
    pixelOnOff[64]=true;
  }
  
  /* Hour=9 - Light "NINE" */
  if (h==9){
    pixelOnOff[73]=true;
    pixelOnOff[74]=true;
    pixelOnOff[75]=true;
    pixelOnOff[76]=true;
  }
  
  /* Hour=10 - Light "TEN" */
  if (h==10){
    pixelOnOff[77]=true;
    pixelOnOff[78]=true;
    pixelOnOff[79]=true;
  }
  
  /* Hour=11 - Light "ELEVEN" */
  if (h==11){
    pixelOnOff[80]=true;
    pixelOnOff[81]=true;
    pixelOnOff[82]=true;
    pixelOnOff[83]=true;
    pixelOnOff[84]=true;
    pixelOnOff[85]=true;
  }
  
  /* Hour=12 - Light "TWELVE" */
  if (h==12){
    pixelOnOff[92]=true;
    pixelOnOff[93]=true;
    pixelOnOff[94]=true;
    pixelOnOff[95]=true;
    pixelOnOff[96]=true;
    pixelOnOff[97]=true;
  }

  short 
    currIteration = 0, // How many iterations of the matrix animation have we completed?
    randomNumber = 0; // Random number to "spawn" new threads in the matrix.
  bool 
    allPixelsMatchOnOff = false, // Are all pixels in the matrix matching the clock pixels?
    allPixelsMatchOn = false; // Are all the litup letters from the clock also lit up in the matrix?
  
  // While the matrix does not match the clock
  while (!allPixelsMatchOnOff) {
    
    // Increase the iteration and reset all flags
    currIteration++;
    allPixelsMatchOnOff = true;
    if (currIteration >= matrixIterations) {
      allPixelsMatchOn = true;
    }

    // Start scanning the matrix
    for (short r = gridRows - 1; r >= 0; r--) {
      for (short c = 0; c < gridCols; c++) {

        // If the matrix animation is ending, check our flags first
        if (currIteration >= matrixIterations) {
          if (
            pixelOnOff[pixelGrid[r][c]] && !pixelColours[pixelGrid[r][c]] ||
            !pixelOnOff[pixelGrid[r][c]] && pixelColours[pixelGrid[r][c]]
          ) {
            allPixelsMatchOnOff = false;

            if (pixelColours[pixelGrid[r][c]] == sizeof(colours) - 1) {
              allPixelsMatchOn = false;
            }
          }
        }
        else {
          allPixelsMatchOnOff = allPixelsMatchOn = false;
        }

        // Transition all top-level coloured letters
        if (pixelColours[pixelGrid[r][c]] == sizeof(colours) - 1) {
          // If we're still doing the animation OR the matrix letter colour does NOT match with the clock
          if (
            currIteration < matrixIterations || 
            (
              pixelOnOff[pixelGrid[r][c]] && !pixelColours[pixelGrid[r][c]] ||
              !pixelOnOff[pixelGrid[r][c]] && pixelColours[pixelGrid[r][c]]
            )
          ) {
            // Transition the current letter to the next colour
            pixelColours[pixelGrid[r][c]]--;

            // If there's a letter beneath the current one, then pass on the colour
            if (r + 1 < gridRows) {
              pixelColours[pixelGrid[r + 1][c]] = sizeof(colours) - 1;
            }
          }
        }        
        // If we're stopping the animation AND 
        // the matrix letter colour DOES match with the clock AND
        // there's a letter beneath the current one AND
        // the matrix letters for the current clock are not all lit up
        else if (
          currIteration >= matrixIterations && 
          (
            pixelOnOff[pixelGrid[r][c]] && !pixelColours[pixelGrid[r][c]] ||
            !pixelOnOff[pixelGrid[r][c]] && pixelColours[pixelGrid[r][c]]
          ) && 
          r + 1 < gridRows && 
          !pixelColours[pixelGrid[r][c]] && 
          !allPixelsMatchOn
        ) {
          // Pass the colour along
          pixelColours[pixelGrid[r + 1][c]] = sizeof(colours) - 1;
        }
        // Transition all other coloured letters
        else if (!pixelColours[pixelGrid[r][c]]) {
          matrixFade(r, c, currIteration);
        }

        // If this is the last/top row and we don't have all clock letters lit
        if (r == 0 && !allPixelsMatchOn) {
          // Pick a random number between 1 and X
          randomNumber = random(1, matrixDensity);
          
          // If the random number is 1
          if (randomNumber == 1) {
            // Start a new thread in the matrix
            pixelColours[pixelGrid[r][c]] = sizeof(colours) - 1;
          }
        }
      }
    }

    colourPixels(millisWait);
  }
}