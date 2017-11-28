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

Panel* panels;
int8_t* pins;
#define numPanels 8



#define bufferSize (48*numPanels)

bool hasData = false;
char buffer[bufferSize];


#define numValues (384*numPanels)

//uint8_t values[numValues];



void setup() 
{
  pins = new int8_t[numPanels];
  
  for (int8_t i=0; i< numPanels; i++) 
  {
        pins[i] = i+4;
  }

  panels = new Panel[numPanels];
  
  for (size_t i=0; i< numPanels; i++) 
  {
      Panel panel;
      panel.setup(i, pins[i]);
      panels[i] = panel;
  }
    

    
        
    #if 0
    int valueCounter  = 0;
    bool isSet = false;
    for(int i=0; i<bufferSize; i++)
    {
      for(int j=0; j<8; j++)
      {
        values[valueCounter] = 1;
        valueCounter++;
      }
    }
    #endif
    //hasData = true;

    
    matrix = new Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, pins, numPanels);
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
  String incomingString = "INCOMING: ";
  if(!hasData)
  {
    if (Serial.available()) 
    {
    
      size_t inByte = 0;
      inByte = Serial.readBytes(buffer, bufferSize);
      
      incomingString = String(inByte);
  
      if(inByte == bufferSize)
      {
          #if 0
          int bitCounter = 0;
          for(size_t i=0; i<bufferSize; i++)
          {
            for (size_t n = 0; n < 8; n++) 
            {
                int currentBit = (buffer[i] >> n) & 1U;
                int isBitOn = (currentBit == 0);
                values[bitCounter] = isBitOn;
                bitCounter++;

            }
          }
          #endif
        isFrameNew = true;
        hasData = true;
      }
    }
 }
  if(isFrameNew)
  {
    matrix->clearScreen();
    matrix->setCursor(0, 0);
     
    size_t counter = 0;

    int bitCounter = 0;
    int charCounter = 0;
    for (size_t i=0; i< numPanels; i++) 
    {
    
        Panel* panel = &panels[i];
        
        for (uint8_t x=panel->startX; x<panel->endX; x++) 
        {
          for (uint8_t y=0; y< 16; y++) 
          {
            
            char currentChar = buffer[charCounter];
            int currentBit = (currentChar >> bitCounter) & 1U;
            int isBitOn = (currentBit == 0);
            if(isBitOn)
            {
              matrix->clrPixel(x, y);
            }else
            {
              matrix->setPixel(x, y);
            }
            if(bitCounter+1 < 8)
            {
              bitCounter++;  
            }else
            {
              bitCounter = 0;
              if(charCounter+ 1 < bufferSize)
              {
                  charCounter++;
              }else
              {
                charCounter = 0;  
              }
            } 
          }
        }
        
    }
    matrix->writeScreen();

   }
}

#if 0
void valueLoop()
{
  int pixelCounter = 0;

  while(1)
  {
      matrix->clearScreen();
      matrix->setCursor(0, 0);
      //matrix->drawGrayscaleBitmap(0, 0, (uint8_t*)values, matrix->width(), matrix->height());

      for (uint8_t x=0; x<matrix->width(); x++) 
      {
        for (uint8_t y=0; y< matrix->height(); y++) 
        {
          if(values[pixelCounter] == 1)
          {
            matrix->setPixel(x, y);
          }else
          {
              matrix->clrPixel(x, y);
          }
          
          
          matrix->writeScreen();
          if(pixelCounter+1 < valueCounter)
          {
              pixelCounter++;
          }else
          {
            pixelCounter = 0;  
          }
        }
      }
      delay(1000);
  }
   
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

void drawDebug()
{
      String debugString = "";
      //delay(2000);

      matrix->clearScreen();
      matrix->setCursor(0, 0);
      
      //debugString.concat(String(panels.size()));
     // debugString.concat(" bufferSize:");
      //debugString.concat(String(bufferSize));

      debugString.concat("hD:");
      debugString.concat(String(hasData)); 
      
      debugString.concat("vC:");
      debugString.concat(String(valueCounter));
      
      
      debugString.concat("bSz:");
      debugString.concat(String(bufferSize));
      
      matrix->print(debugString);
      matrix->writeScreen();
}
#endif