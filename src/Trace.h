

#pragma once
#include "ofMain.h"


class Trace {
	// Traces are static objects that depict the trajectory traveled by the flame particles
	// They are created when a flame agent moves
	// Their intensity depends on the vegetation type

public:	
	int size;
	int intensity;
	int intensityFactor;
	int counter;
	Trace(ofVec2f projectorCoord, int i, int mySize, string v);
	void update();
	void draw();
	float posX;
	float posY;
	int radius;
	string vegetationType;
	ofVec2f projectorCoord;
};
