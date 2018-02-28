#include "StdAfx.h"
#include "FileTags.h"
#include "Tag.h"

CFileTags::CFileTags(void)
{
}

CFileTags::~CFileTags(void)
{
	list<CTag*>::iterator item;
	for (item = m_lstTopTag.begin();
		item != m_lstTopTag.end();
		item++)
	{
		delete *item;;
	}
	m_lstTopTag.clear();

	for (item = m_lstToken.begin();
		item != m_lstToken.end();
		item++)
	{
		delete *item;;
	}
	m_lstToken.clear();
}

void CFileTags::AppendTopTag(CTag* pTag)
{
	m_lstTopTag.push_back(pTag);
}

 

void CFileTags::AppendToken(CTag* pTag)
{
	m_lstToken.push_back(pTag);
}

CTag* CFileTags::GetTags(eTagType type)
{
	CTag* pTag(NULL);
	list<CTag*>::iterator item;
	for (item = m_lstTopTag.begin();
		item != m_lstTopTag.end();
		item++)
	{
		CTag* pTagTmp = *item;
		if (pTagTmp->m_nType == type)
		{
			pTag = pTagTmp;
			break;
		}
	}

	return pTag;
}

CTag* CFileTags::GetIfTag()
{
	CTag* pTag(NULL);
	list<CTag*>::iterator item;
	for (item = m_lstTopTag.begin();
		item != m_lstTopTag.end();
		item++)
	{
		CTag* pTagTmp = *item;
		if (pTagTmp->m_nType == eTagType_Class
			&& pTagTmp->m_bHasInterface)
		{
			pTag = pTagTmp;
			break;
		}
	}

	return pTag;
}

CFileTags* CFileTags::m_pFileTags=new CFileTags();
