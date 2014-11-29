#pragma once
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <list>
#include <map>
#include <boost\any.hpp>
#include <utility>
#include <Windows.h>

using std::wstring;
using std::string;

using ConfigLine = std::pair<std::string, boost::any>;
using ConfigLineList = std::list<ConfigLine>;

class Config
{
	HANDLE fileHandle;

	wstring fileName;
	static wstring fileExt;
	wstring myDocPath;
	wstring folderName;

	wstring getCompPath();
	wstring getFolderPath();
	void setMyDocPath();

	ConfigLineList configLines;
public:
	wstring getFileName();
	Config(wstring p_FileName);

	template <typename T>
	bool addLine(string option, T value);
	
	template <typename T>
	bool editLine(string option, T Value);

	~Config();
};

#endif