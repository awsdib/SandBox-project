#pragma once
#include "ofMain.h"


class UserPointer {
public:
	
	UserPointer(ofVec2f projectorCoord);
	UserPointer(float x, float y);
	void draw();	
	ofVec2f projectorCoord;
	float posX;
	float posY;


};
