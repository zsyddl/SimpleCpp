
#include "StdAfx.h"

#include "MediaFilter.h"
#include "Buffer.h"
#include "MediaSdkDef.h"
#include "Thread.h"
#include "mutex.h"
#include "string.h"
#include "FilterManager.h"
#include "BaseFilter.h"
#include "FilterPinBuffer.h"

using namespace SimpleCpp;


MEDIASDK_API CMediaFilter g_dataFilter;
CMediaFilter::CMediaFilter(void)
: m_nAutoReleaseSize(10*1024)
, m_dataMode(eFDM_Push)
, m_fSpeed(1.0)
// , m_cbProgress(NULL)
// , m_pCbProgressUserData(0)
// , m_pCbProgressReversed(0)
// , m_cbFinished(NULL)
// , m_pCbFinishedUserData(0)
// , m_pCbFinishedReversed(0)
// , m_cbEvent(NULL)
// , m_pCbEventUserData(0)
// , m_pCbEventReversed(0)
, m_s64Sts(0)
, m_s64Pts(0)
, m_s64Dts(0)
, m_playMode(eFPM_Play)
, m_pMutexDoFilter(NULL)
{
	memset(m_szType, 0, sizeof(m_szType));
	memset(m_szKind, 0, sizeof(m_szKind));
	memset(m_szName, 0, sizeof(m_szName));
		
	strcpy(m_szType, "base filter");
	strcpy(m_szKind, "base filter");
	strcpy(m_szName, "base filter");

	m_pMutexDoFilter = new CMutex();
	//m_szType

	CFilterManager::RegistFilter(&g_dataFilter);
}
const char *CMediaFilter::GetParam(char *pSzName)
{
	map<string, string>::iterator item=0;
	item = m_mapParamValue.find(pSzName);

	if (item == m_mapParamValue.end())
		return NULL;

	return item->second.c_str();
}

char* CMediaFilter::GetParam()
{
	return (char*)m_strParam.c_str();
}

void CMediaFilter::SetParam(char *pSzName, char *pSzValue)
{
	m_mapParamValue[pSzName] = pSzValue;
}
CMediaFilter *CMediaFilter::CreateObject()
{
	return new CMediaFilter();
}
void CMediaFilter::Release()
{
	delete this;
}

void *CMediaFilter::Malloc(size_t size)
{
	return new char[size];
}
void CMediaFilter::Free(void *p)
{
	return delete p;
}
/*
void *CDataFilter::operator new (size_t size)
{
	void *p = malloc(size);
	return p;
}*/
int CMediaFilter::Open(char *pSzParam/*=0*/, bool bHasThread/*=false*/)
{
	CBaseFilter::Open(pSzParam, bHasThread);
	return 0;
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
	

	return 0;
}
CMediaFilter::~CMediaFilter(void)
{
	for (int i=0; i<m_lstPinIn.size(); i++)
	{
		if (m_lstPinIn.at(i))
			delete m_lstPinIn.at(i);
	}
	for (int i=0; i<m_lstPinOut.size(); i++)
	{
		if (m_lstPinOut.at(i))
			delete m_lstPinOut.at(i);
	}
	m_lstPinIn.clear();
	m_lstPinOut.clear();

	if (m_pMutexDoFilter)
	{
		delete m_pMutexDoFilter;
		m_pMutexDoFilter = NULL;
	}
}
int CMediaFilter::GetBuffer(CFilterPinBuffer **pBuffer, int nIOType)
{
	if (nIOType == eFPT_In)
	{// 取In的 free 队列
		CFilterPin *pPinIn = m_lstPinIn.at(0);
		pPinIn->GetFreeBuffer(pBuffer);
	}
	else
	{// 取out 的busy 
		CFilterPin *pPinOut = m_lstPinOut.at(0);
		pPinOut->GetPreparedBuffer(pBuffer);
	}
	return 0;
}
int CMediaFilter::SetBufferCount(int nCount, int nIOType)
{
	if (nIOType == eFPT_In)
	{
		
	}
	else
	{


	} 
	return 0;
}
int CMediaFilter::PutBuffer(CFilterPinBuffer *pBuffer, int nIOType, int nReversedMemory)
{
	if (nIOType == eFPT_In)
	{// 放入In的 busy 队列
		CFilterPin *pPinIn = m_lstPinIn.at(0);
		pPinIn->PutPreparedBuffer(pBuffer);
	}
	else
	{// 放入out 的free 
		pBuffer->Clear();
		CFilterPin *pPinOut = m_lstPinOut.at(0);
		pPinOut->PutFreeBuffer(pBuffer, nReversedMemory);
		
	}
	return 0;
}
 
