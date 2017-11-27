#pragma once

#include "ofMain.h"

class Panel
{
public:
    int width;
    int height;
    ofPixels pixels;
    ofImage image;
    vector<unsigned char> bytes;
    int numPixels;
    float circleSize = 3;
    float pitch = circleSize*2;
    
    int drawWidth;
    int drawHeight;
    bool doRandom = true;

    Panel()
    {
        width = 24;
        height = 16;
        numPixels = width*height;
        doRandom = false;
        bytes.resize(numPixels);
        
        drawWidth =  width*pitch;
        drawHeight = height*pitch;

    }
    void setup(string imagePath)
    {
        image.load(imagePath);
        image.resize(width, height);
        image.update();
        pixels = image.getPixels();
        ofColor foregroundColor;
        int counter = 0;

        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                if(doRandom)
                {
                    int rNum = (int)ofRandom(0, 56);
                    if(rNum%2 == 0)
                    {
                        bytes[counter] = '0';
                        
                    }
                    else
                    {
                        bytes[counter] = '1'; 
                        
                    } 
                    
                }else
                {
                    ofColor color = pixels.getColor(x, y);
                    if(color == ofColor::white)
                    {
                        bytes[counter] = '0';
  
                    }else
                    {
                        bytes[counter] = '1'; 
                        
                    }  
                    
                }
                drawWidth = x*pitch;
                drawHeight = y*pitch;
                counter++;
            }
        }
    }
    void draw()
    {
        ofPushMatrix();
        ofPushStyle();
        int counter = 0;
        ofColor color;
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
    
                if(bytes[counter] == '0')
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
        
        for(size_t i=0; i<8; i++)
        {
            Panel panel;
            if(i%2 ==0)
            {
                panel.doRandom = true; 
            }
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
            
            for(size_t i=0; i<2; i++)
            {
                Panel panel = panels[i];
                
                for(size_t n=0; n<panel.bytes.size(); n++)
                {
                    payload.push_back(panel.bytes[n]);
                }
                
                
                
            }
            
            int written = serial.writeBytes(&payload[0], payload.size());
            if(written == payload.size())
            {
                didSend = true;
                ofLog() << "didSend: " << didSend;
                //serial.flush();    
                
            }
            
#if 0
            if(didSend)
            {
                
                int bytes_required = 3;  
                unsigned char bytes[bytes_required];  
                int bytes_remaining = bytes_required;  
                while ( bytes_remaining > 0 )  
                {  
                    int avail = serial.available();
                    //ofSleepMillis(3000);
                    while(serial.available() > 0)
                    {
                        // check for data  
                        // try to read - note offset into the bytes[] array, this is so that we   
                        // don't overwrite the bytes we already have  
                        int result = serial.readBytes( &bytes[bytes_required-bytes_remaining], bytes_remaining ); 
                        
                        ofLog() << "avail :" << avail << " result: " << result;

                        // check for error code  
                        if ( result == OF_SERIAL_ERROR )  
                        {  
                            ofLog() << "unrecoverable error reading serial" ;  
                            break;  
                        }  
                        else if ( result == OF_SERIAL_NO_DATA )  
                        {  
                            //ofLog() << "OF_SERIAL_NO_DATA" ;  
                            
                        }  
                        else  
                        {  
                            // we read some data!  
                            // maybe not all..  
                            bytes_remaining -= result;  
                            ofLog() << "bytes_remaining: " << bytes_remaining <<  " result: " << result; 
                        } 
                        
                    }
                    
                    
                    
                    //ofSleepMillis(1000);
                    
                }  
                
            }
#endif
            
        }
    }

};

