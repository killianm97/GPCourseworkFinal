/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
#include <math.h>
#include "cSpaceShip.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "spacejunk1", "spacejunk2", "spacejunk3", "spacejunk4", "bullet","theSpaceShip","theBackground", "earth"};
	texturesToUse = { "Images\\SpaceJunk1.png", "Images\\SpaceJunk2.png", "Images\\SpaceJunk3.png", "Images\\SpaceJunk4.png", "Images\\bullet.png", "Images\\rocketSprite.png", "Images\\space.png", "Images\\Earth.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 34);
	}
	gameTextList = { "Orbital Garbage Collector"};
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme", "shot", "explosion" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theSpaceShip.setSpritePos({ 500, 350 });
	theSpaceShip.setTexture(theTextureMgr->getTexture("theSpaceShip"));
	theSpaceShip.setSpriteDimensions(theTextureMgr->getTexture("theSpaceShip")->getTWidth(), theTextureMgr->getTexture("theSpaceShip")->getTHeight());
	theSpaceShip.setSpaceShipVelocity({ 0, 0 });

	earth.setSpritePos({ 360, 360 });
	earth.setTexture(theTextureMgr->getTexture("earth"));
	earth.setSpriteDimensions(theTextureMgr->getTexture("earth")->getTWidth() / 2, theTextureMgr->getTexture("earth")->getTHeight() / 2);

	// Create vector array of textures

	for (int junk = 0; junk < 4; junk++)
	{
		theSpaceJunks.push_back(new cSpaceJunk);
	//	theAsteroids[astro]->setSpritePos({ 100 * (rand() % 5 + 1), 50 * (rand() % 5 + 1) });
	//	theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		theSpaceJunks[junk]->setSpritePos({ 340 , 340 });
		theSpaceJunks[junk]->setSpriteTranslation({ 30, 0});
		int randSpaceJunk = rand() % 4;
		theSpaceJunks[junk]->setTexture(theTextureMgr->getTexture(textureName[randSpaceJunk]));
		theSpaceJunks[junk]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randSpaceJunk])->getTWidth(), theTextureMgr->getTexture(textureName[randSpaceJunk])->getTHeight());
		theSpaceJunks[junk]->setSpaceJunkVelocity({ 0, 0 });
		theSpaceJunks[junk]->setActive(true);
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;
	isPressed = false;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	// Render each spaceJunk in the vector array
	for (int draw = 0; draw < theSpaceJunks.size(); draw++)
	{
		theSpaceJunks[draw]->render(theRenderer, &theSpaceJunks[draw]->getSpriteDimensions(), &theSpaceJunks[draw]->getSpritePos(), theSpaceJunks[draw]->getSpriteRotAngle(), &theSpaceJunks[draw]->getSpriteCentre(), theSpaceJunks[draw]->getSpriteScale());
	}
	// Render each bullet in the vector array
	for (int draw = 0; draw < theBullets.size(); draw++)
	{
		theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
	}
	//Render the Earth
	earth.render(theRenderer, NULL, NULL, earth.getSpriteScale());

	// Render the Title
	cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
	SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	FPoint scale = { 1, 1 };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	// render the rocket
	theSpaceShip.render(theRenderer, &theSpaceShip.getSpriteDimensions(), &theSpaceShip.getSpritePos(), theSpaceShip.getSpriteRotAngle(), &theSpaceShip.getSpriteCentre(), theSpaceShip.getSpriteScale());
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Update the visibility and position of each asteriod
	vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin();
	while (spaceJunkIterator != theSpaceJunks.end())
	{
		if ((*spaceJunkIterator)->isActive() == false)
		{
			spaceJunkIterator = theSpaceJunks.erase(spaceJunkIterator);
		}
		else
		{
			(*spaceJunkIterator)->update(deltaTime);
			++spaceJunkIterator;
		}
	}
	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
	while (bulletIterartor != theBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theBullets.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;
		}
	}
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	{
		//(*bulletIterartor)->update(deltaTime);
		for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
		{
			if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*spaceJunkIterator)->setActive(false);
			//	(*bulletIterartor)->setActive(false);
				theSoundMgr->getSnd("explosion")->play(0);
			}
		}
	}


	// Update the Rockets position
	theSpaceShip.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;
				
			case SDLK_RIGHT:
			{
				if (isPressed == true) {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT += 15.0f * (PI / 180);
				}
				else {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT += 1.0f * (PI / 180);
				}

			}
			break;


			case SDLK_LEFT:
			{
				if (isPressed == true) {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT -= 15.0f * (PI / 180);
				} //* elapsedTime * 1000.0f;
				else {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT -= 1.0f * (PI / 180);
				} //* elapsedTime * 1000.0f;
				
			}
			break;

			case SDLK_SPACE:
			{
				
				if (isPressed == true) {
				isPressed = false; }
				else { isPressed = true; }

			}

			/*case SDLK_SPACE:
			{
				theBullets.push_back(new cBullet);
				int numBullets = theBullets.size() - 1;
				theBullets[numBullets]->setSpritePos({ theRocket.getBoundingRect().x + theRocket.getSpriteCentre().x, theRocket.getBoundingRect().y + theRocket.getSpriteCentre().y });
				theBullets[numBullets]->setSpriteTranslation({ 2, 2 });
				theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
				theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				theBullets[numBullets]->setBulletVelocity({ 2, 2 });
				theBullets[numBullets]->setSpriteRotAngle(theRocket.getSpriteRotAngle());
				theBullets[numBullets]->setActive(true);
				cout << "Bullet added to Vector at position - x: " << theRocket.getBoundingRect().x << " y: " << theRocket.getBoundingRect().y << endl;
			}
			theSoundMgr->getSnd("shot")->play(0);
			break;
			default:
				break;
			}*/

			default:
				break;
			}
			

		
			{
			default:
				break;
			}
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

