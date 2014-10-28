#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "SDLO.h"
#include "Sprite.h"
#include "Paddle.h"
#include "Ball.h"

class Game
{
	SDL* sdl;
	Sprite* bkg;
	Paddle* leftPaddle;
	Paddle* rightPaddle;
	Ball* ball;

	int leftScoreValue, rightScoreValue;
	Sprite *leftScoreSprite, *rightScoreSprite;

	TTF_Font *font;

public:
	Game( SDL& iSdl );
	~Game();
	void leftPlayerScored();
	void rightPlayerScored();
	Paddle *getLeftPaddle()
	{
		return leftPaddle;
	}
	Paddle *getRightPaddle()
	{
		return rightPaddle;
	}

	void resetBall();
	void drawScores();
	void update();
	void draw();
};

#endif