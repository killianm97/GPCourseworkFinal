/*
=================
cSpaceShip.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSpaceShip.h"
#include<math.h>

/*
=================================================================
Defualt Constructor
=================================================================
*/
cSpaceShip::cSpaceShip() : cSprite()
{
	this->spaceShipVelocity = { 0, 0 };
	this->circleT = -PI / 2;
}

/*
=================================================================
Update the sprite position
=================================================================
*/
void cSpaceShip::update(double deltaTime)
{
	FPoint circleCentre = {328, 448};
	float circleRadius = 320;
	
	if (circleT < 0)
	{
		circleT += PI * 2;
	}

	if (circleT > PI * 2)
	{
		circleT -= PI * 2;
	}

	float rotAngle = (circleT * (180.0f / PI) * -1.0f) - 90;

	this->setSpriteRotAngle(rotAngle);
	
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() - 360);
	}

	FPoint direction = { 0.0f, 0.0f };
	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->spaceShipVelocity.x = this->spaceShipVelocity.x + direction.X;
	this->spaceShipVelocity.y = this->spaceShipVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	
	currentSpritePos.x = cos(circleT) * circleRadius + circleCentre.X;
	currentSpritePos.y = -sin(circleT) * circleRadius + circleCentre.Y;

	this->spaceShipVelocity.x *= 0.95;
	this->spaceShipVelocity.y *= 0.95;
	
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
}

/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cSpaceShip::setSpaceShipVelocity(SDL_Point spaceShipVel)
{
	spaceShipVelocity = spaceShipVel;
}

/*
=================================================================
Gets the rocket velocity
=================================================================
*/
SDL_Point cSpaceShip::getSpaceShipVelocity()
{
	return spaceShipVelocity;
}
