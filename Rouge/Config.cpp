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
		if (!hasOption(iter->first))
		{
			WriteFile(fileHandle, iter->first.c_str(), iter->first.size(), numBytesWrote, NULL);

			tempBuffer[0] = Constants::Equal;
			WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);

			apply_visitor(visitor, iter->second);

			WriteFile(fileHandle, tempString.c_str(), tempString.size(), numBytesWrote, NULL);

			tempBuffer[0] = Constants::Line_End;
			WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);

			tempBuffer[0] = '\n';
			WriteFile(fileHandle, tempBuffer, 1, numBytesWrote, NULL);
		}
	}
	delete numBytesWrote;
}

void Config::readFromFile()
{	
	char buffer[2] = " ";
	bool fileEnd = false;
	bool lineEnd = false;
	DWORD numBytesRead = 0;
	char ignoredChars[] = {'\n', '#', Constants::Line_End};

	while (!fileEnd)
	{
		this;

 		lineEnd = false;
		string optionName = "";
		string optionValue = "";
		bool isName = true;

		while (!lineEnd)
		{
			buffer[0] = '#';

			bool readRes = ReadFile(fileHandle, buffer, 1, &numBytesRead, NULL);
			bool cond = false;
			
			if (readRes && numBytesRead == 0)
				fileEnd = true;

			if (buffer[0] == Constants::Line_End || (buffer[0] == '\n') || (buffer[0] == '#'))
				lineEnd = true;

			else if (buffer[0] == Constants::Equal)
				isName = false;
			
			else if (isName)
				optionName += buffer[0];

			else
				optionValue += buffer[0];
		}

		bool optFound = hasOption(optionName);

		if (optionName == "")
			optFound = false;
		if (optFound)
		{

		}
		else
		{
			//Variant to which data is stored
			//boost::variant<double,char, string, DiffVariant, KeyVariant>;

			string simpValue = "";

			for (int i = 1; i < optionValue.size(); i++)
				simpValue += optionValue[i];

			if (optionValue[0] == typeCode("double"))
				this->addLine(optionName, lexical_cast<double> (simpValue));

			if (optionValue[0] == typeCode("string"))
				this->addLine(optionName, simpValue);

			if (optionValue[0] == typeCode("difficulty"))
			{
				DiffVariant dif;
				String_To_Diff(dif.diff, simpValue);
				this->addLine(optionName, dif);
			}

			if (optionValue[0] == typeCode("char"))
				this->addLine(optionName, simpValue[0]);

			if (optionValue[0] == typeCode("key"))
			{
				KeyVariant keyVar;
				Str_To_KeyPress(keyVar.key, simpValue);
				this->addLine(optionName, keyVar);
			}
		}
	}
}

bool Config::hasOption(string str)
{
	bool nameFound = false;
	for (VecIter iter = configLines.begin(); iter != configLines.end(); iter++)
	{
		if (iter->first == str)
		{
			nameFound = true;
			break;
		}
	}

	return nameFound;
}

Config::~Config()
{

}


//------------------------------------------- 

void LowerizeStr(string* str)
{
	for (int i = 0; i < str->size(); i++)
	{
		(*str)[i] = std::tolower((*str)[i]);
	}
}