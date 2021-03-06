/*
=================
cSpaceJunk.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSpaceJunk.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cSpaceJunk::cSpaceJunk() : cSprite()
{
	this->spaceJunkVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cSpaceJunk::update(double deltaTime)
{
	


		this->setSpriteRotAngle(this->getSpriteRotAngle() + (20.0f * deltaTime));
		if (this->getSpriteRotAngle() > 360)
		{
			this->setSpriteRotAngle(this->getSpriteRotAngle() - 360);
		}


		SDL_Rect currentSpritePos = this->getSpritePos();
		currentSpritePos.x += this->getSpriteTranslation().x /25;// * deltaTime; For some reason deltaTime fucks it
		currentSpritePos.y += this->getSpriteTranslation().y /25;// * deltaTime;

		this->setSpritePos({ currentSpritePos.x, currentSpritePos.y});
		//cout << "Asteroid position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
		this->setBoundingRect(this->getSpritePos());
	
	
}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cSpaceJunk::setSpaceJunkVelocity(SDL_Point SpaceJunkVel)
{
	spaceJunkVelocity = SpaceJunkVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
SDL_Point cSpaceJunk::getSpaceJunkVelocity()
{
	return spaceJunkVelocity;
}
