#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:

    ofImage image;
    vector<int> panel;
    int panelWidth = 24;
    int panelHeight = 16;

    ofSerial    serial;
    unsigned char* bytes;
    
    string outputString;
    int            nBytesRead = 0;                    // how much did we read?
    bool didSend = false;
    //--------------------------------------------------------------
    void setup(){
        //ofSetVerticalSync(false);
        
        ofSetLogLevel(OF_LOG_VERBOSE);
        //ofSetFrameRate(2);
        bytes  = new unsigned char[panelWidth*panelHeight];
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

        int baud = 115200;
        serial.setup("/dev/ttyACM0", baud); //open the first device
        
        for(int i=0; i<panelWidth*panelHeight; i++)
        {
            if(i%2 == 0)
            {
                panel.push_back(0);
            }else
            {
                
                panel.push_back(1);
            }
            
        }
        
        image.load("of.png");
        image.resize(panelWidth, panelHeight);
        image.update();
        ofLog() << image.getWidth() << " : " << image.getHeight();

        /*
        for(int x=0; x<image.getWidth(); x++)
        {
            for(int y=0; y<image.getHeight(); y++)
            {
                int position = x*y;
                ofColor color = ofColor::red;
                
                if(position%2 == 0)
                {
                    
                    image.setColor(position, ofColor::red);
                    
                }
                
                
            }
        }
        image.update();*/
        ofLog() << "panel.size: " << panel.size();
        
    }
    
    //--------------------------------------------------------------
    void update(){
        
    }
    
    //--------------------------------------------------------------
    void draw(){

        ofPushMatrix();
        ofPushStyle();
        int position = 0;
        int counter = 0;
        ofColor foregroundColor;
        ofTranslate(60, 60);
        int circleSize = 5;
        float pitch = circleSize*2;
        ofPixels pixels = image.getPixels();

        for(int x=0; x<image.getWidth(); x++)
        {
            for(int y=0; y<image.getHeight(); y++)
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
                
                //ofDrawBitmapStringHighlight(info.str(), x*30, y*30, ofColor::black, foregroundColor);

                counter++;
            }
        }
        ofPopStyle();
        ofPopMatrix();
        //ofDrawBitmapStringHighlight(outputString, 100, 100);
        if(!didSend)
        {
            ofSleepMillis(2000);

            int written = serial.writeBytes(bytes, 384);
            if(written == 384)
            {
                didSend = true;
            }
            
        }
        //ofLog() << "didSend: " << didSend;

        
    }
};

