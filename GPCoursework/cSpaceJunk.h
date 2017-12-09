/*
=================
cSpaceJunk.h
- Header file for class definition - SPECIFICATION
- Header file for the SpaceJunk class which is a child of cSprite class
=================
*/
#ifndef _CSPACEJUNK_H
#define _CSPACEJUNK_H
#include "cSprite.h"

class cSpaceJunk : public cSprite
{
private:
	SDL_Point spaceJunkVelocity;

public:
	cSpaceJunk();
	bool isPlaying;
	void update(double deltaTime);		// OrbitalTrashCan update method
	void setSpaceJunkVelocity(SDL_Point spaceJunkVel);   // Sets the velocity for the spacejunk
	SDL_Point getSpaceJunkVelocity();				 // Gets the space junk velocity
};
#endif