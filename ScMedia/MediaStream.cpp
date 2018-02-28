#include "StdAfx.h"
#include "MediaStream.h"
#include "MediaFilter.h"
#include "Thread.h"
#include "Mutex.h"
#include "Render.h"
#include "SourceFilter.h"
#include "Decoder.h"
#include "SourceFilter.h"
#include <MMSystem.h>

CMediaStream::CMediaStream(void)
:m_pThrStream(NULL)
//, m_pMutexFilters(NULL)
, m_dataMode(eSDM_Push)
, m_cbEvent(NULL)
, m_pCbEventUserData(0)
, m_pCbEventReversed(0)
, m_cbFinished(NULL)
, m_pCbFinishedUserData(0)
, m_pCbFinishedReversed(0)
, m_cbProgress(NULL)
, m_pCbProgressReversed(0)
, m_pCbProgressUserData(0)
, m_playMode(eSPM_None)
, m_fSpeed(1.0)
, m_bIsStreamEnd(false)
{

	//m_pMutexFilters = new CMutex();
}

CMediaStream::~CMediaStream(void)
{

	if (m_pMutexFilters)
	{
		delete m_pMutexFilters;
		m_pMutexFilters = NULL;
	}
	if (m_pThrStream)
	{
		delete m_pThrStream;
		m_pThrStream = NULL;
	}
	for (int i=0; i<m_lstFilter.size() /*&& m_state != eSS_Stop*/; i++)
	{
		CMediaFilter *pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (pFilter)
		{
			pFilter->Release();
			m_lstFilter.at(i) = NULL;
		}

		//delete*m_lstFilter.at(i);
	}
	m_lstFilter.clear();
}
CSourceFilter *CMediaStream::GetSourceFilter()
{
	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop && m_state != eSS_ToStop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		//push mode 找到 source, 如果是 pull mode 需要找到 accepter
		if (!pFilter->IsKind(FILTER_KIND_SOURCE))
			continue;
		return  dynamic_cast<CSourceFilter*>(pFilter);
	}
	return NULL;
}
int CMediaStream::ThrStreamProc( void *pParam )
{
	CMediaStream *pStream = NULL;
	pStream = (CMediaStream *)pParam;
	if (pStream)
	{
		pStream->ThrStreamProc();
	}
	return 0;
}
int CMediaStream::ThrStreamProc()
{
// 	while (m_state != eSS_Stop
// 		 && m_state != eSS_ToStop)
// 	{
// 		if (m_state == eSS_Pause)
// 		{
// 			Sleep(20);
// 			continue;;
// 		}
// 		int nSleepTime = 0;
// 		int nExtLoopTime = 1;
// 		CSourceFilter *pSource  = 0;
// 
// 		float fSpeed = 1.0;
// 		pSource = GetSourceFilter();
// 		if (pSource)
// 		{
// 			nSleepTime = pSource->GetPumperSleepTime();
// 			fSpeed = pSource->GetPlaySpeed();
// 			m_bIsStreamEnd = pSource->IsStreamEnd();
// 		}
// 		if (nSleepTime < 4)
// 		{
// 			if (fSpeed > 12 )
// 			{
// 				nExtLoopTime=4;
// 			}
// 			else if (fSpeed > 6)
// 			{
// 				nExtLoopTime=2;
// 			}
// 		}
// 		
// 		 
// 		m_pMutexFilters->Lock();
// 		while (nExtLoopTime--)
// 		{//增加cpu使用
// 			CMediaFilter *pFilter = NULL;
// 			for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop && m_state != eSS_ToStop; i++)
// 			{
// 				pFilter = m_lstFilter.at(i);
// 				if (!pFilter)
// 					continue;
// 				//push mode 找到 source, 如果是 pull mode 需要找到 accepter
// 				//if (!pFilter->IsKind(FILTER_KIND_SOURCE))
// 				//	continue;
// 				pFilter->DoFilter();
// 				CSourceFilter *pSource = dynamic_cast<CSourceFilter*>(pFilter);
// 				if (pSource)
// 				{
// 					nSleepTime = pSource->GetPumperSleepTime();
// 				}
// 			}
// 		}
// 		m_pMutexFilters->Unlock();
// 		//sleep(0) 还是可以获取非常多的cpu的。 sleep(1)可以获取 1% 
// 		Sleep(nSleepTime);
// 	}
// 	m_state = eSS_Stop;

	return 0;
 }
