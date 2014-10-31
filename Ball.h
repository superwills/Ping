#ifndef PONG_H
#define PONG_H

#include "Sprite.h"

class Ball : public Sprite
{
public:
	float lastStartSpeed;
	Ball(float size, string iname);
	void saveLastStartSpeed();
	virtual void update();
};

#endif