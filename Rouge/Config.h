#pragma once
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

using std::wstring;

class Config
{
	wstring fileName;
	static wstring fileExt;
	wstring myDocPath;
	wstring folderName;

	wstring getCompPath();
	wstring getFolderPath();
	void setMyDocPath();

public:
	wstring getFileName();
	Config(wstring p_FileName);
	~Config();
};

#endif