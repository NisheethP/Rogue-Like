#pragma once
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <vector>
#include <map>
#include <boost\variant.hpp>
#include <utility>
#include <Windows.h>

using std::wstring;
using std::string;

using ConfigLine = std::pair<std::string, boost::variant>;
using ConfigLineVec = std::vector<ConfigLine>;

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

	ConfigLineVec configLines;
public:
	wstring getFileName();
	Config(wstring p_FileName);

	template <typename T>
	bool addLine(string option, T value);
	
	template <typename T>
	bool editLine(string option, T Value);
	
	void writeToFile();
	void readFromFile();

	~Config();
};

#endif