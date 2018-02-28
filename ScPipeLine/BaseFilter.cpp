#include "StdAfx.h"
#include "BaseFilter.h"
#include "Thread.h"
#include "FilterPin.h"
#include "FilterPinBuffer.h"

namespace SimpleCpp{

 
	void StringRemove(string &strSrc, char cRemove)
	{
		string::iterator   it;

		for (it =strSrc.begin(); it != strSrc.end(); )
		{
			if ( *it == cRemove)
			{
				it = strSrc.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	void StringRemoveEndpointBlank(string &strSrc)
	{
		if (strSrc.size() <= 0)
			return ;
		string::iterator   it;

		for (it =strSrc.begin(); it != strSrc.end(); )
		{//移除 开始的空格
			if ( *it == ' ')
			{
				it = strSrc.erase(it);
			}
			else
			{//不是空格了，结束
				break;
			}
		}
		it = strSrc.end();
		it--;
		for ( ; it != strSrc.begin(); /*it--*/)
		{//移除 结束的空格
			if ( *it == ' ')
			{
				it = strSrc.erase(it);
				it--;
			}
			else
			{//不是空格了，结束
				break;
			}
		}

	}

CBaseFilter::CBaseFilter(CBasePipeLine* pStream /*=NULL*/)
: m_pStream(pStream)
, m_pThread(NULL)
, m_state(eFilterState_Invalid)
, m_bHasFilterThread(false)
, m_nSleepTime(1)
{
}

CBaseFilter::~CBaseFilter(void)
{
}

void CBaseFilter::ParseParam(char *pParam, int nLen, map<string, string> &mapParamValue)
{
	if (!pParam)
		return ;
	int nCmdStart = 0;
	char *szCmd = new char[nLen+1];
	char szName[1024];
	char *szValue = new char[nLen+1];
	int nMaxLen = nLen;
	if (nMaxLen == 0)
	{
		nMaxLen = strlen(pParam);
	}
	for (int i=0; i<nMaxLen; i++)
	{
		if (pParam[i] != ';')
		{
			continue;
		}
		memset(szCmd, 0, i-nCmdStart+1 );
		strncpy(szCmd, pParam+nCmdStart, i-nCmdStart);
		memset(szName, 0, sizeof(szName));
		memset(szValue, 0, i-nCmdStart+1);

		ParseParamCmd(szCmd, szName, szValue);
		if (strlen(szName) > 0)
		{
			string strName = szName;
			string strValue = szValue;
			//StringRemove(strName, ' ');
			StringRemoveEndpointBlank(strName);
			//StringRemove(strValue, ' ');
			StringRemoveEndpointBlank(strValue);
			mapParamValue[strName] = strValue;
		}
		i++;
		nCmdStart = i;
	}
	delete szValue;
	delete szCmd;
}
void CBaseFilter::ParseParamCmd(char *pParam, char *pName, char *pVal)
{
	int i=0;
	memset(pName, 0, strlen(pName) );
	memset(pVal, 0, strlen(pVal) );

	char *pEque = strstr(pParam, "=");
	if (!pEque)
	{
		return;
	}
	strncpy(pName, pParam, pEque-pParam);

	char *pEnd = pParam + strlen(pParam);
	int nEqueLen = 1;
	strncpy(pVal, pEque+nEqueLen, pEnd-pEque-nEqueLen);
}
int CBaseFilter::Attach( CBasePipeLine* pStream )
{
	m_pStream = pStream;
	return 0;
}
//默认会出发 pinout动作。 pinout动作会触发下一个 pin in的 OnPinIn.
//在这个函数里面就不要操作 pinout的buffer
int CBaseFilter::OnPinIn( CFilterPin* pPrevPinOut, CFilterPin* pPinIn, CFilterPinBuffer *pPinBuffer)
{
	int nRet = 0;
	//m_sigPinOut.emit(this, pPinBuffer);
	//在这里面 拷贝数据 到这个filter
	CFilterPinBuffer*	pPinInBuffer(NULL);
	pPinIn->GetFreeBuffer(&pPinInBuffer);
	if(pPinInBuffer)
	{
		pPinInBuffer->CopyFrom(pPinBuffer);
		pPinIn->PutPreparedBuffer(pPinInBuffer);
	}
	
	//处理 

	//再传输下去
// 	for (int i=0; i<m_lstPinOut.size(); i++)
// 	{//默认
// 		CFilterPin* pPinOut = m_lstPinOut.at(i);
// 		pPinOut->Emit(NULL);
// 	}
//不传输数据到下一个 pin
// 	for (int i=0; i<m_lstPinOut.size(); i++)
// 	{
// 		CFilterPin *pPinOut = m_lstPinOut.at(i);
// 
// 		CFilterPinBuffer *pPinBuffer(NULL);
// 		pPinOut->GetPreparedBuffer(&pPinBuffer);
// 		while (pPinBuffer)
// 		{
// 			//pPinOut->m_sigPinOut.emit(pPinOut, pPinBuffer);
// 			pPinOut->m_pSigPinOut->emit(pPinOut, pPinBuffer);
// 			pPinOut->PutFreeBuffer(pPinBuffer);
// 			pPinBuffer = NULL;
// 			pPinOut->GetPreparedBuffer(&pPinBuffer);
// 		}
// 	}

	return nRet;
}
//

 
/*
int	CBaseFilter::Connect(CBaseFilter* pNextFilter)
{
	typedef int (CBaseFilter::*OnPinIn1)( CBaseFilter*  , CFilterPinBuffer *);
	m_sigPinOut.connect(sigc::mem_fun(pNextFilter, (OnPinIn1)&CBaseFilter::OnPinIn ));

	return 0;
}
*/
int CBaseFilter::DoFilter()
{
	//默认是执行数据 输出操作。
	for (int i=0; i<m_lstPinOut.size(); i++)
	{//传输数据到下一个 pin
		CFilterPin *pPinOut = m_lstPinOut.at(i);

		CFilterPinBuffer *pPinBuffer(NULL);
		pPinOut->GetPreparedBuffer(&pPinBuffer);
		while (pPinBuffer)
		{
			//pPinOut->m_sigPinOut.emit(pPinOut, pPinBuffer);
			pPinOut->m_pSigPinOut->emit(pPinOut, pPinBuffer);
			pPinOut->PutFreeBuffer(pPinBuffer);
			pPinBuffer = NULL;
			pPinOut->GetPreparedBuffer(&pPinBuffer);
		}
	}

	//
	return 0;
}

int CBaseFilter::Open(char* pSzParam, bool bHasThread/*=false*/)
{
	if (m_state >= eFilterState_Open)
	{
		return -1;
	}
	m_mapParamValue.clear();
	int nParamLen(0);
	nParamLen = (pSzParam==NULL)? 0:strlen(pSzParam);
	ParseParam(pSzParam, nParamLen, m_mapParamValue);
	
	CFilterPin *pIn = new CFilterPin(this);
	pIn->Init(eFPT_In, 10);

	m_lstPinIn.push_back(pIn);
	//输出的 pin不需要buffer，需要的buffer 都从 下一个 连接的 输入 pin 取
	CFilterPin *pOut = new CFilterPin(this);
	pOut->Init(eFPT_Out, 10);
	m_lstPinOut.push_back(pOut);

	//m_strParam = pSzParam;
	m_strParam.clear();
	if (pSzParam)
	{
		m_strParam.append(pSzParam);
	}
 
	m_state = eFilterState_Pause;

	m_bHasFilterThread = bHasThread;
	CreateFilterThread(bHasThread);
  
	return 0;
}

bool CBaseFilter::IsKind(char *pSzKind)
{
	if (strcmp(m_szKind, pSzKind) == 0) 
		return true;

	return false;
}
char *CBaseFilter::GetType()
{
	return m_szType;
}
char *CBaseFilter::GetName()
{
	return m_szName;
}
char *CBaseFilter::GetKind()
{
	return m_szKind;
}
int CBaseFilter::Close()
{
	debug("%s: %s++\n", __FUNCTION__, m_strName.data());
	m_state = eFilterState_ToStop;
	if (m_pThread)
	{
		m_pThread->Stop();

		m_pThread->Close();
		delete m_pThread;
		m_pThread = NULL;
	}
	debug("%s: %s--\n", __FUNCTION__, m_strName.data());
	//
	return 0;
}

int CBaseFilter::Start()
{
	m_state = eFilterState_Run;
	return 0;
}

int CBaseFilter::Pause()
{
	m_state = eFilterState_Pause;
	return 0;
}
int CBaseFilter::Resume()
{
	m_state = eFilterState_Run;
	return 0;
}
int CBaseFilter::Stop()
{
	m_state = eFilterState_ToStop;

	if (m_pThread)
	{
		m_pThread->Stop();
		m_pThread->Close();
		delete m_pThread;
		m_pThread = NULL;
	}

	return 0;
}

int __stdcall CBaseFilter::ThreadProc( void *pData )
{
	CBaseFilter *pFilter = (CBaseFilter*)(pData);
	if (pFilter)
	{
		pFilter->Run();
	}
	pFilter->m_state = eFilterState_Stop;

	return 0;
}

CFilterPin* CBaseFilter::GetPinOut( int nIndex )
{
	CFilterPin *pPin(NULL);
	if (nIndex<0 || nIndex >= m_lstPinOut.size())
	{
		return NULL;
	}
	pPin = m_lstPinOut.at(nIndex);
	return pPin;
}

CFilterPin* CBaseFilter::GetPinIn( int nIndex )
{
	CFilterPin *pPin(NULL);
	if (nIndex<0 || nIndex >= m_lstPinIn.size())
	{
		return NULL;
	}
	pPin = m_lstPinIn.at(nIndex);
	return pPin;
}

const char* CBaseFilter::GetParam( char *pName )
{
	const char *pValue=NULL;
	map<string, string>::iterator item;
	item = m_mapParamValue.find( pName ) ;
	if (item != m_mapParamValue.end())
	{
		pValue = item->second.c_str();
	}

	return pValue;
}

void CBaseFilter::CreateFilterThread( bool bHasThread )
{
	if (!m_pThread && bHasThread)
	{
		m_pThread = new CThread();
		m_pThread->Create(ThreadProc, this);
	}
}

CBaseFilter* CBaseFilter::CreateObject()
{
	return new CBaseFilter();
}

void CBaseFilter::DeleteObject(CBaseFilter* pFilter)
{
	if (!pFilter)
	{
		return ;
	}
	delete pFilter;
	pFilter = NULL;
}

bool CBaseFilter::HasFilterThread() const
{
	return m_bHasFilterThread;
}



int CBaseFilter::SetProgressCb(FilterCB_Progress cbProgress, void *pUserData, void *pReversed)
{
	int nRet = 0;

	m_cbProgress = cbProgress;
	m_pCbProgressReversed = pReversed;
	m_pCbProgressUserData = pUserData;

	return nRet;
}
int CBaseFilter::SetFinishedCb(FilterCB_Finished cbFinished, void *pUserData, void *pReversed)
{
	int nRet = 0;

	m_cbFinished = cbFinished;
	m_pCbFinishedUserData = pUserData;
	m_pCbFinishedReversed = pReversed;

	return nRet;
}
int CBaseFilter::SetEventCb(FilterCB_Event cbEvent, void *pUserData, void *pReversed)
{
	int nRet = 0;

	m_cbEvent = cbEvent;
	m_pCbEventUserData = pUserData;
	m_pCbEventReversed = pReversed;

	return nRet;
}

int CBaseFilter::Run()
{
	while (m_state != eFilterState_ToStop
		&& m_state != eFilterState_Stop)
	{

		DoFilter();

		CThread::Sleep(m_nSleepTime);
	}
	return 0;
}



}