// int CMediaStream::Start()
// {
// 	CBasePipeLine::Start();
// 
// 	if (m_pThrStream)
// 	{
// 		return -1;
// 	}
// // 	m_pThrStream = new CThread();
// // 	m_state = eSS_Pause;
// // 	m_playMode = eSPM_None;
// //  	m_pThrStream->Create(ThrStreamProc, this);
// // 	m_pThrStream->SetPriority(eThreadPriority_Normal);
// //  	m_state = eSS_Run;
// 	//m_pMutexFilters->Lock();
// 	CMediaFilter *pFilter = NULL;
// 	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
// 	{
// 		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
// 		if (!pFilter)
// 			continue;
// 
// 		pFilter->Start();
// 	}
// 	//m_pMutexFilters->Unlock();
// 
// 	return 0;
// }
// int CMediaStream::Stop()
// {
// 	CBasePipeLine::Stop();
// // 	if (!m_pThrStream)
// // 	{
// // 		return -1;
// // 	}
// // 	m_state = eSS_ToStop;
// // 	
// // 	int nTries = 0;
// // 	debug("%s, stop ++ \n", __FUNCTION__);
// // 	while (m_state != eSS_Stop
// // 		/*&& nTries--*/)
// // 	{
// // 		nTries++;
// // 		Sleep(10);
// // 	}
// // 	if (m_state == eSS_Stop)
// // 	{
// // 		debug("%s, stop success, tries : %d times\n", __FUNCTION__, nTries);
// // 	}
// // 	else
// // 	{
// // 		debug("%s, stop in unusually way\n", __FUNCTION__);
// // 	}
// // 	m_pThrStream->Stop();
// // 
// // 	delete m_pThrStream;
// // 	m_pThrStream = NULL;
// // 	for (int i=0; i<m_lstFilter.size(); i++)
// // 	{
// // 		m_lstFilter.at(i)->Stop();
// // 	}
// // 	debug("%s, --\n", __FUNCTION__);
// 	return 0;
// }
// int CMediaStream::Pause()
// {
// // 	m_state = eSS_Pause;
// // 	m_playMode = eSPM_None;
// // 	m_pMutexFilters->Lock();
// // 	CMediaFilter *pFilter = NULL;
// // 	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
// // 	{
// // 		pFilter = m_lstFilter.at(i);
// // 		if (!pFilter)
// // 			continue;
// // 		
// // 		pFilter->Pause();
// // 	}
// // 	m_pMutexFilters->Unlock();
// 	return 0;
// }
int CMediaStream::Resume()
{
	if (m_playMode == eSPM_PlayPrevFrame)
	{//须要重新 seek 定位到当前 播放的pts
		int64 s64RenderPts = -1;
		CMediaFilter *pFilter = 0;
		CRender *pRender = 0;
		pFilter = FindFilterByKind(FILTER_KIND_RENDER);
		if (pFilter)
		{
			pRender = dynamic_cast<CRender*>(pFilter);
		}
		if (pRender)
		{
			s64RenderPts = pRender->GetPts();
		}
		if (s64RenderPts >= 0)
		{
			SeekToTimestamp(s64RenderPts, eSSF_UntilDecode|eSSF_Backward);
		}

	}
//	m_state = eSS_Run;
	m_playMode = eSPM_Play;
	m_pMutexFilters->Lock();
	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		
		pFilter->SetPlayMode(eFPM_Play);
		pFilter->Resume();
	}
	m_pMutexFilters->Unlock();
	return 0;
}
int CMediaStream::Seek(int64 s64TimeToSeek, int nFlag/*=0*/)
{
	debug("%s: time: %lld", __FUNCTION__, s64TimeToSeek)
	m_bIsStreamEnd = false;
	
	int nPlayStateOld = m_state;
	int nPlayModeOld = m_playMode;
	//播放，直到 解码器解码到 该时间位置
	CDecoder *pDecoder = dynamic_cast<CDecoder*>(FindFilterByKind(FILTER_KIND_DECODER));
// 	if (!pDecoder)
// 		return -1;
	CSourceFilter *pSource = dynamic_cast<CSourceFilter*>(FindFilterByKind(FILTER_KIND_SOURCE));
// 	if (!pSource)
// 		return -2;
	CRender *pRender = dynamic_cast<CRender*>(FindFilterByKind(FILTER_KIND_RENDER));
// 	if (!pRender)
// 		return -3;
// 	int64 s64Timestamp = 0;
// 	s64Timestamp = pSource->TimeToTimestamp(s64Time);
// 	
// 	int nRet = 0;
// 	nRet = SeekToTimestamp(s64Timestamp, nFlag);
// 
// 	return nRet;


	int64 s64OldPlayTime = 0;
	if (pDecoder && pSource)
	{
		s64OldPlayTime =   pRender->GetPts();
		s64OldPlayTime = pSource->TimestampToTime(s64OldPlayTime);
	}
	Pause();
	m_pMutexFilters->Lock();
	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		
		pFilter->Seek(s64TimeToSeek, nFlag);
	}
	//m_pMutexFilters->Unlock();
	float fSpeedOld = m_fSpeed;
	if (nFlag & eSSF_UntilDecode
		&& pSource && pDecoder)
	{//等待解码
		//Resume();

		int64 s64RenderTime = 0;
		int64 s64RenderTimestamp=0;


		bool bRenderVisible = pRender->IsVisible();
		pRender->SetVisible(false);
		//SetPlaySpeed(32);

		bool bSeekOk =false;
		int nTries = 300;
		int nTotal = nTries;
		//Sleep(10);
		long lTimeBegin = timeGetTime();
		long lTimeNow = timeGetTime();
		long lTimeOut = 3*1000;
		do{
			s64RenderTimestamp = pRender->GetPts();
			s64RenderTime = pSource->TimestampToTime(s64RenderTimestamp);
			debug("%s: tries: %d , decode time %I64d, need seek time: %I64d\n", __FUNCTION__, nTries, s64RenderTime, s64TimeToSeek);
			
			//如果播放在右边 须要seek到左边
			if (s64OldPlayTime > s64TimeToSeek)
			{//已经播放到后面去的，需要截图的时间在 播放的时间的左边
				if (s64OldPlayTime >0 && s64RenderTime >= s64OldPlayTime)
				{//解码的时间还没有退回去
					debug("%s, decode time is newer than old play time, decode[%I64d]:old[%I64d], seek data is not arrive \n", 
						__FUNCTION__,
						s64RenderTime, s64OldPlayTime);
					PlayNextFramePriv();
					continue;
				}
				else
				{
					//Pause();
					//break;
				}
			}
			else
			{//从前往后播放的，需要截图的 时间比播放时间更 远 右边
				//Pause();
				//break;
			}
			if (s64RenderTime >= s64TimeToSeek)
			{//已经播放到须要的位置
				pRender->SetVisible(bRenderVisible);
				PlayNextFramePriv();
				break;
			}

			lTimeNow = timeGetTime();
			PlayNextFramePriv();
		}while ( nTries-- > 0 
			&& (lTimeNow-lTimeBegin)<lTimeOut );

		if (nTries <= 0)
		{ 
			debug("%s, try seek I frame and decode to pos timeout..., try to seek any pos \n", __FUNCTION__);
			//Seek(s64Time, AVSEEK_FLAG_ANY);
			//m_pMutexFilters->Lock();
			CMediaFilter *pFilter = NULL;
			for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
			{
				pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
				if (!pFilter)
					continue;

				pFilter->Seek(s64TimeToSeek, AVSEEK_FLAG_ANY);
			}
			//m_pMutexFilters->Unlock();
			pRender->SetVisible(bRenderVisible);
			PlayNextFramePriv();
			pRender->Refresh();
			bSeekOk = true;
		}
		else
		{
			debug("%s, try [%d]times, time[%lld]:[%lld], seek I frame and decode to pos success\n", __FUNCTION__, nTotal-nTries, 
				s64RenderTime, s64TimeToSeek);
			bSeekOk = true;
		}

		//SetPlaySpeed(fSpeedOld);
		//Resume();
		//Sleep(100);
		pRender->SetVisible(bRenderVisible);
		pRender->Refresh();
	}
	else
	{//不用精确定位
		PlayNextFramePriv();
		PlayNextFramePriv();
		pRender->Refresh();
	}
	//m_pMutexFilters->Lock();
	SetPlayModePriv(nPlayModeOld);
	SetPlayStatePriv(nPlayStateOld);
	m_pMutexFilters->Unlock();

	return 0;
}
int CMediaStream::SeekToTimestamp(int64 s63Timestamp, int nFlag/*=0*/)
{
	m_bIsStreamEnd = false;

	
	//播放，直到 解码器解码到 该时间位置
	CDecoder *pDecoder = dynamic_cast<CDecoder*>(FindFilterByKind(FILTER_KIND_DECODER));
	// 	if (!pDecoder)
	// 		return -1;
	CSourceFilter *pSource = dynamic_cast<CSourceFilter*>(FindFilterByKind(FILTER_KIND_SOURCE));
	// 	if (!pSource)
	// 		return -2;
	CRender *pRender = dynamic_cast<CRender*>(FindFilterByKind(FILTER_KIND_RENDER));
	// 	if (!pRender)
	// 		return -3;
	int nPlayStateOld = 0;
	nPlayStateOld = m_state;
	Pause();
	m_pMutexFilters->Lock();

	int64 s64OldPlayTime = 0;
	int64 s64OldPlayTimestamp = 0;
	if (pDecoder && pSource)
	{
		s64OldPlayTimestamp =  pRender->GetPts();;// pDecoder->GetTimestamp();
		s64OldPlayTime = pSource->TimestampToTime(s64OldPlayTimestamp);
	}
	int64 s64TimeNeedToBe = 0;
	s64TimeNeedToBe = pSource->TimestampToTime(s63Timestamp);
	debug("%s: pts: %I64d, time: %I64d\n", __FUNCTION__, s63Timestamp, s64TimeNeedToBe);
	debug("%s, old pts: %I64d, time; %I64d\n", __FUNCTION__, s64OldPlayTimestamp, s64OldPlayTime);

	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;

		//pFilter->Seek(s64TimeNeedToBe, nFlag);
		pFilter->SeekToTimestamp(s63Timestamp, nFlag);
	}
	
	//m_pMutexFilters->Unlock();

	bool bSeekOk =false;

	float fSpeedOld = m_fSpeed;
	if (nFlag & eSSF_UntilDecode
		&& pSource && pDecoder)
	{//等待解码
		//Resume();

		int64 s64RenderTime = 0;
		int64 s64RenderTimestamp=0;

		bool bRenderVisible = pRender->IsVisible();
		pRender->SetVisible(false);
		//SetPlaySpeed(32);

		int nTries = 300;
		int nTotal = nTries;
		//Sleep(10);
		//Pause();
		long lTimeBegin = timeGetTime();
		long lTimeNow = timeGetTime();
		long lTimeOut = 3*1000;
		do{
			//s64DecodeTimestamp = pDecoder->GetTimestamp();
			s64RenderTimestamp = pRender->GetPts();
			s64RenderTime = pSource->TimestampToTime(s64RenderTimestamp);
			//debug("%s: decode time %d, need seek time: %d\n", __FUNCTION__, (int)s64DecodeTime, (int)s64TimeNeedToBe);


			//判断以前播放的时间是不是比须要的时间 靠右
			if (s64OldPlayTime > s64TimeNeedToBe)
			{//已经播放到后面去的，需要截图的时间在 播放的时间的左边
				if (s64OldPlayTime >0 && s64RenderTime >= s64OldPlayTime)
				{//解码的时间还没有退回去
					//debug("%s, decode time is newer than old play time[%I64d]:[%I64d], seek data is not arrive \n", 
					//	__FUNCTION__,
					//	s64DecodeTime, s64OldPlayTime);
					PlayNextFramePriv();
					continue;
				}
				else
				{//解码到需要的时间戳了
					//Pause();
					//break;
				}
			}
			else
			{//从前往后播放的，需要截图的 时间比播放时间更 远 右边
				//解码到须要的时间戳了
				//Pause();
				
			}
			if (s64RenderTime >= s64TimeNeedToBe)
			{
				debug("%s success\n", __FUNCTION__);
				pRender->SetVisible(bRenderVisible);
				PlayNextFramePriv();
				break;
			}

			lTimeNow = timeGetTime();
			/*Sleep(10);*/
			PlayNextFramePriv();

		}while ( nTries-- > 0 
			&& (lTimeNow-lTimeBegin)<lTimeOut );

		if (nTries <= 0)
		{ 
			debug("%s, try seek I frame and decode to pos timeout..., try to seek any pos \n", __FUNCTION__);
			//Seek(s64TimeNeedToBe, AVSEEK_FLAG_ANY);
			//m_pMutexFilters->Lock();
			CMediaFilter *pFilter = NULL;
			nFlag =  eSSF_Any;
			for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
			{
				pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
				if (!pFilter)
					continue;
				pFilter->SeekToTimestamp(s63Timestamp, nFlag);
			}

			//m_pMutexFilters->Unlock();
			pRender->SetVisible(bRenderVisible);
			PlayNextFramePriv();

			bSeekOk = false;
		}
		else
		{
			debug("%s, try [%d]times, seek I frame and decode to pos success\n", __FUNCTION__, nTotal-nTries);
			bSeekOk = true;
		}

		//SetPlaySpeed(fSpeedOld);
		//Resume();
		//Sleep(100);
		pRender->SetVisible(bRenderVisible);
		pRender->Refresh();

	}
	else
	{
		PlayNextFramePriv();
		PlayNextFramePriv();
		pRender->Refresh();
	}
	//m_pMutexFilters->Lock();
	SetPlayStatePriv(nPlayStateOld);
	m_pMutexFilters->Unlock();
	
	int nRet = 0;
	if (bSeekOk)
	{
		nRet = 0;
	}
	else
	{
		nRet = -1;
	}
	int64 s64RenderPts = 0;
	s64RenderPts = pRender->GetPts();
	debug("%s --: render pts %I64d\n", __FUNCTION__, s64RenderPts);

	return nRet;
}
int CMediaStream::PlayNextFrame()
{//不断播放 next frame 直到 所有的 filter 都返回正确的值。
	debug("%s ++\n", __FUNCTION__);

	//如果是反向 单帧
	if (m_playMode == eSPM_PlayPrevFrame)
	{//须要重新 seek 定位到当前 播放的pts
		int64 s64RenderPts = -1;
		CMediaFilter *pFilter = 0;
		CRender *pRender = 0;
		pFilter = FindFilterByKind(FILTER_KIND_RENDER);
		if (pFilter)
		{
			pRender = dynamic_cast<CRender*>(pFilter);
		}
		if (pRender)
		{
			s64RenderPts = pRender->GetPts();
		}
		if (s64RenderPts >= 0)
		{
			SeekToTimestamp(s64RenderPts, eSSF_UntilDecode|eSSF_Backward);
		}
		
		debug("%s, old play is playPrevFrame, need to seek: %I64d \n", __FUNCTION__, s64RenderPts);

	}
		
	//m_state = eSS_Pause;
	m_playMode = eSPM_PlayNextFrame;
	m_pMutexFilters->Lock();

	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		pFilter->SetPlayMode(eFPM_PlayNextFrame);
		pFilter->Pause();
	}
 

	int nRet = 0;
	bool bRenderNextFrameSuccess = false;
	int nTries = 100;
	int nTried = 0;
	do 
	{
		bool bHaveRender = false;
		for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
		{
			pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
			if (!pFilter)
				continue;
			if (pFilter->IsKind(FILTER_KIND_RENDER))
			{
				bHaveRender = true;
				nRet = pFilter->PlayNextFrame();
				if (nRet >= 0)
				{
					debug("%s, play next frame success \n", __FUNCTION__);
					bRenderNextFrameSuccess = true;
					break;
				}
				else
				{
					debug("%s, play next frame false need decode more \n", __FUNCTION__);
				}
			}
		}
		if (!bHaveRender)
		{//从解码器 获取 是否成功

		}
		if (!bRenderNextFrameSuccess)
		{//播放没成功， 可能没数据了，需要上游的 fliter给数据
			debug("%s, no frame, need decode more ..\n", __FUNCTION__);
			for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
			{
				pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
				if (!pFilter)
					continue;
				if (pFilter->IsKind(FILTER_KIND_SOURCE))
				{//执行一次数据块流动
					pFilter->PlayNextFrame();
				}
			}
		}
		nTried++;
		debug("%s, tries: %d\n", __FUNCTION__, nTried);
	} while (!bRenderNextFrameSuccess 
		&& nTries--
		&& !IsStreamEnd());
	m_pMutexFilters->Unlock();
 
	if (bRenderNextFrameSuccess)
	{
		nRet = 0;
	}
	else
	{
		nRet = -1;
	}
	debug("%s -- %d\n", __FUNCTION__, nRet);
	return nRet;
}
int CMediaStream::PlayPrevFrame()
{
	debug("%s ++\n", __FUNCTION__);
	//不断播放 next frame 直到 所有的 filter 都返回正确的值。
	//m_state = eSS_Pause;
	m_playMode = eSPM_PlayPrevFrame;

	m_pMutexFilters->Lock();
	CMediaFilter *pFilter = NULL;
	int nRet = 0;
	bool bRenderPrevFrameSuccess = false;
	int nTries = 300;
	int nTried = 0;
	//首先清空 缓存
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
	
		 //pFilter->Flush();
		 pFilter->Pause();
		 pFilter->SetPlayMode(eFPM_PlayPrevFrame);
	}

	do 
	{
		int64 s64Pts = 0;
		for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
		{
			pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
			if (!pFilter)
				continue;
			
			if (pFilter->IsKind(FILTER_KIND_RENDER))
			{
				nRet = pFilter->PlayPrevFrame();
				if (nRet >= 0)
				{
					debug("%s, play prev frame success \n", __FUNCTION__);
					bRenderPrevFrameSuccess = true;
					m_nIndexInGop--;
					break;
				}
				else
				{
					debug("%s, play prev frame false ,need decode more \n", __FUNCTION__);
				}
				CRender *pRender = dynamic_cast<CRender*>(pFilter);
				if (pRender)
					s64Pts = pRender->GetPts();
			}
			else
			{//须要触发一下动作，比如解码器
				//pFilter->PlayPrevFrame();
			}
		}
		if (!bRenderPrevFrameSuccess)
		{//播放没成功， 可能没数据了，需要上游的 fliter给数据
			//重置解码器。以便释放最后一帧 可能重复输出的数据
			CMediaFilter *pDecoder = FindFilterByKindPriv(FILTER_KIND_DECODER);
			if (pDecoder)
			{
				pDecoder->Reset();
			}
			//
			int nRetSourcePlay = 0;
			debug("%s, no frame, need decode more, pts: %I64d ..\n", __FUNCTION__, s64Pts);
			for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
			{
				pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
				if (!pFilter)
					continue;
				if (pFilter->IsKind(FILTER_KIND_SOURCE))
				{//执行一次数据块流动,以s64Pts为刻度的前一帧
					
					nRetSourcePlay = pFilter->PlayPrevFrame(s64Pts);
					
				}
			}
			if (nRetSourcePlay < 0)
			{
				//源播放失败 则结束训话
				nRet = -1;
				goto ret;
			}
		}
		nTried++;
		debug("%s, tries: %d\n", __FUNCTION__, nTried);
	} while (!bRenderPrevFrameSuccess
		&& nTries--
		&& !IsStreamEnd());
	
	if (bRenderPrevFrameSuccess == false)
	{//向后播放失败
		nRet = -1;
	}
	else
		nRet = 0;

