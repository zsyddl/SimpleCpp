#include "StdAfx.h"
#include "CommandLine.h"

CCommandLine::CCommandLine(void)
{
}

CCommandLine::~CCommandLine(void)
{
	m_mapName2Value.clear();
}

int CCommandLine::Parse(int nArgc, char* argv[])
{
	int nRet = 0;
	string strName;
	string strValue;
	for (int i=0; i<nArgc; i++)
	{
		char* pTmp = argv[i];
		if (pTmp[0] == '-')
		{//name
			strName = pTmp+1;
			m_mapName2Value[strName] = "";
		}
		else
		{//value
			strValue = pTmp;
			m_mapName2Value[strName] = strValue;
		}
	}

	return nRet;
}

string CCommandLine::GetParam(string strName)
{
	string strTmp;
	map<string, string>::iterator item;
	item = m_mapName2Value.find(strName);
	if (item == m_mapName2Value.end())
	{
		return strTmp;
	}

	return item->second;
}
