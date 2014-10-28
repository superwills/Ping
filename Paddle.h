#ifndef PADDLE_H
#define PADDLE_H

#include "Sprite.h"

class Paddle : public Sprite
{
	float speed;

public:
	Paddle(Vector2f size, float iSpeed, string iname);
	void moveDown();
	void moveUp();
};

#endif