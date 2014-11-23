#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Functions.h"

enum EntityType
{
	BaseEntity,
	Player
};

class Entity
{
	char pegShape;
	Colour pegColour;
	EntityType type;
	bool isPlayer;
	bool isMoveable;
public:
	Entity
		(
		char p_PegShape = ' ',
		Colour p_PegColour = Colour::White,
		EntityType p_Type = EntityType::BaseEntity,
		bool p_IsPlayer = false, 
		bool p_IsMovable = false
		);
	
	virtual void moveEntity() = 0;
	~Entity();
};

#endif