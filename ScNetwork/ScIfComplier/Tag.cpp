#include "StdAfx.h"
#include "Tag.h"

CTag::CTag(void)
: m_pUserData(NULL)
, m_pParent(NULL)
, m_bHasInterface(false)
, m_bIsOneWay(false)
{	 
}

CTag::~CTag(void)
{
	list<CTag*>::iterator item;
	for (item = m_lstChild.begin();
		item != m_lstChild.end();
		item++)
	{
		CTag *pTag = *item;
		if (pTag)
		{
			delete pTag;
		}
	}
	m_lstChild.clear();
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item++)
	{
		CTag *pTag = *item;
		if (pTag)
		{
			delete pTag;
		}
	}
	m_lstProperty.clear();
}

void CTag::CopyChild(CTag* pOther)
{
	list<CTag*>::iterator item;
	for (item = pOther->m_lstChild.begin();
		item != pOther->m_lstChild.end();
		item++)
	{
		CTag* pTagTmp = *item;
		pTagTmp->m_pParent = this;

		m_lstChild.push_back(pTagTmp);
	}
}

CTag* CTag::GetProperty(eTagType type)
{
	CTag* pTag(NULL);
	list<CTag*>::iterator itemClassProperty;
	for (itemClassProperty = m_lstProperty.begin();
		itemClassProperty != m_lstProperty.end();
		itemClassProperty++)
	{
		CTag* pTagProperty = *itemClassProperty;
		if (pTagProperty->m_nType == type)
		{
			pTag = pTagProperty;
		}
	}	
	return pTag;
}

CTag* CTag::GetProperty(int nPos)
{
	CTag* pTag(NULL);
	int nCount =0;
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end() && nCount != nPos;
		item++, nCount++)
	{
	}
	if (item != m_lstProperty.end())
	{
		pTag = *item;
	}
	return pTag;
}

std::string CTag::ToString()
{
	string strTmp;
	int nFiledLen = 1024;
	char *pSzField = new char[nFiledLen];
	bool bHasPropertyOrChild = false;;
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item++)
	{
		CTag* pTag = *item;
		strTmp += pTag->ToString();

		bHasPropertyOrChild = true;
	}
	for (item = m_lstChild.begin();
		item != m_lstChild.end();
		item++)
	{
		CTag* pTag = *item;
		strTmp += pTag->ToString();

		bHasPropertyOrChild = true;
	}

	if (!bHasPropertyOrChild)
	{//body 
		sprintf(pSzField, "%s ", m_strBody.c_str());
		strTmp = pSzField;
	}

	delete pSzField;

	return strTmp;
}
//截至到xxx位置 为止的属性
std::string CTag::ToString(eTagType typeEnd)
{
	string strTmp;
	int nFiledLen = 1024;
	char *pSzField = new char[nFiledLen];
	bool bHasPropertyOrChild = false;;
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item++)
	{
		CTag* pTag = *item;
		
		strTmp += pTag->ToString();

		bHasPropertyOrChild = true;
		if(pTag->m_nType == typeEnd)
		{
			break;
		}
	}
	for (item = m_lstChild.begin();
		item != m_lstChild.end();
		item++)
	{
		CTag* pTag = *item;
		strTmp += pTag->ToString();

		bHasPropertyOrChild = true;
	}

	if (!bHasPropertyOrChild)
	{//body 
		sprintf(pSzField, "%s ", m_strBody.c_str());
		strTmp = pSzField;
	}

	delete pSzField;

	return strTmp;
}

