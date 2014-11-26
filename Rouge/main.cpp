#include "Functions.h"
#include "Menu.h"
#include <Windows.h>
#include <iostream>

Coord MenuMouseProc(MOUSE_EVENT_RECORD mouseEvent);
DWORD oldWindowMode;

int main()
{
	SetDefaultColour();
	
	INPUT_RECORD inputBuffer[128];
	DWORD numInput;

	GetConsoleMode(Global::hStdin, &oldWindowMode);
	SetConsoleMode(Global::hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	Menu mainMenu({ 10,5 });
	Menu newGame({ 0, 0 }, &mainMenu);
	Menu options({ 10, 5 }, &mainMenu);
	Menu quit(Coord(0,0),nullptr, true);

	mainMenu.addCommand("New Game");
	mainMenu.addCommand("Options");
	mainMenu.addCommand("Quit");

	options.addCommand("Difficulty");
	options.addCommand("Back");

	mainMenu.addChildMenu(0, &newGame);
	mainMenu.addChildMenu(1, &options);
	mainMenu.addChildMenu(2, &quit);

	options.addChildMenu(1, &mainMenu);

	Menu* tempMenu = &mainMenu;
	Menu* prevMenu = nullptr;
	Coord tempCoord = Coord(12,0);
	
	tempMenu->drawMenu();
	bool isInMenu = true;
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
				tempCoord = MenuMouseProc(inputBuffer[inpIter].Event.MouseEvent);
				break;
			default:
				break;
			}
		}
		
		if (tempMenu->getMenu(tempCoord) == nullptr)
		{
		}	
		else
		{
			tempMenu = tempMenu->getMenu(tempCoord);

			if (tempMenu->isTerminating)
				isInMenu = false;

			prevMenu = tempMenu->getPrevMenu();
			tempMenu->drawMenu();
		}		
	}
	
	std::cout << "\n\n\n";


	SetConsoleMode(Global::hStdin, oldWindowMode);
	PressAnyKey();
	return 0;
}

Coord MenuMouseProc(MOUSE_EVENT_RECORD mouseEvent)
{
	#ifndef MOUSE_HWHEELED
	#define MOUSE_HWHEELED 0x0008
	#endif
	switch (mouseEvent.dwEventFlags)
	{
	case 0:
		if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			return Coord(mouseEvent.dwMousePosition.X, mouseEvent.dwMousePosition.Y);
		}
	}

	return Coord(-5, -5);		//Negative coordinates are all treated as not being on any menu.
}