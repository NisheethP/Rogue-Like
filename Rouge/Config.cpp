#include "Config.h"
#include <Shlobj.h>
#include "Constants.h"
#include "Functions.h"
#include "boost\lexical_cast.hpp"

using VecIter = ConfigLineVec::iterator;
using boost::lexical_cast;

wstring Config::fileExt = wstring(L"cfg");
/*
//================================
// VARIANT VISITOR OPERATORS
//================================
void Config::WriteVisitor::operator() (double& operand)
{
	
}

void Config::WriteVisitor::operator() (string& operand)
{

}

void Config::WriteVisitor::operator() (DiffVariant& operand)
{

}

void Config::WriteVisitor::operator() (KeyVariant& operand)
{

}


//-------------------------------------//
*/
Config::Config(wstring p_FileName) : fileName(p_FileName)
{
	folderName = L"Rogue";
	setMyDocPath();

	wstring tempPath = L"\\\\?\\";
	tempPath += getFolderPath().c_str();
	CreateDirectoryW(tempPath.c_str(), NULL);
	tempPath = L"\\\\?\\";
	tempPath += getCompPath();
	fileHandle = CreateFileW
					(tempPath.c_str(),						//Path of file - type - LPCWSTR 
					GENERIC_READ | GENERIC_WRITE,			//READ-WRITE Access
					FILE_SHARE_READ | FILE_SHARE_WRITE,		//Share access
					NULL,									//Security attributes. NULL just inherits those of program
					OPEN_ALWAYS,							//What to actually do with file
					FILE_ATTRIBUTE_NORMAL,					//Property of file (hidden etc)
					NULL);									//A template file. NULL means no template file
}

wstring Config::getFileName()
{
	wstring name = L"";
	name = fileName + L"." + fileExt;
	return name;
}

wstring Config::getFolderPath()
{
	wstring path = L"";

	path += myDocPath;
	path += folderName;

	return path;
}

void Config::setMyDocPath()
{
	PWSTR path = L"";
	SHGetKnownFolderPath(FOLDERID_Documents,0,NULL, &path);
	myDocPath = path;
	myDocPath += L"\\";
}

wstring Config::getCompPath()
{
	wstring path = L"";
	
	path += myDocPath;
	path += folderName;
	path += L"\\";
	path += getFileName();

	return path;
}


bool Config::addLine(string option, double value)
{
	for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
	{
		if (iter->first == option)
			return false;
	}

	configLines.push_back(ConfigLine(option, value));
	return true;
}

bool Config::editLine(string option, double value)
{
	for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
	{
		if (iter->first == option)
		{
			iter->second = value;
			return true;
		}
	}

	return false;
}

void Config::writeToFile()
{
	LPDWORD numBytesWrote = new DWORD;
	char tempBuffer[2] = " ";
	std::string tempString = "ERROR";
	
	for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
	{
		WriteFile(fileHandle, iter->first.c_str(), iter->first.size(), numBytesWrote, NULL);

		tempBuffer[0] = Constants::Equal;
		WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);

		//if (iter->second.type() == typeid(Difficulty))
		//{
			//Difficulty tempDiff = apply_visitor(ConfigVisitor(), iter->second);
			//Diff_To_String(tempDiff, tempString);
		//}

		//else if (iter->second.type() == typeid(KeyPress))
		//{
			//KeyPress tempKey = KeyPress(lexical_cast<int>(iter->second));
			//keyPress_To_Char(tempKey, tempBuffer[0]);
		//	tempString = "";
		//	tempString += tempBuffer;
		//}

		//else if (iter->second.type() == typeid(string))
		//{
			//tempString = lexical_cast<string>(iter->second);
		//}

		//else
		//{
			tempString = lexical_cast<string>(iter->second);
		//}

		WriteFile(fileHandle, tempString.c_str(),tempString.size(), numBytesWrote, NULL);

		tempBuffer[0] = Constants::Line_End;
		WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);

		tempBuffer[0] = '\n';
		WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);
		
	}
	delete numBytesWrote;
}

void Config::readFromFile()
{
	char buffer[2] = "E";
	LPDWORD numBytesRead = 0;
	bool isName = true;
	bool nameFound = false;
	
	/*for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
	{
		string optionName = "";
		string optionValue = "";

		while (buffer[0] != Constants::Line_End)
		{
			ReadFile(fileHandle, buffer, 1, numBytesRead, NULL);
			if (buffer[0] == Constants::Equal)
				isName = false;
			else if (isName)
				optionName += buffer;
			else
				optionValue += buffer;
		}

		for (VecIter tempIter = configLines.begin(); tempIter != configLines.end(); ++tempIter)
		{
			if (optionName == tempIter->first)
			{
				nameFound = true;
				if (tempIter->second.type() == typeid(Difficulty))
				{
					//Difficulty tempDiff;
					//String_To_Diff(tempDiff, optionValue);
					//tempIter->second = tempDiff;
				}

				else if (tempIter->second.type() == typeid(KeyPress))
				{
					//KeyPress tempKey;
					//char_To_KeyPress(tempKey, optionValue[0]);
					//tempIter->second = tempKey;
				}

				else if (tempIter->second.type() == typeid(string))
				{
					//tempIter->second = optionValue;
				}

				else
				{
					//tempIter->second = lexical_cast<double>(optionValue);
				}
			}
		}

		if (!nameFound)
		{
			addLine<string>(optionName, optionValue);
		}
	}*/
}

Config::~Config()
{

}