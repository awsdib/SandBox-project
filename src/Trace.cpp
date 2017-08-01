#include "Trace.h"


Trace::Trace(ofVec2f p, int i, int mySize, string v)
{
	// Variable names are self explanatory
	projectorCoord = p;
	intensity = i;
	size = mySize;
	intensityFactor = 5;
	counter = 0;
	vegetationType = v;
}

void Trace::update()
{
	// If the current vegetation type is dense forest the surface will burn for a longer period of time, hence the intesity factor is 5
	
	if (vegetationType ==  "Dense Forest.xml") {
		intensityFactor = 5;
	}
	// If that is not the case we asume that fire is spreading over grass and we decrease the intensity factor, in this case the transition from red to black is faster
	else {

		intensityFactor = 2;
	}
	if (counter%intensityFactor == 0)
		intensity-=2;
	counter++;

	
}

void Trace::draw()
{
	// Draws traces
	ofPushMatrix();
	ofTranslate(projectorCoord);
	ofSetColor(intensity, 0, 0, 200);
	ofCircle(0, 0, size);
	ofPopMatrix();

}
