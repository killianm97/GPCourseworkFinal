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
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


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



	theAreaClicked = { 0, 0 };

	//GameStates
	theGameState = MENU;

	// Store the textures
	textureName = { "spacejunk1", "spacejunk2", "spacejunk3", "spacejunk4", "bullet","theSpaceShip", "theSpaceShipTurbo", "theBackground", "earth", "TopBorder", "BottomBorder", "LeftBorder", "RightBorder" };
	texturesToUse = { "Images\\SpaceJunk1.png", "Images\\SpaceJunk2.png", "Images\\SpaceJunk3.png", "Images\\SpaceJunk4.png", "Images\\bullet.png", "Images\\rocketSprite.png", "Images\\rocketSprite Turbo.png", "Images\\space.png", "Images\\Earth.png", "Images\\TopBorder.png", "Images\\BottomBorder.png", "Images\\LeftBorder.png", "Images\\RightBorder.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the textures
	btnNameList = { "exit_btn", "stop_btn" ,"menu_btn", "play_btn", "replay_btn"};
	btnTexturesToUse = { "Images/Buttons/Quit Button - exit.png", "Images/Buttons/Quit Button - stop.png", "Images/Buttons/Quit Button - menu.png", "Images/Buttons/Play Button - play.png", "Images/Buttons/Play Button - replay.png" };
	btnPos = { { 400, 375 },{ 500, 500 },{50,50},{ 400, 300 },{500, 575} };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theBtnType = EXIT;
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 34);
	}
	gameTextNames = { "Main Screen Title", "Main Screen Subtext", "Playing Screen Title", "High Score" };
	gameTextList = { "Orbital Garbage Collector", "Collect All the Rubbish!", "Orbital Garbage Collector", "High Scoress" };

	theTextureMgr->addTexture("PlayingTitle", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("CreateTxt", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	for (int text = 0; text < gameTextNames.size(); text++)
		//	{
		//		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 228, 213, 238, 255 }, { 0, 0, 0, 0 }));
			//}


			// Load game sounds
	soundList = { "MenuMusic", "shot", "explosion" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/MenuMusic.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}
	
	theSoundMgr->getSnd("MenuMusic")->play(-1);

	


	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());


	theSpaceShip.setSpritePos({ 500, 350 });
	theSpaceShip.setTexture(theTextureMgr->getTexture("theSpaceShip"));
	theSpaceShip.setSpriteDimensions(theTextureMgr->getTexture("theSpaceShip")->getTWidth(), theTextureMgr->getTexture("theSpaceShip")->getTHeight());
	theSpaceShip.setSpaceShipVelocity({ 0, 0 });

		earth.setSpritePos({ 360, 420 });
		earth.setTexture(theTextureMgr->getTexture("earth"));
	//	earth.setSpriteDimensions(50, 50);
		//earth.setSpriteDimensions(theTextureMgr->getTexture("earth")->getTWidth(), theTextureMgr->getTexture("earth")->getTHeight());

		// Create vector array of textures
		
		
		if (theGameState == PLAYING){
			for (int junk = 0; junk < 5; junk++)
			{
				theSpaceJunks.push_back(new cSpaceJunk);
				//theSpaceJunks[junk]->setSpritePos({ 100 * (rand() % 7 + 1), 50 * (rand() % 7 + 1) });
				theSpaceJunks[junk]->setSpritePos({ 360 , 420 });
				int PosOrNeg1 = rand() % 2 + 1;
				int PosOrNeg2 = rand() % 2 + 1;
				int TotalDirection = 75;
				int Direction1 = (TotalDirection - (rand() % TotalDirection + 1));
				//rand() % 20 + 10);
				int Direction2 = (TotalDirection - Direction1);
				//cout << Direction1 << " and " << Direction2 << " equal " << TotalDirection << endl;//rand() % 20 + 10);
				if (PosOrNeg1 > 1) {
					Direction1 = Direction1 * -1;
				}
				if (PosOrNeg2 > 1) {
					Direction2 = Direction2 * -1;
				}
				cout << Direction1 << " and " << Direction2 << " equal " << TotalDirection << endl;
				theSpaceJunks[junk]->setSpriteTranslation({ Direction1, Direction2 });

				//theSpaceJunks[junk]->setSpritePos({ 340 , 340 });
				//int randDirection1 = rand() % 100 + 25;
				//int randDirection2 = rand() % 100 + 50;
				//theSpaceJunks[junk]->setSpriteTranslation({ randDirection1, randDirection2 });
				int randSpaceJunk = rand() % 4;
				theSpaceJunks[junk]->setTexture(theTextureMgr->getTexture(textureName[randSpaceJunk]));
				theSpaceJunks[junk]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randSpaceJunk])->getTWidth(), theTextureMgr->getTexture(textureName[randSpaceJunk])->getTHeight());
				theSpaceJunks[junk]->setSpaceJunkVelocity({ 0, 0 });
				theSpaceJunks[junk]->setActive(true);
			}
		}
	}