ret:
	m_pMutexFilters->Unlock();

	return nRet;
}

int CMediaStream::SetPlaySpeed(float fSpeed)
{
	m_pMutexFilters->Lock();
	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;

		pFilter->SetPlaySpeed(fSpeed);
	}
	m_pMutexFilters->Unlock();
	
	m_fSpeed = fSpeed;

	return 0;
}
float CMediaStream::GetPlaySpeed()
{
	
	return m_fSpeed;
}
bool CMediaStream::IsPaused()
{
	return (m_state == eSS_Pause)? true:false;
}
bool CMediaStream::IsStoped()
{
	return (m_state == eSS_Stop)? true:false;
}
bool CMediaStream::IsStreamEnd()
{

	return m_bIsStreamEnd;
}
//设置 图像需要显示的区域，将 设定的区域显示在整个窗口中。类似于缩放。
int CMediaStream::SetShowRect(int nX, int nY, int nW, int nH)
{
	return 0;
}
//只是 对  BufferSource 有效
int CMediaStream::FillData(char *pData, int nLen)
{

	return 0;
}
int CMediaStream::Refresh()
{
	if (!m_pMutexFilters)
		return -1;
	//这个锁 效率比较低？ 为啥？ 
	m_pMutexFilters->Lock();
	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		//push mode 找到 source, 如果是 pull mode 需要找到 accepter
		if (!pFilter->IsKind(FILTER_KIND_RENDER))
			continue;
		CRender *pRender = (CRender *)pFilter;
		pRender->Refresh();
	}
	m_pMutexFilters->Unlock();
	return 0;
}
// int CMediaStream::AppendFilter(CMediaFilter *pFilter)
// {
// 	if (!m_pMutexFilters)
// 		return -1;
// 	m_pMutexFilters->Lock();
// 	m_lstFilter.push_back(pFilter);
// 
// 	//pFilter->SetEventCb(m_cbEvent, m_pCbEventUserData, m_pCbEventReversed);
// 	//pFilter->SetProgressCb(m_cbProgress, m_pCbProgressUserData, m_pCbProgressReversed);
// 	//pFilter->SetFinishedCb(m_cbFinished, m_pCbFinishedUserData, m_pCbFinishedReversed);
// 
// 	m_pMutexFilters->Unlock();
// 
// 	return 0;
// }
int CMediaStream::SetProgressCb(FilterCB_Progress cbProgress, void *pUserData, void *pReversed)
{
	return  0;
	int nRet = 0;
// 	m_pMutexFilters->Lock();
// 
// 	m_cbProgress =	cbProgress;
// 	m_pCbProgressUserData = pUserData;
// 	m_pCbProgressReversed = pReversed;
// 	for (int i=0; i<m_lstFilter.size(); i++)
// 	{
// 		CMediaFilter *pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
// 		if (pFilter->IsKind("source"))
// 			m_lstFilter.at(i)->SetProgressCb(cbProgress, pUserData, pReversed);
// 	}
// 
// 	m_pMutexFilters->Unlock();
	return nRet;
}
int CMediaStream::SetFinishedCb(FilterCB_Finished cbFinished, void *pUserData, void *pReversed)
{
	return  0;
	int nRet = 0;

// 	m_pMutexFilters->Lock();
// 
// 	m_cbFinished = cbFinished;
// 	m_pCbFinishedUserData = pUserData;
// 	m_pCbFinishedReversed = pReversed;
// 
// 	for (int i=0; i<m_lstFilter.size(); i++)
// 	{
// 		m_lstFilter.at(i)->SetFinishedCb(cbFinished, pUserData, pReversed);
// 	}
// 
// 	m_pMutexFilters->Unlock();
	return nRet;
}
int CMediaStream::SetEventCb(FilterCB_Event cbEvent, void *pUserData, void *pReversed)
{
	return  0;
	int nRet = 0;

// 	m_pMutexFilters->Lock();
// 
// 
// 	m_cbEvent = cbEvent;
// 	m_pCbEventUserData = pUserData;
// 	m_pCbEventReversed = pReversed;
// 	
// 	for (int i=0; i<m_lstFilter.size(); i++)
// 	{
// 		m_lstFilter.at(i)->SetEventCb(cbEvent, pUserData, pReversed);
// 	}
// 
// 	m_pMutexFilters->Unlock();

	return nRet;
}

