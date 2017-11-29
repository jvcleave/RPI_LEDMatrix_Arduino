#pragma once

#include "ofMain.h"

class Panel
{
public:
    int width;
    int height;
    
    vector<unsigned char>payload;
    
    float circleSize = 3;
    float pitch = circleSize*2;
    int drawWidth;
    int drawHeight;
    bool doRandom;
    bool doRows;
    
    Panel()
    {
        width = 24;
        height = 16;
        doRandom = false;
        doRows = false;
        
        drawWidth =  width*pitch;
        drawHeight = height*pitch;
        payload.resize(48);
        
    }
    
    
    
    void processPixels(ofPixels& pixels)
    {
        ofColor foregroundColor;
        int pixelCounter = 0;
        int currentCharIndex = 0;
        int bitIndex = 0;
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
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
                bool isBitOn = (bit == 1);
                leds.push_back(isBitOn);
                
            }
        }
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                
                if(leds[counter])
                {
                    color = ofColor::white;
                    
                    
                }else
                {
                    color = ofColor::black; 
                    
                }
                
                ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
                ofSetColor(color);
                ofDrawCircle(x*pitch, y*pitch, circleSize);
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
    
    vector<Panel> panels;
    bool isFrameNew = false;
    ofFbo fbo;
    ofFbo videoPanelFBO;
    
    ofPixels pixels;
    ofImage image;
    ofVideoPlayer videoPlayer;
    ofTrueTypeFont        font;
    
    void setup()
    {
        //ofSetVerticalSync(false);
        font.load("DIN.otf", 90);
        
        ofSetLogLevel(OF_LOG_VERBOSE);
        ofSetLogLevel("ofSerial", OF_LOG_WARNING);
        //ofSetFrameRate(2);
        
        serial.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
        
        int baud = 115200;
        serial.setup("/dev/cu.usbmodem3A21", baud); //open the first device
        image.load("of.png");
        //image.resize(24, 16);
        //image.update();
        
        fbo.allocate(24, 16, GL_RGB);
        fbo.begin();
        ofClear(0, 0, 0, 0);
        fbo.end();
        pixels.allocate(24, 16, OF_PIXELS_RGB);
        
        videoPanelFBO.allocate(80, 120, GL_RGB);
        videoPanelFBO.begin();
        ofClear(0, 0, 0, 0);
        videoPanelFBO.end();
        
        //pixels = image.getPixels();
        fbo.readToPixels(pixels);
        for(size_t i=0; i<8; i++)
        {
            Panel panel;
            panel.processPixels(pixels);  
            
            //panel.doRows = true;
            
            panels.push_back(panel);
        }
        videoPlayer.load("fingers.mov");
        videoPlayer.setLoopState(OF_LOOP_NORMAL);
        videoPlayer.play();
        
    }
    void fboDrawRects(int i)
    {
        ofSetColor(ofColor::white);
        
        ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
        ofSetColor(ofColor::black);
        for(size_t n=0; n<i+1; n++)
        {
            ofDrawRectangle(n, 0, 1, 1); 
        }
    }
    void fboDrawImage()
    {
        image.draw(0, 0, fbo.getWidth(), fbo.getHeight());
        ofSetColor(ofColor::black);
        for(size_t n=0; n<30; n++)
        {
            ofDrawRectangle(ofRandom(fbo.getWidth()), ofRandom(fbo.getHeight()), 1, 1); 
        }
    }
    
    void update()
    {
        
        videoPlayer.update();
        
        
        ofSetWindowTitle(ofToString(ofGetFrameRate()));
        
        
        
    }
    
    
    void draw()
    {
        
        int videoPanelWidth = videoPlayer.getWidth()/4;
        int videoPanelHeight = videoPlayer.getHeight()/2;
        string messages = "01234567";
        
        
        
        for(size_t i=0; i<panels.size(); i++)
        {
            messages[i] = (char)ofToString(ofRandom(0, 9)).c_str()[0];
            
            //panels[i].doRows = !panels[i].doRows;
            
            videoPanelFBO.begin();
            ofClear(ofColor::black);
            /*
             if(i<=4)
             {
             videoPlayer.getTexture().drawSubsection(0, 0, videoPanelWidth, videoPanelHeight, i*videoPanelWidth, 0);
             }else
             {
             videoPlayer.getTexture().drawSubsection(0, 0, videoPanelWidth, videoPanelHeight, i*videoPanelWidth, videoPanelHeight);
             }*/
            
            ofPushStyle();
            
            ofSetColor(ofColor::white);
            font.drawString(ofToString(messages[i]), 0, videoPanelHeight);
            ofPopStyle();
            
            videoPanelFBO.end();
            
            ofPushStyle();
            fbo.begin();
            ofClear(0, 0, 0, 0);
            //fboDrawImage();
            //fboDrawRects(i);
            
            
            videoPanelFBO.draw(0, 0, fbo.getWidth(), fbo.getHeight());
            
            
            fbo.end();
            ofPopStyle();
            fbo.readToPixels(pixels);
            
            panels[i].processPixels(pixels);
            
            ofPushMatrix();
            ofTranslate(panels[i].drawWidth*i, 0);
            panels[i].draw();
            ofTranslate(0, panels[i].drawHeight);
            fbo.draw(0, 0, panels[i].drawWidth, panels[i].drawHeight);
            
            ofPopMatrix();
            
            
        }
        
        if(ofGetFrameNum() % 50 == 0)
        {
            isFrameNew = true;
        }
        if(isFrameNew)
        {
            vector<unsigned char>payload;
            
            for(size_t i=0; i<8; i++)
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
                /*
                 int readData = serial.readByte();
                 ofLog() << "readData: " << readData;
                 if(readData == 9)
                 {
                 ofLog() << "READY: " << ofGetFrameNum();
                 
                 
                 }*/
            }
            
            isFrameNew = false;
        }  
        
        
        ofTranslate(0, 300);
        //videoPanelFBO.draw(0, 0);
        videoPlayer.draw(0, 0);
        
        
        /*
         int videoPanelWidth = videoPlayer.getWidth()/4;
         int videoPanelHeight = videoPlayer.getHeight()/2;
         
         ofTranslate(0, 100);
         for(size_t i=0; i<panels.size(); i++)
         {
         videoPanelFBO.begin();
         ofClear(0, 0, 0, 0);
         if(i<=4)
         {
         videoPlayer.getTexture().drawSubsection(0, 0, videoPanelWidth, videoPanelHeight, i*videoPanelWidth, 0);
         }else
         {
         videoPlayer.getTexture().drawSubsection(0, 0, videoPanelWidth, videoPanelHeight, i*videoPanelWidth, videoPanelHeight);
         }
         videoPanelFBO.end();
         
         videoPanelFBO.draw(i*24, 0, fbo.getWidth(), fbo.getHeight());
         }*/
        
    }
    
};


