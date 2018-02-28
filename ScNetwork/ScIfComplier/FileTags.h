#pragma once
#include <vector>
#include <list>
using namespace std;
//保存一个文件的tags
 
class CTag;
typedef enum _eTagType eTagType;
class CFileTags
{
public:
	CFileTags(void);
	virtual ~CFileTags(void);

	//
	void AppendTopTag(CTag* pTag);
	//
	void AppendToken(CTag* pTag);
	//
	list<CTag*>&	GetTopTags(){return m_lstTopTag;}
	CTag*			GetTags(eTagType type);
	CTag*			GetIfTag();
	//CFileTags::m_pFileTags
	static CFileTags*	m_pFileTags;
protected:
	//
	list<CTag*>	m_lstTopTag;
	//所有的token
	list<CTag*> m_lstToken;
	string		m_strFileName;
};
