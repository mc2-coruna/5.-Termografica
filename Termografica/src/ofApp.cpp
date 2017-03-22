#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofFile settingsFile;
    string frase;
    string settingsData[10], settingsNumber, parameter[10];
    ofBuffer buff;
    bool isNumber=false;
    
    mono12.load("mono.ttf", 12);
    mono24.load("mono.ttf", 24);
    mono36.load("mono.ttf", 36);
    
    ofSetFrameRate(25);
    ofEnableAlphaBlending();    // turn on alpha blending
    ofSetVerticalSync(true);
    
    
    // List USB cameras and choose Logitech Device
    vector<ofVideoDevice> devices = visibleImage.listDevices();
    printf("\n------------** Video Devices------------\n");
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device and check if it's a Logitech USB camera
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }
        else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    printf("\n----------------------------------------\n");
    
    
    if (devices.size() == 1) imagenVisibleID = 0;
    else if (devices.size() > 1) imagenVisibleID = 1;
    
    
    
    if ((devices.size() > 0) and (imagenVisibleID >=0)){
        visibleImage.setVerbose(true);
        visibleImage.setDeviceID(imagenVisibleID);
        
        //visibleImage.initGrabber(1920,1080);
        visibleImage.initGrabber(1280,960);
        
        visibleImageInit = true;
        printf("Inited USB cam with  ID = %i and %i by %i resolution\n\n", imagenVisibleID, (int)visibleImage.getWidth(), (int)visibleImage.getHeight());
    }
    else printf("  ******  Can't init the USB camera  ******\n");
    
    
    thermoImageTime = 10;
    
    // init the USB cam at 1920 x 1080
    // Default visible image parameters
    visibleImageAspect = visibleImage.getHeight() / visibleImage.getWidth();
    visibleImageSize = 1.0;
    visibleImageCenterX = (int) ofGetWidth()/2;
    visibleImageCenterY = (int) ofGetHeight()/2;
    
    
    // Init the device connected to the blackMagic interface
    // Althow inited, it will throw warning messages if there is not valid input signal
    // Default Thermo image parameters
    thermoImageAspect = 0.75;
    thermoImageWidth  = (int) ofGetWidth();
    thermoImageHeight = thermoImageWidth * thermoImageAspect;
    thermoImageCenterY = (int) ofGetHeight()/2;
    if (thermoImage.setup(0)) {
        thermoImage.start(bmdModePAL);
        thermoImageInit = true;
        printf("BlackMagic device loged for thermo image\n");
    }
    else printf("  ******  Can't init the BlackMagic Device for ThermoImage  ******\n");
    
    
    updateParameters();
    
    
    
    
    
    
    settingsFile.open(ofToDataPath("settings.txt"), ofFile::ReadWrite);
    if (settingsFile.is_open()){
        buff = settingsFile.readToBuffer();
        frase = buff;
        int position = 0;
        
        printf("\n");
        int parameterIndex = 0;
        
        while (position < frase.length())
        {
            if (frase.compare(position,1,":") == 0) {
                isNumber = true;
            }
            
            else if (isNumber){
                if (frase.compare(position,1,";") == 0){
                    position++;
                    isNumber = false;
                    printf("[%i]  %s:\t %i \n", parameterIndex, parameter[parameterIndex].c_str(), ofToInt(settingsData[parameterIndex]));
                    parameterIndex ++;
                }
                else settingsData[parameterIndex] = settingsData[parameterIndex] + frase[position];
            }
            else parameter[parameterIndex] = parameter [parameterIndex]+ frase[position];
            position++;
        }
        
        
        
        
        
        
        
        visibleImageCenterX = ofToInt(settingsData[0]);
        visibleImageCenterY = ofToInt(settingsData[1]);
        visibleImageSize    = (float)ofToInt(settingsData[2]) / 100;
        thermoImageCenterY  = ofToInt(settingsData[3]);
        thermoImageTime     = ofToInt(settingsData[4]);
        updateParameters();
        
        settingsFile.close();
        
        
    }
    else writeToSettingsFile();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (thermoImageOffTime > ofGetElapsedTimeMillis()) showThermo=true;
    else showThermo=false;
    
    
    if (visibleImageInit) {  // call this once per update
        visibleImage.update();
        if (visibleImage.isFrameNew()){
            isNewFrame ++;
            isNewFrameInc ++;
            framesDiffAnt = framesDiff;
            framesDiff = 0;
        }
    }
    
    if (thermoImageInit) thermoImage.update();
    
    if ((menuLevel_prev==0)and (menuLevel>0))
        managePreviousSettings(0);
    
    menuLevel_prev = menuLevel;
    
    
    if (frames %10000 == 0)
    {
        if (visibleImageInit){
            
            visibleImageInit = false;
            visibleImage.close();
            
            visibleImage.setDeviceID(imagenVisibleID);
            visibleImage.initGrabber(1280,960);
            
            visibleImageInit = true;
            
        }
        frames++;
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //char texto[256];
    
    // Update the crossfade variable
    if (showThermo){
        alpha -=10;
        if (alpha <= 0) alpha = 0;}
    else{
        alpha +=10;
        if (alpha >= 255) alpha = 255;}
    
    
    // draws the Thermo Image
    ofSetColor(ofColor (255));
    ofClear(0);
    if (thermoImageInit)
        thermoImage.draw(0, thermoImagePositionY, thermoImageWidth, thermoImageHeight);
    
    // draws the visible Image
    ofSetColor(ofColor (255,alpha));
    if (visibleImageInit){
        visibleImage.draw(visibleImagePositionX, visibleImagePositionY, visibleImageWidth, visibleImageHeight);
    }
    
    
    
    // draws the settings menu
    ofSetColor (255);
    
    if (menuLevel==0)
    {
        if (frames < 500)
            mono12.drawString(menuText0, ofGetWidth()-550,ofGetHeight()-100);
    }
    
    else if (menuLevel>0)
    {
        alpha = 125;
        if      (menuLevel == 1) mono12.drawString(menuText1, 50,50);
        else if (menuLevel == 2) mono12.drawString(menuText2, 50,50);
        else if (menuLevel == 9) mono12.drawString(menuText9, 50,50);
    }
    
    
    frames ++;
    framesDiff ++;
    isNewFrameDiffAnt = isNewFrameInc;
    isNewFrameInc = 0;
    
    
    
    char texto[256];
    //string texto;
    sprintf(texto,"numero de frames: %li;  numero de imagenes: %li;  diferencia: %5li", frames, isNewFrame, isNewFrameDiffAnt-framesDiffAnt);
    mono12.drawString(texto,500,50);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // Image control a: showTermo; s: showVisible
    if (key == 'a')
        thermoImageOffTime = ofGetElapsedTimeMillis()+thermoImageTime*1000;
    
    
    
    
    if (menuLevel == 1)
    {
        frames += 1000;
        // Ajuste de la posición de la pantalla visible
        if (key == 356) visibleImageCenterX--; // mueve la imagen a la izquierda
        if (key == 358) visibleImageCenterX++; // mueve la imagen a la derecha
        
        if (key == 357) visibleImageCenterY--; // sube la imagen
        if (key == 359) visibleImageCenterY++; // baja la imagen
        
        // Ajuste del tamaño de la pantalla visible
        if (key ==  43)                // reduce el tamaño de la imagen
            if (visibleImageSize<3.0) visibleImageSize += 0.01;
        if (key ==  45)                // reduce el tamaño de la imagen
            if (visibleImageSize>1.0) visibleImageSize -= 0.01;
        
        updateParameters();
        
        // Save and quit menu
        if (key == 't') menuLevel = 2;
        if (key == 's'){
            writeToSettingsFile();
            menuLevel = 0;}
        if (key == 'q') menuLevel = 9;
        
        
    }
    
    else if (menuLevel == 2)
    {
        // Ajuste de la posición de la imagen térmica
        if (key == 357) thermoImageCenterY--; // sube la imagen
        if (key == 359) thermoImageCenterY++; // baja la imagen
        
        // Ajuste del ttiempo de exposicion de la imagen termografica
        if (key ==  43)                // reduce el tamaño de la imagen
            if (thermoImageTime<59) thermoImageTime ++;
        if (key ==  45)                // reduce el tamaño de la imagen
            if (thermoImageTime> 6) thermoImageTime --;
        
        updateParameters();
        
        // Save and quit menu
        if (key == 'v') menuLevel = 1;
        if (key == 's'){
            writeToSettingsFile();
            menuLevel = 0;}
        if (key == 'q') menuLevel = 9;
    }
    
    else if (menuLevel == 9)
    {
        if (key == 's'){
            writeToSettingsFile();
            menuLevel = 0;
        }
        if (key == 'n')
        {
            menuLevel = 0;
            managePreviousSettings(1);
        }
    }
    
    
    
    if ((key == 'm') and (menuLevel==0)) menuLevel = 1; // Show Menu
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::updateParameters(){
    
    //Updates the image size and position parameters
    visibleImageWidth  = (int) (visibleImageSize*ofGetWidth());
    visibleImageHeight = (int) (visibleImageSize*ofGetWidth()*visibleImageAspect);
    visibleImagePositionX = visibleImageCenterX - visibleImageWidth/2;
    visibleImagePositionY = visibleImageCenterY - visibleImageHeight/2;
    
    thermoImagePositionY = thermoImageCenterY - thermoImageHeight/2;
    
    
    menuText0      = "|--------------------------------------------------|\n";
    menuText0.append("|      AJUSTES   DE   LA   APLICACION      m       |\n");
    menuText0.append("|--------------------------------------------------|\n");
    
    
    sprintf(parameterText1,"|  Posicion de la imagen  (%4i, %4i)             |\n",visibleImageCenterX-(int) ofGetWidth()/2,
            (int) ofGetHeight()/2-visibleImageCenterY);
    sprintf(parameterText2,"|  Tamano de la imagen: %4i %%                     |\n",int(100*visibleImageSize));
    
    menuText1      = "|--------------------------------------------------|\n";
    menuText1.append("|            AJUSTES   IMAGEN   VISIBLE            |\n");
    menuText1.append("|--------------------------------------------------|\n");
    menuText1.append(parameterText1);
    menuText1.append("|              subir:         flecha up            |\n");
    menuText1.append("|              bajar:         flecha down          |\n");
    menuText1.append("|              izquierda      flecha left          |\n");
    menuText1.append("|              derecha        flecha right         |\n");
    menuText1.append("|                                                  |\n");
    menuText1.append(parameterText2);
    menuText1.append("|              mas grande     tecla +              |\n");
    menuText1.append("|              mas pequeno    tecla -              |\n");
    menuText1.append("|--------------------------------------------------|\n");
    menuText1.append("|              |            |                      |\n");
    menuText1.append("|  s: guardar  |  q: salir  |   t: ajustes termo   |\n");
    menuText1.append("|              |            |                      |\n");
    menuText1.append("|--------------------------------------------------|\n");
    
    
    sprintf(parameterText1,"|  Posicion de la imagen  (   0, %4i)             |\n",
            (int) ofGetHeight()/2-thermoImageCenterY);
    sprintf(parameterText2,"|  Tiempo de la imagen termografica: %4i          |\n",int(thermoImageTime));
    
    menuText2      = "|--------------------------------------------------|\n";
    menuText2.append("|         AJUSTES   IMAGEN   TERMOGRAFICA          |\n");
    menuText2.append("|--------------------------------------------------|\n");
    menuText2.append(parameterText1);
    menuText2.append("|              subir:         flecha up            |\n");
    menuText2.append("|              bajar:         flecha down          |\n");
    menuText2.append("|                                                  |\n");
    menuText2.append(parameterText2);
    menuText2.append("|              mas tiempo     tecla +              |\n");
    menuText2.append("|              memos tiempo   tecla -              |\n");
    menuText2.append("|--------------------------------------------------|\n");
    menuText2.append("|              |            |                      |\n");
    menuText2.append("|  s: guardar  |  q: salir  |  v: ajustes visible  |\n");
    menuText2.append("|              |            |                      |\n");
    menuText2.append("|--------------------------------------------------|\n");
    
    
    menuText9      = "|--------------------------------------------------|\n";
    menuText9.append("|     GUARDAR   CAMBIOS   EN   LOS   AJUSTES       |\n");
    menuText9.append("|--------------------------------------------------|\n");
    menuText9.append("|                       |                          |\n");
    menuText9.append("|   s: guardar ajustes  |  n: descartar cambios    |\n");
    menuText9.append("|                       |                          |\n");
    menuText9.append("|--------------------------------------------------|\n");
    
    
}

//--------------------------------------------------------------
void ofApp::writeToSettingsFile(){
    
    ofFile settingsFile;
    ofBuffer buff;
    
    updateParameters();
    
    
    settingsFile.open(ofToDataPath("settings.txt"), ofFile::ReadWrite);
    if (settingsFile.is_open()) settingsFile.clear();
    else settingsFile.create();
    
    buff = "";
    buff.append("visibleImageCenterX  : ");
    buff.append(ofToString(visibleImageCenterX));
    buff.append("; \n");
    
    buff.append("visibleImageCenterY  : ");
    buff.append(ofToString(visibleImageCenterY));
    buff.append("; \n");
    
    buff.append("visibleImageSize (%) : ");
    buff.append(ofToString(int(visibleImageSize*100)));
    buff.append("; \n");
    
    buff.append("thermoImageCenterY   : ");
    buff.append(ofToString(thermoImageCenterY));
    buff.append("; \n");
    
    buff.append("thermoImageTime      : ");
    buff.append(ofToString(thermoImageTime));
    buff.append("; \n");
    
    ofBufferToFile(settingsFile.getAbsolutePath(), buff);
    settingsFile.close();
    
}

//--------------------------------------------------------------
void ofApp::managePreviousSettings(int function){
    
    // function = 0 copy the data in previous variables
    // funciton = 1 recover the previous variables
    
    if (function ==0)
    {
        visibleImageCenterX_prev = visibleImageCenterX;
        visibleImageCenterY_prev = visibleImageCenterY;
        visibleImageSize_prev    = visibleImageSize;
        
        thermoImageCenterY_prev  = thermoImageCenterY;
        thermoImageTime_prev     = thermoImageTime;
    }
    
    else if (function == 1)
    {
        visibleImageCenterX = visibleImageCenterX_prev;
        visibleImageCenterY = visibleImageCenterY_prev;
        visibleImageSize    = visibleImageSize_prev;
        
        thermoImageCenterY = thermoImageCenterY_prev;
        thermoImageTime    = thermoImageTime_prev;
        
        updateParameters();
    }
}

