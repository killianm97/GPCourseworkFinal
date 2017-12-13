/*
=================
cRocket.cpp
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

	//circleT += 1.0f * deltaTime;
	if (circleT < 0)
	{
		circleT += PI * 2;
	}
	if (circleT > PI * 2)
	{
		circleT -= PI * 2;
	}

	float rotAngle = (circleT * (180.0f / PI) * -1.0f) - 90;

	this->setSpriteRotAngle(rotAngle);//this->getSpriteRotAngle() + /*(20.0f * deltaTime)*/ + PI * 2.0f * cos(circleT)  * -sin(circleT) * deltaTime);
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() - 360);
	}


	

	FPoint direction = { 0.0f, 0.0f };
	//direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	//direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->spaceShipVelocity.x = this->spaceShipVelocity.x + direction.X;
	this->spaceShipVelocity.y = this->spaceShipVelocity.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	//currentSpritePos.x += this->rocketVelocity.x * deltaTime;
	//currentSpritePos.y += this->rocketVelocity.y * deltaTime;

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


/*
=================================================================
Update the sprite position
=================================================================
*/


//
//	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
//	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
//	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));
//
//	spriteVelocityAdd *= spriteTranslation;
//
//	rocketVelocity += spriteVelocityAdd;
//
//	spritePos2D += rocketVelocity * deltaTime;
//
//	rocketVelocity *= 0.95;
//
//	/*
//	==============================================================
//	| Check for collisions
//	==============================================================
//	*/
//	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
//	{
//		(*bulletIterartor)->update(deltaTime);
//		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
//		{
//			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and asteroid to false
//				(*asteroidIterator)->setActive(false);
//				(*bulletIterartor)->setActive(false);
//			}
//		}
//	}
//
//	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
//	while (bulletIterartor != theRocketBullets.end())
//	{
//		if ((*bulletIterartor)->isActive() == false)
//		{
//			bulletIterartor = theRocketBullets.erase(bulletIterartor);
//		}
//		else
//		{
//			//(*bulletIterartor)->update(deltaTime);
//			(*bulletIterartor)->render();
//			(*bulletIterartor)->renderCollisionBox();
//			++bulletIterartor;
//		}
//	}
//}