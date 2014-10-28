#include <stdio.h>
#include <string>
using namespace std;

#include "SDLO.h"
#include "Game.h"
#include "Sprite.h"
#include <time.h>

int main(int argc, char* args[])
{
	srand(time(0));	
	SDL sdl( "Ping", 800, 600 );
	Game game( sdl );
	bool gameRunning = true;
	
	// Game Loop: http://gameprogrammingpatterns.com/game-loop.html
	while( gameRunning )
	{
		// Read controller input
		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				gameRunning = false;
			}
			else if( e.type == SDL_KEYDOWN )
			{
				switch( e.key.keysym.scancode )
				{
				case SDL_SCANCODE_ESCAPE:
					gameRunning=false;
					break;
				}
			}
		}
		
		int x,y;
		SDL_GetRelativeMouseState( &x, &y );
		if( y < 0 )
			game.getRightPaddle()->moveUp();
		else if( y > 0 )
			game.getRightPaddle()->moveDown();

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		if( keystate[SDL_SCANCODE_UP] )
		{
			game.getLeftPaddle()->moveUp();
		}
		if( keystate[SDL_SCANCODE_DOWN] )
		{
			game.getLeftPaddle()->moveDown();
		}
		// Update the game, move the sprites, etc
		game.update();

		// Draw out the new state
		game.draw();
	}

	return 0;
}





