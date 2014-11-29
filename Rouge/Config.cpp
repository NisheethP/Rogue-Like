#include "Config.h"
#include <Shlobj.h>
wstring Config::fileExt = wstring(L"cfg");

Config::Config(wstring p_FileName) : fileName(p_FileName)
{
	folderName = L"Rogue";
	setMyDocPath();

	wstring tempPath = L"\\\\?\\";
	tempPath += getFolderPath().c_str();
	CreateDirectoryW(tempPath.c_str(), NULL);
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


Config::~Config()
{
}
