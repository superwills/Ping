#include "Game.h"
#include "Colors.h"

Game::Game( SDL& iSdl ):
	leftScoreValue(0), rightScoreValue(0), leftScoreSprite(0), rightScoreSprite(0)
{
	// Link up the static reference to this Game object inside Sprite:
	Sprite::game = this;
	sdl = &iSdl;
	gameState = Title;

	font = TTF_OpenFont("Nobile-Bold.ttf", 128);
	if(!font)
	{
		error("TTF_OpenFont: %s", TTF_GetError());
	}
	
	// Create game sprites
	bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
	flashesRem = 0;

	leftPaddle = new Paddle( Vector2f(25,100), 10, "left paddle" );
	rightPaddle = new Paddle( Vector2f(25,100), 10, "right paddle" );
	title = new TitleScreen( "title", "title.jpg" );
	pausedText = Sprite::Text( font, "pause", SDL_ColorMake( 200, 200, 0, 200 ) );
	pausedText->setCenter( sdl->getSize()/2 );

	// Set initial positions for player paddles
	float centerH = sdl->getSize().y/2.f - leftPaddle->rect.h/2;
	leftPaddle->setPos( Vector2f( leftPaddle->getPos().x, centerH ) );
	rightPaddle->setPos( Vector2f( sdl->getSize().x - rightPaddle->rect.w, centerH ) );
	
	// Create the ball
	ball = new Ball( 32, "the ball" );
	resetBall();
	ball->saveLastStartSpeed();
	
	// load the sfx. These sfx were created with SFXR
	// http://www.drpetter.se/project_sfxr.html
	sdl->loadWavSound( "ping0.wav" );
	sdl->loadWavSound( "ping1.wav" );
	sdl->loadWavSound( "ping2.wav" );
	sdl->loadWavSound( "ping3.wav" );
	sdl->loadWavSound( "win.wav" );
	
	setState( Title );
}

Game::~Game()
{
	
}

void Game::leftPlayerScored()
{
	leftScoreValue++;
	resetBall();
	gameState = JustScored;
	sdl->playSound( "win.wav" );
	flashesRem = 60;
}

void Game::rightPlayerScored()
{
	rightScoreValue++;
	resetBall();
	gameState = JustScored;
	sdl->playSound( "win.wav" );
	flashesRem = 60;
}

Game::GameState Game::getState()
{
	return gameState;
}

void Game::setState( GameState newState )
{
	prevState = gameState;
	
	switch( newState )
	{
	case Title:
		// start the title music
		sdl->playMusic( "song 91 7.ogg" );
	 	break;
	
	case Running:
	case JustScored:
		// game song
		// if prevstate was Running, don't restart the music
		if( prevState == Paused )
		{
			Mix_ResumeMusic(); // unpause the music
		}
		else if( prevState == Title )
		{
			// start the in-game music
			sdl->playMusic( "song 81 6.flac" );
		}
		break;
	
	case Paused:
		Mix_PauseMusic();
		break;
	
	case Exiting:
		break;
	}

	gameState = newState;
}

void Game::togglePause()
{
	if( gameState == Paused )
	{
		setState( prevState ); //go back to the previous state
		pausedText->hide();
	}
	else
	{
		setState( Paused );
		pausedText->show();
	}
}

void Game::resetBall()
{
	ball->setCenter( sdl->getSize()/2 );
	ball->vel = Vector2f::random(-1, 1);
	ball->vel.setLen( ball->lastStartSpeed * 2.f );
	drawScores();
}

void Game::drawScores()
{
	if( leftScoreSprite )
	{
		delete leftScoreSprite;
	}
	leftScoreSprite = Sprite::Text( font, makeString( "%d", leftScoreValue ), White );
	leftScoreSprite->scale( 0.48f );
	leftScoreSprite->setCenter( sdl->getSize().x/2 - leftScoreSprite->rect.w,
		leftScoreSprite->rect.h/2 );

	if( rightScoreSprite )
	{
		delete rightScoreSprite;
	}
	rightScoreSprite = Sprite::Text( font, makeString( "%d", rightScoreValue ), White );
	rightScoreSprite->scale( 0.48f );
	rightScoreSprite->setCenter( sdl->getSize().x/2 + rightScoreSprite->rect.w,
		rightScoreSprite->rect.h/2 );
}

void Game::checkForCollisions()
{
	bool ballHit = false;
	// check the ball's rect against the paddle's rects
	if( ball->rect.hit( leftPaddle->rect ) )
	{
		sdl->playSound( makeString( "ping%d.wav", randInt(0,2) ) );
		
		// Push the ball off the paddle, so they don't interpenetrate
		float overlap = leftPaddle->rect.right() - ball->rect.left();
		ball->rect.x += overlap;

		// let the bounce angle be proportional to distance from center paddle
		float y = ball->rect.centroid().y - leftPaddle->rect.centroid().y;
		float a = M_PI/2.f * y/leftPaddle->rect.h;
		ball->vel.setAngle( a );
		
		ballHit = true;
	}

	if( ball->rect.hit( rightPaddle->rect ) )
	{
		sdl->playSound( makeString( "ping%d.wav", randInt(0,2) ) );
		float overlap = rightPaddle->rect.left() - ball->rect.right();
		ball->rect.x += overlap;
		
		float y = rightPaddle->rect.centroid().y - ball->rect.centroid().y;
		float a = M_PI + M_PI/2.f * y/rightPaddle->rect.h;
		ball->vel.setAngle( a );
		
		ballHit = true;
	}

	// when the ball is hit the ball bounces and speeds up a bit
	if( ballHit )
	{
		ball->vel *= 1.2f;
	}
}

void Game::runGame()
{
	// Use the controller state to change gamestate
	if( controller.mouseY < 0 || controller.keystate[SDL_SCANCODE_UP] )
		rightPaddle->moveUp();
	else if( controller.mouseY > 0 || controller.keystate[SDL_SCANCODE_DOWN] )
		rightPaddle->moveDown();

	if( controller.keystate[SDL_SCANCODE_W] )
		leftPaddle->moveUp();
	if( controller.keystate[SDL_SCANCODE_S] )
		leftPaddle->moveDown();
	
	// let the game objects update themselves
	leftPaddle->update();
	rightPaddle->update();
	ball->update();
	
	// Check for collisions after each object moves
	checkForCollisions();
}

void Game::update()
{
	// Get controller inputs first:
	controller.update();

	if( gameState == Title )
	{
		title->update();
	}
	else if( gameState == JustScored )
	{
		// Now if we're in the "JustScored" state, then
		// the action pauses for a bit while the screen flashes
		flashesRem--;
		// change the color only every few frames
		if( every(flashesRem,3) )
		{
			bkgColor = SDL_ColorMake( randInt(0,255), randInt(0,255), randInt(0,255), randInt(0,255) );
		}
		if( !flashesRem )
		{
			bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
			setState( Running );
		}
	}
	else if( gameState == Running )
	{
		runGame();
	}
}

void Game::draw()
{
	// Set the background's color,
	sdl->setColor( bkgColor );

	// clears the bkg to bkgColor
	SDL_RenderClear( sdl->renderer );
	
	if( gameState == Title )
	{
		title->draw();
	}
	else
	{
		leftPaddle->draw();
		rightPaddle->draw();
		ball->draw();
		leftScoreSprite->draw();
		rightScoreSprite->draw();
	}
	
	if( gameState == Paused )
	{
		pausedText->draw();
	}
	
	SDL_RenderPresent( sdl->renderer );
}