CMediaFilter * CMediaStream::FindFilterByKind( char *pSzKind )
{
	CMediaFilter *pFilter(NULL);
	if (!m_pMutexFilters)
		return NULL;
	m_pMutexFilters->Lock();

	for (int i=0; i<m_lstFilter.size(); i++)
	{
		CMediaFilter *pFilterTmp = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilterTmp)
			continue;
		if (pFilterTmp->IsKind(pSzKind))
		{
			pFilter = pFilterTmp;
			break;
		}
	}
	m_pMutexFilters->Unlock();

	return pFilter;
}

CMediaFilter * CMediaStream::FindFilterByKindPriv( char *pSzKind )
{
	CMediaFilter *pFilter(NULL);
	if (!m_pMutexFilters)
		return NULL;

	for (int i=0; i<m_lstFilter.size(); i++)
	{
		CMediaFilter *pFilterTmp = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilterTmp)
			continue;
		if (pFilterTmp->IsKind(pSzKind))
		{
			pFilter = pFilterTmp;
			break;
		}
	}


	return pFilter;
}
int CMediaStream::SetParam( char *pName, char *pValue )
{
	map<string, string>::iterator item;
	item = m_mapParam.find(pName);
	if (item == m_mapParam.end())
	{//不存在

	}
	m_mapParam[pName] = pValue;

	for (int i=0; i<m_lstFilter.size(); i++)
	{
		CMediaFilter *pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		pFilter->SetParam(pName, pValue);
	}

	return 0;
}
int CMediaStream::GetParam(char *pSzName, char *pSzValue)
{

	return 0;
}

