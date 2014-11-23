#include "Entity.h"


Entity::Entity(char p_PegShape,Colour p_PegColour,EntityType p_Type,bool p_IsPlayer,bool p_IsMovable):
pegShape(p_PegShape),
pegColour(p_PegColour),
type(p_Type),
isPlayer(p_IsPlayer),
isMoveable(p_IsMovable)
{}


Entity::~Entity()
{
}
