#pragma once
#ifndef _AIR_H_
#define _AIR_H_

#include "Entity.h"
class Air :
	public Entity
{
public:
	Air();
	void moveEntity();
	~Air();
};

#endif