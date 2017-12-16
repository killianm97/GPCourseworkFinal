#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "oGCGame.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();
	
	static cGame* getInstance();

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	bool loop;
	bool play;
	float timerCurrent;
	float timerTotal;
	float totalTimePlaying;
	int difficulty;


	bool isPlaying;
	int score;
	int shadowScore;
	bool scoreChanged;
	string scoreAsString;
	string shadowScoreAsString;

	int newRecentScore;
	cFileHandler scoreFile;
	bool fileAvailable;


	//josh help
//	bool latestscores;
	string scoreAsString3;
	string NewScoreAsString;
	//int latestscore;
	//bool newscoreChanged;

	// Sprites for displaying background and rocket textures
	gameState theGameState;
	cSprite topBanner;
	cSprite topBorder;
	cSprite bottomBorder;
	cSprite leftBorder;
	cSprite rightBorder;
	cSprite orangeBackground1;
	cSprite orangeBackground2;
	cSprite orangeBackground3;
	cSprite earth;
	cSprite rightArrowKey;
	cSprite leftArrowKey;
	cSprite controlKey;
	cSprite endScreen;
	cSprite spriteBkgd;
	cSpaceShip theSpaceShip;
	cSpaceJunk theSpaceJunk;
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<cSpaceJunk*> theSpaceJunks;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextNames;
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Create vector array of button textures
	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	//vector <cButton> theButtons;
	// Game objects
	bool isPressed;
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	// Define the elements and there position in/on the array/map


//	int renderWidth, renderHeight;
	btnTypes theBtnType;
	SDL_Rect pos;
	FPoint scale;
	SDL_Rect aRect;
	SDL_Color aColour;
	cTexture* tempTextTexture;
	SDL_Point theAreaClicked;
};

#endif
