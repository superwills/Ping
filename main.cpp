#include <stdio.h>
#include <string>
using namespace std;

#include "SDLO.h"
#include "Game.h"
#include "Sprite.h"
#include <time.h>

//      ___                       ___           ___     
//     /\  \          ___        /\__\         /\  \    
//    /::\  \        /\  \      /::|  |       /::\  \   
//   /:/\:\  \       \:\  \    /:|:|  |      /:/\:\  \  
//  /::\~\:\  \      /::\__\  /:/|:|  |__   /:/  \:\  \ 
// /:/\:\ \:\__\  __/:/\/__/ /:/ |:| /\__\ /:/__/_\:\__\
// \/__\:\/:/  / /\/:/  /    \/__|:|/:/  / \:\  /\ \/__/
//      \::/  /  \::/__/         |:/:/  /   \:\ \:\__\  
//       \/__/    \:\__\         |::/  /     \:\/:/  /  
//                 \/__/         /:/  /       \::/  /   
//                               \/__/         \/__/    
// William Sherif
// 30/10/2014
//
int main(int argc, char* args[])
{
	// Seed the random number generator with the __application start__ time.
	// This puts the "starting point" in the random number list,
	// at a random position based on application start time
	// that makes the random numbers generated different on 
	// each run of the program
	srand(time(0));
	
	// Create our SDL window, name it "Ping"
	SDL sdl( "Ping", 800, 600 );
	
	// Construct our Game object, to retain gamestate information
	Game game( sdl );

	// Game Loop: http://gameprogrammingpatterns.com/game-loop.html
	while( game.getState() != Game::Exiting )
	{
		// Read controller input
		SDL_Event e;

		// SDL_PollEvent returns nonzero while there are more events
		while( SDL_PollEvent( &e ) )
		{
			if( e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
			{
				// the game will exit on the next frame
				game.setState( Game::Exiting );
			}
			else if( e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_P )
			{
				game.togglePause();
			}
			else if( e.type == SDL_KEYDOWN && game.getState() == Game::Title )
			{
				game.setState( Game::Running );
			}
		}
		
		// Update the game, update() the sprites, etc
		game.update();

		// Draw out the new state
		game.draw();
	}
	
	return 0;
}





