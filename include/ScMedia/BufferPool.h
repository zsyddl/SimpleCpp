#pragma once

#include <list>

using namespace std;
/*
* 缓存池，用于循环使用缓存，包括空闲的准备好的 
*/
namespace SimpleCpp{
class CMutex;
}

template <class T>
class CBufferPool
{
public:
	CBufferPool(void);
	virtual ~CBufferPool(void);

	int		Create(int nSize);

	void	ClearBusyBuffer();
	T*		GetFreeBuffer();
	void	PutFreeBuffer(T* pBuffer);
	int		GetFreeBufferCount();
	T*		GetBusyBuffer(bool bFromHead=true);
	void    PutBusyBuffer(T *pBuffer, bool bToTail=true);
	int		GetBusyBufferCount();
protected:
	list<T*> m_lstBusyBuffer;
	list<T*> m_lstFreeBuffer;
	//CMutex *m_pMutexBuffer;
};

template <class T>
int CBufferPool<T>::GetBusyBufferCount()
{
	return m_lstBusyBuffer.size();
}

template <class T>
int CBufferPool<T>::GetFreeBufferCount()
{
	return m_lstFreeBuffer.size();
}

template <class T>
CBufferPool<T>::CBufferPool(void)
{
}
template <class T>
CBufferPool<T>::~CBufferPool(void)
{
	list<T*>::iterator item;
	for (item=m_lstBusyBuffer.begin(); item != m_lstBusyBuffer.end(); item++)
	{
		T *pBuffer = *item;
		delete pBuffer;
	}
	m_lstBusyBuffer.clear();
	for (item=m_lstFreeBuffer.begin(); item != m_lstFreeBuffer.end(); item++)
	{
		T *pBuffer = *item;
		delete pBuffer;
	}
	m_lstFreeBuffer.clear();
}
template <class T>
int CBufferPool<T>::Create(int nSize)
{
	for (int i=0; i<nSize; i++)
	{
		T *pBuffer = new T;
		m_lstFreeBuffer.push_back(pBuffer);
	}
	return m_lstFreeBuffer.size();
}
template <class T>
T*		CBufferPool<T>::GetFreeBuffer()
{
	if (m_lstFreeBuffer.size() <= 0)
	{
		return NULL;
	}
	
	T *pBuffer = m_lstFreeBuffer.front();

	m_lstFreeBuffer.pop_front();

	return pBuffer;
}
template <class T>
void	CBufferPool<T>::PutFreeBuffer(T* pBuffer)
{
	m_lstFreeBuffer.push_back(pBuffer);
}
template <class T>
T*		CBufferPool<T>::GetBusyBuffer(bool bFromHead/*=true*/)
{
	if (m_lstBusyBuffer.size() <= 0)
	{
		return NULL;
	}
		
	T *pBuffer = 0;
	if (bFromHead)
	{
		pBuffer = m_lstBusyBuffer.front();
		m_lstBusyBuffer.pop_front();
	}
	else
	{
		pBuffer = m_lstBusyBuffer.back();
		m_lstBusyBuffer.pop_back();
	}

	return pBuffer;
}
template <class T>
void    CBufferPool<T>::PutBusyBuffer(T *pBuffer, bool bToTail/*=true*/)
{
	if (bToTail)
	{
		m_lstBusyBuffer.push_back(pBuffer);
	}
	else
	{
		m_lstBusyBuffer.push_front(pBuffer);
	}
}
template <class T>
void    CBufferPool<T>::ClearBusyBuffer()
{
	list<T*>::iterator item;
	for (item=m_lstBusyBuffer.begin(); item != m_lstBusyBuffer.end(); item++)
	{
		m_lstFreeBuffer.push_back(*item);
	}
	m_lstBusyBuffer.clear();
}
