#include "Menu.h"
#include <iostream>
using std::cout;


Menu::Menu(Coord p_InitCoord, Menu* p_PrevMenu, bool p_IsTerminating) :
initCoord(p_InitCoord),
prevMenu(p_PrevMenu),
childMenu(ChildMenus()),
commands(StringVector()),
isTerminating(p_IsTerminating)
{
	if (prevMenu != nullptr)
		childMenu["Back"] = prevMenu;
}

Coord Menu::backCoord = Coord(5, 45);

void Menu::drawMenu()
{
	ClearScreen();
	gotoxy(initCoord);
	Coord tempCoord = initCoord;
	for (int i = 0; i < commands.size(); i++)
	{
		gotoxy(tempCoord);
		if (i < commands.size())
			cout << commands[i];

		tempCoord.y += 2;
	}
	if (prevMenu != nullptr)
	{
		gotoxy(backCoord);
		cout << "Back";
	}
}

void Menu::addCommand(std::string str)
{
	commands.push_back(str);
}
bool Menu::addChildMenu(int commandNum, Menu* menu)
{
	if (commandNum < commands.size())
		childMenu[commands[commandNum]] = menu;
	else
		return false;

	return true;
}

Menu* Menu::getMenu(std::string str)
{
	if (childMenu.find(str) == childMenu.end())
		return nullptr;
	else
		return childMenu[str];
}

Menu* Menu::getMenu(Coord crd)
{
	bool isBackCrd = false;
	isBackCrd = (crd.y == 45);
	isBackCrd = (isBackCrd && crd.x >= 5 && crd.x <= 9);

	if (isBackCrd)
	{
		return prevMenu;
	}
	else
	{
		Coord delCoord = crd - initCoord;
		if (delCoord.y < 0 || commands.size() == 0)
			return nullptr;

		int strNum = delCoord.y / 2;
		std::string tempStr = commands[strNum];
		if (delCoord.x <= tempStr.size())
			return getMenu(tempStr);
		else
			return nullptr;
	}	
}

Menu* Menu::getPrevMenu()
{
	return prevMenu;
}

Menu::~Menu()
{
}