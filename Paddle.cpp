#include "Paddle.h"

Paddle::Paddle(Vector2f size, float iSpeed, string iname):Sprite(iname)
{
	rect.w = size.x;
	rect.h = size.y;
	speed = iSpeed;
}

void Paddle::moveDown()
{
	rect.y += speed;
	boundTB();
}

void Paddle::moveUp()
{
	rect.y -= speed;
	boundTB();
}
