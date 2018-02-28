#include "StdAfx.h"
#include "FileTemplate.h"

CFileTemplate::CFileTemplate(void)
: m_pFile(NULL)
{
}

CFileTemplate::~CFileTemplate(void)
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

list<CTemplateNode*> CFileTemplate::GetTopNodes(string& strName)
{
	list<CTemplateNode*> lstNode;

	return lstNode;
}

int CFileTemplate::Open(string &strFile)
{

	return 0;
}

void CFileTemplate::Close()
{

}

int CFileTemplate::ToText(string& strText)
{

	return 0;
}

int CFileTemplate::From(FILE* pFile)
{

	return 0;
}

int CFileTemplate::From(string& strText)
{

	return 0;
}
