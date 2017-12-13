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
	play = false;
	score = 0;
	shadowScore = 0;

	//if (play == true)
	//{
	timerCurrent = 3;
	timerTotal = 5;
	totalTimePlaying = 3;
	//}

	cout << "Score is: " << score;
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
	textureName =
	{ "spacejunk1",
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
		"TopBanner"
		};
	texturesToUse = 
	{ "Images\\SpaceJunk1.png",
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
		"Images\\TopBanner.png"
	};
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
	
	theFontMgr->addFont(fontList[0], fontsToUse[0], 47);
	theFontMgr->addFont(fontList[1], fontsToUse[1], 34);


	//for (int fonts = 0; fonts < fontList.size(); fonts++)
	//{
	//	theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 34);
	//}
	gameTextNames = { "MainTitle", "SubTitle", "Score", "Score2", "GameOver", "MainTitlePlaying", "ClockWiseText", "CounterClockWiseText", "TurboText" };
	gameTextList = { "Orbital Garbage Collector", "Spin to Collect SpaceJunk!", "Score: 0", "Score: 0", "Game Over!", "Orbital Garbage Collector", "Spin Clockwise", "Spin Counter-Clockwise", "Hold for Turbo" };

	theTextureMgr->addTexture("MainTitle", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("SubTitle", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score2", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("MainTitlePlaying", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("ClockWiseText", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("CounterClockWiseText", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[7], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("TurboText", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[8], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));


	//for (int text = 0; text < gameTextNames.size(); text++)
		//	{
		//		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 228, 213, 238, 255 }, { 0, 0, 0, 0 }));
			//}


			// Load game sounds
	soundList = { "MainMusic", "shot", "crunch", "beep" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = { "Audio/MainMusic.wav", "Audio/shot007.wav", "Audio/Crunch.wav", "Audio/Beep.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}
	
	theSoundMgr->getSnd("MainMusic")->play(-1);
	
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


	//topBorder.setSpritePos({ 0, 10 });
	//topBorder.setTexture(theTextureMgr->getTexture("TopBorder"));
	//topBorder.setSpriteDimensions(theTextureMgr->getTexture("TopBorder")->getTWidth(), theTextureMgr->getTexture("TopBorder")->getTHeight());

	//bottomBorder.setSpritePos({ 0, 840 });
	//bottomBorder.setTexture(theTextureMgr->getTexture("BottomBorder"));
	//bottomBorder.setSpriteDimensions(theTextureMgr->getTexture("BottomBorder")->getTWidth(), theTextureMgr->getTexture("BottomBorder")->getTHeight());

	//leftBorder.setSpritePos({ 0, 0 });
	//leftBorder.setTexture(theTextureMgr->getTexture("LeftBorder"));
	//leftBorder.setSpriteDimensions(theTextureMgr->getTexture("LeftBorder")->getTWidth(), theTextureMgr->getTexture("LeftBorder")->getTHeight());

	//rightBorder.setSpritePos({ 720, 0 });
	//rightBorder.setTexture(theTextureMgr->getTexture("RightBorder"));
	//rightBorder.setSpriteDimensions(theTextureMgr->getTexture("RightBorder")->getTWidth(), theTextureMgr->getTexture("RightBorder")->getTHeight());
	


	//	earth.setSpriteDimensions(50, 50);
		//earth.setSpriteDimensions(theTextureMgr->getTexture("earth")->getTWidth(), theTextureMgr->getTexture("earth")->getTHeight());

		// Create vector array of textures
		
		
		//if (theGameState == PLAYING){
	
		//}
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
	
	
	switch (theGameState)
	{
	case MENU:
	{
		SDL_RenderClear(theRenderer);
		isPlaying = false;
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		orangeBackground1.render(theRenderer, &orangeBackground1.getSpriteDimensions(), &orangeBackground1.getSpritePos(), orangeBackground1.getSpriteScale());
		orangeBackground2.render(theRenderer, &orangeBackground2.getSpriteDimensions(), &orangeBackground2.getSpritePos(), orangeBackground2.getSpriteScale());
		orangeBackground3.render(theRenderer, &orangeBackground3.getSpriteDimensions(), &orangeBackground3.getSpritePos(), orangeBackground3.getSpriteScale());
		controlKey.render(theRenderer, &controlKey.getSpriteDimensions(), &controlKey.getSpritePos(), controlKey.getSpriteScale());
		rightArrowKey.render(theRenderer, &rightArrowKey.getSpriteDimensions(), &rightArrowKey.getSpritePos(), rightArrowKey.getSpriteScale());
		leftArrowKey.render(theRenderer, &leftArrowKey.getSpriteDimensions(), &leftArrowKey.getSpritePos(), leftArrowKey.getSpriteScale());
		
		// Render the Title

		cTexture* tempTextTexture[4];
		(*tempTextTexture) = theTextureMgr->getTexture("MainTitle");
		SDL_Rect pos1 = { 5, 0, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale1 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos1, scale1);

		(*tempTextTexture) = theTextureMgr->getTexture("SubTitle");
		SDL_Rect pos2 = { 10, 50, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos2, scale2);

		(*tempTextTexture) = theTextureMgr->getTexture("ClockWiseText");
		SDL_Rect pos3 = { 255, 550, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos3, scale3);

		(*tempTextTexture) = theTextureMgr->getTexture("CounterClockWiseText");
		SDL_Rect pos4 = { 255, 375, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale4 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos4, scale4);

		(*tempTextTexture) = theTextureMgr->getTexture("TurboText");
		SDL_Rect pos5 = { 255, 200, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale5 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos5, scale5);

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
		

		SDL_RenderClear(theRenderer);
		isPlaying = true;

		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render each spaceJunk in the vector array
		
		

		
		//tempTextTexture = theTextureMgr->getTexture("Score2");
		//pos = { 10, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//scale = { 1, 1 };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	
		//SDL_RenderPresent(theRenderer);

		//if (PLAYING == true) {
			for (int draw = 0; draw < theSpaceJunks.size(); draw++)
			{
				theSpaceJunks[draw]->render(theRenderer, &theSpaceJunks[draw]->getSpriteDimensions(), &theSpaceJunks[draw]->getSpritePos(), theSpaceJunks[draw]->getSpriteRotAngle(), &theSpaceJunks[draw]->getSpriteCentre(), theSpaceJunks[draw]->getSpriteScale());
			}
			// Render each bullet in the vector array
			///for (int draw = 0; draw < theBullets.size(); draw++)
			//{
			//	theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
			//}
	//	}
		

		
		
		
		// render the rocket
		theSpaceShip.render(theRenderer, &theSpaceShip.getSpriteDimensions(), &theSpaceShip.getSpritePos(), theSpaceShip.getSpriteRotAngle(), &theSpaceShip.getSpriteCentre(), theSpaceShip.getSpriteScale());
		
		// Render the Buttons
		theButtonMgr->getBtn("stop_btn")->setSpritePos({ 310, 30 });
		theButtonMgr->getBtn("stop_btn")->render(theRenderer, &theButtonMgr->getBtn("stop_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("stop_btn")->getSpritePos(), theButtonMgr->getBtn("stop_btn")->getSpriteScale());

		//Render the Earth
		earth.render(theRenderer, &earth.getSpriteDimensions(), &earth.getSpritePos(), earth.getSpriteScale());

		//topBorder.render(theRenderer, &topBorder.getSpriteDimensions(), &topBorder.getSpritePos(), topBorder.getSpriteScale());
		//bottomBorder.render(theRenderer, &bottomBorder.getSpriteDimensions(), &bottomBorder.getSpritePos(), bottomBorder.getSpriteScale());
		//leftBorder.render(theRenderer, &leftBorder.getSpriteDimensions(), &leftBorder.getSpritePos(), leftBorder.getSpriteScale());
		//rightBorder.render(theRenderer, &rightBorder.getSpriteDimensions(), &rightBorder.getSpritePos(), rightBorder.getSpriteScale());
		topBanner.render(theRenderer, &topBanner.getSpriteDimensions(), &topBanner.getSpritePos(), topBanner.getSpriteScale());

		if (scoreChanged) {

			gameTextList[2] = scoreAsString.c_str();
			theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));



			gameTextList[3] = shadowScoreAsString.c_str();
			theTextureMgr->addTexture("Score2", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
			scoreChanged = false;
		}

		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("MainTitlePlaying");
		SDL_Rect pos = { 5, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		// Render the Buttons
		theButtonMgr->getBtn("stop_btn")->setSpritePos({ 310, 30 });
		theButtonMgr->getBtn("stop_btn")->render(theRenderer, &theButtonMgr->getBtn("stop_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("stop_btn")->getSpritePos(), theButtonMgr->getBtn("stop_btn")->getSpriteScale());



	}
	break;
	case END:
	{
		
		//if (!newRecentScore) {
		//	ofstream scoreFile;
		//	scoreFile.open("Data/recentScores.txt");
		//	scoreFile.clear();
		//	scoreFile << score;
		//	newRecentScore = true;

		//}


		//tempTextTexture = theTextureMgr->getTexture("Score2");
		//pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//scale = { 1, 1 };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		score = 0;
		play = false;
		SDL_RenderClear(theRenderer);
		//spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		endScreen.render(theRenderer, NULL, NULL, endScreen.getSpriteScale());
		cTexture* tempTextTexture[3];
		// Render the Title
		(*tempTextTexture) = theTextureMgr->getTexture("MainTitle");
		SDL_Rect pos1 = { 5, 0, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale1 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos1, scale1);


		string theScore = to_string(score);
		scoreAsString = "score: " + theScore;
		theTextureMgr->deleteTexture("Score");
		gameTextList[2] = scoreAsString.c_str();
		theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));



		(*tempTextTexture) = theTextureMgr->getTexture("Score2");
		SDL_Rect pos2 = { 250, 100, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos2, scale2);
		

		(*tempTextTexture) = theTextureMgr->getTexture("GameOver");
		SDL_Rect pos3 = { 230, 150, (*tempTextTexture)->getTextureRect().w, (*tempTextTexture)->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		(*tempTextTexture)->renderTexture(theRenderer, (*tempTextTexture)->getTexture(), &(*tempTextTexture)->getTextureRect(), &pos3, scale3);

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
	switch (theGameState)
	{
	default:
		break;
	case MENU: {
		theSpaceJunks.clear();

		play = false;
		//gameTextList[2] = "Score: 0";
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	}
			   break;
	case PLAYING: {

		/*if (totalTimePlaying == 3) {

			if (theTextureMgr->getTexture("Score") != NULL) {
				theTextureMgr->deleteTexture("Score");
			}
			string theScore = to_string(score);
			scoreAsString = "score: " + theScore;
		}

		if (totalTimePlaying == 3) {

			if (theTextureMgr->getTexture("Score2") != NULL) {
				theTextureMgr->deleteTexture("Score2");
			}
			string theScore2 = to_string(shadowScore);
			shadowScoreAsString = "score: " + theScore2;
		}
		*/

		timerCurrent += deltaTime / 2;
		cout << timerCurrent << endl;



		if (!play) {

			

		if (timerCurrent >= timerTotal) {
			//do timer action


			theSpaceJunks.clear();

			if (totalTimePlaying > 25) {

				difficulty = 2;
			}
			else {

				difficulty = 1;
			}




			for (int junk = 0; junk < difficulty; junk++)
			{

				theSpaceJunks.push_back(new cSpaceJunk);
				theSpaceJunks[junk]->setSpritePos({ 334, 454 });//({ 359, 479 });
																//theSpaceJunks[junk]->setSpritePos({ 100 * (rand() % 7 + 1), 50 * (rand() % 7 + 1) });
				int PosOrNeg1 = rand() % 4 + 2;
				int PosOrNeg2 = rand() % 4 + 2;
				//int TotalDirection = 35;
				//int Direction1 = (TotalDirection - (rand() % TotalDirection));
				//int Direction2 = (TotalDirection - Direction1);
				//rand() % 20 + 10);
				int  help = rand() % 360;
				float helpme = help * PI / 2;
				int Direction1 = cos(helpme) * 1 + 328;
				int Direction2 = -sin(helpme) * 1 + 448;


				//cout << Direction1 << " and " << Direction2 << " equal " << TotalDirection << endl;//rand() % 20 + 10);
				if (PosOrNeg1 > 3) {
					Direction1 = Direction1 * -1;
				}
				if (PosOrNeg2 > 3) {
					Direction2 = Direction2 * -1;
				}
				//cout << Direction1 << " and " << Direction2 << " equal " << TotalDirection << endl;
				theSpaceJunks[junk]->setSpriteTranslation({ ((Direction1) / 10), (Direction2 / 10) });


				//theSpaceJunks[junk]->setSpritePos({ 340 , 340 });
				//int randDirection1 = rand() % 100 + 25;
				//int randDirection2 = rand() % 100 + 50;
				//theSpaceJunks[junk]->setSpriteTranslation({ randDirection1, randDirection2 });
				int randSpaceJunk = rand() % 3;
				theSpaceJunks[junk]->setTexture(theTextureMgr->getTexture(textureName[randSpaceJunk]));
				theSpaceJunks[junk]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randSpaceJunk])->getTWidth(), theTextureMgr->getTexture(textureName[randSpaceJunk])->getTHeight());
				theSpaceJunks[junk]->setSpaceJunkVelocity({ 0, 0 });
				theSpaceJunks[junk]->setActive(true);
			}
			totalTimePlaying += timerCurrent;
			timerCurrent = 0;
			//play = true;

		}
		//
		}
		

		


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
		
	//	if (play == false) {
		
		
			

	//	}
		
	
		for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
		{
			(*spaceJunkIterator)->update(deltaTime);
			for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
			{
				if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &theSpaceShip.getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*spaceJunkIterator)->setActive(false);
					//	(*bulletIterartor)->setActive(false);
					theSoundMgr->getSnd("crunch")->play(0);
					score++;
					shadowScore++;
					if (theTextureMgr->getTexture("Score") != NULL) {
						theTextureMgr->deleteTexture("Score");
					}
					string theScore = to_string(score);
					scoreAsString = "score: " + theScore;

					//cout << "Score is: "<< score;

					if (theTextureMgr->getTexture("Score2") != NULL) {
						theTextureMgr->deleteTexture("Score2");
					}
					string theScore2 = to_string(shadowScore);
					shadowScoreAsString = "score: " + theScore2;

					scoreChanged = true;
				
				
				
				}
			}
		}
		// Update the Rockets position
		theSpaceShip.update(deltaTime);
		

	/*	for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
		{
			(*spaceJunkIterator)->update(deltaTime);
			for (vector<cSpaceJunk*>::iterator spaceJunkIterator = theSpaceJunks.begin(); spaceJunkIterator != theSpaceJunks.end(); ++spaceJunkIterator)
			{
				if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &leftBorder.getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*spaceJunkIterator)->setActive(false);
					theGameState = END;
				}

				/*if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &topBorder.getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*spaceJunkIterator)->setActive(false);
					theGameState = END;
				}

				if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &rightBorder.getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*spaceJunkIterator)->setActive(false);
					//	(*bulletIterartor)->setActive(false);
					//theSoundMgr->getSnd("crunch")->play(0);
					//score++;
					theGameState = END;
					//shadowScore++;
					//if (theTextureMgr->getTexture("Score") != NULL) {
					//	theTextureMgr->deleteTexture("Score");
					//}
				//	string theScore = to_string(score);
				//	scoreAsString = "score: " + theScore;

					//cout << "Score is: "<< score;

				//	if (theTextureMgr->getTexture("Score2") != NULL) {
				//		theTextureMgr->deleteTexture("Score2");
				//	}
				//	string theScore2 = to_string(shadowScore);
			//		shadowScoreAsString = "score: " + theScore2;
			//		scoreChanged = true;
				}

				if ((*spaceJunkIterator)->collidedWith(&(*spaceJunkIterator)->getBoundingRect(), &bottomBorder.getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*spaceJunkIterator)->setActive(false);
					theGameState = END;
				}
			}
		}*/

		theGameState = theButtonMgr->getBtn("stop_btn")->update(theGameState, END, theAreaClicked);
	}
				  break;
	case END: {
		score = 0;
		shadowScore = 0;
		
		//for (int single = 0; single < 0.5; single++) {
	//	if (theTextureMgr->getTexture("Score") != NULL) {
	//		theTextureMgr->deleteTexture("Score");
	//	}
	//	string theScore = to_string(score);
	//	scoreAsString = "score: " + theScore;
	//}
	
	

		theSpaceJunks.clear();
		timerCurrent = 3;
		totalTimePlaying = 3;
		play = false;
		theGameState = theButtonMgr->getBtn("replay_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		
	}
			  break;
	
	}
	
	//if (play == true) {
	

	
	//}

	
	
	
	




	
	// Update the visibility and position of each bullet
	/*vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
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
	}*/
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	


	
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
		
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_LCTRL: 

				isPressed = false;
			}

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
					theSpaceShip.circleT += 45.0f * (PI / 180);
				}
				else {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT += 3.0f * (PI / 180);
				}

			}
			break;


			case SDLK_LEFT:
			{
				if (isPressed == true) {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT -= 45.0f * (PI / 180);
				} //* elapsedTime * 1000.0f;
				else {
					float elapsedTime = (float)this->getElapsedSeconds();
					theSpaceShip.circleT -= 3.0f * (PI / 180);
				} //* elapsedTime * 1000.0f;
				
			}
			break;

			//case SDLK_LCTRL:
			//{
				
			//	if (isPressed == true) {
				//	theSpaceShip.setTexture{};
			//	isPressed = false; }
			//	else { isPressed = true;
				//theSpaceShip.setTexture{};
			//	}

		//	}
			break;

			case SDLK_LCTRL:
			{
				isPressed = true;
				/*theBullets.push_back(new cBullet);
				int numBullets = theBullets.size() - 1;
				theBullets[numBullets]->setSpritePos({ theSpaceShip.getBoundingRect().x + theSpaceShip.getSpriteCentre().x - 2, theSpaceShip.getBoundingRect().y + theSpaceShip.getSpriteCentre().y - 12});
				theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
				theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
				theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				theBullets[numBullets]->setBulletVelocity({ 0, 0 });
				theBullets[numBullets]->setSpriteRotAngle(theSpaceShip.getSpriteRotAngle());
				theBullets[numBullets]->setActive(true);
				cout << "Bullet added to Vector at position - x: " << theSpaceShip.getBoundingRect().x << " y: " << theSpaceShip.getBoundingRect().y << endl;
			*/}
		//	theSoundMgr->getSnd("shot")->play(0);
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

