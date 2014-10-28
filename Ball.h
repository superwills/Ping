#ifndef PONG_H
#define PONG_H

#include "Sprite.h"

class Ball : public Sprite
{
public:
	Ball(float size, string iname);
	virtual void move();
};

#endif