#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#ifndef MEDIASDK_API
#define MEDIASDK_API
#endif

class MEDIASDK_API CBuffer
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
	
protected:
	unsigned char *m_pMemory;
	int m_nUsedSize;
	int m_nAllocSize;
	int m_nMaxSize;
};

#endif