void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;
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
	
	switch (theGameState)
	{
	case MENU:
	{
		
		//theSoundMgr->getSnd("MenuMusic")->play(-1);
		isPlaying = false;
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title

		cTexture* tempTextTexture[2];
		(*tempTextTexture) = theTextureMgr->getTexture("PlayingTitle");
		SDL_Rect pos1 = { 10, 10, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale1 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos1, scale1);

		(*tempTextTexture) = theTextureMgr->getTexture("CreateTxt");
		SDL_Rect pos2 = { 10, 50, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos2, scale2);

		//tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		//pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//scale = { 1, 1 };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//tempTextTexture = theTextureMgr->getTexture("CreateTxt");
		//pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 5, 735 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 615, 735 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

	}
	break;
	case PLAYING:
	{
		isPlaying = true;
	//	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		SDL_RenderClear(theRenderer);
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render each spaceJunk in the vector array
		
		

		//if (PLAYING == true) {
			for (int draw = 0; draw < theSpaceJunks.size(); draw++)
			{
				theSpaceJunks[draw]->render(theRenderer, &theSpaceJunks[draw]->getSpriteDimensions(), &theSpaceJunks[draw]->getSpritePos(), theSpaceJunks[draw]->getSpriteRotAngle(), &theSpaceJunks[draw]->getSpriteCentre(), theSpaceJunks[draw]->getSpriteScale());
			}
			// Render each bullet in the vector array
			for (int draw = 0; draw < theBullets.size(); draw++)
			{
				theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
			}
	//	}
		
	//Render the Earth
			earth.render(theRenderer, NULL, NULL, earth.getSpriteScale());

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		
		// render the rocket
		theSpaceShip.render(theRenderer, &theSpaceShip.getSpriteDimensions(), &theSpaceShip.getSpritePos(), theSpaceShip.getSpriteRotAngle(), &theSpaceShip.getSpriteCentre(), theSpaceShip.getSpriteScale());
		// Render the Buttons
		theButtonMgr->getBtn("stop_btn")->setSpritePos({ 615, 735 });
		theButtonMgr->getBtn("stop_btn")->render(theRenderer, &theButtonMgr->getBtn("stop_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("stop_btn")->getSpritePos(), theButtonMgr->getBtn("stop_btn")->getSpriteScale());


	}
	break;
	case END:
	{
		//theSoundMgr->getSnd("EndMusic")->play(-1);
		isPlaying = false;
		//spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		//pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		//pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		//pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("replay_btn")->setSpritePos({ 5, 735 });
		theButtonMgr->getBtn("replay_btn")->render(theRenderer, &theButtonMgr->getBtn("replay_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("replay_btn")->getSpritePos(), theButtonMgr->getBtn("replay_btn")->getSpriteScale());

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 615, 735 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		
		}
	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);


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
	// CHeck Button clicked and change state
	//if (theGameState == MENU || theGameState == END)
//	{
		//theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		
	//}
	//else
	//{
		//theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, END, theAreaClicked);
	//}
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
	theGameState = theButtonMgr->getBtn("replay_btn")->update(theGameState, PLAYING, theAreaClicked);
	theGameState = theButtonMgr->getBtn("stop_btn")->update(theGameState, END, theAreaClicked);
	theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	theGameState = theButtonMgr->getBtn("stop_btn")->update(theGameState, END, theAreaClicked);



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
		(*bulletIterartor)->update(deltaTime);
		for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
		{
			if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*spaceJunkIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
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
				theAreaClicked = { event.motion.x, event.motion.y };
				cout << "the mouse position is x= " << event.motion.x << "and y= " << event.motion.y << endl;
				//cout << "Asteroid position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;

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
			{

				theGameState = theButtonMgr->getBtn("stop_btn")->update(theGameState, END, theAreaClicked);
				if (theGameState == MENU) {
				theLoop = false; }
			else
			{
				if (theGameState == PLAYING) { theGameState = END; }
				else { theGameState = MENU; }
			}

			}
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

			case SDLK_LCTRL:
			{
				
				if (isPressed == true) {
				//	theSpaceShip.setTexture{};
				isPressed = false; }
				else { isPressed = true;
				//theSpaceShip.setTexture{};
				}

			}
			break;

			case SDLK_SPACE:
			{
				theBullets.push_back(new cBullet);
				int numBullets = theBullets.size() - 1;
				theBullets[numBullets]->setSpritePos({ theSpaceShip.getBoundingRect().x + theSpaceShip.getSpriteCentre().x - 2, theSpaceShip.getBoundingRect().y + theSpaceShip.getSpriteCentre().y - 12});
				theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
				theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
				theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				theBullets[numBullets]->setBulletVelocity({ 0, 0 });
				theBullets[numBullets]->setSpriteRotAngle(theSpaceShip.getSpriteRotAngle());
				theBullets[numBullets]->setActive(true);
				cout << "Bullet added to Vector at position - x: " << theSpaceShip.getBoundingRect().x << " y: " << theSpaceShip.getBoundingRect().y << endl;
			}
			theSoundMgr->getSnd("shot")->play(0);
			break;
			default:
				break;
			}

			default:
				break;
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

