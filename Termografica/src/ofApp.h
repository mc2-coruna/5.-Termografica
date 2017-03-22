#pragma once

#include "ofMain.h"
#include "ofxDeckLink.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    void updateParameters();
    void writeToSettingsFile();
    void managePreviousSettings(int function);
    
    
    // Visible camera
    ofVideoGrabber visibleImage;
    bool visibleImageInit = false;
    int imagenVisibleID = -1;
    int visibleImageWidth,visibleImageHeight;
    int visibleImageCenterX, visibleImageCenterY;
    int visibleImagePositionX, visibleImagePositionY;
    float visibleImageSize, visibleImageAspect;
    
    
    // Infrared camera
    ofxDeckLinkAPI::Input thermoImage;
    bool thermoImageInit = false;
    int thermoImageCenterY;
    int thermoImageWidth, thermoImageHeight;
    int thermoImagePositionY;
    float thermoImageAspect;
    
    // Auxiliars
    int alpha = 255;
    bool showThermo = false;
    ofTrueTypeFont mono12;
    ofTrueTypeFont mono24;
    ofTrueTypeFont mono36;
    
    uint64_t thermoImageOffTime;
    int thermoImageTime;      // seconds
    
    int menuLevel = 0;
    int menuLevel_prev = 0;
    int visibleImageCenterX_prev;
    int visibleImageCenterY_prev;
    float visibleImageSize_prev;
    int thermoImageCenterY_prev;
    int thermoImageTime_prev;
    long frames = 0;
    long framesAnt = 0;
    long isNewFrame = 0, isNewFrameInc=0, isNewFrameDiffAnt = 0;
    long framesDiff, framesDiffAnt;
    
    
    string menuText0;
    string menuText1;
    string menuText2;
    string menuText9;
    
    char parameterText1[100];
    char parameterText2[100];
    
    
};
