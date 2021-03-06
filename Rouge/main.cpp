#include "Functions.h"
#include "Menu.h"
#include <Windows.h>
#include <iostream>
#include "Constants.h"
#include "Room.h"
#include "Config.h"

using std::cout;
using CommandsCoords = std::pair<Coord, int>;
using CommandsCoordsVec = std::vector<CommandsCoords>;

Coord MenuMouseProc(MOUSE_EVENT_RECORD mouseEvent, bool& mouseClicked);
void ShowConsoleCursor(bool showFlag);

DWORD oldWindowMode;

const int BOARD_UPDATE_TIME = 800;

//=============================================
//CONFIG RELATED STUFF

namespace Configs
{
	//CONFIG FILES:
	
	//Store controls for the game
	Config controls(L"controls");
	
	//CONFIG FILE OPERATIONS:
	void controlSetup();
}
//=============================================


//=============================================
//MENU RELATED STUFF

namespace Menus
{
	const Coord MenuCoord(30, 10);

	//VARIOUS MENUS:

	Menu mainMenu(MenuCoord);
	Menu newGame(MenuCoord, &mainMenu);
	Menu options(MenuCoord, &mainMenu);
	Menu quit(Coord(0, 0), nullptr, GameState::Quitting);
	
	Menu difficulty(MenuCoord, &options);
	
	Menu startGame(Coord(0, 0), nullptr, GameState::InGame);

	//MENU RELATED OPERATIONS:

	void mainMenuSetup();
	void optionsSetup();
	void newGameSetup();
}

using Menus::MenuCoord;

//=============================================

int main()
{
	SetDefaultColour();
	
	ShowConsoleCursor(false);

	INPUT_RECORD inputBuffer[128];
	DWORD numInput;
	
	GameState state = InMenu;

	GetConsoleMode(Global::hStdin, &oldWindowMode);
	SetConsoleMode(Global::hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	Menus:: mainMenuSetup();
	Menus::optionsSetup();
	Menus::newGameSetup();

	Menu* tempMenu = &(Menus::mainMenu);
	Menu* prevMenu = nullptr;
	Coord tempCoord(0,0);
	Coord curCoord(0, 0);
	Coord curHiCoord(0, 0);

	tempMenu->drawMenu();
	bool mouseClicked = false;
	std::string curMenuName = Constants::ErrorMenu;
	state = tempMenu->state;

	while (state == GameState::InMenu)
	{
		curMenuName = Constants::ErrorMenu;

		if (!ReadConsoleInput(
			Global::hStdin,
			inputBuffer,
			128,
			&numInput)	)
		{
			state = GameState::State_Error;
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

			state = tempMenu->state;

			prevMenu = tempMenu->getPrevMenu();
			tempMenu->drawMenu();
		}
	}

	Room room1;
	Room room2;

	room1.setRoom(Dir_Up, &room2);
	
	while (state == GameState::InGame)
	{		
		room1.drawRoom();
		Sleep(BOARD_UPDATE_TIME);
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

//Set if the horizontal-blinking cursor is visible or not
void ShowConsoleCursor(bool showFlag)
{
	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(Global::hStdout, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(Global::hStdout, &cursorInfo);
}


//CONFIG OPERATORS:
void Configs::controlSetup()
{
	controls.readFromFile();

	controls.addLine("Up", KeyVariant(Key_W));
	controls.addLine("Down", KeyVariant(Key_S));
	controls.addLine("Left", KeyVariant(Key_A));
	controls.addLine("Right", KeyVariant(Key_D));

	controls.writeToFile();
}

//MENU OPERATOR:
void Menus::mainMenuSetup()
{
	mainMenu.addCommand("New Game");
	mainMenu.addCommand("Options");
	mainMenu.addCommand("Quit");

	mainMenu.addChildMenu(0, &newGame);
	mainMenu.addChildMenu(1, &options);
	mainMenu.addChildMenu(2, &quit);
}

void Menus::optionsSetup()
{
	options.addCommand("Difficulty");

	options.addChildMenu(0, &difficulty);
}

void Menus::newGameSetup()
{
	newGame.addCommand("Begin Game");
	
	newGame.addChildMenu(0, &startGame);
}