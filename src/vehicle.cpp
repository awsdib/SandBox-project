#include "vehicle.h"

Vehicle::Vehicle(std::shared_ptr<KinectProjector> const& k, ofPoint slocation, ofRectangle sborders) {
    // Vehicle construnctor receives as input the location of the agent and the sand table region that is retrieved by the kinnect
	kinectProjector = k;   
    location = slocation;
    borders = sborders;
}

void FlameParticle::setIgnitionTime(int myTime)
{
	// Sets the ignition of the flame particle
	ignitionTime = myTime;
}

void FlameParticle::slopeEffect()
{
	// Predicts the location of the agent 50 time steps ahead so as to calculate the slope with respect with the current location
	ofPoint distanceVector;
	if (timeStepCounter % 50 == 0) {
		// Computing the slope generated from my current position and the posision 50 time steps ahead
		futureLocation = location + vel * 50;
		elevation = kinectProjector->elevationAtKinectCoord(location.x, location.y);
		futureElevation = kinectProjector->elevationAtKinectCoord(futureLocation.x, futureLocation.y);
		deltaElevation = futureElevation - elevation;
		distanceVector = futureLocation - location;
		dist = distanceVector.length();
		slope = (deltaElevation / dist) * 100;
		//  The fire will increase its speed as it travels uphill by two times for a 10 degree slope this behaviour is captured in the factor variable
		factor = abs(slope * 2 / 20);
		if (deltaElevation != 0 && dist != 0) {
			// If the slope is negative (downhill) the speed decreases, hence we return 1/factor
			if (slope < 0) {
				slopeFactor = 1 / factor;
			}
			else {
				slopeFactor = factor;
			}
		}

	}
}

