#include "stdafx.h"
#include "Queue.h"
#include "mutex.h"

namespace SimpleCpp
{

CQueue::CQueue()
{
	m_pQueueMutex = new SimpleCpp::CMutex;
}

CQueue::~CQueue()
{

}

int CQueue::PushToQueue(void *pJob)
{
	m_pQueueMutex->Lock();
	m_Queue.push(pJob);
	m_pQueueMutex->Unlock();

	return 0;
}

void CQueue::PopFromQueue()
{
	m_pQueueMutex->Lock();
	m_Queue.pop();
	m_pQueueMutex->Unlock();
}

void *CQueue::GetFromQueue()
{
	void *pJob = NULL;

	m_pQueueMutex->Lock();
	pJob = m_Queue.front();
	m_pQueueMutex->Unlock();

	return pJob;
}

int CQueue::QueueSize()
{
	int nSize = 0;

	m_pQueueMutex->Lock();
	nSize = m_Queue.size();
	m_pQueueMutex->Unlock();

	return nSize;
}

bool CQueue::QueueIsEmpty()
{
	bool bRet = false;

	m_pQueueMutex->Lock();
	bRet = m_Queue.empty();
	m_pQueueMutex->Unlock();

	return bRet;
}

}