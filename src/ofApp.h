#pragma once

#include "ofMain.h"

class Panel
{
public:
    int width;
    int height;
    ofPixels pixels;
    ofImage image;
    vector<unsigned char>payload;
    
    int numPixels;
    float circleSize = 3;
    float pitch = circleSize*2;
    bitset<384> bits;
    int drawWidth;
    int drawHeight;
    bool doRandom;
    bool doRows;
    
    Panel()
    {
        width = 24;
        height = 16;
        numPixels = width*height;
        doRandom = false;
        doRows = false;
        //bits.resize(numPixels);
        
        drawWidth =  width*pitch;
        drawHeight = height*pitch;
        payload.resize(48);
        
        
    }
    
    void setValue(int currentCharIndex, int index, int value )
    {
        
    }
    
    void setup(string imagePath)
    {
        image.load(imagePath);
        image.resize(width, height);
        image.update();
        pixels = image.getPixels();
        ofColor foregroundColor;
        int pixelCounter = 0;
        int currentCharIndex = 0;
        int bitIndex = 0;
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                //ofLog() << " pixelCounter : " << pixelCounter << " currentCharIndex: " << currentCharIndex << " bitIndex: " << bitIndex;
                unsigned char* currentChar = &payload[currentCharIndex];
                unsigned long newbit;
                bool isOn = false;
                
                if(doRandom)
                {
                    int rNum = (int)ofRandom(0, 56);
                    
                    if(rNum%2 == 0)
                    {                        
                        isOn = true;
                    }
                    
                    
                }
                if(doRows)
                {
                    if(x%2 == 0)
                    {
                        isOn = true;
                        
                    }
                    
                }
                if(!doRows && !doRandom)
                {
                    ofColor color = pixels.getColor(x, y);
                    if(color == ofColor::white)
                    {
                        isOn = true; 
                        
                    } 
                    
                }
                if(isOn)
                {
                    newbit = !!1;
                }else
                {
                    newbit = !!0;
                }
                *currentChar ^= (-newbit ^ *currentChar) & (1UL << bitIndex);
                //leds.push_back(isOn);
                
                drawWidth = x*pitch;
                drawHeight = y*pitch;
                pixelCounter++;
                if(pixelCounter % 8 == 0)
                {
                    currentCharIndex++;
                    bitIndex = 0;
                }else
                {
                    bitIndex++;
                    
                }
                
            }
        }
        
    }
    void draw()
    {
        ofPushMatrix();
        ofPushStyle();
        int counter = 0;
        ofColor color;
        vector<bool>leds;
        for(size_t i=0; i<payload.size(); i++)
        {
            for (size_t n = 0; n < 8; n++) 
            {
                int bit = (payload[i] >> n) & 1U;
                bool isBitOn = (bit == 0);
                leds.push_back(isBitOn);
                
            }
        }
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                
                if(leds[counter] == 0)
                {
                    color = ofColor::white;
                    
                    
                }else
                {
                    color = ofColor::black; 
                    
                }
                
                ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
                ofSetColor(color);
                ofDrawCircle(x*pitch, y*pitch, circleSize);
                
                //ofDrawBitmapStringHighlight(info.str(), x*30, y*30, ofColor::black, foregroundColor);
                
                counter++;
            }
        }
        ofPopStyle();
        ofPopMatrix();
        
    }
    
};

class ofApp : public ofBaseApp{
    
public:
    
    
    
    ofSerial    serial;
    string outputString;
    bool didSend = false;
    
    vector<Panel> panels;
    void setup()
    {
        //ofSetVerticalSync(false);
        
        ofSetLogLevel(OF_LOG_VERBOSE);
        //ofSetFrameRate(2);
        
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
        
        int baud = 115200;
        serial.setup("/dev/cu.usbmodem3A21", baud); //open the first device
        
        for(size_t i=0; i<8; i++)
        {
            Panel panel;
            if(i%2 ==0)
            {
                //panel.doRows = true; 
            }
            //panel.doRows = true;
            panel.setup("of.png");
            panels.push_back(panel);
        }
        
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        int spacing = 60;
        ofPushMatrix();
        ofTranslate(60, 60);
        for(size_t i=0; i<panels.size(); i++)
        {
            ofPushMatrix();
            ofTranslate(panels[i].drawWidth*i, 0);
            panels[i].draw();
            ofPopMatrix();
            
        }
        ofPopMatrix();
        
        
        //ofDrawBitmapStringHighlight(outputString, 100, 100);
        if(!didSend)
        {
            ofSleepMillis(2000);
            vector<unsigned char>payload;
            
            for(size_t i=0; i<3; i++)
            {
                Panel panel = panels[i];
                
                for(size_t n=0; n<panel.payload.size(); n++)
                {
                    payload.push_back(panel.payload[n]);
                }
                
                
                
            }
            
            int written = serial.writeBytes(&payload[0], payload.size());
            if(written == payload.size())
            {
                didSend = true;
                ofLog() << "didSend: " << didSend;
                
            }
            
            
            
        }
    }
    
};


