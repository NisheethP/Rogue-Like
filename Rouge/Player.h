#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Entity.h"
class Player :
	public Entity
{
public:
	Player();
	void moveEntity();
	~Player();
};

#endif