#include "Room.h"
#include <iostream>

using std::cout;
Coord Room::roomSize = Coord(45,25);

const Direction allDir[5] = { Dir_Up, Dir_Down, Dir_Left, Dir_Right, Dir_Error };

Room::Room():
entities(EntityVector2d(45, EntityVector(25, new Air())))
{
	for (int i = 0; i < roomSize.y; i++)
	{
		entities[0][i] = new Wall();
		entities[roomSize.x-1][i] = new Wall();
	}

	for (int i = 0; i < roomSize.x; i++)
	{
		entities[i][0] = new Wall();
		entities[i][roomSize.y - 1] = new Wall();
	}

	for (int i = 0; i < 5; i++)
	{
		Direction curDir = allDir[i];
		sideRooms[curDir] = nullptr;
	}

	/*sideRooms[Dir_Up] = nullptr;
	sideRooms[Dir_Down] = nullptr;
	sideRooms[Dir_Left] = nullptr;
	sideRooms[Dir_Right] = nullptr;
	sideRooms[Dir_Error] = nullptr;*/
}

void Room::drawRoom()
{
	Coord tempCoord = roomCoord;
	gotoxy(roomCoord.x, roomCoord.y);
	for (int row = 0; row < roomSize.y; row++)
	{
		for (int col = 0; col < roomSize.x; col++)
		{
			tempCoord = Coord(col, row) + roomCoord;
			gotoxy(tempCoord.x, tempCoord.y);
			cout << ' ';
		}
	}

	gotoxy(roomCoord.x, roomCoord.y);
	tempCoord = roomCoord;

	for (int row = 0; row < roomSize.y; row++)
	{
		for (int col = 0; col < roomSize.x; col++)
		{
			tempCoord = Coord(col, row) + roomCoord;
			gotoxy(tempCoord.x, tempCoord.y);
			cout << entities[col][row]->getPegShape();
		}
	}
}

int Room::numAdjRooms()
{
	int numRooms = 0;
	for (int i = 0; i < sideRooms.size(); i++)
	{
		Direction curDir = allDir[i];
		if (sideRooms[curDir] != nullptr)
			numRooms++;
	}

	return numRooms;
}

bool Room::hasAdjRoom(Direction dir)
{
	if (sideRooms[dir] == nullptr)
		return false;

	return true;
}

bool Room::setRoom(Direction dir, Room* room)
{
	if (dir == Direction::Dir_Error)
		return false;
	else
	{
		sideRooms[dir] = room;
		setDoor(dir);
	}	

	return true;
}

bool Room::setDoor(Direction dir)
{
	Door tempDoor;

	if (dir == Dir_Error)
		return false;

	Coord doorCoord(0, 0);
	Coord centreCoord(roomSize);

	centreCoord -= Coord(1, 1);
	centreCoord.x /= 2;
	centreCoord.y /= 2;
	
	switch (dir)
	{
	case Dir_Up:
		doorCoord.x = centreCoord.x;
		break;
	case Dir_Down:
		doorCoord.x = centreCoord.x;
		doorCoord.y = roomSize.y - 1;
		break;
	case Dir_Left:
		doorCoord.y = centreCoord.y;
		break;
	case Dir_Right:
		doorCoord.x = roomSize.x - 1;
		doorCoord.y = centreCoord.y;
		break;
	}

	//doorCoord -= (tempDoor.getSize() )/ 2;
	entities[doorCoord.x][doorCoord.y] = new Door();

	return true;
}

Room::~Room()
{
	for (int row = 0; row < roomSize.y; row++)
	{
		for (int col = 0; col < roomSize.x; col++)
		{
			delete entities[col][row];
		}
	}
}