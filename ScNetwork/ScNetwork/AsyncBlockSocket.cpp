#include "AsyncBlockSocket.h"
#include "Thread.h"
#include "Buffer.h"
#include "mutex.h"
#include "Event.h"
#include "stdafx.h"

using namespace SimpleCpp;

CAsyncBlockSocket::CAsyncBlockSocket(void)
: m_pBuffer(NULL)
, m_pThread(NULL)
, m_nReadyReadBytes(0)
{
	m_pBuffer = new CBuffer();
	m_pBuffer->Resize(100*1024);

	m_pMutexRecvBuffer = new CMutex();

	m_pEventReadyRead = new CEvent();
	int nRet = 0;
	nRet = m_pEventReadyRead->Create();
}

CAsyncBlockSocket::~CAsyncBlockSocket(void)
{
	
	CSocket::Close();
	 
	if (m_pThread)
	{
		m_pThread->Stop();
		m_pThread->Close();
		delete m_pThread;
		m_pThread = NULL;
	}

	if (m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	if (m_pMutexRecvBuffer)
	{
		delete m_pMutexRecvBuffer;
		m_pMutexRecvBuffer = NULL;
	}
	if (m_pEventReadyRead)
	{
		m_pEventReadyRead->Set();
		delete m_pEventReadyRead;
		m_pEventReadyRead = NULL;
	}

}

int CAsyncBlockSocket::ThreadProc(void *pData)
{
	CAsyncBlockSocket* pSocket = (CAsyncBlockSocket*)(pData);
	if (pSocket)
	{
		pSocket->Run();
	}
	return 0;
}

int CAsyncBlockSocket::Run()
{
	char *pRecvTmp = new char[100*1024];
	while (m_pThread->GetState() != eThreadState_Stop
			&& m_pThread->GetState() != eThreadState_ToStop)
	{
		int nRecvRet = 0;
		
		if (m_hSocket == NULL)
		{
			//m_pMutexRecvBuffer->Unlock();
			CThread::Sleep(1);
			continue;
		} 
		m_pMutexRecvBuffer->Lock();
		int nBufferRemain = m_pBuffer->GetAllocSize()-m_pBuffer->GetUsedSize();
		m_pMutexRecvBuffer->Unlock();
		nRecvRet = CSocket::Recv( pRecvTmp, nBufferRemain );
		if (nRecvRet > 0)
		{
			m_pMutexRecvBuffer->Lock();
			m_pBuffer->Append((unsigned char*)pRecvTmp, nRecvRet);
			m_pMutexRecvBuffer->Unlock();
		}
		
		if (nRecvRet == -1)
		{
			CThread::Sleep(1);
		}
		//判断 是 listen 还是 普通 socket
		//

		if (m_pBuffer->GetUsedSize() > 0)
		{
			if (m_pBuffer->GetUsedSize() >= m_nReadyReadBytes )
			{
				m_pEventReadyRead->Set();
			}

			OnRecv(0);
		}
	
	}
	delete pRecvTmp;
	debug("%s--\n", __FUNCTION__);
	return 0;
}

int SimpleCpp::CAsyncBlockSocket::Recv(char *pBuf, int nLen)
{
	if (m_pMutexRecvBuffer->Lock(10) == false)
		return -1;
	int nRecvRet = 0;
	nRecvRet = min(nLen, m_pBuffer->GetUsedSize());
	if (nRecvRet > 0)
	{
		memcpy(pBuf, m_pBuffer->GetData(), nRecvRet);
		m_pBuffer->Clear(nRecvRet);
	}
	//清空标记
	if (m_pBuffer->GetUsedSize() <= 0)
	{
		m_pEventReadyRead->Reset();
	}

	m_pMutexRecvBuffer->Unlock();

	return nRecvRet;
}

int SimpleCpp::CAsyncBlockSocket::Attach(HSOCKET hSocket)
{
	int nRet = 0;
	nRet = CSocket::Attach(hSocket);
	CSocket::SetBlock(true);
	if (!m_pThread)
	{
		m_pThread = new SimpleCpp::CThread();
		if (m_pThread->Create(ThreadProc, this) < 0)
		{
			nRet = -1;
		}
	}
	
	return nRet;
}

int SimpleCpp::CAsyncBlockSocket::Create(eSocketType nType)
{
	int nRet=0;
	nRet = CSocket::Create(nType);
	if (nRet < 0)
	{
		return -1;
	}
	CSocket::SetBlock(true);
	if (!m_pThread)
	{
		m_pThread = new SimpleCpp::CThread();
		if (m_pThread->Create(ThreadProc, this) < 0)
		{
			nRet = -1;
		}
	}

	return nRet;
}

bool SimpleCpp::CAsyncBlockSocket::WaitForReadyRead(int nMilliSeconds/*=1000*/, int nBytes/*=0*/)
{
	m_nReadyReadBytes = nBytes;
	bool bRet = m_pEventReadyRead->Wait(nMilliSeconds);

	return bRet;
}

int CAsyncBlockSocket::Close()
{
	int nRet = 0;
	nRet = CSocket::Close();

	if (m_pThread)
	{
		m_pThread->Stop();
		m_pThread->Close();
		delete m_pThread;
		m_pThread = NULL;
	}
	
	return nRet;
}
// 
// int SimpleSocket::CAsyncBlockSocket::Accept(CSocket *pConnectedsocket)
// {
// 	int nRet = -1;
// 	nRet = CSocket::Accept(pConnectedsocket);
// 	CAsyncSocket* pAsyncSocket = dynamic_cast<CAsyncSocket*>(pConnectedsocket);
//  
// 
// 	return nRet;
// }
