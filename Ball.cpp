#include "Ball.h"
#include "Game.h"

Ball::Ball(float size, string iname):Sprite( iname )
{
	rect.w = rect.h = size;
	color.b = 0;
}

void Ball::move()
{
	if( !vel.x ) vel.x = randFloat( -1, 1 );
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

	// hitting the paddles
	if( rect.hit( game->getLeftPaddle()->rect ) )
	{
		puts( "Ping" ) ;
		vel.x = -vel.x;
		vel *= 1.1f;
	}

	if( rect.hit( game->getRightPaddle()->rect ) )
	{
		puts( "Pong" ) ;
		vel.x = -vel.x;
		vel *= 1.1f;
	}
}