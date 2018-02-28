#include "stdafx.h"
#include "WinTestSemaphore.h"
#include "Semaphore.h"
#include "Thread.h"
#include "mutex.h"
#include <iostream>

using namespace SimpleCpp;
using namespace std;

TestSemaphore::TestSemaphore()
:m_pSemaphore(NULL)
,m_pThreadProducer(NULL)
,m_pThreadConsumer(NULL)
,m_bProducerExitFlag(false)
,m_bConsumerExitFlag(false)
,m_nCounter(0)
{

}

TestSemaphore::~TestSemaphore()
{

}

int TestSemaphore::Open()
{
	long lRet = 0;

	m_pProducerMutex = new CMutex;
	m_pConsumerMutex = new CMutex;

	if (NULL == m_pProducerMutex || NULL == m_pConsumerMutex)
	{
		return -1;
	}

	m_pSemaphore = new CSemaphore;
	if (NULL == m_pSemaphore)
	{
		return -1;
	}

	lRet = m_pSemaphore->Init(0);
	if (lRet < 0)
	{
		return -1;
	}

	m_pThreadProducer = new CThread;
	m_pThreadConsumer = new CThread;
	if (NULL == m_pThreadProducer || NULL == m_pThreadConsumer)
	{
		return -1;
	}

	lRet = m_pThreadProducer->Create(ProducerThread, this);
	lRet |= m_pThreadConsumer->Create(ConsumerThread, this);

	if (0 != lRet)
	{
		return -1;
	}

	return 0;
}

int TestSemaphore::ProducerThread(void *pObj)
{
	TestSemaphore *pTestSemaphore = NULL;

	pTestSemaphore = (TestSemaphore *)pObj;
	while (1)
	{
		pTestSemaphore->m_pProducerMutex->Lock();
		if (true == pTestSemaphore->m_bProducerExitFlag)
		{
			pTestSemaphore->m_pProducerMutex->Unlock();
			break;
		}
		pTestSemaphore->m_pProducerMutex->Unlock();

		pTestSemaphore->m_nCounter++;
		cout<<"Producer m_nCounter = "<<pTestSemaphore->m_nCounter<<endl;
		pTestSemaphore->m_pSemaphore->Post();

		Sleep(1000);
	}

	return 0;
}

int TestSemaphore::ConsumerThread(void *pObj)
{
	bool bRet = false;
	TestSemaphore *pTestSemaphore = NULL;

	pTestSemaphore = (TestSemaphore *)pObj;
	while (1)
	{
		pTestSemaphore->m_pConsumerMutex->Lock();
		if (true == pTestSemaphore->m_bConsumerExitFlag)
		{
			pTestSemaphore->m_pConsumerMutex->Unlock();
			break;
		}
		pTestSemaphore->m_pConsumerMutex->Unlock();

		bRet = pTestSemaphore->m_pSemaphore->Wait();
		if (false == bRet)
		{
			Sleep(1000);
			continue;
		}

		pTestSemaphore->m_nCounter--;
		cout<<"Consumer m_nCounter = "<<pTestSemaphore->m_nCounter<<endl;

		Sleep(1000);
	}

	return 0;
}

int TestSemaphore::Test()
{
	return 0;
}

int TestSemaphore::Close()
{
	m_pProducerMutex->Lock();
	m_bProducerExitFlag = true;
	m_pProducerMutex->Unlock();

	m_pConsumerMutex->Lock();
	m_bConsumerExitFlag = true;
	m_pConsumerMutex->Unlock();

	if (NULL != m_pSemaphore)
	{
		m_pSemaphore->Destory();
		delete m_pSemaphore;
		m_pSemaphore = NULL;
	}

	return 0;
}