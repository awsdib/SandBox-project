/***********************************************************************
ofApp.h - main openframeworks app
Copyright (c) 2016 Thomas Wolf

This file is part of the Magic Sand.

The Magic Sand is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Magic Sand is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Augmented Reality Sandbox; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
***********************************************************************/

#pragma once


#include "ofxDatGui.h"
#include "KinectProjector/KinectProjector.h"
#include "SandSurfaceRenderer/SandSurfaceRenderer.h"
#include "vehicle.h"
#include "Trace.h"
#include "Stopwatch.h"


//This Class controls the interaction between agents (e.g. traces, fire, house, timer)

using Poco::Stopwatch;
class ofApp : public ofBaseApp {

public:

	// Stopwath corresponds to the timer that is shown on the screen
	Stopwatch stopwatch;
	ofTrueTypeFont myFont;
	// These variable correspond to the information that is shown on the screen
	float velocityInfo;
	float distInfo;

	// Starting position of the pointer
	float pointerX;
	float pointerY;
	// Starting position of the house
	float houseX;
	float houseY;
	// By default fire particles have not been started and the house has not been reached by fire
	bool startFire = false;
	bool reachHouse = false;

	// These methods generate and draw different type of agents
	void addNewFlame();
	void addNewPointer();
	void addNewHouse();
	void resetParticles();
	void drawStopWatch();
	void resetreachHouse();

	// Sets the location of vehicles
	bool setRandomVehicleLocation(ofRectangle area, ofVec2f & location);
	void update();
	void setup();
	void draw();
	void drawProjWindow(ofEventArgs& args);
	void drawVehicles();

	// Conter that is used to keep track of the number of executions of the update method
	int ofAppCounter;
	// Starting number of flames, this attribute is controlled by the user with a slider
	int numberOfFlames;
	// Starting fire intensity
	int fireIntensity;

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void setupGui();
	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	std::shared_ptr<ofAppBaseWindow> projWindow;


private:
	std::shared_ptr<KinectProjector> kinectProjector;
	SandSurfaceRenderer* sandSurfaceRenderer;
	
	// Projector and kinect variables
	ofVec2f projRes;
	ofVec2f kinectRes;
	ofRectangle kinectROI;
	
	// FBos
	ofFbo fboVehicles;

	// These vector store multiples agents (i.e. flame, house, trace)
	vector < Trace > myTraces;	
	vector <House> houses;
	vector <FlameParticle> flames;
	vector <UserPointer> userPointers;

	// GUI	Attributes
	bool waitingToInitialiseVehicles;
	vector <UserPointer> myPointers;		
	ofxDatGui* gui;
	float widthSand;
	float heightSand;
	float pointSeekHeight;
	float pointSeekHeightTemp;
	bool northSouth = false; 
	
};
