#include "Ball.h"
#include "Game.h"

Ball::Ball(float size, string iname):Sprite( iname )
{
	rect.w = rect.h = size;
	color.b = 0;
}

void Ball::move()
{
	if( fabsf( vel.x ) < 1 )
		vel.x = signum(vel.x);
	//if( !vel.x ) vel.x = randFloat( -1, 1 );
	rect.xy() += vel;
	
	// don't exit top or bottom
	boundTB();
	
	if( rect.left() < 0 )
	{
		// right player score
		game->leftPlayerScored();
	}
	if( rect.right() > sdl->getSize().x )
	{
		game->rightPlayerScored();
	}

}