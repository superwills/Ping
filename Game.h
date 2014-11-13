#ifndef GAME_H
#define GAME_H

#include "SDLO.h"
#include "Sprite.h"
#include "Paddle.h"
#include "Ball.h"
#include "Controller.h"
#include "TitleScreen.h"

class Game
{
public:
	enum GameState { Title, Running, JustScored, Paused, Exiting };
	
private:
	// The SDL object, which allows us to draw and play sounds
	SDL* sdl;
	
	// Need a ball and paddles for this game
	Ball* ball;
	Paddle* leftPaddle;
	Paddle* rightPaddle;

	TitleScreen* title;
	Sprite* pausedText;
	SDL_Color bkgColor;	// the current background color
	int flashesRem;

	// Keeping track of scores
	int leftScoreValue, rightScoreValue;
	Sprite *leftScoreSprite, *rightScoreSprite;
	
	// The font we use to draw the scores
	TTF_Font* font;
	
	// The previous state the game was in (used for unpausing)
	GameState prevState;

	// what mode the game is running in
	GameState gameState;
	
	// An object to represent the game controller
	Controller controller;

public:
	Game( SDL& iSdl );
	~Game();
	void leftPlayerScored();
	void rightPlayerScored();
	GameState getState();
	// change fsm state
	void setState( GameState newState );
	void togglePause();
	void resetBall();
	void drawScores();
	void checkForCollisions();
	void runGame();
	void update();
	void draw();
};

#endif