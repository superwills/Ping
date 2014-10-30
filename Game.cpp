#include "Game.h"
#include "Colors.h"

Game::Game( SDL& iSdl ):
	leftScoreValue(0), rightScoreValue(0), leftScoreSprite(0), rightScoreSprite(0)
{
	// Link up the static reference to this Game object
	// inside Sprite:
	Sprite::game = this;
	sdl = &iSdl;
	gameState = Title;

	// Start up true-type fonts
	TTF_Init();
	font = TTF_OpenFont("Nobile-Bold.ttf", 224);
	if(!font)
	{
		error("TTF_OpenFont: %s", TTF_GetError());
	}
	
	// Start up the audio mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		error( "SDL_mixer Error: %s\n", Mix_GetError() );
	}
	
	// Create game sprites
	bkgColor = SDL_ColorMake( 0, 0, 40, 255 );

	leftPaddle = new Paddle( Vector2f(25,100), 10, "left paddle" );
	rightPaddle = new Paddle( Vector2f(25,100), 10, "right paddle" );
	
	// Set initial positions for player paddles
	float centerH = sdl->getSize().y/2.f - leftPaddle->rect.h/2;
	leftPaddle->setPos( Vector2f( leftPaddle->getPos().x, centerH ) );
	rightPaddle->setPos( Vector2f( sdl->getSize().x - rightPaddle->rect.w, centerH ) );
	
	// Create the ball
	ball = new Ball( 32, "the ball" );
	resetBall();

	// Load the music & play it
	sdl->playMusic( "song 81 6.flac" );
	
	// load the sfx. These sfx were created with SFXR
	// http://www.drpetter.se/project_sfxr.html
	sdl->loadWavSound( "ping.wav" );
	sdl->loadWavSound( "ping1.wav" );
}

Game::~Game()
{
	
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

Game::GameState Game::getState()
{
	return gameState;
}

void Game::setState( GameState newState )
{
	gameState = newState;
	switch( gameState )
	{
	case Title:
		// start the title music
	 	break;

	case Running:
		// game song
		sdl->playMusic( "song 91 7.ogg" );
		break;

	case Paused:
		// 
		break;

	case Exiting:
		break;
	}
}

void Game::resetBall()
{
	ball->setCenter( sdl->getSize()/2 );
	ball->vel = Vector2f::random(-7.5, 7.5);
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

void Game::checkForCollisions()
{
	bool ballHit = false;
	// check the ball's rect against the paddle's rects
	if( ball->rect.hit( leftPaddle->rect ) )
	{
		sdl->playSound( "ping.wav" );
		float overlap = leftPaddle->rect.right() - ball->rect.left();
		ball->rect.x += overlap;
		ballHit = true;
	}

	if( ball->rect.hit( rightPaddle->rect ) )
	{
		sdl->playSound( "ping1.wav" );
		float overlap = rightPaddle->rect.left() - ball->rect.right();
		ball->rect.x += overlap;
		ballHit = true;
	}

	// when the ball is hit the ball bounces and speeds up a bit
	if( ballHit )
	{
		ball->vel.x = -ball->vel.x;
		ball->vel *= 1.1f;
	}
}

void Game::update()
{
	// Get controller inputs first:
	controller.update();

	// Use the controller state to change gamestate
	if( controller.mouseY < 0 )
		rightPaddle->moveUp();
	else if( controller.mouseY > 0 )
		rightPaddle->moveDown();

	if( controller.keystate[SDL_SCANCODE_UP] )
		leftPaddle->moveUp();
	if( controller.keystate[SDL_SCANCODE_DOWN] )
		leftPaddle->moveDown();
	
	// let the game objects update themselves
	leftPaddle->move();
	rightPaddle->move();
	ball->move();

	// Check for collisions after each object moves
	checkForCollisions();
}

void Game::draw()
{
	sdl->setColor( bkgColor );
	SDL_RenderClear( sdl->renderer );
		
	leftPaddle->draw();
	rightPaddle->draw();
	ball->draw();
	
	leftScoreSprite->draw();
	rightScoreSprite->draw();
	
	SDL_RenderPresent( sdl->renderer );
}

