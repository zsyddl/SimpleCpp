#include "stdafx.h"
#include "newQueue.h"

namespace SimpleCpp
{

CQueue::CQueue()
{

}

CQueue::~CQueue()
{

}

int CQueue::PushToQueue(void *pJob)
{
	m_Queue.push(pJob);
	return 0;
}

void CQueue::PopFromQueue()
{
	m_Queue.pop();
}

void *CQueue::GetFromQueue()
{
	void *pJob = NULL;

	pJob = m_Queue.front();

	return pJob;
}

int CQueue::QueueSize()
{
	int nSize = 0;

	nSize = m_Queue.size();

	return nSize;
}

bool CQueue::QueueIsEmpty()
{
	bool bRet = false;

	bRet = m_Queue.empty();

	return bRet;
}

}