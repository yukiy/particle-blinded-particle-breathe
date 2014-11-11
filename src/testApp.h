#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "particle.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;

    float mouthOpen;
    
    vector <particle> mouthParticles;

    vector <particle> particles;
    bool	bRepel;
    float	radius;
    float	strength;
    
    bool drawFaceOutline;
};
