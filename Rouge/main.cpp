#include "Functions.h"
#include "Menu.h"
#include "MainMenu.h"
#include <iostream>

int main()
{
	SetDefaultColour();
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

	options.addChildMenu(1, &mainMenu);

	Menu* tempMenu = &mainMenu;
	Menu* prevMenu = nullptr;
	Coord tempCoord = Coord(12,0);
	
	tempMenu->drawMenu();
	bool isInMenu = true;
	while (isInMenu)
	{
		std::cin >> tempCoord.y;
		//PressAnyKey();
		
		
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

	PressAnyKey();
	return 0;
}