#pragma once

#include <list>

using namespace std;
//
//文件模板
//
class CTemplateNode{
public:
	CTemplateNode();
	~CTemplateNode();
	//设置一个标签的文本
	virtual int SetText(string& strText);
	//
	string	m_strName;
	//
	string  m_strText;
};
//
class CFileTemplate
{
public:
	CFileTemplate(void);
	virtual ~CFileTemplate(void);
	//
	virtual int Open(string &strFile);
	virtual void Close();//

	list<CTemplateNode*>& GetTopNodes(){return m_lstTopNode;};
	list<CTemplateNode*>  GetTopNodes(string& strName);
	//翻译到文字
	virtual int ToText(string& strText);
	//
protected:
	//
	virtual int From(FILE* pFile);
	virtual int From(string& strText);

	//
	string	m_strFileName;
	FILE*	m_pFile;
	//所有的节点
	list<CTemplateNode*> m_lstTopNode;
	//
};