std::string CTag::ToString(eTagType typeStart, eTagType typeEnd)
{
	string strTmp;
	int nFiledLen = 1024;
	char *pSzField = new char[nFiledLen];
	bool bHasPropertyOrChild = false;;

	bool bStarted = false;
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item++)
	{
		CTag* pTag = *item;
		if (pTag->m_nType == typeStart)
		{
			bStarted = true;
		}
		if (!bStarted)
			continue;

		strTmp += pTag->ToString();

		bHasPropertyOrChild = true;
		if(pTag->m_nType == typeEnd)
		{
			break;
		}
	}
	for (item = m_lstChild.begin();
		item != m_lstChild.end();
		item++)
	{
		CTag* pTag = *item;
		strTmp += pTag->ToString();

		bHasPropertyOrChild = true;
	}

	if (!bHasPropertyOrChild)
	{//body 
		sprintf(pSzField, "%s ", m_strBody.c_str());
		strTmp = pSzField;
	}

	delete pSzField;

	return strTmp;
}

void CTag::CopyFrom(CTag* pOther)
{
	m_strName = pOther->m_strName;
	m_nType = pOther->m_nType;
	m_strBody = pOther->m_strBody;

	list<CTag*>::iterator item;
	for (item = pOther->m_lstProperty.begin();
		item != pOther->m_lstProperty.end();
		item++)
	{
		CTag* pTag = *item;
		CTag* pNewTag = new CTag();
		pNewTag->CopyFrom(pTag);
		m_lstProperty.push_back(pNewTag);
	}
	for (item = pOther->m_lstChild.begin();
		item != pOther->m_lstChild.end();
		item++)
	{
		CTag* pTag = *item;
		CTag* pNewTag = new CTag();
		pNewTag->CopyFrom(pTag);
		m_lstChild.push_back(pNewTag);
	}
}

void CTag::AppendProperty(CTag* pTag)
{
	m_lstProperty.push_back(pTag);
}

void CTag::AppendChild(CTag* pTag)
{
	m_lstChild.push_back(pTag);
}

CTagEnum::CTagEnum()

{
	m_nType = (eTagType_Enum);
}

CTagEnum::~CTagEnum()
{

}

std::string CTagEnum::ToString()
{
	string strTmp;
 
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item++)
	{
		CTag* pTag = *item;
		
		if (pTag->m_nType == eTagType_EnumMembers)
		{
			CTag* pTagMembers = pTag;
			list<CTag*>::iterator itemMember;
			for (itemMember = pTagMembers->m_lstChild.begin();
				itemMember != pTagMembers->m_lstChild.end();
				itemMember++)
			{
				CTag* pTagMember = *itemMember;
				strTmp += "\t" + pTagMember->ToString() + ",\r\n";
			}
		}
		else if (pTag->m_nType == eTagType_TokenBrancesLeft)
		{
			strTmp += pTag->ToString();
			strTmp += "\r\n";
		}
		else
		{
			strTmp += pTag->ToString();
		}
	}
 
	return strTmp;
}

CTagBaseClasses::CTagBaseClasses()

{
	m_nType = (eTagType_BaseClasses);
}

CTagBaseClasses::~CTagBaseClasses()
{

}

std::string CTagBaseClasses::ToString()
{
	string strTmp;
	if (m_lstChild.size() <= 0)
		return strTmp;
	strTmp = ": ";

	list<CTag*>::iterator item, itemTmp;
	for (item = m_lstChild.begin();
		item != m_lstChild.end();
		item++)
	{
		CTag* pTag = *item;
		strTmp += pTag->ToString();
		itemTmp = item;
		itemTmp++;
		if (itemTmp != m_lstChild.end())
		{
			strTmp += ", ";
		}
	}

	return strTmp;
}

CTagParams::CTagParams()

{
	m_nType = (eTagType_Params);
}

CTagParams::~CTagParams()
{
	
}
//参数
std::string CTagParams::ToString()
{
	string strTmp;

	list<CTag*>::iterator item, itemTmp;
	for (item = m_lstChild.begin();
		item != m_lstChild.end();
		item++)
	{
		CTag* pTag = *item;

		
		strTmp += pTag->ToString();
 
		itemTmp = item;
		itemTmp++;
		if (itemTmp != m_lstChild.end())
		{
			strTmp += ", ";
		}
	}

	return strTmp;
}

CTagInclude::CTagInclude()

{
	m_nType = (eTagType_Include);
}

CTagInclude::~CTagInclude()
{

}

