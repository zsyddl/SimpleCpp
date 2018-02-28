

#include "StdAfx.h"

#include "ScBuffer.h"
#include <memory.h>
#include <string.h>
//#include "SerializeObject.h"

#define  debug

//using namespace SimpleSocket;
using namespace SimpleCpp;
CBuffer::CBuffer(int nInitSize, int nMaxSize)
:m_nAllocSize(nInitSize)
, m_nUsedSize(0)
, m_nMaxSize(nMaxSize)
, m_nCursor(0)
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
	m_nCursor = 0;

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
	m_nCursor -= nPos;
	if (m_nCursor < 0)
	{
		m_nCursor = 0;
	}
	return;
}

int CBuffer::SerializeWrite( char *pBuffer, int nLen )
{
	int nRet = Append((unsigned char*)pBuffer, nLen);

	return nRet;
}

int CBuffer::SerializeRead( char *pBuffer, int nLen )
{
	if (m_nUsedSize < nLen)
	{
		return -1;
	}
	memcpy(pBuffer, m_pMemory+m_nUsedSize-1-nLen, nLen);
	m_nUsedSize -= nLen;

	return nLen;
}

int CBuffer::Seek( int nPos )
{
	if (nPos >= m_nMaxSize)
	{
		nPos = m_nUsedSize-1;
	}
	else if (nPos >= m_nUsedSize)
	{
		nPos = m_nUsedSize-1;
	}
	else if (nPos < 0)
	{
		nPos = 0;
	}

	m_nCursor = nPos;

	return nPos;
}

int CBuffer::Write( char *pBuffer, int nLen )
{
	int nRet = Append((unsigned char*)pBuffer, nLen);
	
	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(char* pText)
{
	int nLen = strlen(pText);
	int nRet = Append((unsigned char*)pText, nLen);

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int8 val)
{
	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint8 val)
{

	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int16 val)
{

	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint16 val)
{

	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int32 val)
{

	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint32 val)
{

	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int64 val)
{

	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint64 val)
{
	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(float val)
{
	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(double val)
{
	int nRet = Append((unsigned char*)&val, sizeof(val));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
/*
int		CBuffer::Write(int8* val)
{
	int nRet = Append((unsigned char*)val, sizeof(int8));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint8* val)
{

	int nRet = Append((unsigned char*)val, sizeof(uint8));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int16* val)
{

	int nRet = Append((unsigned char*)val, sizeof(int16));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint16* val)
{

	int nRet = Append((unsigned char*)val, sizeof(uint16));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int32* val)
{

	int nRet = Append((unsigned char*)val, sizeof(int32));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint32* val)
{

	int nRet = Append((unsigned char*)val, sizeof(uint32));
	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(int64* val)
{

	int nRet = Append((unsigned char*)val, sizeof(int64));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(uint64* val)
{
	int nRet = Append((unsigned char*)val, sizeof(uint64));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(float* val)
{
	int nRet = Append((unsigned char*)val, sizeof(float));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}
int		CBuffer::Write(double* val)
{
	int nRet = Append((unsigned char*)val, sizeof(double));

	m_nCursor = m_nUsedSize-1;

	return nRet;
}*/
int CBuffer::Write( string &strBuffer )
{
	int nBufferLen = strBuffer.size();
	Write((char*)&nBufferLen, sizeof(nBufferLen));

	char *pBuffer = (char*)strBuffer.data();
	Write(pBuffer, nBufferLen);

	return 0;
}
int		CBuffer::Write(buffer& val)
{
	int nBufferLen = val.size();
	Write((char*)&nBufferLen, sizeof(nBufferLen));

	char *pBuffer = (char*)&val.at(0);
	Write(pBuffer, nBufferLen);

	return 0;
}

// int CBuffer::Write(CSerializeObject& object)
// {
// 
// 	return object.Write(this);
// }

int CBuffer::Read( char *pBuffer, int nLen )
{
	if (m_nUsedSize < (m_nCursor+nLen) )
	{
		return -1;
	}
	memcpy(pBuffer, m_pMemory+m_nCursor, nLen);
	m_nCursor += nLen;

	return nLen;
}


int CBuffer::Read( string &strBuffer )
{
	int nBufferLen(0);
	if (m_nUsedSize < (m_nCursor+sizeof(nBufferLen)) )
	{//
		debug("%s: error string len\n", __FUNCTION__);
		return -1;
	}

	//Write((char*)&nBufferLen, sizeof(nBufferLen));
	Read((char*)&nBufferLen, sizeof(nBufferLen));

	if (m_nUsedSize < (m_nCursor+nBufferLen) )
	{//
		debug("%s: error string len\n", __FUNCTION__);
		return -1;
	}
	strBuffer.resize(nBufferLen);
	char *pBuffer = (char*)strBuffer.data();
	Read(pBuffer, nBufferLen);

	return 0;
}
int		CBuffer::Read(buffer& val)
{
	int nBufferLen(0);
	if (m_nUsedSize < (m_nCursor+sizeof(nBufferLen)) )
	{//
		debug("%s: error string len\n", __FUNCTION__);
		return -1;
	}

	//Write((char*)&nBufferLen, sizeof(nBufferLen));
	Read((char*)&nBufferLen, sizeof(nBufferLen));
	if (m_nUsedSize < (m_nCursor+nBufferLen) )
	{//
		debug("%s: error string len\n", __FUNCTION__);
		return -1;
	}
	val.resize(nBufferLen);
	char *pBuffer = (char*)&val.at(0);
	Read(pBuffer, nBufferLen);
	
	return 0;
}
int		CBuffer::Read(int8& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;

	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(uint8& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(int16& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(uint16& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(int32& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(uint32& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(int64& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(uint64& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(float& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
int		CBuffer::Read(double& val)
{
	if (m_nUsedSize < (m_nCursor+sizeof(val)) )
	{
		return -1;
	}
	int nLen = 0;
	nLen = Read((char*)&val, sizeof(val));
	return nLen;
}
// 
// int CBuffer::Read(CSerializeObject& object)
// {
// 
// 	return object.Read(this);
// }

int CBuffer::GetPos()
{
	return m_nCursor;
}


#ifdef CVARIANT_TYPE
// int CBuffer::Write( CVariant& varBuffer )
// {
// 	return 0;
// }
// int CBuffer::Read( CVariant& varBuffer )
// {
// 
// 	return 0;
// }

#endif





//inline unsigned char* CBuffer::GetData()
//{
//	return m_pMemory;
//}
//inline int CBuffer::GetLength()
//{
//	return m_nUsedSize;
//}
