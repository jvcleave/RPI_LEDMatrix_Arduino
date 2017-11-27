#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR   3 


  
Adafruit_HT1632LEDMatrix* matrix;

class Panel
{
  public:
    int8_t pin;
    int id;

    uint8_t startX = 0;
    uint8_t endX = 24;
    uint8_t startY = 0;
    uint8_t endY = 16;
    Panel()
    {
      
    }
    void setup(int id_, int8_t pin_)
    {
      id = id_;
      startX = id*24;
      endX = startX+24;
      
      pin = pin_;
    }
};

Vector<Panel*> panels = Vector<Panel*>();
Vector<int8_t> pins = Vector<int8_t>();
char* buffer;
int bufferSize;

bool doDrawDebug = false;
String debugString = "";
bool hasData = false;

void setup() 
{

  for (int8_t i=4; i< 12; i++) 
  {
        pins.push_back(i);
      
  }
  for (size_t i=0; i< 2; i++) 
  {
      Panel* panel = new Panel();
      panel->setup(i, pins[i]);
      panels.push_back(panel);
  }
    bufferSize = 384*panels.size();
    buffer = new char [bufferSize];
    matrix = new Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, pins);
    Serial.setTimeout(10000);
    Serial.begin(115200);
    matrix->begin(ADA_HT1632_COMMON_16NMOS);
    matrix->fillScreen();
    delay(500);
    matrix->clearScreen();
    matrix->setTextWrap(false);
}


void drawDebug()
{
      debugString = "";
      matrix->clearScreen();
      matrix->setCursor(0, 0);
      debugString.concat(String(panels.size()));
      debugString.concat(" bufferSize:");
      debugString.concat(String(bufferSize));
      matrix->print(debugString);
      matrix->writeScreen();
}
void loop() 
{

  bool isFrameNew = false;
  String incomingString = "INCOMING: ";
  if (Serial.available()) 
  {
 
    size_t inByte = 0;
    inByte = Serial.readBytes(buffer, bufferSize);
    
    incomingString = String(inByte);

    if(inByte == bufferSize)
    {
      isFrameNew = true;
      hasData = true;
    }

  }else
  {
    if(!hasData)
    {
        drawDebug(); 
        return;  
    }
 
  }

if(isFrameNew)
  {
    matrix->clearScreen();
    matrix->setCursor(0, 0);
   
   size_t counter = 0;

    for (size_t i=0; i< panels.size(); i++) 
    {

        Panel* panel = panels[i];
        
        for (uint8_t x=panel->startX; x<panel->endX; x++) 
        {
          for (uint8_t y=0; y< 16; y++) 
          {
            //String charString = String(buffer[counter]);
            //String zerostring = String(0);
            if(buffer[counter]== '0')
            {
              matrix->clrPixel(x, y);
            }else
            {
              matrix->setPixel(x, y);
            }
            counter++;
          

          }
        }
        
    }
    matrix->writeScreen();

       /*
        * delay(1000);
     
      size_t numWritten = Serial.write("NEW");
      incomingString.concat(":");
      incomingString.concat(String(numWritten));
      matrix->setCursor(24, 0);

      matrix->print(incomingString);
      matrix->writeScreen();*/

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
