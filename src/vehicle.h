/***********************************************************************
vehicle.h - vehicle class (fish & rabbits moving in the sandbox)
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
Hola
***********************************************************************/

#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Trace.h"

#include "KinectProjector/KinectProjector.h"


class Vehicle{

public:
    Vehicle(std::shared_ptr<KinectProjector> const& k, ofPoint slocation, ofRectangle sborders);
    
    // Agent Coordinates		
	ofVec2f projectorCoord;
	ofVec2f projectorCoordFire;
	ofVec2f projectorCoordPointer;
	ofVec2f projectorCoordHouse;

    virtual void setup() = 0;
    
    const ofPoint& getLocation() const {
        return location;
    }

	ofImage image;
  
    
protected:
	//added
	vector <Trace> myTraces;    
    std::shared_ptr<KinectProjector> kinectProjector;
    ofPoint location;    
    ofRectangle borders;    

};



class FlameParticle : public Vehicle {

// This class represents the flame agents
public:
	FlameParticle(std::shared_ptr<KinectProjector> const& k, ofPoint slocation, ofRectangle sborders) : Vehicle(k, slocation, sborders) {}
	// These methods control the flames behaviour specially their speed
	void slopeEffect();
	void windEffect(bool direction);
	void particleState();
	void controlSpeedOutliers();
	ofPoint startingLocation;
	
	// These variables control the maximum and minimum speed and coordinates of the agents
	float maxVX;
	float maxVY;
	float minVX;
	float minVY;
	float maxY;
	float minY;
	
	void setup();
	void applyBehaviours();

	// The name of the variables are self explanatory
	bool  alive;
	int ignitionTime;
	void setIgnitionTime(int myTime);
	int timeStepCounter;	
	bool northSouth = false;
	string vegetationType;
	float vegetationFactor;
	int elevation;
	int futureElevation;
	void update(bool northSouth);
	void draw();
	int deltaElevation;

	ofPoint vel;
	// Friction force caused by wind and vegetation type
	ofPoint friction;
	ofPoint futureLocation;

	// These variables control the slope effect in the speed
	float slope;
	float dist;
	float factor = 1;
	float slopeFactor = 1;
	float fireSpeedConstant;

	// These attributes are used to provide a random factor to each of the flame particles
	float drag;
	float uniqueVal;
	
	// Scale to transtorm sanbox units to real world units
	float scale;
	//Size of the particle
	int size;
	
	void setDirection(bool d);

};



class UserPointer : public Vehicle {
// This class represents the pointer that the user can move in order to selecct the starting location of the flame agents
public:
	UserPointer(std::shared_ptr<KinectProjector> const& k, ofPoint slocation, ofRectangle sborders) : Vehicle(k, slocation, sborders) {}

	void drawPointer();

	void setup();
	
	void updatePointer(float x, float y);

	int size;

	ofImage userPointer;

};


class House : public Vehicle {
// Class that depicts the object of interest, by default we decided to set a house
public:
	House(std::shared_ptr<KinectProjector> const& k, ofPoint slocation, ofRectangle sborders) : Vehicle(k, slocation, sborders) {}

	void drawHouse();

	void setup();

	void applyBehaviour(vector <FlameParticle> flames);

	void updateHouse(float x, float y);

	ofImage house;

	// If the house is reached  by a flame agent the attribute burned becomes true

	bool burned;
	
	float dist;

};






