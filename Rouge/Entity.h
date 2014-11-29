#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Functions.h"

enum EntityType
{
	BaseEntity,
	PlayerEntity,
	AirEntity,
	DoorEntity,
	WallEntity
};

class Entity
{
protected:
	char pegShape;
	Colour pegColour;
	EntityType type;
	bool isPlayer;
	bool isMoveable;
	Coord entitySize;		//The size when looking UP.
	Direction viewDir;
public:
	Entity
		(
		char p_PegShape = ' ',
		Colour p_PegColour = Colour::White,
		EntityType p_Type = EntityType::BaseEntity,
		bool p_IsPlayer = false, 
		bool p_IsMovable = false,
		Coord p_Size = Coord(1,1),
		Direction p_Dir = Direction::Dir_Up
		);
	
	virtual void moveEntity() = 0;
	char getPegShape();
	Coord getSize();
		

	~Entity();
};

#endif