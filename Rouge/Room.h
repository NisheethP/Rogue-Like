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
#include <map>

class Room;

using EntityVector = std::vector<Entity*>;
using EntityVector2d = std::vector<EntityVector>;
using RoomDir = std::map<Direction, Room*>;

const Coord roomCoord = Coord(17,10);

class Room
{
	static Coord roomSize;
	EntityVector2d entities;
	RoomDir sideRooms;

public:
	Room();
	
	void drawRoom();
	
	bool hasAdjRoom(Direction dir);

	bool setRoom(Direction dir,Room* room);
	bool setDoor(Direction dir);

	int numAdjRooms();
	~Room();
};

#endif