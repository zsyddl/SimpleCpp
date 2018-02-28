#pragma once
#include "filetemplate.h"
//interface 模板代码，用于自动生成代码时候的填写
//
class CIfTemplate :
	public CFileTemplate
{
public:
	CIfTemplate(void);
	virtual ~CIfTemplate(void);
	
	virtual int Open(string &strFile);

	virtual int ToText(string& strText);
protected:
	virtual int From(FILE* pFile);
	
};