void FlameParticle::windEffect(bool direction)
{
	//Adding  Wind effect
	// This function defines the direction of the particles, on top of that is adds a wiggling effect so that the particle movement looks more realistic
	
	//North South Wind Effect
	if (direction) {
		
		float windEffectX = ofSignedNoise(location.x * 0.003, location.y * 0.006, ofGetElapsedTimef() * 0.6);
		friction.x = windEffectX * 0.25 + ofSignedNoise(uniqueVal, location.y * 0.04) * 0.6;
		friction.y = ofSignedNoise(uniqueVal, location.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;
	}
	//East West Wind Effect
	else {		
		float windEffectY = ofSignedNoise(location.y * 0.003, location.x * 0.006, ofGetElapsedTimef() * 0.6);
		friction.x = ofSignedNoise(uniqueVal, location.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;
		friction.y = windEffectY * 0.25 + ofSignedNoise(uniqueVal, location.y * 0.04) * 0.6;
	}
}

void FlameParticle::particleState()
{
	// This method changes the alive state . Each flame particle dies for the following reasons:
	// Is located inside a water body
	// Location outside the sand box
	// Ignition time equals to zero
	int currentElevation = kinectProjector->elevationAtKinectCoord(location.x, location.y);

	if (currentElevation < 0) {
		alive = false;
	}
	else if (location.x > 560) {
		alive = false;
	}
	else if (location.y > 380) {
		alive = false;
	}

	else if (location.y > maxY && northSouth == false) {
		alive = false;
	}
	else if (location.y < minY  && northSouth == false) {
		alive = false;
	}

	else if (ignitionTime == 0) {
		alive = false;
	}

}

void FlameParticle::controlSpeedOutliers()
{
	// The porpuse of this method is to control tha maximum and minimum velocity of the flame particle
	// Without this function the code might crash because of extreme velocity outliers
	if (abs(vel.x) > maxVX) {
		if (vel.x < 0)
			vel.x = maxVX*-1;
		else
			vel.x = maxVX;
	}

	if (abs(vel.y) > maxVY) {
		if (vel.y < 0)
			vel.y = maxVY*-1;
		else
			vel.y = maxVY;
	}

	if (abs(vel.x) < minVX) {
		if (vel.x < 0)
			vel.x = minVX*-1;
		else
			vel.x = minVX;
	}

	if (abs(vel.y) < minVY) {
		if (vel.y < 0)
			vel.y = minVY*-1;
		else
			vel.y = minVY;
	}

}

void FlameParticle::setup()
{
	// In this function I define the flame state
	//The vegetation type parameter changes according to the vegetation type, the values were extracted from the literature	
	
	if (vegetationType == "Dense Forest.xml") {
		// Value for dense forest
		vegetationFactor = 0.4;
	}
	else {
		// Value for long grass
		vegetationFactor = 0.6;
	}
	
	// Maximum and minimum velocity of the flame particles, this values were determined by trial and error
	maxVX = 1.3;
	maxVY = 1.3;
	minVX = 0.05;
	minVY = 0.05;	
	// Maximum and minimum fire width on the y axis of the sand box table
	maxY = location.y + 70;
	minY = location.y - 70;
	// Random constants whose porpuse is to make each particle unique
	uniqueVal = ofRandom(-10000, 10000);
	fireSpeedConstant = 0.03;
	scale = ofRandom(0.5, 1.0);
	size = 10 * scale;
	drag = ofRandom(0.97, 0.99);
	vel.x = fabs(vel.x) * 2.0;


	//Starting Velocity
	vel.x = 0;
	vel.y = 0;
	// By default the particles are alive
	alive = true;
	slopeFactor = 1;
	friction = ofPoint(0, 0, 0);
	timeStepCounter = 0;	
	image.load("flamePointer.png");

}

void FlameParticle::applyBehaviours()
{	
	// This method calls all the functions that change the state of the flame agent
	slopeEffect();
	windEffect(northSouth);
	particleState();
	controlSpeedOutliers();
}

void FlameParticle::update (bool northSouth)
{
	// Maps agent coordinates to kinnect coordinates
	projectorCoordFire = kinectProjector->kinectCoordToProjCoord(location.x, location.y);
	vel *= drag;
	if (timeStepCounter % 50 == 0 ) {
		// The velocity changes according to the current slope factor and the vegetation type
		// Since the slope factor is updated every 50 times steps this statement is executed once every 50 time steps.
		vel += friction * fireSpeedConstant * slopeFactor*vegetationFactor;
	}
	else {
		// If we have not completed 50 time steps we only apply the friction caused by the wind and the vegetation factor to the agent velocity
		vel += friction * fireSpeedConstant*vegetationFactor;
	}	
	if (alive) {
		// Updates the location according to the current velocity
		location += vel;
	}
	// In each time step the particle's ignition decreases by one. At some point this value is zero as a result the particle dies
	ignitionTime--;
	timeStepCounter++;
}

void FlameParticle::draw()
{
	// Draws particle
	ofPushMatrix();
	ofTranslate(projectorCoordFire);	
	ofSetColor(255, 255, 255);
	image.draw(-10, -10, 20, 20);	
	ofPopMatrix();

}

void FlameParticle::setDirection(bool d)
{
	// This method sets the wind direction
	northSouth = d;
}

void UserPointer::setup()
{
	// Defining starting position of the pointer
	location.x = 250;
	location.y = 250;
	projectorCoordPointer = kinectProjector->kinectCoordToProjCoord(location.x, location.y);
	cout << "A pointer has been created" << endl;
	userPointer.loadImage("Pointer.png");
}

void UserPointer::updatePointer(float x, float y)
{
	// Moves the pointer
	location.x = x;
	location.y = y;
	projectorCoordPointer = kinectProjector->kinectCoordToProjCoord(location.x, location.y);
}


void UserPointer::drawPointer()
{
	// Draws pointer
	ofPushMatrix();
	ofTranslate(projectorCoordPointer);
	ofSetColor(255, 255, 255);
	userPointer.draw(-7, -7, 15, 15);
	ofPopMatrix();
}

void House::setup()
{
	// Defining  house values, in the first time step we asume that the house is not burned
	burned = false	;
	location.x = 250;
	location.y = 250;
	projectorCoordHouse = kinectProjector->kinectCoordToProjCoord(location.x, location.y);
	house.loadImage("house2.png");
	cout << "A new house has been created" << endl;

}

void House::applyBehaviour(vector<FlameParticle> flames)
{
	// This method determines whether the house has been reached by fire
	for (auto & f : flames) {
		// We loop through each of the fire particles and calculate their distance with respect to the house, if the distance is below a certain threhold then the fire burns.
		ofPoint flameCoordinate = f.getLocation();
		ofPoint houseCoordinate = location;
		if (flameCoordinate.x >= houseCoordinate.x && flameCoordinate.x <= houseCoordinate.x + 40 &&
			flameCoordinate.y >= houseCoordinate.y && flameCoordinate.y <= houseCoordinate.y + 40) {
			burned = true;
			ofPoint flameCoordinate = f.getLocation();
			ofPoint deltaDist = f.startingLocation - flameCoordinate;
			dist = deltaDist.length();
		}
	}
}

void House::drawHouse()
{
	// Draws the house
	ofPushMatrix();
	ofTranslate(projectorCoordHouse);
	if (burned) {
		// If the house is burned we set their color to red
		ofSetColor(255, 0, 0);		
	}
	else {
		ofSetColor(255, 255, 255);
	}	
	house.draw(0, 0,30,40);
	ofPopMatrix();

}

void House::updateHouse(float x, float y)
{
	// Changes house position
	location.x = x;
	location.y = y;
	projectorCoordHouse = kinectProjector->kinectCoordToProjCoord(location.x, location.y);
}

