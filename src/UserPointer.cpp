#include "UserPointer.h"


UserPointer::UserPointer(ofVec2f p)
{
	projectorCoord = p;
}

UserPointer::UserPointer(float X, float Y)
{
	posX = X;
	posY = Y;
	projectorCoord.x = posX;
	projectorCoord.y = posY;
}


void UserPointer::draw()
{
	ofPushMatrix();
	ofTranslate(projectorCoord);
		

	ofSetColor(255, 0, 255);

	ofCircle(0, 0, 15);
	ofPopMatrix();

}