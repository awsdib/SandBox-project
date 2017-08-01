/***********************************************************************
ofApp.cpp - main openframeworks app
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

#include "ofApp.h"
#include <list>
void ofApp::setup() {
	
	//Initial values of the pointer, house and pointer
	pointerX = 250;
	pointerY = 200;
	widthSand = 250;
	heightSand = 200;
	houseX = 203;
	houseY = 145;
	myFont.load("iron.ttf", 30);
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetLogLevel("ofThread", OF_LOG_WARNING);

	// Setup kinectProjector
	kinectProjector = std::make_shared<KinectProjector>(projWindow);
	kinectProjector->setup(true);
	
	// Setup sandSurfaceRenderer
	sandSurfaceRenderer = new SandSurfaceRenderer(kinectProjector, projWindow);
	sandSurfaceRenderer->setup(true);
	
	// Retrieve variables
	kinectRes = kinectProjector->getKinectRes();
	projRes = ofVec2f(projWindow->getWidth(), projWindow->getHeight());
	kinectROI = kinectProjector->getKinectROI();	
	fboVehicles.allocate(projRes.x, projRes.y, GL_RGBA);
	fboVehicles.begin();
	fboVehicles.end();	
	setupGui();	
	ofSetVerticalSync(true);
	
	//By default the counter start at 0 and set 10 flame agents
	ofAppCounter = 0;
	numberOfFlames = 10;
	addNewPointer();
	addNewHouse();
	fireIntensity = 255;

}


void ofApp::drawStopWatch()
{
	//This method draws the timer and information regarding the average speed and distance traveled by the flame agents
	ofVec2f location;
	string  info = "" + ofToString(stopwatch.elapsedSeconds()) + "";
	myFont.drawString("Time", 20, 350);
	myFont.drawString("Hours", 90, 400);
	myFont.drawString(info, 50, 400);
	if (reachHouse == true) {
		std::ostringstream ss;
		std::ostringstream sd;
		ss << velocityInfo;
		sd << distInfo;
		string velocityInfo(ss.str());
		string distInfo(sd.str());
		myFont.drawString("Particule Velocity", 20, 450);
		myFont.drawString(velocityInfo, 20, 500);
		myFont.drawString("m h", 175, 500);
		myFont.drawString("Particule Distance", 20, 550);
		myFont.drawString("m", 175, 585);
		myFont.drawString(distInfo, 20, 585);
	}
}


void ofApp::resetreachHouse()
{
	// Resets the house attributes
	ofVec2f location;
	auto r = House(kinectProjector, location, kinectROI);
	r.burned = false;
	reachHouse = false;
}

void ofApp::addNewFlame()
{
	// Creates a new flame agent
	ofVec2f location;
	setRandomVehicleLocation(kinectROI, location);
	auto r = FlameParticle(kinectProjector, location, kinectROI);
	r.setup();
	r.setDirection(northSouth);
	flames.push_back(r);
}



void ofApp::addNewPointer()
{	
	// Creates a new pointer, which is usefull for defining the starting point of the fire
	ofVec2f location;
	setRandomVehicleLocation(kinectROI, location);
	auto r = UserPointer(kinectProjector, location, kinectROI);
	r.setup();
	userPointers.push_back(r);
}


void ofApp::addNewHouse()
{	
	// Creates a new house
	ofVec2f location;
	setRandomVehicleLocation(kinectROI, location);
	auto r = House(kinectProjector, location, kinectROI);
	r.setup();
	houses.push_back(r);
}


void ofApp::resetParticles()
{
	// Sets all the flame agents to their original state
	for (unsigned int i = 0; i < flames.size(); i++) {	
		flames[i].setup();
		flames[i].vegetationType = sandSurfaceRenderer->vegetationType;
		flames[i].setDirection(northSouth);
		// The ignition time of the flame agents  changes according to a linear function, hence not all of them have the same ignition as a result some particles are extinguished before
		flames[i].setIgnitionTime(i * 2 + 1000);
	}	
}



bool ofApp::setRandomVehicleLocation(ofRectangle area, ofVec2f & location) {
	// Sets the location of vehicles
	ofPoint pointSeek;
	ofPoint pointLocation;
	location = ofVec2f(widthSand, heightSand);
	pointLocation = location;
	bool okwater = false;
	return okwater;
}


void ofApp::update() {
    // This method is executed in each time step and basically controls the action of the agents (i.e. movement, and general behaviour) and draw them
	ofAppCounter++;
	kinectProjector->update();
    
	sandSurfaceRenderer->update();
    
    if (kinectProjector->isROIUpdated())
        kinectROI = kinectProjector->getKinectROI();
	
	if (kinectProjector->isImageStabilized()) {
		
		for (auto & f : flames) {
			if (startFire) {
				// Changes state of each of the flame agents				
				f.applyBehaviours();				
				// Moves flame agents
				f.update(northSouth);			
			}			
		}

		for (auto & t : myTraces) {
			// Updates traces state at the beginning they have as intense red color that days over time
			t.update();
		}
		for (auto & u : userPointers) {
			// Moves the pointer position
			u.updatePointer(pointerX, pointerY);
		}


		for (auto & h : houses) {
			// We apply this method every 5 time steps in order to enhance the code efficiency
			if (ofAppCounter % 5 == 0) {
				// Every 5 time steps checks whether the house has been reached by fire
				h.applyBehaviour(flames);
			}
			
			
			if (h.burned) {
				while (reachHouse == false)
				{
					// If the house is reached by fire we stop the timer and show the average speed and distance of the flame agents
					stopwatch.stop();
					velocityInfo = 8272.882*(0.001*(h.dist / stopwatch.elapsedSeconds()));
					distInfo = 8272.882 * (0.001*h.dist);
					reachHouse = true;
				}
				h.updateHouse(houseX, houseY);
			}
		}

	    drawVehicles();
	}
	gui->update();
}


void ofApp::draw() {
	// Draws vehicles
	sandSurfaceRenderer->drawMainWindow(300, 30, 600, 450);//400, 20, 400, 300);
	fboVehicles.draw(300, 30, 600, 450);
	kinectProjector->drawMainWindow(300, 30, 600, 450);
	drawStopWatch();
	gui->draw();
}

void ofApp::drawProjWindow(ofEventArgs &args) {
	// Draws sand
	kinectProjector->drawProjectorWindow();	
	if (!kinectProjector->isCalibrating()){
	    sandSurfaceRenderer->drawProjectorWindow();
	    fboVehicles.draw(0,0);
	}
}

void ofApp::drawVehicles()
{
    fboVehicles.begin();

	ofClear(255, 255, 255, 0);
	
	if (startFire == false) {
		// Clears all that is shown on the screen
		ofClear(255, 255, 255, 0);
	}

   
	for (auto & t : myTraces) {
		// Loops through the traces vector and draws each of their instances
		t.draw();
	}
	
	for (auto & f : flames) {
		if (f.alive) {
			// If the flame agent is alive we draw it
			f.draw();
			if (ofAppCounter % 5 == 0) {	
				// For each of the flame agents that are alive we generate a new trace. Traces are static objects that depict the trajectory traveled by the flame particles
				// Traces or embers start with high intensity (red color), which decays over time. When the intensity reaches a value of zero traces become black, this means that the area is not burning.
				Trace t = Trace(f.projectorCoordFire, fireIntensity, f.size, sandSurfaceRenderer->vegetationType);
				myTraces.push_back(t);				
			}		
		}
	}

	for (auto & u : userPointers) {
		u.drawPointer();
	}


	for (auto & h : houses) {
		h.drawHouse();
	}


    fboVehicles.end();


}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {

}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::mouseEntered(int x, int y) {

}

void ofApp::mouseExited(int x, int y) {

}

void ofApp::windowResized(int w, int h) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
	// This method creates a rectangle that contains a small pointer. The user can move this pointer so as to select the starting location of fire
	widthSand = e.x;
	heightSand = e.y;
	pointerX = e.x;
	pointerY = e.y;
	cout << "x: " << e.x << " y: " << e.y << endl;

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::setupGui(){
    // instantiate and position the gui //
    gui = new ofxDatGui();   
	// Sliders that the user can drag to change the number of particles that describe fire
    gui->addSlider("# of fire particles", 0, 100, flames.size())->setPrecision(0);
	// Sliders that the user can drag to move the house
	gui->addSlider("House X", 0, 550)->setPrecision(0);
	gui->addSlider("House Y", 0, 300)->setPrecision(0);
	ofRectangle rect(0, 0, kinectROI.getWidth(), kinectROI.getHeight());
	gui->add2dPad("x", rect);
	gui-> on2dPadEvent(this, &ofApp::on2dPadEvent);	
    // Button that restarts the scenario by removing all the particles and  traces
	gui->addButton("Restart Fire");
	// Buttom that starts fire generation
	gui->addButton("Start Fire");
    gui->addBreak();
    gui->addHeader(":: Game ::", false);
	// Toggle that enables the user to change the wind direction
	gui->addToggle("North - South Wind");
	gui->addToggle("East - West Wind");
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onToggleEvent(this, &ofApp::onToggleEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->setLabelAlignment(ofxDatGuiAlignment::CENTER);    
    gui->setPosition(ofxDatGuiAnchor::BOTTOM_RIGHT); 
	gui->setAutoDraw(false); 
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
	// Starts fire
	if (e.target->is("Start Fire")) {
		startFire = true;
		stopwatch.restart();
		stopwatch.reset();
		stopwatch.start();
	}
	// Restart fire, clears traces and flame agents
	else if (e.target->is("Restart Fire")) {
		gui->getSlider("# of fire particles")->setValue(0);
		ofClear(255, 255, 255, 0);
		myTraces.clear();
		flames.clear();
		resetParticles();
		
		stopwatch.restart();
		stopwatch.reset();
		resetreachHouse();
		for (auto &h : houses) {
			h.setup();
		}
		startFire = false;
		
		
	}
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e){
    // Sets wind direction
	if  (e.target->is("North - South Wind")) {
		northSouth = true;
		resetParticles();
	}
	else if ("East - West Wind") {
		northSouth = false;
		resetParticles();
	}
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
   // Sets the starting number of particles
	if (e.target->is("# of fire particles")) {
        if (e.value > flames.size())
            while (e.value > flames.size()){
                addNewFlame();
            }
		resetParticles();
        if (e.value < flames.size())
            while (e.value < flames.size()){
				flames.pop_back();
            }
		resetParticles();
    }
   else if (e.target->is("House X")) {
	   houseX = e.value;	
	   cout << houseX << endl;
   }
   else if (e.target->is("House Y")) {
	   houseY = e.value;
	   cout << houseY << endl;
   }
}
