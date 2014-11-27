#pragma once
#ifndef _WALL_H_
#define _WALL_H_

#include "Entity.h"
class Wall :
	public Entity
{
public:
	Wall();
	void moveEntity();
	~Wall();
};

#endif