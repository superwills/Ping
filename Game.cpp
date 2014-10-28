#include "Game.h"
#include "Colors.h"

Game::Game( SDL& iSdl ):leftScoreValue(0),rightScoreValue(0), leftScoreSprite(0), rightScoreSprite(0)
{
	Sprite::game = this;
	sdl = &iSdl;
	TTF_Init();
	font=TTF_OpenFont("Nobile-Bold.ttf", 224);
	if(!font)
	{
		error("TTF_OpenFont: %s", TTF_GetError());
	}
	
	bkg = new Sprite( "background", "bkg.png" );
	leftPaddle = new Paddle( Vector2f(25,100), 10, "left paddle" );
	rightPaddle = new Paddle( Vector2f(25,100), 10, "right paddle" );
	
	float centerH = sdl->getSize().y/2.f - leftPaddle->rect.h/2;
	leftPaddle->setPos( Vector2f( leftPaddle->getPos().x, centerH ) );
	rightPaddle->setPos( Vector2f( sdl->getSize().x - rightPaddle->rect.w, centerH ) );

	ball = new Ball( 32, "the ball" );
	resetBall();
}

Game::~Game()
{
	delete bkg;
}

void Game::leftPlayerScored()
{
	leftScoreValue++;
	resetBall();
}

void Game::rightPlayerScored()
{
	rightScoreValue++;
	resetBall();
}

void Game::resetBall()
{
	ball->setCenter( sdl->getSize()/2 );
	ball->vel = Vector2f::random(-5,5);
	drawScores();
}

void Game::drawScores()
{
	if( leftScoreSprite ) delete leftScoreSprite;
	leftScoreSprite = Sprite::Text( font, makeString( "%d", leftScoreValue ), White );
	leftScoreSprite->scale( 0.25f );
	leftScoreSprite->setCenter( sdl->getSize().x/2 - leftScoreSprite->rect.w,
		leftScoreSprite->rect.h/2 );

	if( rightScoreSprite ) delete rightScoreSprite;
	rightScoreSprite = Sprite::Text( font, makeString( "%d", rightScoreValue ), White );
	rightScoreSprite->scale( 0.25f );
	rightScoreSprite->setCenter( sdl->getSize().x/2 + rightScoreSprite->rect.w,
		rightScoreSprite->rect.h/2 );

}

void Game::update()
{
	leftPaddle->move();
	rightPaddle->move();
	ball->move();
}

void Game::draw()
{
	SDL_RenderClear( sdl->renderer );
	bkg->draw();
	
	leftPaddle->draw();
	rightPaddle->draw();
	ball->draw();
	
	leftScoreSprite->draw();
	rightScoreSprite->draw();

	SDL_RenderPresent( sdl->renderer );
}

