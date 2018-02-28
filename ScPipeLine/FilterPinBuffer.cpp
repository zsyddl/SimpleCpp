
#include "stdafx.h"
#include "FilterPinBuffer.h"
#include "ScBuffer.h"

namespace SimpleCpp{
CFilterPinBuffer::CFilterPinBuffer(int nInitSize, int nMaxSize)
: m_nMaxSize(nMaxSize)
, m_pMemory(0)
, m_nAllocSize(nInitSize)
, m_nUsedSize(0)
, m_pExtData(0)
, m_nDataType(0)
, m_nPinId(0)
{
	m_pMemory = new char[nInitSize];
	memset(m_pMemory, 0, nInitSize);
	if (nMaxSize < nInitSize)
		m_nMaxSize = nInitSize;

	m_pExtData = new CBuffer;
}
CFilterPinBuffer::~CFilterPinBuffer()
{

	if (m_pMemory)
	{
		delete m_pMemory;
		m_pMemory = NULL;
	}
	m_nUsedSize = 0;
	m_nAllocSize = 0;
	if (m_pExtData)
	{
		delete m_pExtData;
		m_pExtData = NULL;
	}
}
CFilterPinBuffer* CFilterPinBuffer::CreateObject()
{
	return new CFilterPinBuffer();
}
void CFilterPinBuffer::Release()
{
	delete this;
}
int CFilterPinBuffer::CopyFrom(CFilterPinBuffer *pBuffer)
{
	m_nUsedSize = 0;
	m_nMaxSize = pBuffer->m_nMaxSize;
	int nAppendLen = Append(pBuffer->GetData(), pBuffer->GetLength());
	if (m_pExtData)
	{
		m_pExtData->Clear();
		CBuffer *pExtSrc = pBuffer->GetExtData();
		if (pExtSrc)
		{
			m_pExtData->Append(pExtSrc->GetData(), pExtSrc->GetLength());
		}
	}
	return 0;
}
CBuffer *CFilterPinBuffer::GetExtData()
{

	return m_pExtData;
}
int CFilterPinBuffer::Append(char *pData, int nLen)
{
	/*m_lstBuffer.p*/
	if ((m_nUsedSize + nLen ) > m_nMaxSize)
	{// 超过大小限制
		/*	Clear();*/
		return 0;
	}
	else if ((m_nUsedSize + nLen ) > m_nAllocSize)
	{
		if (Resize(m_nUsedSize+nLen) <0)
			return 0;
	}

	memcpy(m_pMemory+m_nUsedSize, pData, nLen);
	m_nUsedSize += nLen;


	return nLen;
}
int CFilterPinBuffer::Append(CFilterPinBuffer *pPinBuffer)
{
	int nAppendLen = Append(pPinBuffer->GetData(), pPinBuffer->GetLength());
	return nAppendLen;
}
int CFilterPinBuffer::GetLength()
{

	return m_nUsedSize;
}
char *CFilterPinBuffer::GetData()
{

	return  m_pMemory;
}
int CFilterPinBuffer::SetUsedSize(int nUsed)
{
	if (nUsed > m_nAllocSize)
		return -1;
	m_nUsedSize = nUsed;

	return m_nUsedSize;
}
int CFilterPinBuffer::Resize(int nSize)
{
	if (nSize > m_nMaxSize)
	{
		return -1;
	}

	if (nSize <= m_nAllocSize)
		return nSize;


	unsigned char *pNewMemory = new unsigned char[nSize];
	memset(pNewMemory, 0, nSize);
	memcpy(pNewMemory, m_pMemory, m_nUsedSize);
	delete m_pMemory;
	m_pMemory = (char*)pNewMemory;

	m_nAllocSize = nSize;
	//m_nMaxSize = nSize;

	return nSize;

	// 	if (nSize <= m_nAllocSize)
	// 		return nSize;
	// 	char *pNewMemory = new char[nSize];
	// 	memset(pNewMemory, 0, nSize);
	// 	memcpy(pNewMemory, m_pMemory, m_nUsedSize);
	// 	delete m_pMemory;
	// 	m_pMemory = pNewMemory;
	// 
	// 	m_nAllocSize = nSize;
	// 	m_nMaxSize = nSize;
	// 	return nSize;
}
int CFilterPinBuffer::SetMaxSize(int nMaxSize)
{
	m_nMaxSize = nMaxSize;
	return m_nMaxSize;
}
int CFilterPinBuffer::SetAllocSize(int nSize)
{
	if (nSize > m_nMaxSize)
	{
		return -1;
	}
	if (nSize > m_nAllocSize)
	{
		char *pNewMemory = new char[nSize];
		memcpy(pNewMemory, m_pMemory, m_nUsedSize);
		delete m_pMemory;
		m_pMemory = pNewMemory;
	}
	else if (nSize < m_nAllocSize)
	{//比现有内存小
		char *pNewMemory = new char[nSize];
		memcpy(pNewMemory, m_pMemory, nSize);
		delete m_pMemory;
		m_pMemory = pNewMemory;
		m_nUsedSize = nSize;
	}
	m_nAllocSize = nSize;
	return 0;
}
int CFilterPinBuffer::GetAllocSize()
{
	return m_nAllocSize;
}
void CFilterPinBuffer::Clear(int nReversedMemoryLen)
{
	m_nUsedSize = 0;
	if (nReversedMemoryLen > 0 && nReversedMemoryLen != m_nAllocSize)
	{
		char *pNewMemory = new char[nReversedMemoryLen];
		memset(pNewMemory, 0, nReversedMemoryLen);
		delete m_pMemory;
		m_pMemory = pNewMemory;
		m_nAllocSize = nReversedMemoryLen;
	}
}
void CFilterPinBuffer::Clear(int nPos, int nSize)
{
	if (nPos<0 || nPos >=m_nUsedSize || nSize<=0)
		return ;
	if ((nSize+nPos) >= m_nUsedSize)
		nSize = m_nUsedSize-nPos;
	if (nPos == 0)
	{
		memmove(m_pMemory, m_pMemory+nSize, m_nUsedSize-nSize);
		m_nUsedSize -= nSize;
	}else{

	}
	return;
}
void *CFilterPinBuffer::operator new(size_t size){
	void *p = new char[size];
	return p;
};

int CFilterPinBuffer::GetPinId()
{
	return m_nPinId;
}

}