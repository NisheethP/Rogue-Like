#include "Config.h"
#include <Shlobj.h>
#include <Shlwapi.h>
using ListIter = ConfigLineList::iterator;
wstring Config::fileExt = wstring(L"cfg");

Config::Config(wstring p_FileName) : fileName(p_FileName)
{
	folderName = L"Rogue";
	setMyDocPath();

	wstring tempPath = L"\\\\?\\";
	tempPath += getFolderPath().c_str();
	CreateDirectoryW(tempPath.c_str(), NULL);
	tempPath = L"\\\\?\\";
	tempPath += getCompPath();
	if (!PathFileExistsW(tempPath.c_str()))
		fileHandle = CreateFileW
						(tempPath.c_str(),						//Path of file - type - LPCWSTR 
						GENERIC_READ | GENERIC_WRITE,			//READ-WRITE Access
						FILE_SHARE_READ | FILE_SHARE_WRITE,		//Share access
						NULL,									//Security attributes. NULL just inherits those of program
						CREATE_NEW,								//What to actually do with file
						FILE_ATTRIBUTE_NORMAL,					//Property of file (hidden etc)
						NULL);									//A template file. NULL means no template file
	else
		fileHandle = CreateFileW
						(tempPath.c_str(),						//Path of file - type - LPCWSTR 
						GENERIC_READ | GENERIC_WRITE,			//READ-WRITE Access
						FILE_SHARE_READ | FILE_SHARE_WRITE,		//Share access
						NULL,									//Security attributes. NULL just inherits those of program
						OPEN_EXISTING,							//What to actually do with file
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

template <typename T>
bool Config::addLine(string option, T value)
{
	for (ListIter listIter = configLines.begin(); listIter != configLines.end(); ++listIter)
	{
		if (listIter->first == option)
			return false;
	}

	list.push_back(option, value);
	return true;
}
template <typename T>
bool Config::editLine(string option, T value)
{
	for (ListIter listIter = configLines.begin(); listIter != configLines.end(); ++listIter)
	{
		if (listIter->first == option)
		{
			if (listIter->second.type() == typeid(value))
			{
				listIter->second = value;
				return true;
			}
		}
	}

	return false;
}

Config::~Config()
{

}
