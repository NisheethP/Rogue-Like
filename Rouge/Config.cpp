#include "Config.h"
#include <Shlobj.h>
wstring Config::fileExt = wstring(L"cfg");

Config::Config(wstring p_FileName)
{
	folderName = L"Rogue";
	
	LPCWSTR path = getCompPath().c_str();
	//CreateDirectory();
}

wstring Config::getFileName()
{
	wstring name = L"";
	name = fileName + L"." + fileExt;
	return name;
}

void Config::getMyDocPath()
{
	PWSTR path = L"";
	SHGetKnownFolderPath(FOLDERID_Documents,0,NULL, &path);
	myDocPath = path;
	myDocPath += L"\\";
}

wstring Config::getCompPath()
{
	wstring path = L"";
	
	path = myDocPath;
	path += folderName;
	path += L"\\";
	path += getFileName();

	return path;
}


Config::~Config()
{
}
