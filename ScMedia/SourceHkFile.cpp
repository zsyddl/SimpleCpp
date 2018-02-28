#include "StdAfx.h"
#include "SourceHkFile.h"
#include "Buffer.h"
#include "PlayM4.h"
#include "VideoFramePinBuffer.h"
#include "FfmpegCodec.h"
#include "mutex.h"
#include "FilterManager.h"

CSourceHkFile g_regSourceHkFile;


void  CSourceHkFile::HkDecDataCallBack(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	if (pFrameInfo->nType == T_AUDIO8
		|| pFrameInfo->nType == T_AUDIO16)
	{
		return ;
	}

	//debug("%s\n", __FUNCTION__);
	CSourceHkFile *pHkFile = (CSourceHkFile*) nReserved1;
	pHkFile->m_nTimePerFrame = 1000/pFrameInfo->nFrameRate;

	if (pHkFile->GetState() == eFilterState_Stop
		|| pHkFile->GetState() == eFilterState_ToStop)
		return ;

	CFilterPin *pPinOut = pHkFile->m_lstPinOut.at(0);
	if (!pPinOut)
		return;
	while (pPinOut->GetFreeBufferCount() <= 0
		&& pHkFile->GetState() == eFilterState_Run)
	{
		Sleep(2);
	};

	CFilterPinBuffer  *pPinBuffer = NULL;
	pPinOut->GetFreeBuffer(&pPinBuffer);
	if (!pPinBuffer)
		return ;

	CVideoFramePinBufer *pVideoFrame = 0;
	pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pPinBuffer);
	if (!pVideoFrame)
	{
		pPinOut->PutFreeBuffer(pPinBuffer);
		return ;
	}
	pPinOut->SetAutoFreeMemory(0);
	// copy yuv to video frame;
	pVideoFrame->SetFrameSize(pFrameInfo->nWidth, pFrameInfo->nHeight);
	pVideoFrame->SetUsedSize(pFrameInfo->nWidth*pFrameInfo->nHeight*3);
	
	CBuffer *pBufferExt = pVideoFrame->GetExtData();
	if (pBufferExt->GetAllocSize() < sizeof(sFfmpegFrameInfo))
	{
		pBufferExt->SetAllocSize(sizeof(sFfmpegFrameInfo));
		pBufferExt->SetUsed(sizeof(sFfmpegFrameInfo));

	}
	sFfmpegFrameInfo *pFfmpegFrameInfo = (sFfmpegFrameInfo*)pBufferExt->GetData();
	memset(pFfmpegFrameInfo, 0, sizeof(sFfmpegFrameInfo));
	//这个是毫秒，需要转换到 时间戳，时间戳 是 timebase 的一个计数
	
	pFfmpegFrameInfo->m_nTimeDen = pFrameInfo->nFrameRate;
	if (pFfmpegFrameInfo->m_nTimeDen <= 0)
		pFfmpegFrameInfo->m_nTimeDen = 25;
	pFfmpegFrameInfo->m_nTimeNum = 1;;
	pFfmpegFrameInfo->m_s64Pts = pFrameInfo->nStamp/(1000*pFfmpegFrameInfo->m_nTimeNum/pFfmpegFrameInfo->m_nTimeDen);//PlayM4_GetPlayedTime(pHkFile->m_nHkStreamHandle);
	//debug("%s, timestamp: time %d,->%d\n ", __FUNCTION__,  pFrameInfo->nStamp, pFfmpegFrameInfo->m_s64Pts);

	pHkFile->m_s64CurrTime = pFrameInfo->nStamp;

	pFfmpegFrameInfo->m_videoInfo.m_nWidth = pFrameInfo->nWidth;
	pFfmpegFrameInfo->m_videoInfo.m_nHeight = pFrameInfo->nHeight;
	pFfmpegFrameInfo->m_s64Bts = 0;
	pFfmpegFrameInfo->m_videoInfo.m_nFps = pFrameInfo->nFrameRate;
	pBufferExt->SetUsed(sizeof(sFfmpegFrameInfo));


	memcpy(pVideoFrame->m_pY, (char *)pBuf, pFrameInfo->nWidth * pFrameInfo->nHeight);
	memcpy(pVideoFrame->m_pV, (char *)(pBuf + pFrameInfo->nWidth * pFrameInfo->nHeight), pFrameInfo->nWidth * pFrameInfo->nHeight/4);
	memcpy(pVideoFrame->m_pU, (char *)(pBuf + pFrameInfo->nWidth * pFrameInfo->nHeight * 5/4), pFrameInfo->nWidth * pFrameInfo->nHeight/4);


	pPinOut->PutPreparedBuffer(pVideoFrame);

	
	//pHkFile->DoFilter();
}

