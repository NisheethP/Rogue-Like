#include "Functions.h"
#include "Menu.h"
#include <Windows.h>
#include <iostream>
#include "Constants.h"

using CommandsCoords = std::pair<Coord, int>;
using CommandsCoordsVec = std::vector<CommandsCoords>;

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
	
	mainMenu.addChildMenu(0, &newGame);
	mainMenu.addChildMenu(1, &options);
	mainMenu.addChildMenu(2, &quit);

	options.addChildMenu(0, &difficulty);

	Menu* tempMenu = &mainMenu;
	Menu* prevMenu = nullptr;
	Coord tempCoord(0,0);
	Coord curCoord(0, 0);
	Coord curHiCoord(0, 0);

	tempMenu->drawMenu();
	bool isInMenu = true;
	bool mouseClicked = false;
	std::string curMenuName = Constants::ErrorMenu;
	while (isInMenu)
	{
		curMenuName = Constants::ErrorMenu;

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
		{
			curCoord = tempCoord;
			curMenuName = tempMenu->getMenuName(curCoord);
		}
		if (!mouseClicked)
		{
			CommandsCoordsVec commandCoords;
			Coord crd = tempMenu->getInitCoord();
			
			if (tempMenu->canGoBack() && tempCoord.y == Menu::backCoord.y)
			{
				if (tempCoord.x >= Menu::backCoord.x && tempCoord.x < Menu::backCoord.x + 4)
				{
					SetColour(curHiCoord, 40, Colour::White, Colour::None);

					curHiCoord = tempCoord;
					curHiCoord.x = Menu::backCoord.x;
					SetColour(curHiCoord, 4);
				}
				
			}
			else
			{
				int maxCoordIter = tempMenu->getNumChild();

				if (tempMenu->canGoBack())
					maxCoordIter--;
				for (int coordIter = 0; coordIter < maxCoordIter; coordIter++)
				{
					commandCoords.push_back({ crd, tempMenu->getMenuName(crd).length() });
					crd.y += 2;
				}

				for (int cit = 0; cit < maxCoordIter; cit++)
				{
					if (commandCoords[cit].first.y == tempCoord.y)
					{
						if (tempCoord.x >= commandCoords[cit].first.x && tempCoord.x < commandCoords[cit].first.x + commandCoords[cit].second)
						{
							int len = tempMenu->getMenuName(curHiCoord).length();
							SetColour(curHiCoord, len, Colour::White, Colour::None);

							curHiCoord = tempCoord;
							curHiCoord.x = tempMenu->getInitCoord().x;
							len = tempMenu->getMenuName(curHiCoord).length();
							SetColour(curHiCoord, len);
						}
					}
				}
			}						
		}
		
		if (curMenuName != Constants::ErrorMenu)
		{
			if (curMenuName == "Back")
				tempMenu = tempMenu->getPrevMenu();
			else
				tempMenu = tempMenu->getMenu(curMenuName);

			if (tempMenu->isTerminating)
				isInMenu = false;

			prevMenu = tempMenu->getPrevMenu();
			tempMenu->drawMenu();
		}
	}

	SetConsoleMode(Global::hStdin, oldWindowMode);
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