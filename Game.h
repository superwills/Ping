#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "SDLO.h"
#include "Sprite.h"
#include "Paddle.h"
#include "Ball.h"
#include "Controller.h"

class Game
{
public:
	enum GameState { Title, Running, Paused, Exiting };

private:
	// The SDL object, which allows us to draw and play sounds
	SDL* sdl;
	
	// The 3 game objects we have in the game
	Paddle* leftPaddle;
	Paddle* rightPaddle;
	Ball* ball;
	SDL_Color bkgColor;	// the current background color

	// Keeping track of scores
	int leftScoreValue, rightScoreValue;
	Sprite *leftScoreSprite, *rightScoreSprite;
	
	// The font we use to draw the scores
	TTF_Font* font;

	// what mode the game is running in
	GameState gameState;
	
	// An object to represent the game controller
	Controller controller;

public:
	Game( SDL& iSdl );
	~Game();
	void leftPlayerScored();
	void rightPlayerScored();

	// change fsm state
	GameState getState();
	void setState( GameState newState );
	void resetBall();
	void drawScores();
	void checkForCollisions();
	void update();
	void draw();
};

#endif