CSourceHkFile::CSourceHkFile(void)
:CSourceFilter()
, m_pFile(NULL)
, m_pBufferRead(NULL)
, m_s64TotalLen(0)
, m_s64CurPos(0)
, m_nHkStreamHandle(-1)
, m_bHkInit(false)
, m_nHkHeaderLen(0)
, m_lTimeLastRead(0)
, m_nTimePerFrame(25)
, m_lWindow(0)
, m_pMutexDoFilter(NULL)
{
	strcpy(m_szName, FILTER_SOURCE_HK_FILE);
	strcpy(m_szType, FILTER_SOURCE_HK_FILE);
	strcpy(m_szFormatName, AvFormat_HK);
	strcpy(m_szFormatSubName, AvFormat_HK);

	m_pMutexDoFilter = new CMutex();

	CFilterManager::RegistFilter(&g_regSourceHkFile);
}

CSourceHkFile::~CSourceHkFile(void)
{
	if (m_pBufferRead)
	{
		delete m_pBufferRead;
		m_pBufferRead = NULL;
	}
	if (m_pFile)
	{
		/*delete m_pFile;*/
		fclose(m_pFile);
		m_pFile = NULL;
	}

	int nRet = 0;
	int nError = 0;
	if (m_nHkStreamHandle != -1)
	{
		nRet = PlayM4_Stop(m_nHkStreamHandle);
		if(!nRet)
		{
			nError = PlayM4_GetLastError(m_nHkStreamHandle);
			//vivs_warning("Stop failed for reason %d, error code(%d)", hr, m);
		}

		nRet = PlayM4_CloseFile(m_nHkStreamHandle);
		if(!nRet)
		{
			nError = PlayM4_GetLastError(m_nHkStreamHandle);
			//vivs_warning("CloseStream failed for reason %d, error code(%d)", hr, m);
		}

		PlayM4_FreePort(m_nHkStreamHandle);

		m_nHkStreamHandle = -1;
	}

	if (m_pMutexDoFilter)
	{
		delete m_pMutexDoFilter;
		m_pMutexDoFilter = NULL;
	}
}

int CSourceHkFile::DoFilter()
{
	//读取文件放入到 outpin
// 	long lTimeNow = timeGetTime();
// 	else if (lTimeNow - m_lTimeLastRead < m_nTimePerFrame/m_fSpeed)
// 	{//不到时间
// 		Sleep(5);
// 		return CSourceFilter::DoFilter();
// 	}
	m_pMutexDoFilter->Lock();
	//恢复
	if (m_nHkStreamHandle >= 0
		&& m_state != eFilterState_Pause)
	{
		PlayM4_Pause(m_nHkStreamHandle, 0);
	}

	if (m_fSpeed >16 )
	{
		m_nNeedSleep = 0;
	}

	long lTotalTime = 0, lCurTime=0;
	lCurTime = PlayM4_GetPlayedTimeEx(m_nHkStreamHandle);
	lTotalTime = PlayM4_GetFileTime(m_nHkStreamHandle)*1000;
	if (m_cbProgress)
	{
	
		m_cbProgress(m_fSpeed, lCurTime, lTotalTime, m_pCbProgressUserData, m_pCbProgressReversed);
	
	}
	if ( (lCurTime >= lTotalTime || lCurTime< 0)
		&& m_cbFinished)
	{
		m_cbFinished(m_pCbFinishedUserData, m_pCbFinishedReversed);
	}

	CSourceFilter::DoFilter();
	m_pMutexDoFilter->Unlock();
	return 0;
/*
	CFilterPin *pPinOut = NULL;
	pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	//读取数据
	ReadFrame();
	//
	// 取所有去一个 连接的 pin 
	CFilterPinBuffer *pOutPinBuffer = NULL;
	if (m_pBufferRead->GetLength()>0)
	{
		pPinOut->GetFreeBuffer(&pOutPinBuffer);
		if (pOutPinBuffer)
		{
			if (pOutPinBuffer->GetMaxSize() < m_pBufferRead->GetLength())
			{
				pOutPinBuffer->SetMaxSize(m_pBufferRead->GetLength());
			}
			int nAppend = pOutPinBuffer->Append((char*)m_pBufferRead->GetData(), m_pBufferRead->GetLength());
			m_pBufferRead->Clear(0, nAppend);
			pPinOut->PutPreparedBuffer(pOutPinBuffer);
		}
	}
	Sleep(33);
	//
	return CSourceFilter::DoFilter();
	*/
}

