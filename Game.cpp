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
	font = TTF_OpenFont("Nobile-Bold.ttf", 128);
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
	flashesRem = 0;

	leftPaddle = new Paddle( Vector2f(25,100), 10, "left paddle" );
	rightPaddle = new Paddle( Vector2f(25,100), 10, "right paddle" );
	title = new Sprite( "title", "title.png" );
	title->color = Black;
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
	sdl->loadWavSound( "ping.wav" );
	sdl->loadWavSound( "ping1.wav" );
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
	GameState prevState = gameState;
	switch( newState )
	{
	case Title:
		// start the title music
		sdl->playMusic( "song 91 7.ogg" );
	 	break;

	case Running:
		// game song
		// if prevstate was Running, don't restart the music
		if( prevState == Paused )
		{
			info( "Music unpaused" );
			Mix_ResumeMusic(); // unpause the music
		}
		else if( prevState != JustScored )
			sdl->playMusic( "song 81 6.flac" );
		break;

	case Paused:
		info( "Music paused" );
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
		puts( "Unpause" );
		setState( Running );
		pausedText->hide();
	}
	else
	{
		puts( "Pause" );
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
	leftPaddle->move();
	rightPaddle->move();
	ball->move();

	// Check for collisions after each object moves
	checkForCollisions();
}

void Game::update()
{
	// Get controller inputs first:
	controller.update();

	// Now if we're in the "JustScored" state, then
	// the action pauses for a bit while the screen flashes
	if( gameState == JustScored )
	{
		flashesRem--;
		bkgColor = SDL_ColorMake( randInt(0,255), randInt(0,255), randInt(0,255), randInt(0,255) );
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
	sdl->setColor( bkgColor );
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

