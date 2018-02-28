#include "StdAfx.h"
#include "BasePipeLine.h"
#include "BaseFilter.h"
#include "Thread.h"
#include "mutex.h"
#include "FilterPin.h"

namespace SimpleCpp{
CBasePipeLine::CBasePipeLine(void)
: m_pThrStream(NULL)
, m_nSleepTime(1)
, m_state(ePipeLineState_None)
, m_pMutexFilters(NULL)
{  
	m_pMutexFilters = new CMutex();
}

CBasePipeLine::~CBasePipeLine(void)
{
	for (int i=0; i<m_lstFilter.size(); i++)
	{
		delete m_lstFilter.at(i);
	}
	m_lstFilter.clear();

	if (m_pThrStream)
	{
		m_pThrStream->Stop();
		m_pThrStream->Close();
		delete m_pThrStream;
		m_pThrStream = NULL;
	}
	if (m_pMutexFilters)
	{
		delete m_pMutexFilters;
		m_pMutexFilters = NULL;
	}
}

int CBasePipeLine::Start()
{

	m_pMutexFilters->Lock();
	m_state = ePipeLineState_Pause;
	if (!m_pThrStream ||
		!m_pThrStream->IsRunning())
	{
		if (m_pThrStream)
		{
			delete m_pThrStream;
			m_pThrStream = NULL;
		}
		m_pThrStream = new CThread();
		m_pThrStream->Create(ThreadProc, this);
	}
		
	
	for (int i=0;
		i<m_lstFilter.size();
		i++)
	{
		CBaseFilter *pFilter = m_lstFilter.at(i);
		if (pFilter)
		{
			pFilter->Start();
		}
	}
	
	m_state = ePipeLineState_Run;
	m_pMutexFilters->Unlock();

	return 0;
}


int CBasePipeLine::Stop()
{
	m_pMutexFilters->Lock();
	m_state = ePipeLineState_ToStop;
	if (m_pThrStream)
	{
		m_pThrStream->Stop();
	}

	for (int i=0;
		i<m_lstFilter.size();
		i++)
	{
		CBaseFilter *pFilter = m_lstFilter.at(i);
		if (pFilter)
		{
			pFilter->Stop();
		}
	}
	m_pMutexFilters->Unlock();
	
	return  0;
}

int CBasePipeLine::Pause()
{
	m_pMutexFilters->Lock();
	m_state = ePipeLineState_Pause;
 

	for (int i=0;
		i<m_lstFilter.size();
		i++)
	{
		CBaseFilter *pFilter = m_lstFilter.at(i);
		if (pFilter)
		{
			pFilter->Pause();
		}
	}
	m_pMutexFilters->Unlock();

	return  0;
}

int CBasePipeLine::Resume()
{
	m_pMutexFilters->Lock();
	m_state = ePipeLineState_Run;


	for (int i=0;
		i<m_lstFilter.size();
		i++)
	{
		CBaseFilter *pFilter = m_lstFilter.at(i);
		if (pFilter)
		{
			pFilter->Resume();
		}
	}
	m_pMutexFilters->Unlock();

	return  0;
}

int CBasePipeLine::Open(string &strParam/*=string("")*/)
{
	if (m_state != ePipeLineState_None)
	{
		return 1;
	}
	int nRet=0;
	
	m_state = ePipeLineState_Pause;
	if (!m_pThrStream)
	{
		m_pThrStream = new CThread();
		m_pThrStream->Create(ThreadProc, this);
	}

	return nRet;
}

int CBasePipeLine::Close()
{
	m_state = ePipeLineState_Close;
	if (m_pThrStream)
	{
		m_pThrStream->Stop();
		m_pThrStream->Close();
		delete m_pThrStream;
		m_pThrStream = NULL;
	}

	return  0;
}

int CBasePipeLine::AppendFilter( CBaseFilter* pFilter )
{
	m_pMutexFilters->Lock();
	m_lstFilter.push_back(pFilter);
	m_pMutexFilters->Unlock();

	return 0;
}

CBaseFilter* CBasePipeLine::QueryFilterByName( string &strName )
{
	CBaseFilter*	pFilter(NULL);
	for (int i=0;
		i<m_lstFilter.size();
		i++)
	{
		CBaseFilter* pFilterTmp = m_lstFilter.at(i);
		if (pFilterTmp->GetName() == strName)
		{
			pFilter = pFilterTmp;
			break;
		}
	}

	return pFilter;
}

CBaseFilter* CBasePipeLine::QueryFilterByType( string &strType )
{
	CBaseFilter*	pFilter(NULL);
	for (int i=0;
		i<m_lstFilter.size();
		i++)
	{
		CBaseFilter* pFilterTmp = m_lstFilter.at(i);
		if (pFilterTmp->GetType() == strType)
		{
			pFilter = pFilterTmp;
			break;
		}
	}

	return pFilter;
}

CBaseFilter* CBasePipeLine::GetFirstFilter()
{	
	CBaseFilter *pFilter(NULL);

	if (m_lstFilter.size() > 0)
	{
		pFilter = m_lstFilter.at(0);
	}

	return pFilter;
}

CBaseFilter* CBasePipeLine::GetLastFilter()
{
	CBaseFilter *pFilter(NULL);

	if (m_lstFilter.size() > 0)
	{
		pFilter = m_lstFilter.back();
	}

	return pFilter;
}

int __stdcall CBasePipeLine::ThreadProc(void *pData)
{

	int nRet = 0;
	
	CBasePipeLine* pPipeLine = (CBasePipeLine*)pData;
	if (pPipeLine)
	{
		pPipeLine->Run();
	}

	return nRet;
}
//给所有 filter 有run的机会
int CBasePipeLine::Run()
{
	int nRet = 0;

	while (m_state != ePipeLineState_Stop
		&& m_state != ePipeLineState_ToStop)
	{
		if (m_state == ePipeLineState_Pause)
		{
			CThread::Sleep(1);
			continue;
		}
		m_pMutexFilters->Lock();
		for (int i=0;
			i<m_lstFilter.size();
			i++)
		{
			CBaseFilter* pFilterTmp = m_lstFilter.at(i);
			if (pFilterTmp->HasFilterThread())
			{
				continue;
			}
			pFilterTmp->DoFilter();
		}
		m_pMutexFilters->Unlock();

		CThread::Sleep(m_nSleepTime);
	}
	return nRet;
}


}
