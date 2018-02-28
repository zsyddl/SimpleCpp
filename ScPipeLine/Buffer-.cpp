

#include "StdAfx.h"


#include "Buffer.h"
#include <memory.h>
#include <string.h>

CBuffer::CBuffer(int nInitSize, int nMaxSize)
:m_nAllocSize(nInitSize)
, m_nUsedSize(0)
, m_nMaxSize(nMaxSize)
{
	if (nMaxSize < nInitSize)
	{
		m_nMaxSize = nInitSize;
	}
	m_pMemory = new unsigned char[nInitSize];
	m_nUsedSize = 0;
	memset(m_pMemory, 0, nInitSize);


}

CBuffer::~CBuffer(void)
{
	if (m_pMemory)
	{
		delete m_pMemory;
		m_pMemory = 0;
		m_nUsedSize = 0;
		m_nAllocSize = 0;
		m_nMaxSize = 0;
	}
}
int CBuffer::CopyFrom(CBuffer *pBufferFrom)
{
	if (!pBufferFrom 
		|| pBufferFrom == this)
	{
		return -1;
	}
	if (Resize(pBufferFrom->GetLength()) < 0)
		return -1;
	
	memcpy(m_pMemory, pBufferFrom->GetData(), pBufferFrom->GetLength());

	m_nUsedSize = pBufferFrom->GetLength();

	return m_nUsedSize;
}
int CBuffer::Append(unsigned char *pData, int nLen)
{
	if ((m_nUsedSize + nLen ) >= m_nMaxSize)
	{// 超过大小限制
		/*	Clear();*/
		//debug("append error\n")
		return -1;
	}
	else if ((m_nUsedSize + nLen ) >= m_nAllocSize)
	{
		Resize(m_nUsedSize+nLen) ;
	}

	memcpy(m_pMemory+m_nUsedSize, pData, nLen);
	m_nUsedSize += nLen;


	return m_nUsedSize;
}
int CBuffer::Append(CBuffer *pBuffer)
{
	if (!pBuffer)
		return -1;
	int nSizeTotal = m_nUsedSize + pBuffer->GetLength();
	if (nSizeTotal > m_nMaxSize)
		return -2;
	if (nSizeTotal > m_nAllocSize)
	{
		Resize(nSizeTotal);
	}
	memcpy(m_pMemory+m_nUsedSize, pBuffer->GetData(), pBuffer->GetLength());
	m_nUsedSize = nSizeTotal;
	
	return 0;
}
int CBuffer::SetUsed(int nUsedSize)
{
	if (nUsedSize < 0)
	{
		return 0;
	}
	if (nUsedSize > m_nMaxSize)
	{
		return -1;
	}
	else if (nUsedSize > m_nAllocSize)
	{

		return -2;
	}

	m_nUsedSize = nUsedSize;

	return 0;
}
int CBuffer::SetMaxSize(int nMaxSize)
{
	if (nMaxSize >m_nMaxSize)
	{
		m_nMaxSize = nMaxSize;
	}
	else if (nMaxSize < m_nMaxSize && nMaxSize > m_nAllocSize)
	{
		m_nMaxSize = nMaxSize;
	}
	else if (nMaxSize < m_nAllocSize && nMaxSize > m_nUsedSize)
	{//重新分配内存, 但是 使用的 长度不变
		char *pNewMemory = new char[nMaxSize];
		memcpy(pNewMemory, m_pMemory, m_nUsedSize);
		delete m_pMemory;
		m_pMemory = (unsigned char*)pNewMemory;
		m_nAllocSize = nMaxSize;
		m_nMaxSize = nMaxSize;
	}
	else if (nMaxSize < m_nUsedSize )
	{
		//重新分配内存,  使用的 长度 被截断
		char *pNewMemory = new char[nMaxSize];
		memcpy(pNewMemory, m_pMemory, nMaxSize);
		delete m_pMemory;
		m_pMemory = (unsigned char*)pNewMemory;
		m_nAllocSize = nMaxSize;
		m_nMaxSize = nMaxSize;
		m_nUsedSize = nMaxSize;
	}
	return 0;
}
int CBuffer::SetAllocSize(int nAllocSize)
{
	if (nAllocSize > m_nMaxSize)
	{
		m_nMaxSize = nAllocSize;
	}
	if (nAllocSize > m_nAllocSize )
	{//需要更多的内存，重新分配内存, 数据不变
		char *pNewMemory = new char[nAllocSize];
		memcpy(pNewMemory, m_pMemory, m_nUsedSize);
		delete m_pMemory;
		m_pMemory = (unsigned char*)pNewMemory;
		m_nAllocSize = nAllocSize;
	}
	else if (nAllocSize < m_nAllocSize && nAllocSize > m_nUsedSize)
	{//
		//需要更少的内存，重新分配内存, 数据不变
		char *pNewMemory = new char[nAllocSize];
		memcpy(pNewMemory, m_pMemory, m_nUsedSize);
		delete m_pMemory;
		m_pMemory = (unsigned char*)pNewMemory;
		m_nAllocSize = nAllocSize;
	}
	else  if (nAllocSize < m_nUsedSize)
	{
		//需要更少的内存，重新分配内存, 数据截断
		char *pNewMemory = new char[nAllocSize];
		memcpy(pNewMemory, m_pMemory, nAllocSize);
		delete m_pMemory;
		m_pMemory = (unsigned char*)pNewMemory;
		m_nAllocSize = nAllocSize;
		m_nUsedSize = nAllocSize;
	}
	return 0;
}
int CBuffer::GetAllocSize()
{
	return m_nAllocSize;
}
int CBuffer::GetUsedSize()
{
	return m_nUsedSize;
}
int CBuffer::Resize(int nSize)
{
	if (nSize >= m_nMaxSize)
	{
		return -1;
	}

	if (nSize <= m_nAllocSize)
		return nSize;



	unsigned char *pNewMemory = new unsigned char[nSize];
	memset(pNewMemory, 0, nSize);
	memcpy(pNewMemory, m_pMemory, m_nUsedSize);
	delete m_pMemory;
	m_pMemory = pNewMemory;

	m_nAllocSize = nSize;
	
	return nSize;
}
void CBuffer::Clear(int nReversedMemoryLen)
{
	m_nUsedSize = 0;
	if (nReversedMemoryLen > 0)
	{
		unsigned char *pNewMemory = new unsigned char[nReversedMemoryLen];
		memset(pNewMemory, 0, nReversedMemoryLen);
		delete m_pMemory;
		m_pMemory = pNewMemory;
		m_nAllocSize = nReversedMemoryLen;
	}
}
void CBuffer::Clear(int nPos, int nSize)
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

//inline unsigned char* CBuffer::GetData()
//{
//	return m_pMemory;
//}
//inline int CBuffer::GetLength()
//{
//	return m_nUsedSize;
//}
