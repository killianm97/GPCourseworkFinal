/*
=================
cSpaceShip.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CSPACESHIP_H
#define _CSPACESHIP_H
#include "cSprite.h"

class cSpaceShip : public cSprite
{
private:
	SDL_Point spaceShipVelocity;

public:
	cSpaceShip();
	void update(double deltaTime);		// Spaceship update method
	void setSpaceShipVelocity(SDL_Point spaceShipVel);   // Sets the velocity for the spaceship
	SDL_Point getSpaceShipVelocity();				 // Gets the spaceship's velocity
	float circleT;
};
#endif