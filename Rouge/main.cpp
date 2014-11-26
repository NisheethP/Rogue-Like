#include "Functions.h"
#include "Menu.h"
#include <Windows.h>
#include <iostream>
#include "Constants.h"


Coord MenuMouseProc(MOUSE_EVENT_RECORD mouseEvent, bool& mouseClicked);
DWORD oldWindowMode;

int main()
{
	SetDefaultColour();
	
	INPUT_RECORD inputBuffer[128];
	DWORD numInput;
	const Coord MenuCoord(30,10);

	GetConsoleMode(Global::hStdin, &oldWindowMode);
	SetConsoleMode(Global::hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	Menu mainMenu(MenuCoord);
	Menu newGame({ 0, 0 }, &mainMenu);
	Menu options(MenuCoord, &mainMenu);
	Menu quit(Coord(0,0),nullptr, true);
	Menu difficulty(MenuCoord, &options);

	mainMenu.addCommand("New Game");
	mainMenu.addCommand("Options");
	mainMenu.addCommand("Quit");

	options.addCommand("Difficulty");
	//options.addCommand("Back");

	mainMenu.addChildMenu(0, &newGame);
	mainMenu.addChildMenu(1, &options);
	mainMenu.addChildMenu(2, &quit);

	options.addChildMenu(0, &difficulty);

	Menu* tempMenu = &mainMenu;
	Menu* prevMenu = nullptr;
	Coord tempCoord(0,0);
	Coord curCoord(0, 0);
	
	tempMenu->drawMenu();
	bool isInMenu = true;
	bool mouseClicked = false;
	while (isInMenu)
	{
		if (!ReadConsoleInput(
			Global::hStdin,
			inputBuffer,
			128,
			&numInput)	)
		{
			isInMenu = false;
			std::cout << "\n\nERROR IN READ INPUT";
		}
		for (int inpIter = 0; inpIter < numInput; inpIter++)
		{
			switch (inputBuffer[inpIter].EventType)
			{
			case MOUSE_EVENT:
				tempCoord = MenuMouseProc(inputBuffer[inpIter].Event.MouseEvent, mouseClicked);
				break;
			default:
				break;
			}
		}
		if (mouseClicked)
			curCoord = tempCoord;

		std::string curMenuName = tempMenu->getMenuName(curCoord);
		if (!mouseClicked)
		{
			int menuLen = tempMenu->getMenuName(tempCoord).length();
			if (tempMenu->getMenuName(tempCoord) != Constants::ErrorMenu)
				SetColour(tempCoord, menuLen);

		}
		
		if (curMenuName != Constants::ErrorMenu)
		{
			tempMenu = tempMenu->getMenu(curMenuName);

			if (tempMenu->isTerminating)
				isInMenu = false;

			prevMenu = tempMenu->getPrevMenu();
			tempMenu->drawMenu();
		}
		SetColour(tempCoord, 40, Colour::White, Colour::None);
	}
	
	std::cout << "\n\n\n";


	SetConsoleMode(Global::hStdin, oldWindowMode);
	PressAnyKey();
	return 0;
}

Coord MenuMouseProc(MOUSE_EVENT_RECORD mouseEvent, bool& mouseClicked)
{
	#ifndef MOUSE_HWHEELED
	#define MOUSE_HWHEELED 0x0008
	#endif
	
	mouseClicked = false;

	switch (mouseEvent.dwEventFlags)
	{
	case 0:
		if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			mouseClicked = true;
			return Coord(mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
		}
	case MOUSE_MOVED:
		return Coord(mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
	default:
		break;
	}

	return Coord(-5, -5);		//Negative coordinates are all treated as not being on any menu.
}