#pragma once
#ifndef _ROOM_H_
#define _ROOM_H_

#include "Air.h"
#include "Door.h"
#include "Player.h"
#include "Sword.h"
#include "Wall.h"
#include "Entity.h"
#include <vector>

using EntityVector = std::vector<Entity*>;
using EntityVector2d = std::vector<EntityVector>;

const Coord roomCoord = Coord(17,10);

class Room
{
	static Coord roomSize;
	EntityVector2d entities;
	Direction doorDirections[4];

public:
	Room();
	void drawRoom();
	~Room();
};

#endif