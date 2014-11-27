#include "Room.h"
#include <iostream>

using std::cout;
Coord Room::roomSize = Coord(45,25);

Room::Room():
entities(EntityVector2d(45, EntityVector(25, new Air())))
{
	for (int i = 0; i < 4; i++)
		doorDirections[i] = Direction::Dir_Error;

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


Room::~Room()
{
}