int CSourceHkFile::Open(char *pSzParam, bool bCreateThread/*=false*/)
{
	CSourceFilter::Open(pSzParam, bCreateThread);

	m_pBufferRead = new CBuffer();

	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	pPinOut->Clear();

	pPinOut->CreateBuffer<CVideoFramePinBufer>(4);
	//

	if (!PlayM4_GetPort((LONG *)&m_nHkStreamHandle))
	{
		// 		self->priv->could_work = FALSE;
		// 		vivs_warning("PlayM4_GetPort return false. so play error. form handle is %d, port(%d)", 
		// 			self->priv->port, self->priv->port);
		debug("%s: PlayM4_GetPort false\n", __FUNCTION__);
		return -1;
	}
	else
	{
		//		vivs_debug("hk Listener get port (%d), form handle(%d)", self->priv->port, self->priv->port);
	}
	char *pFile = (char*)GetParam("source");
	if (!pFile || strlen(pFile) <= 0)
	{
		debug("%s: PlayM4_GetPort false\n", __FUNCTION__);
		return -2;
	}
	
	if (!PlayM4_OpenFile(m_nHkStreamHandle, pFile))
	{
		debug("%s: PlayM4_OpenFile false\n", __FUNCTION__);
		return -1;
	}
	char *pHwnd = (char*)GetParam("window");
	long lHwnd = 0;
	if (pHwnd && strlen(pHwnd) > 0)
	{
		 lHwnd = atol(pHwnd);
	}
	m_lWindow = lHwnd;

	if (!PlayM4_Play(m_nHkStreamHandle, (HWND)lHwnd))
	{
		debug("%s: PlayM4_Play false\n", __FUNCTION__);
		return -1;
	}
	PlayM4_GetPictureSize(m_nHkStreamHandle, (long*)&m_nW, (long*)&m_nH);
	m_s64TotalTime = PlayM4_GetFileTime(m_nHkStreamHandle) * 1000;
	//没有显示的时候，必定回调数据
	if (lHwnd == 0)
	{
		PlayM4_SetDecCallBackMend(m_nHkStreamHandle, HkDecDataCallBack, (long)this);
	}
	PlayM4_Pause(m_nHkStreamHandle, 1);

	m_s64TotalTime = PlayM4_GetFileTime(m_nHkStreamHandle)*1000;
	//
	/*m_nHkHeaderLen = PlayM4_GetFileHeadLength();

	if (!PlayM4_SetStreamOpenMode(m_nHkStreamHandle, STREAME_FILE))
	{   
		//self->priv->could_work = FALSE;
		PlayM4_FreePort(m_nHkStreamHandle);
		m_nHkStreamHandle = -1;
		//vivs_warning("SetStreamOpenMode() failed, error(%d)", PlayM4_GetLastError(self->priv->port));
	}*/

	return 0;
}
// int CSourceHkFile::Open(char *pUrl)
// {
// 	//
// 	if (m_pFile)
// 		return -1;
// 
// 	m_pFile = fopen(pUrl, "r+b");
// 	if (!m_pFile)
// 		return -2;
// 	
// 	//打开海康的 文件
// 	_fseeki64(m_pFile, 0, SEEK_END );
// 	m_s64TotalLen = _ftelli64(m_pFile);
// 	m_s64CurPos = 0;
// 	_fseeki64(m_pFile, 0, SEEK_SET );
// 
// 	return 0;
// }
int CSourceHkFile::ReadFrame()
{
	if (m_pBufferRead->GetMaxSize() - m_pBufferRead->GetLength() < HK_FRAME_READ_LEN)
	{
		return -1;
	}
	//可以读取
	if (!m_pFile)
	{
		return -2;
	}
	char *pRead = new char[HK_FRAME_READ_LEN];
	int nRead = fread(pRead, 1, HK_FRAME_READ_LEN, m_pFile);
	if (nRead>0)
	{
		m_pBufferRead->Append((unsigned char *)pRead, nRead);
		m_s64CurPos += nRead;
	}
	else
	{//结束 
		if (m_cbFinished)
		{
			m_cbFinished(m_pCbFinishedUserData, m_pCbFinishedReversed);
		}
	}
	delete pRead;
	//
	if (m_cbProgress)
	{//上报进度消息
		float fProgress = 0;
		if (m_s64TotalLen != 0)
		{
			fProgress = (double)m_s64CurPos/(double)m_s64TotalLen;
		}
		m_cbProgress(1.0,  m_s64CurPos, m_s64TotalLen, m_pCbProgressUserData, m_pCbProgressReversed);
	}
	//
	return nRead;
}

