
#ifndef FILTER_PIN_BUFFER_H
#define FILTER_PIN_BUFFER_H

#include "PipeLineDef.h"
	
namespace SimpleCpp{
class CBuffer;
class PIPELINE_API CFilterPinBuffer
{
public:
	CFilterPinBuffer(int nInitSize=10*1024, int nMaxSize=1024*1024);
	virtual ~CFilterPinBuffer();

	virtual int CopyFrom(CFilterPinBuffer *pBuffer);

	int Append(char *pData, int nLen);
	int Append(CFilterPinBuffer *pPinBuffer);

	int GetLength();
	char *GetData();
	virtual void Clear(int nReversedMemoryLen=0);
	virtual void Clear(int nPos, int nSize);
	int Resize(int nSize);
	int SetMaxSize(int nMaxSize);
	int GetMaxSize(){return m_nMaxSize;}
	int SetUsedSize(int nUsed);
	int GetUsedSize(){return m_nUsedSize;}
	//
	int SetAllocSize(int nSize);
	int GetAllocSize();


	//
	CBuffer *GetExtData();
	void SetDataType(int nType){m_nDataType = nType;};

	void *operator new(size_t size);
	int		m_nDataType;

	virtual CFilterPinBuffer* CreateObject();
	virtual void Release();
	//
	int		GetPinId();
protected:
	char	*m_pMemory;
	int		m_nAllocSize;
	int		m_nUsedSize;
	int		m_nMaxSize ;

	CBuffer *m_pExtData;
	//
	int		m_nPinId;
};
}
#endif