int CMediaFilter::InputData(char *pData, int nLen)
{
	return 0;
}
int CMediaFilter::OutputData(char *pData, int &nLen)
{

	return 0;
}
void CMediaFilter::GetOutPin(vector<CFilterPin *> &lstPin)
{
	lstPin = m_lstPinOut;
}
void CMediaFilter::GetInPin(vector<CFilterPin *> &lstPin)
{
	lstPin = m_lstPinIn;
}
CFilterPin *CMediaFilter::GetOutPin(int nIndex)
{
	if (nIndex<0 || nIndex>=m_lstPinOut.size())
		return NULL;

	return m_lstPinOut.at(nIndex);
}
CFilterPin *CMediaFilter::GetInPin(int nIndex)
{
	if (nIndex<0 || nIndex>=m_lstPinIn.size())
		return NULL;

	return m_lstPinIn.at(nIndex);
}
int CMediaFilter::DoFilter()
{
	// sub class to do :
	if (m_lstPinOut.size() <= 0)
	{
		return -1;
	}
	//没有直接连接的pin 使用信号流程
	/*CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (pPinOut->GetConnectedPinCount() <= 0)
	{
		CFilterPinBuffer *pBuffer(NULL);
		CFilterPin *pPinOut = NULL;
		pPinOut = m_lstPinOut.at(0);
		CFilterPinBuffer *pPinBuffer(NULL);

		pPinOut->GetPreparedBuffer(&pPinBuffer);
		while (pPinBuffer)
		{
			m_sigPinOut.emit(this, pPinBuffer);
			pPinOut->PutFreeBuffer(pPinBuffer);
			pPinBuffer = NULL;
			pPinOut->GetPreparedBuffer(&pPinBuffer);
		}
	}
	else{*/
	//
#if 1
	for (int i=0; i<m_lstPinOut.size(); i++)
	{//传输数据到下一个 pin
		CFilterPin *pPinOut = m_lstPinOut.at(i);

		CFilterPinBuffer *pPinBuffer(NULL);
		pPinOut->GetPreparedBuffer(&pPinBuffer);
		while (pPinBuffer)
		{
			pPinOut->m_pSigPinOut->emit(pPinOut, pPinBuffer);
			pPinOut->PutFreeBuffer(pPinBuffer);
			pPinBuffer = NULL;
			pPinOut->GetPreparedBuffer(&pPinBuffer);
		}
	} 
#else	//	
		//2 处理数据
		m_pMutexDoFilter->Lock();
		//3 把数据放入到 下一个 filter的 in pin
		for (int i=0; i<m_lstPinOut.size(); i++)
		{//传输数据到下一个 pin
			m_lstPinOut.at(i)->Flush();
		}
		// then call this DoFilter
		//执行 下一个 filter
		//推模式
		//找到 输出的pin 连接的 filter并执行 DoFilter
		CMediaFilter *pNextFilter = NULL;
		vector<CMediaFilter *> lstNextFilters;
		for (int i=0; i<m_lstPinOut.size(); i++)
		{
			CFilterPin *pPinOut = NULL;
			pPinOut = m_lstPinOut.at(i);
			if (!pPinOut)
				continue;
			lstNextFilters.clear();
			GetConnectedFilter(pPinOut, lstNextFilters);
			for (int j=0; j<lstNextFilters.size(); j++)
			{
				pNextFilter = lstNextFilters.at(j);
				if (!pNextFilter)
					continue;
				pNextFilter->DoFilter();
			}
		}
		//拉模式

		m_pMutexDoFilter->Unlock();
#endif
	//}
	return 0;
}
int CMediaFilter::Start()
{
	CBaseFilter::Start();
	m_state = eFilterState_Run;
	m_playMode = eFPM_Play;
	return 0;
}
int CMediaFilter::Stop()
{
	m_state = eFilterState_Stop;
	return 0;
}
int CMediaFilter::Pause()
{
	m_state = eFilterState_Pause;
	return 0;
}
int CMediaFilter::Resume()
{
	m_state = eFilterState_Run;
	m_playMode = eFPM_Play;
	return 0;
}
int CMediaFilter::Seek(char *pPos)
{
	
	return 0;
}
int CMediaFilter::Seek(int64 nTime, int nFlag)
{
	return 0;
}
int CMediaFilter::SeekToTimestamp(int64 s64Timestamp, int nFlag)
{
	
	return 0;
}
int CMediaFilter::SetPlaySpeed(float fSpeed)
{
	m_fSpeed = fSpeed;

	return 0;
}
float CMediaFilter::GetPlaySpeed()
{

	return m_fSpeed;
}
int CMediaFilter::PlayNextFrame()
{
	m_playMode = eFPM_PlayNextFrame;
	return 0;
}
int CMediaFilter::PlayPrevFrame(int64 s64PtsNow/*=0*/)
{
	m_playMode = eFPM_PlayPrevFrame;
	return 0;
}
int CMediaFilter::SetShowRect(int nX, int nY, int nW, int nH)
{

	return 0;
}
int CMediaFilter::Connect(CFilterPin *pPinOut, CFilterPin *pPinIn)
{
	//
	pPinOut->Connect(pPinIn);
	 
	return 0;
}
/*
int CMediaFilter::Connect( CBaseFilter* pNextFilter )
{

	return CBaseFilter::Connect(pNextFilter);
}
*/
int CMediaFilter::Disconnect(CFilterPin *pPinOut, CFilterPin *pPinIn)
{
	int nIndex = -1;
	vector<CFilterPin *>::iterator item;
	vector<CFilterPin*> &lstNextPins = pPinOut->GetNextPins();
	bool bFind = false;

	for (item = lstNextPins.begin(); item != lstNextPins.end(); item++ )
	{
		if (*item == pPinIn)
		{
			bFind = true;
			break;
		}
	}
	if (bFind)
	{
		lstNextPins.erase(item);
	}


	return 0;
}
/*
int CMediaFilter::GetConnectedFilter(CFilterPin *pPin, vector<CMediaFilter*> &lstFilter)
{
	
	CMediaFilter *pFilter = NULL;
	if (!pPin)
	{
		return NULL;
	}
	if (pPin->GetIoType() == eFPT_In)
	{//输入pin，与之连接的是一个输出 out pin
		for (int i=0; i<pPin->m_lstPrevPinOut.size(); i++)
		{
			CFilterPin *pPinIn = NULL;
			pPinIn = pPin->m_lstPrevPinOut.at(i);
			if (!pPinIn)
				continue;
			lstFilter.push_back(pPinIn->m_pFilter);
		}
	}
	else if (pPin->GetIoType() == eFPT_Out)
	{//输出pin 与之连接的 是 输入in pin，可能有多个
		for (int i=0; i<pPin->m_lstNextPinIn.size(); i++)
		{
			CFilterPin *pPinIn = NULL;
			pPinIn = pPin->m_lstNextPinIn.at(i);
			if (!pPinIn)
				continue;
			lstFilter.push_back(pPinIn->m_pFilter);
		}
	}
	else
	{//无效

	}

	return lstFilter.size();
}*/

