/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSpaceJunk.h"
#include "cGame.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cSpaceJunk::cSpaceJunk() : cSprite()
{
	this->spaceJunkVelocity = { 50, 50 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cSpaceJunk::update(double deltaTime)
{
	


		this->setSpriteRotAngle(this->getSpriteRotAngle() + (2.0f * deltaTime));
		if (this->getSpriteRotAngle() > 360)
		{
			this->setSpriteRotAngle(this->getSpriteRotAngle() - 360);
		}

		SDL_Rect currentSpritePos = this->getSpritePos();
		currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
		currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

		this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
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
