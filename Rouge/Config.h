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

using std::wstring;
using std::string;
using boost::apply_visitor;


struct DiffVariant
{
	Difficulty diff;
	DiffVariant()
	{
		diff = Difficulty(0);
	}
};
struct KeyVariant
{
	KeyPress key;
	KeyVariant()
	{
		key = KeyPress(0);
	}
};

using ConfigDatas = boost::variant<double,char, string, DiffVariant, KeyVariant>;

using ConfigLine = std::pair<std::string, ConfigDatas>;
using ConfigLineVec = std::vector<ConfigLine>;

class Config
{
	class WriteVisitor :
		public boost::static_visitor<>
	{
	public:
		WriteVisitor(): str (nullptr)
		{ };
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
public:
	wstring getFileName();
	Config(wstring p_FileName);

	bool addLine(string option, double value);
	
	bool editLine(string option, double Value);
	
	void writeToFile();
	void readFromFile();

	~Config();
};

#endif