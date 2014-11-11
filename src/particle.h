#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class particle
{
    public:
        ofVec2f pos;
        ofVec2f vel;
        ofVec2f frc;   // frc is also know as acceleration (newton says "f=ma")
			
        void resetForce();
		void addForce(float x, float y);
		void addRepulsionForce(float x, float y, float radius, float scale);
		void addAttractionForce(float x, float y, float radius, float scale);
		void addRepulsionForce(particle &p, float radius, float scale);
		void addAttractionForce(particle &p, float radius, float scale);
		
		void addDampingForce();
        
		void setup(float px, float py, float vx, float vy, float r);
        void update();
        void draw();
	
		void bounceOffWalls();
	
	
		float damping;
    
    float radius;
    string letter;
    ofColor col;
    float uniqueVal;


    protected:
    private:
};

#endif // PARTICLE_H
