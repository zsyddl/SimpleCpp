#pragma once
#ifndef CommandLine_H
#define CommandLine_H

#include <map>
#include <string>
using namespace std;

class CCommandLine
{
public:
	CCommandLine(void);
	virtual ~CCommandLine(void);

	int Parse(int nArgc, char* argv[]);
	
	string  GetParam(string strName);
protected:
	map<string, string> m_mapName2Value;

};

#endif

