#include "Config.h"
#include <Shlobj.h>
#include "Constants.h"
#include "Functions.h"
#include <cctype>

void LowerizeStr(string* str);

wstring Config::fileExt = wstring(L"cfg");

//================================
// VARIANT VISITOR OPERATORS
//================================
void Config::WriteVisitor::operator() (double& operand)
{
	*str = typeCode("double");
	*str += lexical_cast<string>(operand);
}

void Config::WriteVisitor::operator() (string& operand)
{
	*str = typeCode("string");
	*str += operand;
}

void Config::WriteVisitor::operator() (DiffVariant& operand)
{
	Difficulty tempDiff = operand.diff;
	string tempString = *str;
	Diff_To_String(tempDiff, tempString);

	*str = typeCode("difficulty");
	*str += tempString;
}

void Config::WriteVisitor::operator() (char& operand)
{
	*str = typeCode("char");
	*str += operand;
}

void Config::WriteVisitor::operator() (KeyVariant& operand)
{
	KeyPress tempKey = operand.key;
	string tempString = " ";
	keyPress_To_Str(tempKey, tempString);

	*str = typeCode("key");
	*str += tempString;
}

//-------------------------------------//

char Config::typeCode(string type)
{
	LowerizeStr(&type);

	if (type == "double")
		return 'd';
	else if (type == "string")
		return 's';
	else if (type == "difficulty")
		return 'D';
	else if (type == "char")
		return 'c';
	else if (type == "key")
		return 'k';
	
	return '#';
}

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

void Config::writeToFile()
{
	LPDWORD numBytesWrote = new DWORD;
	char tempBuffer[2] = " ";
	string tempString = "ERROR";

	WriteVisitor visitor;
	visitor.str = &tempString;
	
	for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
	{
		WriteFile(fileHandle, iter->first.c_str(), iter->first.size(), numBytesWrote, NULL);

		tempBuffer[0] = Constants::Equal;
		WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);

		apply_visitor(visitor, iter->second);

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
	char buffer[33] = "#";
	DWORD numBytesRead;
	bool isName = true;
	bool nameFound = false;
	bool endOfFile = false;

	while (!endOfFile)
	{
		buffer[0] = '#';
		string optionName = "";
		string optionValue = "";
		BOOL readRes = FALSE;

		while (buffer[0] != Constants::Line_End)
		{
			readRes = ReadFile(fileHandle, buffer, 1, &numBytesRead, NULL);
 			
			if (readRes && numBytesRead == 0)
				endOfFile = true;

			if (buffer[0] == '\n')
				break;

			if (buffer[0] == Constants::Equal)
				isName = false;
			else if (isName)
				optionName += buffer;
			else
				optionValue += buffer;
		}

		for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
		{
			if (optionName == iter->first)
			{
				string simpOptName = "";
				for (int i = 0; i < optionName.size(); i++)
				{
					simpOptName += optionName[i];
				}

				nameFound = true;
				if (optionValue[0] == typeCode("double"))
				{
					iter->second = lexical_cast<double>(simpOptName);
				}

				else if (optionValue[0] == typeCode("difficulty"))
				{
					iter->second = (simpOptName);
				}

				else if (optionValue[0] == typeCode("string"))
				{
					DiffVariant diffVar;
					String_To_Diff(diffVar.diff, simpOptName);
					iter->second = diffVar;
				}

				else if (optionValue[0] == typeCode("char"))
				{
					iter->second = optionValue[0];
				}

				else if (optionValue[0] == typeCode("key"))
				{
					KeyVariant keyVar;
					Str_To_KeyPress(keyVar.key, simpOptName);
					iter->second = keyVar;
				}
			}
		}

		if (!nameFound)
		{
			addLine<string>(optionName, optionValue);
		}
	}
}

Config::~Config()
{

}

void LowerizeStr(string* str)
{
	for (int i = 0; i < str->size(); i++)
	{
		(*str)[i] = std::tolower((*str)[i]);
	}
}