CMediaFilter * CSourceHkFile::CreateObject()
{

	return new CSourceHkFile;
}

int CSourceHkFile::ProbMediaInfo( char *pSzFile )
{
	m_nW = 0;
	m_nH = 0;
	m_s64TotalTime = 0;
	long lStreamHandle = -1;
	int nRet =0;
	if (!PlayM4_GetPort((LONG *)&lStreamHandle))
	{
		// 		self->priv->could_work = FALSE;
		// 		vivs_warning("PlayM4_GetPort return false. so play error. form handle is %d, port(%d)", 
		// 		self->priv->port, self->priv->port);
		debug("%s: PlayM4_GetPort false\n", __FUNCTION__);
		
		nRet =  -1;
		goto ret;
	}
	else
	{
		//		vivs_debug("hk Listener get port (%d), form handle(%d)", self->priv->port, self->priv->port);
	}
	char *pFile = pSzFile;
	if (!pFile || strlen(pFile) <= 0)
	{
		debug("%s: PlayM4_GetPort false\n", __FUNCTION__);
		nRet =  -1;
		goto ret;

	}

	if (!PlayM4_OpenFile(lStreamHandle, pFile))
	{
		debug("%s: PlayM4_OpenFile false\n", __FUNCTION__);
		nRet =  -1;
		goto ret;
	}


	if (PlayM4_Play(lStreamHandle, (HWND)0))
	{
		PlayM4_GetPictureSize(lStreamHandle, (long*)&m_nW, (long*)&m_nH);
		m_s64TotalTime = PlayM4_GetFileTime(lStreamHandle) * 1000;
		PlayM4_Stop(lStreamHandle);
		// return -1;
	}else
	{
		debug("%s: PlayM4_Play false\n", __FUNCTION__);
	}

ret:
	if (lStreamHandle != -1)
	{
		PlayM4_CloseFile(lStreamHandle);
		PlayM4_FreePort(lStreamHandle);
		lStreamHandle = -1;
	}
	
	return nRet;
}

int CSourceHkFile::Pause()
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	PlayM4_Pause(m_nHkStreamHandle, 1);
	m_state = eFilterState_Pause;

	return 0;
}

int CSourceHkFile::Resume()
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	
	PlayM4_Pause(m_nHkStreamHandle, 0);

	SetPlaySpeed(m_fSpeed);
	
	m_state = eFilterState_Run;

	return 0;
}

