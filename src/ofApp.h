#pragma once

#include "ofMain.h"

class Panel
{
public:
    int width;
    int height;
    ofPixels pixels;
    ofImage image;
    unsigned char* bytes;
    int numPixels;
    int circleSize = 5;
    float pitch = circleSize*2;
    
    int drawWidth;
    int drawHeight;
    Panel()
    {
        width = 24;
        height = 16;
        numPixels = width*height;
        
        bytes  = new unsigned char[numPixels];
        
        drawWidth =  width*pitch;
        drawHeight = height*pitch;

    }
    void setup(string imagePath)
    {
        image.load(imagePath);
        image.resize(width, height);
        image.update();
        pixels = image.getPixels();
        
    }
    void draw()
    {
        ofPushMatrix();
        ofPushStyle();
        int counter = 0;
        ofColor foregroundColor;
        
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                
                ofColor color = pixels.getColor(x, y);
                if(color == ofColor::white)
                {
                    bytes[counter] = '0';
                    foregroundColor = ofColor::white;
                    
                    
                }else
                {
                    bytes[counter] = '1'; 
                    foregroundColor = ofColor::black; 
                    
                } 
                
                
                ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
                ofSetColor(foregroundColor);
                ofDrawCircle(x*pitch, y*pitch, circleSize);
                drawWidth = x*pitch;
                drawHeight = y*pitch;
                //ofDrawBitmapStringHighlight(info.str(), x*30, y*30, ofColor::black, foregroundColor);
                
                counter++;
            }
        }
        ofPopStyle();
        ofPopMatrix();
        //ofLog() << "drawWidth: " << drawWidth << " drawHeight: " << drawHeight;

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
        serial.setup("/dev/ttyACM0", baud); //open the first device
        
        
        Panel panel;
        panel.setup("of.png");
        
        Panel panel2;
        panel2.setup("of.png");
        
        panels.push_back(panel);
        panels.push_back(panel2);

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

            for(size_t i=0; i<panels.size(); i++)
            {
                Panel panel = panels[i];
                int written = serial.writeBytes(panel.bytes, panel.numPixels);
                if(written == panel.numPixels)
                {
                    didSend = true;
                }
                
            }
            
            
            
        }
        //ofLog() << "didSend: " << didSend;

        
    }
};