int CMediaStream::Control( char *pSzName, char *pSzValue )
{
	for (int i=0; i<m_lstFilter.size(); i++)
	{
		CMediaFilter *pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		pFilter->Control(pSzName, pSzValue);
	}
	return 0;
}
int CMediaStream::Snapshot(char *pSzSaveFile)
{
	int nRet = 0;

	debug("%s: ++n", __FUNCTION__);
	CMediaFilter *pFilter(0);
	CRender *pRender(0);
	pFilter = FindFilterByKind(FILTER_KIND_RENDER);

	if(pFilter)
	{
		pRender = dynamic_cast<CRender*>(pFilter);
		nRet = pRender->SnapShot(pSzSaveFile);
	}

	return 0;
}

//保存指定时间的一个 截图
int CMediaStream::Snapshot( char *pSzSaveFile, int64 s64Time )
{
	//播放，直到 解码器解码到 该时间位置
	CDecoder *pDecoder = dynamic_cast<CDecoder*>(FindFilterByKind(FILTER_KIND_DECODER));
	if (!pDecoder)
		return -1;
	CSourceFilter *pSource = dynamic_cast<CSourceFilter*>(FindFilterByKind(FILTER_KIND_SOURCE));
	if (!pSource)
		return -2;

#ifdef OLD
	int nRet = 0;
	//seek 到一个位置 s64Time
	//定位到一个I帧
	float fSpeed = m_fSpeed;

	int64 s64OldPlayTime = 0;
	s64OldPlayTime =   pDecoder->GetTimestamp();
	s64OldPlayTime = pSource->TimestampToTime(s64OldPlayTime);
	SetPlaySpeed(32);
	//找到 前一个 I帧 
	Seek(s64Time, AVSEEK_FLAG_BACKWARD);
	Resume();

	int64 s64DecodeTime = 0;
	int64 s64DecodeTimestamp=0;
	
	bool bSeekOk =false;
	int nTries = 5000;
	int nTotal = nTries;
	long lTimeBegin = timeGetTime();
	long lTimeNow = timeGetTime();
	long lTimeOut = 5*1000;

	Sleep(10);
	do{
		s64DecodeTimestamp = pDecoder->GetTimestamp();
		s64DecodeTime = pSource->TimestampToTime(s64DecodeTimestamp);
		debug("%s: need to seek,  decode time %I64d, need seek time: %I64d\n", 
			__FUNCTION__,  
			s64DecodeTime, s64Time);
		
		if (s64DecodeTime >= s64Time)
		{
			if (s64OldPlayTime > s64Time)
			{//已经播放到后面去的，需要截图的时间在 播放的时间的左边
				if (s64OldPlayTime >0 && s64DecodeTime >= s64OldPlayTime)
				{//解码的时间还没有退回去
					debug("%s, decode time is newer than old play time[%I64d]:[%I64d], seek data is not arrive \n", 
						__FUNCTION__,
						s64DecodeTime, s64OldPlayTime);
					continue;
				}
				else
				{
					Pause();
					break;
				}
			}
			else
			{//从前往后播放的，需要截图的 时间比播放时间更 远 右边
				Pause();
				break;
			}
		}
		
		Sleep(10);
		lTimeNow = timeGetTime();
	}while ( nTries-- > 0
		&& (lTimeNow-lTimeBegin)<lTimeOut );

	if (nTries < 0)
	{ 
		debug("%s, try seek I frame and decode to pos timeout..., try to seek any pos \n", __FUNCTION__);
		Seek(s64Time, AVSEEK_FLAG_ANY);

		Sleep(40);
		Pause();
		bSeekOk = true;
	}
	else
	{
		debug("%s, try [%d]times, seek I frame and decode to pos success , decode timestamp[%I64d]\n", 
			__FUNCTION__, nTotal-nTries, pDecoder->GetTimestamp()
			);
		bSeekOk = true;
	}
	
	//从解码器获取 图片
	if (bSeekOk)
	{//找到图片
		int nH = pSource->GetHeight();
		int nW = pSource->GetWidth();
		int nLen = nW*nH*4;
		char *pImage = new char[nLen];
		int nCopyLen = nLen;
		pDecoder->CopyFrame((unsigned char*)pImage, nCopyLen, nW, nH, AV_PIX_FMT_YUV420P);
		
		if (nCopyLen > 0)
		{//正常
			CFfmpegCodec *pCodec = new CFfmpegCodec();
			sFfmpegVideoInfo videoInfo;
			videoInfo.m_nBitRate = 40*1024*8;
			videoInfo.m_nCodecId = CODEC_ID_MJPEG;//CODEC_ID_JPEG2000;
			videoInfo.m_nGopSize = 1;
			videoInfo.m_nPixFormat = PIX_FMT_YUV420P;
			videoInfo.m_nHeight = nH;
			videoInfo.m_nWidth = nW;

			pCodec->SetVideoParam(videoInfo);
			int nRet = 0;
			nRet = pCodec->Open(false);
			if (nRet < 0)
			{
				debug("%s, open ffmpeg to encode jpeg false [%d] \n", __FUNCTION__, nRet);
			}
			//压缩率假定为 5 
			int nJpegLen = nW*nH*3/2/5;
			char *pJpeg = new char[nJpegLen];
			pCodec->Encode((unsigned char*)pImage, videoInfo.m_nWidth*videoInfo.m_nHeight*3/2, 
				(uint8*)pJpeg, nJpegLen);
			//nJpegLen = 0;
			if (nJpegLen > 0)
			{
				debug("%s: encode picture success, data len[%d]\n", __FUNCTION__, nJpegLen);
				FILE *pFile = 0;
				pFile = fopen(pSzSaveFile, "w+b");
				if (pFile)
				{
					fwrite(pJpeg, nJpegLen, 1, pFile);
					fclose(pFile);
					pFile = NULL;
				}
			}
			else
			{
				debug("%s: encode picture false\n", __FUNCTION__);
			}

			delete pJpeg;
			delete pCodec;
		}

		delete pImage;
	}
	else{
		nRet = -3;
	}

	SetPlaySpeed(fSpeed);
	
	Resume();
#else
	int nRet = 0;
	int64 s64Timestamp(0);
	s64Timestamp = pSource->TimeToTimestamp(s64Time);
	int nOldeState = m_state;
	Pause();
	nRet = SeekToTimestamp(s64Timestamp, eSSF_UntilDecode|eSSF_Backward);
	if (nRet >= 0)
	{
		debug("%s: seek to timestamp false\n", __FUNCTION__);
		CMediaFilter *pFilter(0);
		CRender *pRender(0);
		pFilter = FindFilterByKind(FILTER_KIND_RENDER);
		if(pFilter)
		{
			pRender = dynamic_cast<CRender*>(pFilter);
			nRet = pRender->SnapShot(pSzSaveFile);
		}
	}

	m_pMutexFilters->Lock();
	SetPlayStatePriv(nOldeState);
	m_pMutexFilters->Unlock();
#endif


	return nRet;
}
int CMediaStream::Snapshot(char *pImage, int &nLen, int &nW, int &nH, int64 s64Time)
{

 
	//播放，直到 解码器解码到 该时间位置
	CDecoder *pDecoder = dynamic_cast<CDecoder*>(FindFilterByKind(FILTER_KIND_DECODER));
	if (!pDecoder)
		return -1;
	CSourceFilter *pSource = dynamic_cast<CSourceFilter*>(FindFilterByKind(FILTER_KIND_SOURCE));
	if (!pSource)
		return -2;

#ifdef OLD
	int nRet = 0;
	//seek 到一个位置 s64Time
	//定位到一个I帧
	float fSpeed = m_fSpeed;



	int64 s64OldPlayTime = 0;
	s64OldPlayTime =   pDecoder->GetTimestamp();
	s64OldPlayTime = pSource->TimestampToTime(s64OldPlayTime);
	SetPlaySpeed(32);
	//找到 前一个 I帧 
	Seek(s64Time, AVSEEK_FLAG_BACKWARD);
	Resume();

	int64 s64DecodeTime = 0;
	int64 s64DecodeTimestamp=0;

	bool bSeekOk =false;
	int nTries = 5000;
	int nTotal = nTries;
	long lTimeBegin = timeGetTime();
	long lTimeNow = timeGetTime();
	long lTimeOut = 5*1000;

	Sleep(10);
	do{
		s64DecodeTimestamp = pDecoder->GetTimestamp();
		s64DecodeTime = pSource->TimestampToTime(s64DecodeTimestamp);
		debug("%s: need to seek,  decode time %I64d, need seek time: %I64d\n", 
			__FUNCTION__,  
			s64DecodeTime, s64Time);

		if (s64DecodeTime >= s64Time)
		{
			if (s64OldPlayTime > s64Time)
			{//已经播放到后面去的，需要截图的时间在 播放的时间的左边
				if (s64OldPlayTime >0 && s64DecodeTime >= s64OldPlayTime)
				{//解码的时间还没有退回去
					debug("%s, decode time is newer than old play time[%I64d]:[%I64d], seek data is not arrive \n", 
						__FUNCTION__,
						s64DecodeTime, s64OldPlayTime);
					continue;
				}
				else
				{
					Pause();
					break;
				}
			}
			else
			{//从前往后播放的，需要截图的 时间比播放时间更 远 右边
				Pause();
				break;
			}
		}

		Sleep(10);
		lTimeNow = timeGetTime();
	}while ( nTries-- > 0
		&& (lTimeNow-lTimeBegin)<lTimeOut );

	if (nTries < 0)
	{ 
		debug("%s, try seek I frame and decode to pos timeout..., try to seek any pos \n", __FUNCTION__);
		Seek(s64Time, AVSEEK_FLAG_ANY);

		Sleep(40);
		Pause();
		bSeekOk = true;
	}
	else
	{
		debug("%s, try [%d]times, seek I frame and decode to pos success , decode timestamp[%I64d]\n", 
			__FUNCTION__, nTotal-nTries, pDecoder->GetTimestamp()
			);
		bSeekOk = true;
	}

	//从解码器获取 图片
	if (bSeekOk)
	{//找到图片
		int nH = pSource->GetHeight();
		int nW = pSource->GetWidth();
		int nLen = nW*nH*4;
		//char *pImage = new char[nLen];
		int nCopyLen = nLen;
		pDecoder->CopyFrame((unsigned char*)pImage, nCopyLen, nW, nH, AV_PIX_FMT_BGRA);
		nLen = nW*nH*4;
	}
	else{
		nRet = -3;
	}

	SetPlaySpeed(fSpeed);

	Resume();

#else
	
	int nRet = 0;
	int64 s64Timestamp = 0;
	if (s64Time <0)
	{
		 
		CMediaFilter *pFilter(0);
		CRender *pRender(0);
		pFilter = FindFilterByKind(FILTER_KIND_RENDER);

		if(pFilter)
		{
			pRender = dynamic_cast<CRender*>(pFilter);
			nRet = pRender->SnapShot((unsigned char*)pImage, nW, nH);
		}
	}
	else{
		s64Timestamp = pSource->TimeToTimestamp(s64Time);
		int nOldeState = m_state;
		Pause();
		nRet = SeekToTimestamp(s64Timestamp, eSSF_UntilDecode|eSSF_Backward);
		if (nRet >= 0)
		{

			int nH = pSource->GetHeight();
			int nW = pSource->GetWidth();
			int nLen = nW*nH*4;
			//char *pImage = new char[nLen];
			int nCopyLen = nLen;
			pDecoder->CopyFrame((unsigned char*)pImage, nCopyLen, nW, nH, AV_PIX_FMT_BGRA);
			nLen = nW*nH*4;

		}
		m_pMutexFilters->Lock();
		SetPlayStatePriv(nOldeState);
		m_pMutexFilters->Unlock();
	}
#endif


	return nRet;
}