int CSourceHkFile::SetPlaySpeed( float fSpeed )
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	m_fSpeed = fSpeed;
	if (m_state != eFilterState_Run)
		PlayM4_Pause(m_nHkStreamHandle, 0);
	//为什么开始pause的时候 设置倍速无效
	PlayM4_Play(m_nHkStreamHandle, (HWND)m_lWindow);

	if (fSpeed >= 0.99)
	{//>=1
		int nTimes = 0;
		nTimes = (int) fSpeed;
		for (int i=2; i<=nTimes; )
		{
			PlayM4_Fast(m_nHkStreamHandle);
			i *= 2;
		}
	}
	else
	{// < 1
		int nTimes = 0;
		nTimes = (int) (1.0/fSpeed);
		for (int i=2; i<=nTimes; )
		{
			PlayM4_Slow(m_nHkStreamHandle);
			i *= 2;
		}

	}

	if (m_state == eFilterState_Pause)
		PlayM4_Pause(m_nHkStreamHandle, 1);

	if (m_fSpeed >16 )
	{
		m_nNeedSleep = 0;
	}
	return 0;
}

int CSourceHkFile::Seek( int64 nPos , int nFlag/*=0*/)
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	
	PlayM4_SetPlayedTimeEx(m_nHkStreamHandle, (DWORD)nPos);
	
	int nStateOld = m_state;
	m_state = eFilterState_Run;

	DoFilter();
	//bool bRet = PlayM4_OneByOne(m_nHkStreamHandle);

	m_state = (eFilterState)nStateOld;


	return 0;
}


int64 CSourceHkFile::GetCurrTime()
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}

	long lCurTime = PlayM4_GetPlayedTimeEx(m_nHkStreamHandle);

	return lCurTime;

}
/*
int64 CSourceHkFile::GetTotalTime()
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	int64 lTotalTime = 0;
	lTotalTime = PlayM4_GetFileTime(m_nHkStreamHandle)*1000;

	return lTotalTime;
}
*/

int CSourceHkFile::SeekToTimestamp( int64 s64Timestamp , int nFlag/*=0*/)
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	DWORD dwTime = 0;
	int nFramRate = 25;
	nFramRate = PlayM4_GetCurrentFrameRate(m_nHkStreamHandle);
	if (nFramRate <= 0)
	{
		nFramRate = 25;
	}
	dwTime = s64Timestamp*1000/nFramRate;
	PlayM4_SetPlayedTimeEx(m_nHkStreamHandle, dwTime);

	bool bRet = PlayM4_OneByOne(m_nHkStreamHandle);

	return 0;
}

int CSourceHkFile::PlayNextFrame()
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	int nRet = 0;
	if (m_state != eFilterState_Run)
	{
		PlayM4_Pause(m_nHkStreamHandle, 0);
	}
	bool bRet = PlayM4_OneByOne(m_nHkStreamHandle);
	if (bRet)
	{

		DoFilter();

		nRet = 0;
	}
	else
	{
		nRet = -2;
	}
	if (m_state != eFilterState_Run)
	{
		PlayM4_Pause(m_nHkStreamHandle, 1);
	}

	return nRet;

}

int CSourceHkFile::PlayPrevFrame( int64 s64PtsNow/*=0*/ )
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	int nRet = 0;
	if (m_state != eFilterState_Run)
	{
		PlayM4_Pause(m_nHkStreamHandle, 0);
	}

	bool bRet = PlayM4_OneByOneBack(m_nHkStreamHandle);
	if (bRet)
	{
		DoFilter();
		nRet = 0;
	}
	else
	{
		nRet = -2;
	}
	if (m_state != eFilterState_Run)
	{
		PlayM4_Pause(m_nHkStreamHandle, 1);
	}

	return nRet;
}

int CSourceHkFile::SetPlayState( int nState )
{
	
	return CMediaFilter::SetPlayState(nState);
}

int CSourceHkFile::SetPlayMode( eFilterPlayMode playMode )
{
	if (m_nHkStreamHandle == -1)
	{
		return -1;
	}
	if (playMode == eFPM_Play 
		|| playMode == eFPM_None)
	{
		PlayM4_Play(m_nHkStreamHandle, 0);
	}
	return CMediaFilter::SetPlayMode(playMode);
}