std::string CTagInclude::ToString()
{
	string strTmp;
	int nFiledLen = 1024;
	char *pSzField = new char[nFiledLen];
	bool bHasPropertyOrChild = false;;
	bool bIncludeBody = false;;
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item++)
	{
		CTag* pTag = *item;

		strTmp += pTag->m_strBody;

		if (pTag->m_nType == eTagType_TokenInclude)
		{
			strTmp += " ";
		}
	 
	}


	delete pSzField;

	return strTmp;
}

bool CTagType::IsRef()
{
	bool bRet = false;
	if (m_lstProperty.size() >= 1)
	{
		CTag *pTag = m_lstProperty.back();
		if (pTag->m_nType == eTagType_TokenAnd)
		{//找到 & 说明是引用
			bRet = true;
		}
	}

	return bRet;
}

bool CTagType::IsPointer()
{
	bool bRet = false;
	if (m_lstProperty.size() >= 1)
	{
		CTag *pTag = m_lstProperty.back();
		if (pTag->m_nType == eTagType_TokenStar)
		{//找到 * 说明是指针
			bRet = true;
		}
	}

	return bRet;
}
//获取 base type，比如 什么指针， 什么引用
std::string CTagType::GetBaseType()
{
	string strText;
	list<CTag*>::iterator item;
	for (item = m_lstProperty.begin();
		item != m_lstProperty.end();
		item ++)
	{
		CTag *pTag = *item;
		if (pTag->m_nType == eTagType_TokenStar
			|| pTag->m_nType == eTagType_TokenAnd)
		{
			break;
		}
		else
		{
			strText += pTag->ToString();
		}
	}

	return strText;
}

CTagType::CTagType()
{
	m_nType = eTagType_VarType;
}

CTagType::~CTagType()
{
	
}

bool CTagType::IsAtom()
{
	bool bRet = false;
	if (m_strName == "int8"
		|| m_strName == "uint8"
		|| m_strName == "int16"
		|| m_strName == "uint16"
		|| m_strName == "int32"
		|| m_strName == "uint32"
		|| m_strName == "int64"
		|| m_strName == "uint64"
		|| m_strName == "float"
		|| m_strName == "double"
		|| m_strName == "string"
		|| m_strName == "buffer"
		|| m_strName == "int"
		|| m_strName == "long"
		)
	{
		bRet = true;
	}
	return bRet;
}

bool CTagType::IsType(string &strName)
{
	bool bRet = false;
	int pos = m_strName.find(strName);
	if ( pos >= 0 )
		bRet = true;

	return bRet;
}

CTagClass::CTagClass()
{
	m_nType = eTagType_Class;
}
CTagClass::~CTagClass()
{

}


std::string CTagClass::ToString()
{
	string strTmp;
	strTmp = CTag::ToString(eTagType_TokenBrancesLeft) + "\r\n";
	//fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	//写构造函数 和析构函数
	//TransConstructor2Client(pTopTag, pFileH, pFileCPP);
	//TransDestructor2Client(pTopTag, pFileH, pFileCPP);
	//转换 接口
	//class 写构造函数等？	
	CTag* pTagMembers(NULL);
	pTagMembers = GetProperty(eTagType_Members);


	//写入 各种接口函数
	list<CTag*>::iterator itemClassMember;
	for(itemClassMember = pTagMembers->m_lstChild.begin();
		itemClassMember != pTagMembers->m_lstChild.end();
		itemClassMember++)
	{
		CTag* pTagClassMember = *itemClassMember;
		//
		if (pTagClassMember->m_nType == eTagType_Access)
		{
			strTmp = strTmp + "\t" + pTagClassMember->ToString() + ":\r\n";
		}
		else
		{
			strTmp = strTmp + "\t" + pTagClassMember->ToString() + "\r\n";
		}
		//
		//
	}
	//写 class 结尾
	strTmp += CTag::ToString(eTagType_TokenBrancesRight, eTagType_Unknown) + "\r\n";
	//fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	return strTmp;
}

