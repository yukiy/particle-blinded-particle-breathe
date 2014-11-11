#include "testApp.h"

using namespace ofxCv;

// comparison routine for sort...
//bool comparisonFunction(  particle * a, particle * b ) {
bool comparisonFunction(  particle  a, particle  b ) {
//    return a->pos.x < b->pos.x;
    return a.pos.x < b.pos.x;
}


void testApp::setup() {
    ofEnableAlphaBlending();
	cam.initGrabber(640, 480);
	tracker.setup();
    drawFaceOutline = false;

    for (int i = 0; i < 500; i++){
        particle myParticle;
        myParticle.setup(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), 0, 0, ofRandom(1,10));
        myParticle.col = ofColor(255,250,50);
        particles.push_back(myParticle);
    }
    bRepel		= true;
    radius		= 40;
    strength	= 0.5f;
}

void testApp::update() {
	cam.update();

    if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}

    mouthOpen = tracker.getGesture(ofxFaceTracker::JAW_OPENNESS);
    ofPoint mouth = tracker.getImagePoint(61);
    ofPoint rEyePos = tracker.getImagePoint(36);
    ofPoint lEyePos = tracker.getImagePoint(45);
    ofPoint nose = tracker.getImagePoint(27);
    ofPoint nose2 = tracker.getImagePoint(28);
    ofPoint nose3 = tracker.getImagePoint(29);
    ofPoint nose4 = tracker.getImagePoint(30);
    
    particle lEye;
    lEye.setup(lEyePos.x, lEyePos.y, 0, 0, 1);
    particle rEye;
    rEye.setup(rEyePos.x, rEyePos.y, 0, 0, 1);
    
    
    //---french methods---------
    sort( particles.begin(), particles.end(), comparisonFunction );

    for (int i = 0; i < particles.size(); i++){
    
        particles[i].resetForce();

        for (int j = i-1; j >= 0; j--){
            if ( fabs(particles[j].pos.x - particles[i].pos.x) > 10) break;
            particles[i].addRepulsionForce(particles[j], 50, 0.3);
            particles[i].addAttractionForce(lEye, 1000, 0.1);
            particles[i].addAttractionForce(rEye, 1000, 0.1);
            particles[i].addRepulsionForce(mouth.x, mouth.y, 100, 1);
            particles[i].addRepulsionForce(nose.x, nose.y, 30, 1);
            particles[i].addRepulsionForce(nose2.x, nose2.y, 30, 1);
            particles[i].addRepulsionForce(nose3.x, nose3.y, 30, 1);
            particles[i].addRepulsionForce(nose4.x, nose4.y, 30, 1);
        }

        for(int j=0; j<mouthParticles.size(); j++){
            particles[i].addRepulsionForce(mouthParticles[j].pos.x, mouthParticles[j].pos.y, 100, 10);
        }

    }

//    //---conventional method----
//    for (int i = 0; i < particles.size(); i++){
//        particles[i].resetForce();
//        particles[i].addRepulsionForce(mouthPoint.x, mouthPoint.y, 100, 1.4);
//
//        for(int j=0; j<mouthParticles.size(); j++){
//            particles[i].addRepulsionForce(mouthParticles[j].pos.x, mouthParticles[j].pos.y, 100, 10);
//        }
//
//        
//        for (int j = 0; j < i; j++){
//            particles[i].addRepulsionForce(particles[j], 50, 0.4);
////            particles[i].addAttractionForce(particles[j], 500, 0.005);
//            particles[i].addAttractionForce(lEye, 500, 0.02);
//            particles[i].addAttractionForce(rEye, 500, 0.02);
//        }
//        
//        //for (int j = 0; j < i; j++){
//        //			if (bRepel){
//        //				particles[i].addRepulsionForce(particles[j], radius, strength);
//        //			} else {
//        //				particles[i].addAttractionForce(particles[j], radius, strength);
//        //			}
//        //		}
//    }
    

    
    for (int i = 0; i < particles.size(); i++){
        particles[i].addDampingForce();
        particles[i].update();
    }
    
    
    //--- mouth particles
    //--- if mouth is bigger than (threshold:24) add particles
    if(mouthOpen > 24){
        if(mouthParticles.size() > 30){
            mouthParticles.erase(mouthParticles.begin());
        }
        particle myParticle;
        myParticle.setup(mouth.x, mouth.y, 0, 0, 20);
        myParticle.damping = ofRandom(0.01, 0.05);
        myParticle.col = ofColor(240,240,240);
        mouthParticles.push_back(myParticle);
    }
    
    //--- update mouth particles
    for (int i = 0; i < mouthParticles.size(); i++){
        mouthParticles[i].resetForce();
        mouthParticles[i].addDampingForce();
        mouthParticles[i].addRepulsionForce(mouth.x, mouth.y, 300, 3);
        for (int j = 0; j < i; j++){
            mouthParticles[i].addRepulsionForce(mouthParticles[j], 50, 3);
        }
        mouthParticles[i].update();
    }
}



void testApp::draw() {
    ofSetColor(255, 255, 255, 255);
    cam.draw(0, 0);
	ofSetLineWidth(2);

    ofSetColor(255, 255, 255, 200);
    if(drawFaceOutline){
        tracker.draw();
    }
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    

    for (int i = 0; i < particles.size(); i++){
        particles[i].draw();
    }


    ofSetColor(255, 255, 255, 100);
    for(int i=0; i<mouthParticles.size(); i++){
        mouthParticles[i].draw();
    }

}


void testApp::keyPressed(int key) {
	if(key == 'f') {
		tracker.reset();
	}

    switch (key){
            
        case ' ':
            // reposition everything:
            for (int i = 0; i < particles.size(); i++){
                particles[i].setup(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0, 5);
            }
            break;
        case 'a':
            strength += 0.02f;
            break;
        case 's':
            strength -= 0.02f;
            if (strength <= 0) strength = 0.02f;
            break;
        case 'z':
            radius += 5.0f;
            break;
        case 'x':
            radius -= 5.0f;
            if (radius <= 0) radius = 5.0f;
            break;
        case 'r':
            bRepel = !bRepel;
            break;
            
        case 'o':
            if(drawFaceOutline){
                drawFaceOutline = false;
            }else{
                drawFaceOutline = true;
            }
    }

}



/*
 LEFT_JAW
 RIGHT_JAW
 JAW
 LEFT_EYEBROW
 RIGHT_EYEBROW
 LEFT_EYE
 RIGHT_EYE
 OUTER_MOUTH
 INNER_MOUTH
 NOSE_BRIDGE
 NOSE_BASE
 FACE_OUTLINE
 */
/*
 ofVec2f position = tracker.getPosition();
 float scale = tracker.getScale();
 ofVec3f orientation = tracker.getOrientation();
 float mouthW = tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH);
 float mouthH = tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT);
 */

