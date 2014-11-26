#pragma once
#ifndef _MENU_H_
#define _MENU_H_
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "Functions.h"

class Menu;

using StringVector = std::vector<std::string>;
using ChildMenus = std::map<std::string, Menu*>;

class Menu
{
protected:
	Coord initCoord;
	Menu* prevMenu;
	ChildMenus childMenu;
	StringVector commands;
	static Coord backCoord;

public:
	const bool isTerminating;

	void addCommand(std::string str);
	bool addChildMenu(int commandNum, Menu* menu);
	
	Menu(Coord p_InitCoord = { 0, 0 }, Menu* p_PrevMenu = nullptr, bool p_IsTerminating = false);
	void drawMenu();
	Menu* getMenu(std::string str);
	std::string getMenuName(Coord crd);
	Menu* getPrevMenu();

	int getNumChild();
	int getCommandLength(int commandNum);

	~Menu();
};

#endif