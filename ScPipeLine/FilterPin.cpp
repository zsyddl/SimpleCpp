#include "StdAfx.h"
#include "FilterPin.h"
#include "BaseFilter.h"
#include "Thread.h"
#include "mutex.h"
#include "FilterPinBuffer.h"

namespace SimpleCpp{
using namespace SimpleCpp;

CFilterPin::CFilterPin(CBaseFilter *pFilter)
: m_nBufferCount(0)
, m_pFilter(pFilter)
, m_nReversePinBufferMemorySize(10*1024)
{
	m_pMutexBusyBuffer = new CMutex();
	m_pMutexFreeBuffer = new CMutex();

	m_pSigPinOut = new sigc::signal<int, CFilterPin*, CFilterPinBuffer*>;
}
CFilterPin::~CFilterPin()
{
	list<CFilterPinBuffer*>::iterator item;
	for (item = m_lstFreeBuffer.begin(); item != m_lstFreeBuffer.end(); item++)
	{
		CFilterPinBuffer* pPinBuffer = *item;
		if (pPinBuffer)
		{
			delete pPinBuffer;
			*item = NULL;
		}
	}
	for (item = m_lstBusyBuffer.begin(); item != m_lstBusyBuffer.end(); item++)
	{
		CFilterPinBuffer* pPinBuffer = *item;
		if (pPinBuffer)
		{
			delete pPinBuffer;
			*item = NULL;
		}
	}
	m_lstBusyBuffer.clear();
	m_lstFreeBuffer.clear();

	if (m_pMutexFreeBuffer)
	{
		delete m_pMutexFreeBuffer;
		m_pMutexFreeBuffer = NULL;
	}
	if (m_pMutexBusyBuffer)
	{
		delete m_pMutexBusyBuffer;
		m_pMutexBusyBuffer = NULL;
	}

	if (m_pSigPinOut)
	{
		delete m_pSigPinOut;
		m_pSigPinOut = NULL;
	}
}
int CFilterPin::SetAutoFreeMemory(int nReversedMemorySize)
{
	m_nReversePinBufferMemorySize = nReversedMemorySize;
	return m_nReversePinBufferMemorySize;
}
int CFilterPin::GetBuffer(CFilterPinBuffer **pBuffer)
{
	if (m_nIOType == eFPT_In)
	{
		if (m_lstFreeBuffer.size() > 0)
		{	
			m_pMutexFreeBuffer->Lock();
			*pBuffer = m_lstFreeBuffer.front();
			m_lstFreeBuffer.pop_front();
			m_pMutexFreeBuffer->Unlock();
		}
	}
	else
	{
		if (m_lstBusyBuffer.size() > 0)
		{
			m_pMutexBusyBuffer->Lock();
			*pBuffer = m_lstBusyBuffer.front();
			m_lstBusyBuffer.pop_front();
			m_pMutexBusyBuffer->Unlock();
		}
	}
	return 0;
}
int CFilterPin::PutBuffer(CFilterPinBuffer *pBuffer)
{
	if (pBuffer == NULL)
		return -1;
	if (m_nIOType == eFPT_In)
	{
		m_pMutexBusyBuffer->Lock();
		m_lstBusyBuffer.push_back(pBuffer);
		m_pMutexBusyBuffer->Unlock();
	}
	else
	{
		m_pMutexFreeBuffer->Lock();
		m_lstFreeBuffer.push_back(pBuffer);
		m_pMutexFreeBuffer->Unlock();
	}
	return 0;
}
int CFilterPin::Init(eFilterPinIOType ioType, int nBufferCount)
{
	m_nIOType = ioType;
	m_nBufferCount = nBufferCount;
	for (int i=0; i<nBufferCount; i++)
	{
		CFilterPinBuffer *pBuffer = new CFilterPinBuffer;
		m_lstFreeBuffer.push_back(pBuffer);
	}
	return 0;
}
int CFilterPin::GetFreeBuffer(CFilterPinBuffer **pBuffer)
{	
	m_pMutexFreeBuffer->Lock();
	if (m_lstFreeBuffer.size() <= 0)
	{
		m_pMutexFreeBuffer->Unlock();
		return -1;
	}
	*pBuffer = m_lstFreeBuffer.front();

	m_lstFreeBuffer.pop_front();
	m_pMutexFreeBuffer->Unlock();
	return m_lstFreeBuffer.size();
}
int CFilterPin::PutPreparedBuffer(CFilterPinBuffer *pBuffer,  bool bToTail/* = true*/  )
{
	if (!pBuffer)
	{
		return -1;
	}
	m_pMutexBusyBuffer->Lock();
	//如果是 输出 pin， 将数据分发到  所有连接的 输入pin

	if (m_nIOType == eFPT_Out)
	{
		if (bToTail)
		{
			m_lstBusyBuffer.push_back(pBuffer);
		}else{
			m_lstBusyBuffer.push_front(pBuffer);
		}

	}
	else
	{//输入的 pin 不需要数据分发
		if (bToTail)
		{
			m_lstBusyBuffer.push_back(pBuffer);
		}else{
			m_lstBusyBuffer.push_front(pBuffer);
		}
	}
	//
	m_pMutexBusyBuffer->Unlock();


	return m_lstBusyBuffer.size();
}
int CFilterPin::Flush()
{
	//m_pMutexBusyBuffer->Lock();
	//把准备好的数据传送到下一个 filter的pin
	m_pMutexBusyBuffer->Lock();
	for (int i=0; i<m_lstBusyBuffer.size() ; i++)
	{

		CFilterPinBuffer *pPreparedBuffer = 0;
		bool bIsBusy = false;

		for (int j=0; j<m_lstNextPinIn.size(); j++)
		{
			CFilterPin *pPinIn = NULL;
			pPinIn = m_lstNextPinIn.at(j);
			if (!pPinIn)
				continue;
			CFilterPinBuffer *pPinInBuffer = NULL;
			int nFreeBuffers = pPinIn->GetFreeBufferCount();
			if (nFreeBuffers<= 0)
			{
				bIsBusy = true;
				break;;
			}
		}
		if (bIsBusy)
		{

			break ;
		}
		//int nBusyBufferCount = 0;
		//nBusyBufferCount = m_lstBusyBuffer.size();

		pPreparedBuffer = m_lstBusyBuffer.front();
		if (!pPreparedBuffer)
		{
			//m_pMutexBusyBuffer->Unlock();
			break;
		}

		m_lstBusyBuffer.pop_front();
		//m_pMutexBusyBuffer->Unlock();

		for (int j=0; j<m_lstNextPinIn.size(); j++)
		{//数据传输 使用 copy方式 
			CFilterPin *pPinIn = NULL;
			pPinIn = m_lstNextPinIn.at(j);
			if (!pPinIn)
				continue;
			CFilterPinBuffer *pPinInBuffer = NULL;
			pPinIn->GetFreeBuffer(&pPinInBuffer);
			//pPinInBuffer->Append(pBuffer->GetData(), pBuffer->GetLength());
			pPinInBuffer->CopyFrom(pPreparedBuffer);
			pPinIn->PutPreparedBuffer(pPinInBuffer);
		}
		if (pPreparedBuffer)
		{
			PutFreeBuffer(pPreparedBuffer);
		}
	}
	m_pMutexBusyBuffer->Unlock();

	return 0;
}
int CFilterPin::GetPreparedBuffer(CFilterPinBuffer **pBuffer, bool bFromHead /*= true*/)
{
	m_pMutexBusyBuffer->Lock();
	if (m_lstBusyBuffer.size() <= 0)
	{
		m_pMutexBusyBuffer->Unlock();
		return -1;
	}
	if (bFromHead)
	{
		*pBuffer = m_lstBusyBuffer.front();
		m_lstBusyBuffer.pop_front();
	}
	else
	{
		*pBuffer = m_lstBusyBuffer.back();
		m_lstBusyBuffer.pop_back();
	}
	m_pMutexBusyBuffer->Unlock();
	return m_lstBusyBuffer.size();
}
int CFilterPin::PutFreeBuffer(CFilterPinBuffer *pBuffer, int nReverseMemory)
{
	if (!pBuffer)
	{
		return -1;
	}
	if (m_nReversePinBufferMemorySize>0)
	{
		pBuffer->Clear(m_nReversePinBufferMemorySize);
	}
	else
	{
		pBuffer->Clear();
	}
	m_pMutexFreeBuffer->Lock();
	m_lstFreeBuffer.push_back(pBuffer);
	m_pMutexFreeBuffer->Unlock();
	return m_lstFreeBuffer.size();
}
int CFilterPin::ResizeBuffers(int nBufferCount)
{
	if (nBufferCount <= m_nBufferCount)
	{
		return m_nBufferCount;
	}
	int nNeedMore = nBufferCount - m_nBufferCount ;

	m_pMutexFreeBuffer->Lock();
	for (int i=0; i<nNeedMore; i++)
	{
		CFilterPinBuffer *pBuffer = new CFilterPinBuffer;
		m_lstFreeBuffer.push_back(pBuffer);
	}
	m_pMutexFreeBuffer->Unlock();
	return m_nBufferCount;
}

int CFilterPin::OnPinIn(CFilterPin* pPinPrevOut, CFilterPinBuffer* pPinBuffer)
{
	if (m_pFilter)
	{
		m_pFilter->OnPinIn(pPinPrevOut, this, pPinBuffer);
	}
	//

	return 0;
}

int CFilterPin::Connect(CFilterPin* pNextPinIn)
{
	int nIndex = -1;
	if (!FindNextPinIn(pNextPinIn/*, nIndex*/))
	{
		m_lstNextPinIn.push_back(pNextPinIn);
	
		pNextPinIn->m_lstPrevPinOut.push_back(this);
		//连接信号
		//m_sigPinOut.connect( sigc::mem_fun(pNextPinIn, &CFilterPin::OnPinIn)  );

		m_pSigPinOut->connect( sigc::mem_fun(pNextPinIn, &CFilterPin::OnPinIn)  );
	}
	//if (pNextPinIn->m_pPrevPinOut == NULL)

	return 0;
}

bool CFilterPin::FindNextPinIn(CFilterPin *pPin/*, int &nIndex*/)
{
	bool bFind = false;
	for (int i=0; i<m_lstNextPinIn.size(); i++)
	{
		if (m_lstNextPinIn.at(i) == pPin)
		{
			bFind = true;
			//nIndex = i;
			break;
		}
	}
	return bFind;
}
int CFilterPin::Clear()
{
	m_pMutexFreeBuffer->Lock();
	list<CFilterPinBuffer*>::iterator item;
	for (item = m_lstFreeBuffer.begin(); item != m_lstFreeBuffer.end(); item++)
	{
		if (*item)
			delete *item;
	}
	m_lstFreeBuffer.clear();
	m_pMutexFreeBuffer->Unlock();

	m_pMutexBusyBuffer->Lock();
	for (item = m_lstBusyBuffer.begin(); item != m_lstBusyBuffer.end(); item++)
	{
		if (*item)
			delete *item;
	}
	m_lstBusyBuffer.clear();
	m_pMutexBusyBuffer->Unlock();

	return 0;
}
const char *CFilterPin::GetType() 
{
	return m_strType.data();
}
eFilterPinIOType CFilterPin::GetIoType()
{
	return m_nIOType;
}

void CFilterPin::FreeAllBuffer()
{

	for (int i=0; i<m_lstBusyBuffer.size(); i++)
	{
		CFilterPinBuffer *pPinBuffer = m_lstBusyBuffer.front();
		m_lstBusyBuffer.pop_front();
		if (!pPinBuffer)
			continue;
		m_lstFreeBuffer.push_back(pPinBuffer);
	}


}

int CFilterPin::GetConnectedPinCount()
{
	return m_lstNextPinIn.size();
}

int CFilterPin::Emit(CFilterPinBuffer *pPinBuffer)
{
	if (m_pSigPinOut)
	{
		m_pSigPinOut->emit(this, pPinBuffer);
	}
	return 0;
}

}