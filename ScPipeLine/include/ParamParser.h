#pragma once

#include <map>
#include <string>
using namespace  std;
//参数解析器
//解析 name=value;name1=value;...这种参数
//
class CParamParser
{
public:
	CParamParser(void);
	virtual ~CParamParser(void);
	
	int Parse(char* pParam, int nLen);
	map<string, string>& GetParams();

	static void ParseParam(char *pParam, int nLen, map<string, string> &mapParamValue);
	static void ParseParamCmd(char *pParam, char *pName, char *pVal);

protected:
	map<string, string> m_mapName2Value;
};
