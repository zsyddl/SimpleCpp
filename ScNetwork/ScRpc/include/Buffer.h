#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#ifndef MEDIASDK_API
#define MEDIASDK_API
#endif

#include <string>
#include "Types.h"
#include "ScpDefine.h"

using namespace std;

class CSerializeObject;
//
typedef enum _eValueStorageFlag{
	eValueStorageFlag_Value=0,
	eValueStorageFlag_Type = 0x01,
	eValueStorageFlag_Name = 0x02,
}eValueStorageFlag;
//
//
class SCP_API CBuffer
{
public:
	CBuffer(int nInitSize = 1024, int nMaxSize=1024*1024);
	virtual ~CBuffer(void);

	
	
	int CopyFrom(CBuffer *pBufferFrom);
	int Append(unsigned char *pData, int nLen);
	int Append(CBuffer *pBuffer);

	int Resize(int nSize);
	int SetUsed(int nUsedSize);
	int SetMaxSize(int nMaxSize);
	int SetAllocSize(int nAllocSize);
	int GetAllocSize();
	int GetUsedSize();
	//从开始保留一定长度， 选择释放多于的内存, ==0 保持原有内存
	void Clear(int nReversedMemoryLen=0);
	//清除 之中  nLen长的数据
	void Clear(int nPos, int nLen);
	//从开始保留一定长度， 选择释放多于的内存
	//void Clear(int nReversedMemoryLen);
	inline char *GetData(char *pBuffer, int nStart, int nEnd){
		if (nStart > nEnd)
			return NULL;
		if (nEnd > m_nUsedSize)
			return NULL;
		memcpy(pBuffer, m_pMemory+nStart, nEnd-nStart+1);

		return pBuffer;
	};
    inline unsigned char* GetData()
    {
        return m_pMemory;
    }
    inline int GetLength(){
        return m_nUsedSize;
    }
	inline  int GetMaxSize()
	{
		return m_nMaxSize;
	}

	//
	int		Seek(int nPos);
	int		GetPos();
	int		Write(char *pBuffer, int nLen);
	int		Write(CSerializeObject& objet);
	template <class T>
	int		Write(vector<T*>& arrVal);
	template <class T>
	int		Write(vector<T>& arrVal);
	template <class T> 
	int		Write(T& object);
	//void	BeginRead();

	//
	int		Write(buffer& val);
	int		Write(string &val);
	int		Write(char* pText);
	int		Write(int8 val);
	int		Write(uint8 val);
	int		Write(int16 val);
	int		Write(uint16 val);
	int		Write(int32 val);
	int		Write(uint32 val);
	int		Write(int64 val);
	int		Write(uint64 val);
	int		Write(float val);
	int		Write(double val);

//	int		Write(int8* val);
// 	int		Write(uint8* val);
// 	int		Write(int16* val);
// 	int		Write(uint16* val);
// 	int		Write(int32* val);
// 	int		Write(uint32* val);
// 	int		Write(int64* val);
// 	int		Write(uint64* val);
// 	int		Write(float* val);
// 	int		Write(double* val);

	int		Read(char *pBuffer, int nLen);
	int		Read(string &strBuffer);
	int		Read(buffer& val);
	int		Read(CSerializeObject& object);
	template <class T>
	int		Read(T& object);

	template <class T>
	int		Read(vector<T*>& arrVal);
	template <class T>
	int		Read(vector<T>& arrVal);

	int		Read(int8& val);
	int		Read(uint8& val);
	int		Read(int16& val);
	int		Read(uint16& val);
	int		Read(int32& val);
	int		Read(uint32& val);
	int		Read(int64& val);
	int		Read(uint64& val);
	int		Read(float& val);
	int		Read(double& val);

	template <class T>
	static void	Free(vector<T*>& arrVal);
	template <class T>
	static void	Free(vector<T>& arrVal);
	//
#ifdef CVARIANT_TYPE
//	int		Write(CVariant& varBuffer);
//	int		Read(CVariant&	varBuffer);
#endif
	//void	EndRead();
	//写在末尾
	int		SerializeWrite(char *pBuffer, int nLen);
	//从末尾读取, 读取之后，末尾数据标记为无效

	//
	int		SerializeRead(char *pBuffer, int nLen);
	//
protected:
	unsigned char *m_pMemory;
	int		m_nUsedSize;
	int		m_nAllocSize;
	int		m_nMaxSize;
	//
	int		m_nCursor;
	//数据存储标识
	eValueStorageFlag		m_nValueFlag;
};



template <class T>
void CBuffer::Free(vector<T>& arrVal)
{
	arrVal.clear();
}

template <class T>
void CBuffer::Free(vector<T*>& arrVal)
{
	for (int i=0; i<arrVal.size(); i++)
	{
		delete arrVal.at(i);
	}
	arrVal.clear();
}

template <class T>
int CBuffer::Write(T& object)
{

	return object.Write(this);
}

template <class T>
int CBuffer::Read(T& object)
{

	return object.Read(this);
}
template <class T>
int CBuffer::Read(vector<T>& arrVal)
{
	int nRet = 0;
	int nReadLen = 0;
	for (int i=0; i<arrVal.size(); i++)
	{
		T newObject;
		nRet = Read(newObject);
		arrVal.push_back(newObject);
		if (nRet < 0)
		{
			return nRet;
		}

		nReadLen += nRet;
	}
	return nReadLen;
}

template <class T>
int CBuffer::Read(vector<T*>& arrVal)
{
	int nRet = 0;
	int nCount = 0;
	Read(nCount);

	int nReadLen = 0;
	for (int i=0; i<nCount; i++)
	{
		T* pNewObj = new T;
		arrVal.push_back(pNewObj);

		nRet = Read(*pNewObj);
		if (nRet < 0)
		{
			return nRet;
		}
		nReadLen += nRet;
	}
	return nReadLen;
}

template <class T>
int CBuffer::Write(vector<T>& arrVal)
{
	int nRet = 0;
	int nCount = arrVal.size();
	Write(nCount);

	int nWriteLen = 0;
	for (int i=0; i<arrVal.size(); i++)
	{
		nRet = Write(arrVal.at(i));
		if (nRet < 0)
		{
			return nRet;
		}
		nWriteLen += nRet;
	}
	return nWriteLen;
}

template <class T>
int CBuffer::Write(vector<T*>& arrVal)
{
	int nRet = 0;
	int nCount = arrVal.size();
	Write(nCount);

	int nWriteLen = 0;
	for (int i=0; i<arrVal.size(); i++)
	{
		nRet = Write(*arrVal.at(i));
		if (nRet < 0)
		{
			return nRet;
		}
		nWriteLen += nRet;
	}
	return nWriteLen;
}

#endif
