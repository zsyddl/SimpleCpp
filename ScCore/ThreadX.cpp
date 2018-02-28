
#include "Thread.h"
#include "mutex.h"
#include "ThreadMessage.h"

using namespace std;
using namespace SimpleCpp;

CThreadMessage * SimpleCpp::CThread::GetMessage()
{
    CThreadMessage *pMessage = NULL;

	m_pMutexMessageQueue->Lock();
	pMessage = m_queueMessage.front();
	m_queueMessage.pop_front();
	m_pMutexMessageQueue->Unlock();

	return pMessage;
}

int SimpleCpp::CThread::PostMessage( CThreadMessage *pMessage )
{
	m_pMutexMessageQueue->Lock();
	m_queueMessage.push_back(pMessage);
	m_pMutexMessageQueue->Unlock();
	return 0;
}

int SimpleCpp::CThread::PostMessage( CThread *pSender, CThreadMessage* pMessage, CThread *pRecver )
{
	pMessage->m_pSender = pSender;
	pMessage->m_pReceiver = pRecver;

	pRecver->PostMessage(pMessage);

	return 0;
}

int SimpleCpp::CThread::PostMessage( CThreadMessage *pMessage, CThread *pReceiver )
{
	pMessage->m_pSender = this;
	pMessage->m_pReceiver = pReceiver;

	m_pMutexMessageQueue->Lock();
	m_queueMessage.push_back(pMessage);
	m_pMutexMessageQueue->Unlock();

	return 0;
}

int SimpleCpp::CThread::GetMessageCount()
{
	int nSize = 0;
	m_pMutexMessageQueue->Lock();
	nSize = m_queueMessage.size();
	m_pMutexMessageQueue->Unlock();

	return nSize;
}

CThreadMessage * SimpleCpp::CThread::NewMessage()
{
	CThreadMessage *pMessage =  new CThreadMessage();
	return pMessage;
}

void SimpleCpp::CThread::DeleteMessage( CThreadMessage* pMessage )
{
	delete pMessage;
}

