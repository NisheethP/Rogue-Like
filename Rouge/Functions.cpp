#include "Functions.h"
#include <iostream>
#include <cctype>
#include "Constants.h"


HANDLE Global::hStdin = GetStdHandle(STD_INPUT_HANDLE);
HANDLE Global::hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
int PressAnyKey(const char *prompt)
{
	DWORD        mode;
	HANDLE       hstdin;
	INPUT_RECORD inrec;
	DWORD        count;
	char         default_prompt[] = "Press a key to continue...";

	/* Set the console mode to no-echo, raw input, */
	/* and no window or mouse events.              */
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hstdin == INVALID_HANDLE_VALUE
		|| !GetConsoleMode(hstdin, &mode)
		|| !SetConsoleMode(hstdin, 0))
		return 0;

	if (!prompt) prompt = default_prompt;

	std::cout << prompt;

	FlushConsoleInputBuffer(hstdin);

	/* Get a single key RELEASE */
	do ReadConsoleInput(hstdin, &inrec, 1, &count);
	while ((inrec.EventType != KEY_EVENT) || inrec.Event.KeyEvent.bKeyDown);

	/* Restore the original console mode */
	SetConsoleMode(hstdin, mode);

	return inrec.Event.KeyEvent.wVirtualKeyCode;
}

void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
		);
}
int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	return csbi.dwCursorPosition.Y;
}
int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
		))
		return -1;
	return csbi.dwCursorPosition.Y;
}
void gotoxy(Coord crd)
{
	gotoxy(crd.x, crd.y);
}

WORD ColourToFore(Colour p_Colour)
{
	WORD fore;
	switch (p_Colour)
	{
	case 0:
		fore = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		break;
	case 1:
		fore = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case 2:
		fore = FOREGROUND_RED;
		break;
	case 3:
		fore = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case 4:
		fore = FOREGROUND_BLUE;
		break;
	case 5:
		fore = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case 6:
		fore = FOREGROUND_GREEN;
		break;
	case 7:
		fore = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case 8:
		fore = FOREGROUND_BLUE | FOREGROUND_GREEN;
		break;
	case 9:
		fore = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case 10:
		fore = FOREGROUND_BLUE | FOREGROUND_RED;
		break;
	case 11:
		fore = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case 12:
		fore = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case 13:
		fore = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	}

	return fore;
}
WORD ColourToBack(Colour p_Colour)
{
	WORD back;
	switch (p_Colour)
	{
	case 0:
		back = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;
		break;
	case 1:
		back = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	case 2:
		back = BACKGROUND_RED;
		break;
	case 3:
		back = BACKGROUND_RED | BACKGROUND_INTENSITY;
		break;
	case 4:
		back = BACKGROUND_BLUE;
		break;
	case 5:
		back = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		break;
	case 6:
		back = BACKGROUND_GREEN;
		break;
	case 7:
		back = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	case 8:
		back = BACKGROUND_BLUE | BACKGROUND_GREEN;
		break;
	case 9:
		back = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	case 10:
		back = BACKGROUND_BLUE | BACKGROUND_RED;
		break;
	case 11:
		back = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
		break;
	case 12:
		back = BACKGROUND_RED | BACKGROUND_GREEN;
		break;
	case 13:
		back = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		break;
	}

	return back;
}

void SetDefaultColour(Colour pcolour)
{
	WORD fore = ColourToFore(pcolour);
	SetConsoleTextAttribute(Global::hStdout, fore);
}
void SetColour(Coord crd, int length, Colour Fore, Colour Back)
{
	COORD cord;
	DWORD wrd;
	cord.X = crd.x;
	cord.Y = crd.y;

	if (Back == Colour::None)
	{
		WORD fore = ColourToFore(Fore);
		FillConsoleOutputAttribute(Global::hStdout, fore, length, cord, &wrd);
	}
	else
	{
		WORD fore = ColourToFore(Fore);
		WORD back = ColourToBack(Back);
		FillConsoleOutputAttribute(Global::hStdout, fore | back, length, cord, &wrd);
	}

	//cout << endl;
	//cout << wrd;
}

KeyInput getArrowInput()
{
	char x = _getch();
	char y;
	bool isArrow = false;
	KeyPress arrowKey;

	//_getch() puts two numbers in the buffer if one of the arrow keys is pressed. One of them is constant
	//char(224) is the constant part identifying it as an ARROW KEY
	//The second _getch() won't ever trigger as the number in the buffer will simply be returned to y.

	if (x == (char)224)
	{
		y = _getch();

		switch (y)
		{
		case 72 /*UP*/:
			arrowKey = KeyPress::Key_Up;
			break;
		case 75 /*LEFT*/:
			arrowKey = KeyPress::Key_Left;
			break;
		case 77 /*RIGHT*/:
			arrowKey = KeyPress::Key_Right;
			break;
		case 80 /*DOWN*/:
			arrowKey = KeyPress::Key_Down;
			break;
		default:
			arrowKey = KeyPress::Key_Other;
			break;
		}

		isArrow = true;
	}
	else
	{
		y = 1;
		char_To_KeyPress(arrowKey, x);
	}

	return{ x, arrowKey, isArrow };
}

