#pragma once
#ifndef _CONSTANTS_H_
#define _CONSTATNS_H_
#include <string>

class Constants
{
public:
	//ERROR STRINGS
	static std::string ErrorMenu;
	static std::string ErrorDiff;	

	//SHAPES FOR ENTITIES IN GAME
	static char Air;
	static char Wall;
	static char Door;
	static char Line_End;
	static char Equal;
};

#endif