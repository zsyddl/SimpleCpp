#include "StdAfx.h"
#include "ParamParser.h"

CParamParser::CParamParser(void)
{
}

CParamParser::~CParamParser(void)
{
}
void StringRemove(string &strSrc, char cRemove)
{
	string::iterator   it;

	for (it =strSrc.begin(); it != strSrc.end(); )
	{
		if ( *it == cRemove)
		{
			it = strSrc.erase(it);
		}
		else
		{
			it++;
		}
	}
}
void StringRemoveEndpointBlank(string &strSrc)
{
	if (strSrc.size() <= 0)
		return ;
	string::iterator   it;

	for (it =strSrc.begin(); it != strSrc.end(); )
	{//移除 开始的空格
		if ( *it == ' ')
		{
			it = strSrc.erase(it);
		}
		else
		{//不是空格了，结束
			break;
		}
	}
	it = strSrc.end();
	it--;
	for ( ; it != strSrc.begin(); /*it--*/)
	{//移除 结束的空格
		if ( *it == ' ')
		{
			it = strSrc.erase(it);
			it--;
		}
		else
		{//不是空格了，结束
			break;
		}
	}

}
void CParamParser::ParseParam(char *pParam, int nLen, map<string, string> &mapParamValue)
{
	if (!pParam)
		return ;
	int nCmdStart = 0;
	char *szCmd = new char[nLen+1];
	char szName[1024];
	char *szValue = new char[nLen+1];
	int nMaxLen = nLen;
	if (nMaxLen == 0)
	{
		nMaxLen = strlen(pParam);
	}
	for (int i=0; i<nMaxLen; i++)
	{
		if (pParam[i] != ';')
		{
			continue;
		}
		memset(szCmd, 0, i-nCmdStart+1 );
		strncpy(szCmd, pParam+nCmdStart, i-nCmdStart);
		memset(szName, 0, sizeof(szName));
		memset(szValue, 0, i-nCmdStart+1);

		ParseParamCmd(szCmd, szName, szValue);
		if (strlen(szName) > 0)
		{
			string strName = szName;
			string strValue = szValue;
			//StringRemove(strName, ' ');
			StringRemoveEndpointBlank(strName);
			//StringRemove(strValue, ' ');
			StringRemoveEndpointBlank(strValue);
			mapParamValue[strName] = strValue;
		}
		i++;
		nCmdStart = i;
	}
	delete szValue;
	delete szCmd;
}
void CParamParser::ParseParamCmd(char *pParam, char *pName, char *pVal)
{
	int i=0;
	memset(pName, 0, strlen(pName) );
	memset(pVal, 0, strlen(pVal) );

	char *pEque = strstr(pParam, "=");
	if (!pEque)
	{
		return;
	}
	strncpy(pName, pParam, pEque-pParam);

	char *pEnd = pParam + strlen(pParam);
	int nEqueLen = 1;
	strncpy(pVal, pEque+nEqueLen, pEnd-pEque-nEqueLen);
}

int CParamParser::Parse( char* pParam, int nLen )
{
	ParseParam(pParam, nLen, m_mapName2Value);
	return 0;
}

map<string, string>& CParamParser::GetParams()
{
	return m_mapName2Value;
}