Coord DirToNum(Direction pDir)
{
	switch(pDir)
	{
	case Dir_Up:
		return {0,-1};
		break;
	case Dir_Down:
		return{ 0, 1 };
		break;
	case Dir_Left:
		return{ -1, 0 };
		break;
	case Dir_Right:
		return{ 1, 0 };
		break;
	default:
		break;
	}
	return{ 0, 0 };
}

//Gives the Direction opposite to current direction
Direction oppDir(Direction pDir)
{
	Direction rDir;
	switch (pDir)
	{
	case Dir_Right:
		rDir = Dir_Left;
		break;
	case Dir_Left:
		rDir = Dir_Right;
		break;
	case Dir_Down:
		rDir = Dir_Up;
		break;
	case Dir_Up:
		rDir = Dir_Down;
		break;
	case Dir_Error:
	default:
		break;
	}

	return rDir;
}

//================================================
//Conversion Functions for Read and Write to File
//================================================

bool keyPress_To_Char(KeyPress key, char& chr)
{
	switch (key)
	{
	case Key_Down:
	case Key_Up:
	case Key_Right:
	case Key_Left:
		return false;

	case Key_Enter:
		chr = char(13);
		return true;

	case Key_A:
		chr = 'A';
		return true;
	case Key_B:
		chr = 'B';
		return true;
	case Key_C:
		chr = 'C';
		return true;
	case Key_D:
		chr = 'D';
		return true;
	case Key_E:
		chr = 'E';
		return true;
	case Key_F:
		chr = 'F';
		return true;
	case Key_G:
		chr = 'G';
		return true;
	case Key_H:
		chr = 'H';
		return true;
	case Key_I:
		chr = 'I';
		return true;
	case Key_J:
		chr = 'J';
		return true;
	case Key_K:
		chr = 'K';
		return true;
	case Key_L:
		chr = 'L';
		return true;
	case Key_M:
		chr = 'M';
		return true;
	case Key_N:
		chr = 'N';
		return true;
	case Key_O:
		chr = 'O';
		return true;
	case Key_P:
		chr = 'P';
		return true;
	case Key_Q:
		chr = 'Q';
		return true;
	case Key_R:
		chr = 'R';
		return true;
	case Key_S:
		chr = 'S';
		return true;
	case Key_T:
		chr = 'T';
		return true;
	case Key_U:
		chr = 'U';
		return true;
	case Key_V:
		chr = 'V';
		return true;
	case Key_W:
		chr = 'W';
		return true;
	case Key_X:
		chr = 'X';
		return true;
	case Key_Y:
		chr = 'Y';
		return true;
	case Key_Z:
		chr = 'Z';
		return true;

	default:
		return false;
	}

	return false;
}

bool char_To_KeyPress(KeyPress& key, char chr)
{
	switch (chr)
	{
	case 13:
		key = Key_Enter;
		return true;

	case 'a':
	case 'A':
		key = Key_A;
		return true;
	case 'b':
	case 'B':
		key = Key_B;
		return true;
	case 'c':
	case 'C':
		key = Key_C;
		return true;
	case 'd':
	case 'D':
		key = Key_D;
		return true;
	case 'e':
	case 'E':
		key = Key_E;
		return true;
	case 'f':
	case 'F':
		key = Key_F;
		return true;
	case 'g':
	case 'G':
		key = Key_G;
		return true;
	case 'h':
	case 'H':
		key = Key_H;
		return true;
	case 'i':
	case 'I':
		key = Key_I;
		return true;
	case 'j':
	case 'J':
		key = Key_J;
		return true;
	case 'k':
	case 'K':
		key = Key_K;
		return true;
	case 'l':
	case 'L':
		key = Key_L;
		return true;
	case 'm':
	case 'M':
		key = Key_M;
		return true;
	case 'n':
	case 'N':
		key = Key_N;
		return true;
	case 'o':
	case 'O':
		key = Key_O;
		return true;
	case 'p':
	case 'P':
		key = Key_P;
		return true;
	case 'q':
	case 'Q':
		key = Key_Q;
		return true;
	case 'r':
	case 'R':
		key = Key_R;
		return true;
	case 's':
	case 'S':
		key = Key_S;
		return true;
	case 't':
	case 'T':
		key = Key_T;
		return true;
	case 'u':
	case 'U':
		key = Key_U;
		return true;
	case 'v':
	case 'V':
		key = Key_V;
		return true;
	case 'w':
	case 'W':
		key = Key_W;
		return true;
	case 'x':
	case 'X':
		key = Key_X;
		return true;
	case 'y':
	case 'Y':
		key = Key_Y;
		return true;
	case 'z':
	case 'Z':
		key = Key_Z;
		return true;

	default:
		return false;
	}
	return false;
}

bool Diff_To_String(Difficulty diff, std::string& str)
{
	switch (diff)
	{
	case Diff_Easy:
		str = "Easy";
		return true;
	case Diff_Normal:
		str = "Easy";
		return true;
	case Diff_Hard:
		str = "Easy";
		return true;
	case Diff_Error:
		str = Constants::ErrorDiff;
	default:
		return false;
	}
	return false;
}

bool String_To_Diff(Difficulty& diff, std::string str)
{
	for (int i = 0; i < str.size(); i++)
		str[i] = std::tolower(str[i]);

	if (str == "easy")
		diff = Diff_Easy;
	else if (str == "normal")
		diff = Diff_Easy;
	else if (str == "hard")
		diff = Diff_Easy;
	else
		return false;
}