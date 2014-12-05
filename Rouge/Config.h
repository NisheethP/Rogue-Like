#pragma once
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <vector>
#include <map>
#include <boost\variant.hpp>
#include <utility>
#include <Windows.h>
#include "Functions.h"
#include "boost\lexical_cast.hpp"

using std::wstring;
using std::string;
using boost::apply_visitor;


struct DiffVariant
{
	Difficulty diff;
	DiffVariant(Difficulty p_Diff = Difficulty(0))
	{
		diff = Difficulty(p_Diff);
	}
};
struct KeyVariant
{
	KeyPress key;
	KeyVariant(KeyPress p_Key = KeyPress(0))
	{
		key = KeyPress(p_Key);
	}
};

using ConfigDatas = boost::variant<double,char, string, DiffVariant, KeyVariant>;
using ConfigLine = std::pair<std::string, ConfigDatas>;
using ConfigLineVec = std::vector<ConfigLine>;
using VecIter = ConfigLineVec::iterator;
using boost::lexical_cast;

class Config
{
	class WriteVisitor :
		public boost::static_visitor<>
	{
	public:
		void operator() (double& operand);
		void operator() (string& operand);
		void operator() (DiffVariant& operand);
		void operator() (KeyVariant& operand);
		void operator() (char& operand);
		
		string* str;
	};

	HANDLE fileHandle;

	wstring fileName;
	static wstring fileExt;
	wstring myDocPath;
	wstring folderName;

	wstring getCompPath();
	wstring getFolderPath();
	void setMyDocPath();

	ConfigLineVec configLines;

	static char typeCode(string type);
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


//Must be kept here. Linker can't handle templates being in separate files...
template <typename T>
bool Config::addLine(string option, T value)
{
	
	for (VecIter iter = configLines.begin(); iter != configLines.end(); ++iter)
	{
		if (iter->first == option)
			return false;
	}
	
	ConfigDatas dat;
	dat = value;

	configLines.push_back(ConfigLine(option, dat));
	return true;
}

template <typename T>
bool Config::editLine(string option, T value)
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


#endif