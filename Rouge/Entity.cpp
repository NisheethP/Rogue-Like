#include "Entity.h"


Entity::Entity(char p_PegShape, Colour p_PegColour, EntityType p_Type, bool p_IsPlayer, bool p_IsMovable, Coord p_Size, Direction p_Dir) :
pegShape(p_PegShape),
pegColour(p_PegColour),
type(p_Type),
isPlayer(p_IsPlayer),
isMoveable(p_IsMovable),
entitySize(p_Size),
viewDir(p_Dir)
{}

char Entity::getPegShape()
{
	return pegShape;
}
Coord Entity::getSize()
{
	return entitySize;
}

Entity::~Entity()
{
}
