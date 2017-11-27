#include "Adafruit_HT1632.h"

//orange 17

#define HT_DATA 2

//yellow 5

#define HT_WR   3 
#define HT_CS0   4
#define HT_CS1  5
#define HT_CS2  6
#define HT_CS3  7
#define HT_CS4  8
// use this line for single matrix
//Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
// use this line for two matrices!

  Vector<int8_t> pins = Vector<int8_t>();
  

  
Adafruit_HT1632LEDMatrix* matrix;



//pins.push_back(9);





void setup() 
{
    pins.push_back(4);
    pins.push_back(5);
    pins.push_back(6);
    pins.push_back(7);
    pins.push_back(8);
    pins.push_back(9);
    pins.push_back(10);
    pins.push_back(11);


  //
    matrix = new Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, pins);
    Serial.setTimeout(10000);
    Serial.begin(115200);
    matrix->begin(ADA_HT1632_COMMON_16NMOS);
    matrix->fillScreen();
    delay(500);
    matrix->clearScreen();
    matrix->setTextWrap(false);
}

void loop() 
{
  
  bool isFrameNew = false;
  char buffer[384];
  String incomingString = "INCOMING: ";
  if (Serial.available()) 
  {
    
    size_t inByte = 0;
    inByte = Serial.readBytes(buffer, 384);
    
    incomingString = String(inByte);

    if(inByte == 384)
    {
      isFrameNew = true;
    }
    incomingString.concat("NEW");

  }/*
  else
  {
    matrix->print("NO SERIAL DATA");  
    matrix->writeScreen();
    //demoLoop();
  }*/
/*
  if(isFrameNew)
  {
    matrix->clearScreen();
    matrix->setCursor(0, 0);
    matrix->print(incomingString);
    matrix->writeScreen();
  }
  incomingString = "";
  */

if(isFrameNew)
  {
    size_t counter = 0;
    matrix->clearScreen();
    matrix->setCursor(0, 0);
    for (uint8_t x=0; x<24; x++) 
      {
        for (uint8_t y=0; y< 16; y++) 
        {
          String charString = String(buffer[counter]);
          String zerostring = String(0);
          if(charString.equals(zerostring))
          {
            matrix->clrPixel(x, y);
          }else
          {
            matrix->setPixel(x, y);
          }
          counter++;

    
        }
      }
      matrix->writeScreen();

  }


  //delay(50);

}

void demoLoop()
{
  for(uint8_t i=0; i<4; i++) 
    {
      matrix->setRotation(i);
      matrix->clearScreen();
      matrix->setCursor(0, 0);
      matrix->print("Hello");
      matrix->writeScreen();
      delay(1000);
    
      for (uint8_t y=0; y<matrix->height(); y++) 
      {
        for (uint8_t x=0; x< matrix->width(); x++) 
        {
          matrix->setPixel(x, y);
          matrix->writeScreen();
    
        }
      }
      
      // blink!
      matrix->blink(true);
      delay(2000);
      matrix->blink(false);
    
      // Adjust the brightness down
      for (int8_t i=15; i>=0; i--) 
      {
        matrix->setBrightness(i);
        delay(20);
      }
      
      // then back up
      for (uint8_t i=0; i<16; i++) 
      {
        matrix->setBrightness(i);
        delay(20);
      }
    
      for (uint8_t y=0; y<matrix->height(); y++)
      {
        for (uint8_t x=0; x< matrix->width(); x++) 
        {
          matrix->clrPixel(x, y);
          matrix->writeScreen();
        }
      }
      
    }  
}