int CMediaFilter::Flush()
{

	return 0;
}

int CMediaFilter::Control( char *pSzName, char *pSzValue )
{

	return 0;
}

int CMediaFilter::SetPlayMode( eFilterPlayMode playMode )
{
	m_playMode = playMode;
	return 0;
}

void CMediaFilter::Reset()
{
 
}

int CMediaFilter::SetPlayState( int nState )
{
	m_state = (eFilterState)nState;
	return 0;
}

int CMediaFilter::GetOutPinCount()
{
	return m_lstPinOut.size();
}

int CMediaFilter::GetInPinCount()
{
	return m_lstPinIn.size();
}

CFilterPin * CMediaFilter::AppendOutPin()
{
	CFilterPin *pPin = new CFilterPin(this);

	pPin->Init(eFPT_Out, 2);
	m_lstPinOut.push_back(pPin);

	return pPin;
}

CFilterPin * CMediaFilter::AppendInPin()
{
	CFilterPin *pPin = new CFilterPin(this);

	pPin->Init(eFPT_In, 2);
	m_lstPinIn.push_back(pPin);

	return pPin;
}

void CMediaFilter::CopyParam( CMediaFilter *pFilter )
{
	m_mapParamValue = pFilter->m_mapParamValue;
}

int CMediaFilter::OnPinIn(CFilterPin* pPrevPinOut,  CFilterPin* pPinIn, CFilterPinBuffer *pPinBuffer )
{
 
	//CFilterPin *pPinIn = m_lstPinIn.at(0);

	CFilterPinBuffer*	pPinInBuffer(NULL);
	pPinIn->GetFreeBuffer(&pPinInBuffer);
	if(pPinInBuffer)
	{
		pPinInBuffer->CopyFrom(pPinBuffer);
		pPinIn->PutPreparedBuffer(pPinInBuffer);
	}
	//调用原来的流程
	//DoFilter();
	
	//将空的 filter pin buffer 放回去
	//
	return 0;
}