int CMediaStream::PlayNextFramePriv()
{
	//debug("%s ++\n", __FUNCTION__);

	//m_state = eSS_Pause;
	m_playMode = eSPM_PlayNextFrame;
  
	CMediaFilter *pFilter = NULL;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		pFilter->SetPlayMode(eFPM_PlayNextFrame);
		pFilter->Pause();
	}


	int nRet = 0;
	bool bRenderNextFrameSuccess = false;
	int nTries = 100;
	int nTried = 0;
	do 
	{
		bool bHaveRender = false;
		for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
		{
			pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
			if (!pFilter)
				continue;
			if (pFilter->IsKind(FILTER_KIND_RENDER))
			{
				bHaveRender = true;
				nRet = pFilter->PlayNextFrame();
				if (nRet >= 0)
				{
					//debug("%s, play next frame success \n", __FUNCTION__);
					bRenderNextFrameSuccess = true;
					break;
				}
				else
				{
					//debug("%s, play next frame false need decode more \n", __FUNCTION__);
				}
			}
		}
		if (!bHaveRender)
		{//从解码器 获取 是否成功

		}
		if (!bRenderNextFrameSuccess)
		{//播放没成功， 可能没数据了，需要上游的 fliter给数据
			//debug("%s, no frame, need decode more ..\n", __FUNCTION__);
			for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
			{
				pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
				if (!pFilter)
					continue;
				if (pFilter->IsKind(FILTER_KIND_SOURCE))
				{//执行一次数据块流动
					pFilter->PlayNextFrame();
				}
			}
		}
		nTried++;
		//debug("%s, tries: %d\n", __FUNCTION__, nTried);
	} while (!bRenderNextFrameSuccess 
		&& nTries--
		&& !IsStreamEnd());
 
	if (bRenderNextFrameSuccess)
	{
		nRet = 0;
	}
	else
	{
		nRet = -1;
	}
	//debug("%s -- %d\n", __FUNCTION__, nRet);
	return nRet;
}

int CMediaStream::SetPlayStatePriv( int nState )
{
	
	CMediaFilter *pFilter = 0;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		 pFilter->SetPlayState(nState);
	}

	m_state = (ePipeLineState)nState;
	return 0;
}

int CMediaStream::SetPlayModePriv( int nMode )
{

	CMediaFilter *pFilter = 0;
	for (int i=0; i<m_lstFilter.size() && m_state != eSS_Stop; i++)
	{
		pFilter = dynamic_cast<CMediaFilter*>(m_lstFilter.at(i));
		if (!pFilter)
			continue;
		pFilter->SetPlayMode((eFilterPlayMode)nMode);
	}

	m_playMode = (eStreamPlayMode)nMode;

	return 0;
}

// void CBaseStream::TestStdAllocator( vector<int> &lstValue )
// {
// 	for (int i=0; i<1000; i++)
// 	{
// 		lstValue.push_back(i);
// 	}
// 	
// }
