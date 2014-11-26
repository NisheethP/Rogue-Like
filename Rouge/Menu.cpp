#include "Menu.h"
#include <iostream>
#include "Constants.h"
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

std::string Menu::getMenuName(Coord crd)
{
	bool isBackCrd = false;
	isBackCrd = (crd.y == 45);
	isBackCrd = (isBackCrd && crd.x >= 5 && crd.x <= 9);

	if (isBackCrd)
	{
		return "Back";
	}
	else
	{
		Coord delCoord = crd - initCoord;
		if (delCoord.y < 0 || commands.size() == 0)
			return Constants::ErrorMenu;

		int strNum = delCoord.y / 2;
		if (strNum >= commands.size())
			return Constants::ErrorMenu;

		std::string tempStr = commands[strNum];

		if (delCoord.x < tempStr.size() && delCoord.x >= 0)
			return tempStr;
		else
			return Constants::ErrorMenu;
	}	
}

int Menu::getCommandLength(int commandNum)
{
	if (commandNum < 0 || commandNum >= commands.size())
		return 0;
	else
		return commands[commandNum].length();
}

int Menu::getNumChild()
{
	return childMenu.size();
}

Menu* Menu::getPrevMenu()
{
	return prevMenu;
}

Menu::~Menu()
{
}