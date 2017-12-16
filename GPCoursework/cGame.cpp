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
	{ pInstance = new cGame(); }
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Play refers to the main game state (Not MENU or END)
	play = false;
	
	// Score is for the display during PLAYING game state and shadow score for END game state 
	score = 0;
	shadowScore = 0;

	// Called whenever the player catches space junk
	scoreChanged = false;

	// Set filename
	theScoreFile.setFileName("Data/HighScores.txt");

	// Checks if the file is available
	if (!theScoreFile.openFile(ios::out)) //open file for input output
	{
		cout << "Could not open the score file '" << theScoreFile.getFileName() << "'. Error " << SDL_GetError() << endl;
		fileAvailable = false;
	}
	else
	{
		cout << "File '" << theScoreFile.getFileName() << "' opened for input and ready to go!" << endl;
		fileAvailable = true;
	}

	// For timer and spawning
	timerCurrent = 3;
	timerTotal = 5;
	totalTimePlaying = 3;
	
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	
	// Clear the buffer with a black background and set up the different managers
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// For debug
	theAreaClicked = { 0, 0 };

	// Starting game state
	theGameState = MENU;

	// Store the main textures
	textureName =
	{   "spacejunk1",
		"spacejunk2",
		"spacejunk3", 
		"bullet",
		"theSpaceShip", 
		"theSpaceShipTurbo",
		"theBackground",
		"earth",
		"TopBorder",
		"BottomBorder",
		"LeftBorder",
		"RightBorder",
		"EndScreen",
		"LeftControl",
		"LeftArrow",
		"RightArrow",
		"OrangeBackground1",
		"OrangeBackground2",
		"OrangeBackground3",
		"TopBanner"};
	texturesToUse = 
	{   "Images\\SpaceJunk1.png",
		"Images\\SpaceJunk2.png",
		"Images\\SpaceJunk3.png",
		"Images\\bullet.png",
		"Images\\rocketSprite.png",
		"Images\\rocketSprite Turbo.png",
		"Images\\space.png",
		"Images\\Earth.png",
		"Images\\TopBorder.png",
		"Images\\BottomBorder.png",
		"Images\\LeftBorder.png",
		"Images\\RightBorder.png",
		"Images\\EndScreen.jpg",
		"Images\\ControlKey.png",
		"Images\\LeftArrowKey.png",
		"Images\\RightArrowKey.png",
		"Images/Buttons/Options Button.png",
		"Images/Buttons/Options Button.png",
		"Images/Buttons/Options Button.png",
		"Images\\TopBanner.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the button textures
	btnNameList = 
	{   "exit_btn",
		"stop_btn",
		"menu_btn",
		"play_btn",
		"replay_btn"};
	btnTexturesToUse = 
	{   "Images/Buttons/Quit Button - exit.png",
		"Images/Buttons/Quit Button - stop.png",
		"Images/Buttons/Quit Button - menu.png",
		"Images/Buttons/Play Button - play.png",
		"Images/Buttons/Play Button - replay.png"};
	btnPos = { { 400, 375 },{ 500, 500 },{50,50},{ 400, 300 },{500, 575} };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{ theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]); }
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theBtnType = EXIT;

	// Create textures for Game Text
	fontList = { "digital", "spaceAge", "spaceAgeSmall" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf"};
	theFontMgr->addFont(fontList[0], fontsToUse[0], 47);
	theFontMgr->addFont(fontList[1], fontsToUse[1], 34);
	theFontMgr->addFont(fontList[2], fontsToUse[1], 15);

	gameTextNames = 
	{   "MainTitle",
		"SubTitle",
		"Score",
		"Score2",
		"GameOver",
		"MainTitlePlaying",
		"ClockWiseText",
		"CounterClockWiseText",
		"TurboText",
		"HelpText1",
		"HelpText2",
		"HelpText3",
		"Credits"};
	gameTextList = 
	{ "Orbital Garbage Collector",
		"Spin to Collect SpaceJunk!",
		"Score: 0",
		"Score: 0",
		"Game Over!",
		"Orbital Garbage Collector",
		"Spin Clockwise",
		"Spin Counter-Clockwise",
		"Hold for Turbo",
		"Move into the way of SpaceJunk to catch it",
		"When 1 spawns, you get 1 point per catch",
		"When 2 spawn, you get 2 points per catch",
		"Music by Paddy Hennessy"};

	theTextureMgr->addTexture("MainTitle", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("SubTitle", theFontMgr->getFont("spaceAgeSmall")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score2", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("MainTitlePlaying", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("ClockWiseText", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("CounterClockWiseText", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[7], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("TurboText", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[8], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HelpText1", theFontMgr->getFont("spaceAgeSmall")->createTextTexture(theRenderer, gameTextList[9], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HelpText2", theFontMgr->getFont("spaceAgeSmall")->createTextTexture(theRenderer, gameTextList[10], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HelpText3", theFontMgr->getFont("spaceAgeSmall")->createTextTexture(theRenderer, gameTextList[11], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Credits", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[12], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	
	// Load game sounds
	soundList = 
	{   "MainMusic",
		"shot",
		"crunch",
		"beep" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = 
	{   "Audio/MainMusic.wav",
		"Audio/shot007.wav",
		"Audio/Crunch.wav",
		"Audio/Beep.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}
	theSoundMgr->getSnd("MainMusic")->play(-1);
	
	//Connect textures to sprites
	orangeBackground1.setSpritePos({ 100, 150 });
	orangeBackground1.setTexture(theTextureMgr->getTexture("OrangeBackground1"));
	orangeBackground1.setSpriteDimensions(theTextureMgr->getTexture("OrangeBackground1")->getTWidth(), theTextureMgr->getTexture("OrangeBackground1")->getTHeight());

	orangeBackground2.setSpritePos({ 100, 325 });
	orangeBackground2.setTexture(theTextureMgr->getTexture("OrangeBackground2"));
	orangeBackground2.setSpriteDimensions(theTextureMgr->getTexture("OrangeBackground2")->getTWidth(), theTextureMgr->getTexture("OrangeBackground2")->getTHeight());

	orangeBackground3.setSpritePos({ 100, 500 });
	orangeBackground3.setTexture(theTextureMgr->getTexture("OrangeBackground3"));
	orangeBackground3.setSpriteDimensions(theTextureMgr->getTexture("OrangeBackground3")->getTWidth(), theTextureMgr->getTexture("OrangeBackground3")->getTHeight());

	earth.setSpritePos({ 278, 398 });
	earth.setTexture(theTextureMgr->getTexture("earth"));
	earth.setSpriteDimensions(theTextureMgr->getTexture("earth")->getTWidth(), theTextureMgr->getTexture("earth")->getTHeight());

	controlKey.setSpritePos({ 125, 175 });
	controlKey.setTexture(theTextureMgr->getTexture("LeftControl"));
	controlKey.setSpriteDimensions(theTextureMgr->getTexture("LeftControl")->getTWidth(), theTextureMgr->getTexture("LeftControl")->getTHeight());

	leftArrowKey.setSpritePos({ 135, 360 });
	leftArrowKey.setTexture(theTextureMgr->getTexture("LeftArrow"));
	leftArrowKey.setSpriteDimensions(theTextureMgr->getTexture("LeftArrow")->getTWidth(), theTextureMgr->getTexture("LeftArrow")->getTHeight());

	rightArrowKey.setSpritePos({ 135, 540 });
	rightArrowKey.setTexture(theTextureMgr->getTexture("RightArrow"));
	rightArrowKey.setSpriteDimensions(theTextureMgr->getTexture("RightArrow")->getTWidth(), theTextureMgr->getTexture("RightArrow")->getTHeight());
	
	endScreen.setSpritePos({ 0, 0 });
	endScreen.setTexture(theTextureMgr->getTexture("EndScreen"));
	endScreen.setSpriteDimensions(theTextureMgr->getTexture("EndScreen")->getTWidth(), theTextureMgr->getTexture("EndScreen")->getTHeight());

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theSpaceShip.setSpritePos({ 328, 768 });
	theSpaceShip.setTexture(theTextureMgr->getTexture("theSpaceShip"));
	theSpaceShip.setSpriteDimensions(theTextureMgr->getTexture("theSpaceShip")->getTWidth(), theTextureMgr->getTexture("theSpaceShip")->getTHeight());
	theSpaceShip.setSpaceShipVelocity({ 0, 0 });

	topBanner.setSpritePos({ 0, 0 });
	topBanner.setTexture(theTextureMgr->getTexture("TopBanner"));
	topBanner.setSpriteDimensions(theTextureMgr->getTexture("TopBanner")->getTWidth(), theTextureMgr->getTexture("TopBanner")->getTHeight());
}


void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	//Starts the game loop and turns turbo to off
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
	switch (theGameState)
	{
	case MENU:
	{
		SDL_RenderClear(theRenderer);
		
		// Render the images
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		orangeBackground1.render(theRenderer, &orangeBackground1.getSpriteDimensions(), &orangeBackground1.getSpritePos(), orangeBackground1.getSpriteScale());
		orangeBackground2.render(theRenderer, &orangeBackground2.getSpriteDimensions(), &orangeBackground2.getSpritePos(), orangeBackground2.getSpriteScale());
		orangeBackground3.render(theRenderer, &orangeBackground3.getSpriteDimensions(), &orangeBackground3.getSpritePos(), orangeBackground3.getSpriteScale());
		controlKey.render(theRenderer, &controlKey.getSpriteDimensions(), &controlKey.getSpritePos(), controlKey.getSpriteScale());
		rightArrowKey.render(theRenderer, &rightArrowKey.getSpriteDimensions(), &rightArrowKey.getSpritePos(), rightArrowKey.getSpriteScale());
		leftArrowKey.render(theRenderer, &leftArrowKey.getSpriteDimensions(), &leftArrowKey.getSpritePos(), leftArrowKey.getSpriteScale());
		
		// Render the Text
		cTexture* tempTextTexture[7];
		(*tempTextTexture) = theTextureMgr->getTexture("MainTitle");
		SDL_Rect pos1 = { 5, 0, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale1 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos1, scale1);

		(*tempTextTexture) = theTextureMgr->getTexture("SubTitle");
		SDL_Rect pos2 = { 200, 40, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos2, scale2);

		(*tempTextTexture) = theTextureMgr->getTexture("ClockWiseText");
		SDL_Rect pos3 = { 255, 375, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos3, scale3);

		(*tempTextTexture) = theTextureMgr->getTexture("CounterClockWiseText");
		SDL_Rect pos4 = { 255, 550, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale4 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos4, scale4);

		(*tempTextTexture) = theTextureMgr->getTexture("TurboText");
		SDL_Rect pos5 = { 255, 200, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale5 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos5, scale5);

		(*tempTextTexture) = theTextureMgr->getTexture("HelpText1");
		SDL_Rect pos6 = { 115, 755, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale6 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos6, scale6);

		(*tempTextTexture) = theTextureMgr->getTexture("HelpText2");
		SDL_Rect pos7 = { 130, 780, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale7 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos7, scale7);

		(*tempTextTexture) = theTextureMgr->getTexture("HelpText3");
		SDL_Rect pos8 = { 125, 805, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale8 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos8, scale8);

		//Render Buttons
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 5, 735 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 615, 735 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;

	case PLAYING:
	{
		SDL_RenderClear(theRenderer);
		
		// Sets the necessary boolean to true so space junk can spawn
		isPlaying = true;

		// Renders the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Renders each spaceJunk in the vector array
		for (int draw = 0; draw < theSpaceJunks.size(); draw++)
			{
				theSpaceJunks[draw]->render(theRenderer, &theSpaceJunks[draw]->getSpriteDimensions(), &theSpaceJunks[draw]->getSpritePos(), theSpaceJunks[draw]->getSpriteRotAngle(), &theSpaceJunks[draw]->getSpriteCentre(), theSpaceJunks[draw]->getSpriteScale());
			}
			
		// Renders the spaceship
		theSpaceShip.render(theRenderer, &theSpaceShip.getSpriteDimensions(), &theSpaceShip.getSpritePos(), theSpaceShip.getSpriteRotAngle(), &theSpaceShip.getSpriteCentre(), theSpaceShip.getSpriteScale());
		
		// Renders the Buttons
		theButtonMgr->getBtn("stop_btn")->setSpritePos({ 350, 30 });
		theButtonMgr->getBtn("stop_btn")->render(theRenderer, &theButtonMgr->getBtn("stop_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("stop_btn")->getSpritePos(), theButtonMgr->getBtn("stop_btn")->getSpriteScale());

		// Renders the Earth
		earth.render(theRenderer, &earth.getSpriteDimensions(), &earth.getSpritePos(), earth.getSpriteScale());

		// Renders the top banner image to visually separate score and title from rest of screen
		topBanner.render(theRenderer, &topBanner.getSpriteDimensions(), &topBanner.getSpritePos(), topBanner.getSpriteScale());

		// Updates score textures if the score changes
		if (scoreChanged) {
			gameTextList[2] = scoreAsString.c_str();
			theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
			
			gameTextList[3] = shadowScoreAsString.c_str();
			theTextureMgr->addTexture("Score2", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

			ofstream scoreSave;
			scoreSave.open("Data/HighScores.txt");
			scoreSave.clear();
			cout << shadowScoreAsString;
			scoreSave << shadowScoreAsString;
			cout << "ShadowScoreAsString";
			scoreSave.close();

			scoreChanged = false;
		}

		// Renders the score
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Renders the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("MainTitlePlaying");
		SDL_Rect pos = { 5, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		// Render the Buttons
		theButtonMgr->getBtn("stop_btn")->setSpritePos({ 325, 35 });
		theButtonMgr->getBtn("stop_btn")->render(theRenderer, &theButtonMgr->getBtn("stop_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("stop_btn")->getSpritePos(), theButtonMgr->getBtn("stop_btn")->getSpriteScale());
	}
	break;

	case END:
	{
		theSpaceShip.circleT = -PI / 2;
		score = 0;

		// Renders the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Saves score
		if (!scoreChanged) {
			ifstream scoreSaving;
			scoreSaving.open("Data/HighScores.txt");
			scoreSaving.clear();
			scoreSaving >> shadowScoreAsString;
			scoreSaving.close();
			scoreChanged = true;
		}

		//Stops space junk from spawning
		play = false;

		SDL_RenderClear(theRenderer);
		endScreen.render(theRenderer, NULL, NULL, endScreen.getSpriteScale());
		
		// Updates score string going into end screen
		string theScore = to_string(score);
		scoreAsString = "score: " + theScore;

		// Renders the Text
		cTexture* tempTextTexture[3];
		
		(*tempTextTexture) = theTextureMgr->getTexture("MainTitle");
		SDL_Rect pos1 = { 5, 0, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale1 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos1, scale1);

		(*tempTextTexture) = theTextureMgr->getTexture("Score2");
		SDL_Rect pos2 = { 250, 100, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos2, scale2);

		(*tempTextTexture) = theTextureMgr->getTexture("GameOver");
		SDL_Rect pos3 = { 230, 150, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos3, scale3);

		(*tempTextTexture) = theTextureMgr->getTexture("Credits");
		SDL_Rect pos4 = { 125, 775, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale4 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos4, scale4);

		// Tells game that is not in PLAYING game state
		isPlaying = false;

		// Renders the Buttons
		theButtonMgr->getBtn("replay_btn")->setSpritePos({ 5, 635 });
		theButtonMgr->getBtn("replay_btn")->render(theRenderer, &theButtonMgr->getBtn("replay_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("replay_btn")->getSpritePos(), theButtonMgr->getBtn("replay_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 615, 635 });
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
	switch (theGameState)
	{
		default:
		break;

		case MENU: 
		{
			theSpaceJunks.clear();
			play = false;
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		}
		break;

		case PLAYING:
		{
			// Operates timer
			timerCurrent += deltaTime / 2;
	
		if (!play) 
		{
			if (timerCurrent >= timerTotal) 
			{
				// Clears space junk
				theSpaceJunks.clear();

				// Makes the game more difficult after a while
				if (totalTimePlaying > 25) {difficulty = 2;}
				else {difficulty = 1;}

				// Spawns space junk
				for (int junk = 0; junk < difficulty; junk++)
				{
					theSpaceJunks.push_back(new cSpaceJunk);
					theSpaceJunks[junk]->setSpritePos({328, 448});//({ 334, 454 });
					int  help = rand() % 360;
					float Direction1 = -sin(help + 328) * 45;
					cout << "Direction 1 is: " << Direction1 << endl;
					float Direction2 = cos(help + 448) * 45;
					cout << "Direction 2 is: " << Direction2 << endl;
					theSpaceJunks[junk]->setSpriteTranslation({ (((int)Direction1)), ((int)Direction2) });
					int randSpaceJunk = rand() % 3;
					theSpaceJunks[junk]->setTexture(theTextureMgr->getTexture(textureName[randSpaceJunk]));
					theSpaceJunks[junk]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randSpaceJunk])->getTWidth(), theTextureMgr->getTexture(textureName[randSpaceJunk])->getTHeight());
					theSpaceJunks[junk]->setSpaceJunkVelocity({ 0, 0 });
					theSpaceJunks[junk]->setActive(true);
				}
				totalTimePlaying += timerCurrent;
				timerCurrent = 0;
			}
		}

		// Updates the visibility and position of each piece of space junk 
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
		
		// For collisions
		for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
		{
			(*spaceJunkIterator)->update(deltaTime);

			for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
			{
				if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &theSpaceShip.getBoundingRect()))
				{
					// Destroys space junk, plays sound and increases score
					(*spaceJunkIterator)->setActive(false);
					theSoundMgr->getSnd("crunch")->play(0);
					score++;
					shadowScore++;

					//For refreshing score textures
					if (theTextureMgr->getTexture("Score") != NULL) 
					{
						theTextureMgr->deleteTexture("Score");
					}
					string theScore = to_string(score);
					scoreAsString = "score: " + theScore;

					if (theTextureMgr->getTexture("Score2") != NULL) 
					{
						theTextureMgr->deleteTexture("Score2");
					}
					string theShadowScore = to_string(shadowScore);
					shadowScoreAsString = "score: " + theShadowScore;
					scoreChanged = true;
				}
			}
		}
		
	// Update the Rockets position
	theSpaceShip.update(deltaTime);

	//Button
	theGameState = theButtonMgr->getBtn("stop_btn")->update(theGameState, END, theAreaClicked);
	}
	break;
	
	case END: 
	{
		theSpaceJunks.clear();

		// Resets scores
		score = 0;
		shadowScore = 0;

		for (int single = 0; single < 0.5; single++) 
		{
			if (theTextureMgr->getTexture("Score") != NULL) 
			{
				theTextureMgr->deleteTexture("Score");
			}
			string theScore = to_string(score);
			scoreAsString = "score: " + theScore;
		}
		theSpaceJunks.clear();
		timerCurrent = 3;
		totalTimePlaying = 3;
		play = false;
		
		// Button
		theGameState = theButtonMgr->getBtn("replay_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		}
	break;

	}	
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
					theSoundMgr->getSnd("beep")->play(0);
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
		
			case SDL_KEYUP:
			switch (event.key.keysym.sym) 
			{
				case SDLK_LCTRL: 
				isPressed = false;
			}
			break;
	
			case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_RIGHT:
				{
					if (isPressed == true) 
					{
						float elapsedTime = (float)this->getElapsedSeconds();
						theSpaceShip.circleT += 90.0f * (PI / 180);
					}
					else 
					{
						float elapsedTime = (float)this->getElapsedSeconds();
						theSpaceShip.circleT += 2.0f * (PI / 180);
					}
				}
				break;


				case SDLK_LEFT:
				{
					if (isPressed == true) 
					{
						float elapsedTime = (float)this->getElapsedSeconds();
						theSpaceShip.circleT -= 90.0f * (PI / 180);
					}
					else 
					{
						float elapsedTime = (float)this->getElapsedSeconds();
						theSpaceShip.circleT -= 2.0f * (PI / 180);
					}	
				}
				break;

				case SDLK_LCTRL:
				{
					isPressed = true;
				}
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

