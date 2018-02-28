
#include "stdafx.h"
#include "Variant.h"
#include "Buffer.h"

namespace SimpleCpp{

int CVariant::m_mapTypeSize[]={
		1,1, sizeof(short), sizeof(short), sizeof(int), sizeof(int),
		8, 8, sizeof(float), sizeof(double), sizeof(bool), sizeof(void*),

	};
CVariant::CVariant()
: m_nType(eVariantType_invalid)

{
	memset(&m_value, 0, sizeof(m_value));
}
CVariant::CVariant(int8 val)
:m_nType(eVariantType_int8)

{
	m_value.m_i8 = val;
}
CVariant::CVariant(uint8 val)
:m_nType(eVariantType_uint8)

{
	m_value.m_u8 = val;
}
CVariant::CVariant(int16 val)
:m_nType(eVariantType_int16)

{
	m_value.m_i16 = val;
}
CVariant::CVariant(uint16 val)
:m_nType(eVariantType_uint16)

{
	m_value.m_u16 = val;
}
CVariant::CVariant(int32 val)
:m_nType(eVariantType_int32)

{
	m_value.m_i32 = val;
}
CVariant::CVariant(uint32 val)
:m_nType(eVariantType_uint32)

{
	m_value.m_u32 = val;
}
CVariant::CVariant(int64 val)
:m_nType(eVariantType_int64)

{
	m_value.m_i64 = val;
}
CVariant::CVariant(uint64 val)
:m_nType(eVariantType_uint64)
{
	m_value.m_u64 = val;
}
CVariant::CVariant(float val)
:m_nType(eVariantType_float)

{
	m_value.m_float = val;
}
CVariant::CVariant(double val)
:m_nType(eVariantType_double)
{
	m_value.m_double = val;
}
CVariant::CVariant(void* val)
:m_nType(eVariantType_voidPtr)

{
	m_value.m_ptr = val;
}

CVariant::CVariant( string& val)
:m_nType(eVariantType_string)

{
	m_value.m_pObject = new string;
	*((string*)m_value.m_pObject) = val;
}

CVariant::CVariant( const CVariant &other )
{
// 	m_nType = other.m_nType;
// 	m_value = other.m_value;
	CopyFrom((CVariant*)&other);
}


CVariant::CVariant( eVariantType type )
{
	m_nType = type;
	if (type == eVariantType_struct)
	{
		m_value.m_pStruct = new sVariantStruct;
	}
	else if (type == eVariantType_vector)
	{
		m_value.m_pObject = new vector<CVariant*>();
	}
	else if (type == eVariantType_buffer)
	{
		m_value.m_pObject = new vector<char>;
	}
	else if (type == eVariantType_string)
	{
		m_value.m_pObject = new string;
	}
	
}

CVariant::CVariant(char* val)
{
	m_nType =  eVariantType_string;
	string *pStr = new string(val );
	//pStr->append(val, strlen(val));
	//m_value.m_pObject = new string(val);
	//*((string*)m_value.m_pObject) = val;

	m_value.m_pObject = pStr;
}

CVariant& CVariant::operator=(const CVariant &other )
{

	CopyFrom((CVariant*)&other);

	return *this;
}

CVariant::~CVariant()
{
	if (m_nType >= eVariantType_object 
		&& m_nType  < eVariantType_objectEnd )
	{
		if (m_value.m_pObject)
		{
			delete m_value.m_pObject;
			m_value.m_pObject = NULL;
		}
	}
	if (m_nType == eVariantType_struct)
	{
		delete m_value.m_pStruct;
		m_value.m_pStruct = NULL;
	}

}

void CVariant::CopyFrom( CVariant *pOther )
{//无法复制啊？
	m_nType = pOther->m_nType;
	m_value = pOther->m_value;
	if (m_nType == eVariantType_string 
		/*||  m_nType == eVariantType_buffer */)
	{
		m_value.m_pObject = new string();
		*(string*)m_value.m_pObject = *(string*)pOther->m_value.m_pObject;
	}
	else if (m_nType == eVariantType_buffer )
	{
		vector<char>* pBuffer = new vector<char>();
		vector<char>* pOtherBuffer = (vector<char>* )pOther->m_value.m_pObject;
		pBuffer->resize(pOtherBuffer->size());
		memcpy(&pBuffer->at(0), &pOtherBuffer->at(0), pOtherBuffer->size());
	}
	else if (m_nType == eVariantType_struct
		|| m_nType == eVariantType_class)
	{
		m_value.m_pStruct = new sVariantStruct;
		for (int i=0; i<pOther->m_value.m_pStruct->m_arrMembers.size(); i++)
		{
			CVariant *pMemberVar = pOther->m_value.m_pStruct->m_arrMembers.at(i);
			StructAppendMember(pMemberVar);
		}
	}
}

int CVariant::WriteTo( CBuffer *pBuffer )
{
	short sType = m_nType;
	pBuffer->Write((char*)&sType, sizeof(sType));

	int nSize(0);
	if (m_nType>0 && m_nType < eVariantType_metaType)
	{
		nSize = m_mapTypeSize[m_nType];
		pBuffer->Write((char*)&m_value, nSize );
	}
	else if (m_nType == eVariantType_buffer)
	{//
		vector<char> &arrBuffer = *(vector<char>*)m_value.m_pObject;
		int nSize = arrBuffer.size();
		pBuffer->Write( (char*)&nSize, sizeof(nSize) );
		char* pBufferData = &arrBuffer.at(0);
		pBuffer->Write(pBufferData, nSize );
	
	}
	else if (m_nType == eVariantType_vector
		|| m_nType == eVariantType_list)
	{//这里写入 
		//
		vector<CVariant*>& arrValue = *(vector<CVariant*>*)m_value.m_pObject;
 		int nSize = arrValue.size();
 		pBuffer->Write( (char*)&nSize, sizeof(nSize) );
 
 		for (int i=0; i<arrValue.size();
 			i++)
 		{
 			CVariant* pVar = arrValue.at(i);
 			pVar->WriteTo(pBuffer);
 		}
		//
	}
	else
	{
		string *pStr;
		switch(sType)
		{
		case eVariantType_string:
			//string &strTmp = *((string*)m_value.m_pObject);
			pStr = (string*)m_value.m_pObject;
			pBuffer->Write(*pStr);
			break;;
		case eVariantType_struct:
			//结构体
			short nStuctMemberCount = m_value.m_pStruct->m_arrMembers.size();
			pBuffer->Write((char*)&nStuctMemberCount, sizeof(nStuctMemberCount));
			for (int i=0; i< nStuctMemberCount;
				i++)
			{
				m_value.m_pStruct->m_arrMembers.at(i)->WriteTo(pBuffer);
			}
			
			break;
		}
	}
	

	return 0;
}

int CVariant::ReadFrom( CBuffer *pBuffer )
{
	short sType(0);
	pBuffer->Read((char*)&sType, sizeof(sType));
	m_nType = sType;

	int nSize(0);
	if (m_nType>0 && m_nType < eVariantType_metaType)
	{
		nSize = m_mapTypeSize[m_nType];
		pBuffer->Read((char*)&m_value, nSize );
	}
	else
	{
		switch(sType)
		{
		case eVariantType_string:
			{
				if (m_value.m_pObject == NULL)
				{
					m_value.m_pObject = new string();
				}
				pBuffer->Read(*(string*)m_value.m_pObject);
			}
			break;;
		case eVariantType_struct:
			//结构体
			short nStuctMemberCount(0);// = m_value.m_pStruct->m_arrVars.size();
			pBuffer->Read((char*)&nStuctMemberCount, sizeof(nStuctMemberCount));
			for (int i=0; i<nStuctMemberCount;
				i++)
			{
				CVariant *varTmp = new CVariant();
				varTmp->ReadFrom(pBuffer);
				m_value.m_pStruct->m_arrMembers.push_back(varTmp);
			}

			break;
		}
	}
	

	return 0;
}

void CVariant::Clear()
{
	if (m_nType >= eVariantType_object
		&& m_nType < eVariantType_struct)
	{
		if (m_value.m_pObject)
		{
			delete m_value.m_pObject;
			m_value.m_pObject = NULL;
		}
	}
	else if (m_nType == eVariantType_struct)
	{
		if (m_value.m_pStruct)
		{
			delete m_value.m_pStruct;
			m_value.m_pStruct = NULL;
		}
	}
}

void CVariant::StructAppendMember( CVariant *varMember )
{
	if (m_nType != eVariantType_struct )
	{
		return ;
	}
	CVariant* pNewMember = new CVariant();
	pNewMember->CopyFrom(varMember);
	m_value.m_pStruct->m_arrMembers.push_back(pNewMember);
}

void CVariant::SetValue(char* pSzText)
{
	m_value.m_pObject = new string(pSzText);
	m_nType = eVariantType_string;
}
void CVariant::SetValue(string& val)
{
	m_value.m_pObject = new string(val);
	m_nType = eVariantType_string;
}
void CVariant::SetValue(int8 val)
{
	m_value.m_i8 = val;
	m_nType = eVariantType_int8;
}
void CVariant::SetValue(uint8 val)
{
	m_value.m_u8 = val;
	m_nType = eVariantType_uint8;
}
void CVariant::SetValue(int16 val)
{
	m_value.m_i16 = val;
	m_nType = eVariantType_int16;
}
void CVariant::SetValue(uint16 val)
{
	m_value.m_u16 = val;
	m_nType = eVariantType_uint16;
}
void CVariant::SetValue(int32 val)
{
	m_value.m_i32 = val;
	m_nType = eVariantType_int32;
}
void CVariant::SetValue(uint32 val)
{
	m_value.m_u32 = val;
	m_nType = eVariantType_uint32;
}
void CVariant::SetValue(int64 val)
{
	m_value.m_i64 = val;
	m_nType = eVariantType_int64;
}
void CVariant::SetValue(uint64 val)
{
	m_value.m_u64 = val;
	m_nType = eVariantType_uint64;
}
void CVariant::SetValue(float val)
{
	m_value.m_float = val;
	m_nType = eVariantType_float;
}
void CVariant::SetValue(double val)
{
	m_value.m_double = val;
	m_nType = eVariantType_double;
}
void CVariant::SetValue(void* val)
{
	m_value.m_ptr = val;
	m_nType = eVariantType_voidPtr;
}


CVariant::_sVariantStruct::~_sVariantStruct()
{
	for (int i=0; i<m_arrMembers.size(); i++)
	{
		delete m_arrMembers.at(i);
	}
	m_arrMembers.clear();
}

CVariant::_sVariantStruct::_sVariantStruct()
{
	
}

}
