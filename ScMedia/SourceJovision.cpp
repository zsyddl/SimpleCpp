#include "StdAfx.h"
#include "SourceJovision.h"
#include "JVSDKP.h"
#include "FfmpegCodec.h"
#include "Buffer.h"
#include "VideoFramePinBuffer.h"
#include "FfmpegScale.h"
#include "FilterManager.h"

CSourceJovision g_regSourceJovision;


bool CSourceJovision::m_bInitSdk=false;

CSourceJovision::CSourceJovision(void)
:CSourceFilter()
, m_nPlayerHandle(-1)
, m_pBufferDecode(0)
{
	strcpy(m_szName, FILTER_SOURCE_JOVISION_FILE);
	strcpy(m_szType, FILTER_SOURCE_JOVISION_FILE);
	strcpy(m_szFormatName, AvFormat_JOVISION);

	m_pBufferDecode = new CBuffer(512*1024);
	m_pFfmpegScale = new CFfmpegScale();

	CFilterManager::RegistFilter(&g_regSourceJovision);
}

CSourceJovision::~CSourceJovision(void)
{
	if (m_nPlayerHandle != -1)
	{
		JVSP_Stop(m_nPlayerHandle);

		JVSP_ReleasePlayer(m_nPlayerHandle);
		m_nPlayerHandle = -1;
	}
	if (m_pBufferDecode)
	{
		delete m_pBufferDecode;
		m_pBufferDecode = NULL;
	}

	if (m_pFfmpegScale)
	{
		delete m_pFfmpegScale;
		m_pFfmpegScale = NULL;
	}
}

CMediaFilter * CSourceJovision::CreateObject()
{
	return new CSourceJovision();
}

int CSourceJovision::ProbMediaInfo( char *pSzFile )
{
	if (!m_bInitSdk)
	{
		if (JVSP_InitSDK(true))
		{

			debug("%s, init sdk success\n", __FUNCTION__);
			m_bInitSdk = true;
		}
		else
		{
			debug("%s, init sdk false\n", __FUNCTION__);
			return -1;
		}
	}

	if (m_nPlayerHandle != -1)
		return -3;

	//false: yuv  ;true: rgb32
	bool bRgb = false;
	int nPlayer = -1;
	int nRet = -1;
	nPlayer = JVSP_CreatePlayer( bRgb );
	if (nPlayer == -1)
	{
		debug("%s: create player false\n", __FUNCTION__);
		return -2;
	}

	m_nPlayerHandle = nPlayer;

	if (JVSP_OpenFile(m_nPlayerHandle, pSzFile))
	{
		debug("%s, open file success\n", __FUNCTION__);

		m_nTotalFrame = JVSP_GetTotalFrames(m_nPlayerHandle);

		JVS_HEADER header;
		JVSP_GetHeaderData(m_nPlayerHandle, &header);
		m_nH = header.i_frame_height;
		m_nW = header.i_frame_width;

		m_s64TotalTime = m_nTotalFrame/25*1000;
		int nTries = 0;
		JVSP_Play(m_nPlayerHandle);
		int nCurFrame = 0;
		nCurFrame = JVSP_GetCurFrameNum(m_nPlayerHandle);
		while(nTries--)
		{
			JVSP_StepForward(m_nPlayerHandle);

			nCurFrame = JVSP_GetCurFrameNum(m_nPlayerHandle);

			debug("%s: progress: %d/%d\n", __FUNCTION__, nCurFrame, m_nTotalFrame);
			Sleep(100);
		}

		JVSP_Stop(m_nPlayerHandle);
		nRet = 0;

	}
	else
	{
		debug("%s, open file false\n", __FUNCTION__);
		nRet = -1;
	}

	JVSP_ReleasePlayer(m_nPlayerHandle);
	m_nPlayerHandle = -1;

	return nRet;
}

int CSourceJovision::Init( char *pSzParam/*=0*/ )
{
	CSourceFilter::Init(pSzParam);

	if (!m_bInitSdk)
	{
		if (JVSP_InitSDK(true))
		{
			
			debug("%s, init sdk success\n", __FUNCTION__);
			m_bInitSdk = true;
		}
		else
		{
			debug("%s, init sdk false\n", __FUNCTION__);
			return -1;
		}
	}

	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	pPinOut->Clear();

	pPinOut->CreateBuffer<CVideoFramePinBufer>(4);


	OpenPlay();
	if (m_nPlayerHandle != -1)
	{
		int nTries = 0;
		JVSP_Play(m_nPlayerHandle);

		JVSP_SetFPS(m_nPlayerHandle, true, 200);

		JVSP_StepForward(m_nPlayerHandle);

		int nCurFrame = 0;
		nCurFrame = JVSP_GetCurFrameNum(m_nPlayerHandle);
		while(nTries--)
		{
			JVSP_StepForward(m_nPlayerHandle);

			nCurFrame = JVSP_GetCurFrameNum(m_nPlayerHandle);

			debug("%s: progress: %d/%d\n", __FUNCTION__, nCurFrame, m_nTotalFrame);
			Sleep(100);
		}

	}

	return 0;
}
//跨线程使用 解码失败。。。
int CSourceJovision::DoFilter()
{
	if (m_nPlayerHandle == -1)
	{
		OpenPlay();
	}
	if (m_nPlayerHandle == -1)
		return -1;

	int nTries = 10;
	int nCurFrame = 0;
 

	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;
	if (pPinOut->GetFreeBufferCount() <= 0 )
	{
		return CSourceFilter::DoFilter();
	};

	CFilterPinBuffer  *pPinBuffer = NULL;
	pPinOut->GetFreeBuffer(&pPinBuffer);
	if (!pPinBuffer)
		return -1;

	CVideoFramePinBufer *pVideoFrame = 0;
	pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pPinBuffer);
	if (!pVideoFrame)
	{
		pPinOut->PutFreeBuffer(pPinBuffer);
		return -3;
	}
	pPinOut->SetAutoFreeMemory(0);
	// copy yuv to video frame;
	pVideoFrame->SetFrameSize(m_nW, m_nH);
	pVideoFrame->SetUsedSize(m_nW*m_nH*3);

	CBuffer *pBufferExt = pVideoFrame->GetExtData();
	if (pBufferExt->GetAllocSize() < sizeof(sFfmpegFrameInfo))
	{
		pBufferExt->SetAllocSize(sizeof(sFfmpegFrameInfo));
		pBufferExt->SetUsed(sizeof(sFfmpegFrameInfo));

	}


	int nFps = 25;
	
	nFps = 25*m_fSpeed;
	bool bFast = false;
	if (nFps > 27)
	{
		bFast = true;
	}
	if (m_fSpeed > 16)
	{
		m_nNeedSleep = 0;
	}
	else
	{
		m_nNeedSleep = 1000/nFps;
	}
	//JVSP_Play(m_nPlayerHandle);
	//该接口调用导致不解码。以及关闭卡死, 在 界面Init函数调用则可以
	//JVSP_SetFPS(m_nPlayerHandle, bFast, nFps);

	nCurFrame = JVSP_GetCurFrameNum(m_nPlayerHandle);
 
	while (m_nLastFrameNum == nCurFrame
		&& (nCurFrame < m_nTotalFrame)
		&& nTries--)
	{
		JVSP_StepForward(m_nPlayerHandle);
		Sleep(1);
		nCurFrame = JVSP_GetCurFrameNum(m_nPlayerHandle);
		//debug("%s: progress: %d/%d\n", __FUNCTION__, nCurFrame, m_nTotalFrame);
	}
	m_nLastFrameNum = nCurFrame;
	if (nTries <= 0)
	{
		debug("%s: play next frame time out\n", __FUNCTION__);
		pPinOut->PutFreeBuffer(pVideoFrame);
	}
	else{
		//

		m_s64CurrTime = nCurFrame*40;
		if (m_cbProgress)
		{
			m_cbProgress(m_fSpeed, m_s64CurrTime, m_s64TotalTime, m_pCbProgressUserData, m_pCbProgressReversed );
		}
#if 1
		//
		sFfmpegFrameInfo *pFfmpegFrameInfo = (sFfmpegFrameInfo*)pBufferExt->GetData();
		memset(pFfmpegFrameInfo, 0, sizeof(sFfmpegFrameInfo));
		//这个是毫秒，需要转换到 时间戳，时间戳 是 timebase 的一个计数

		pFfmpegFrameInfo->m_nTimeDen = 25;
		if (pFfmpegFrameInfo->m_nTimeDen <= 0)
			pFfmpegFrameInfo->m_nTimeDen = 25;
		pFfmpegFrameInfo->m_nTimeNum = 1;;
		pFfmpegFrameInfo->m_s64Pts = m_nLastFrameNum;//PlayM4_GetPlayedTime(pHkFile->m_nHkStreamHandle);
		//debug("%s, timestamp: time %d,->%d\n ", __FUNCTION__,  pFrameInfo->nStamp, pFfmpegFrameInfo->m_s64Pts);

		pFfmpegFrameInfo->m_videoInfo.m_nWidth = m_nW;
		pFfmpegFrameInfo->m_videoInfo.m_nHeight = m_nH;
		pFfmpegFrameInfo->m_s64Bts = 0;
		pFfmpegFrameInfo->m_videoInfo.m_nFps = 25;
		pBufferExt->SetUsed(sizeof(sFfmpegFrameInfo));

		
		m_pBufferDecode->SetAllocSize(m_nW*m_nH*4);
		m_pBufferDecode->SetUsed(m_nW*m_nH*4);
		unsigned char *pDecodeBuffer = m_pBufferDecode->GetData();

		JVSP_GetDecodedFrame(m_nPlayerHandle, pDecodeBuffer);
		//上下翻转图片
		
		unsigned char *pLineTmp = new unsigned char[m_nW*4];
		unsigned char *pLineLow, *pLineHig;
		for (int y=0; y<m_nH/2; y++)
		{
			pLineLow = pDecodeBuffer + y*m_nW*4;
			pLineHig = pDecodeBuffer + m_nW*(m_nH-y-2)*4;
			memcpy(pLineTmp, pLineLow, m_nW*4 );
			memcpy(pLineLow, pLineHig, m_nW*4 );
			memcpy(pLineHig, pLineTmp, m_nW*4 );
		}
		delete pLineTmp;
		
		//

		m_pFfmpegScale->Scale(pDecodeBuffer, m_nW, m_nH, AV_PIX_FMT_RGB32,  
			pVideoFrame->m_pY, m_nW, m_nH, AV_PIX_FMT_YUV420P );

		pVideoFrame->m_pV = pVideoFrame->m_pY + m_nW*m_nH;
		pVideoFrame->m_pU = pVideoFrame->m_pY + m_nW*m_nH*5/4;
#endif

		pPinOut->PutPreparedBuffer(pVideoFrame);
	}
	

	return CSourceFilter::DoFilter();
}

int CSourceJovision::OpenPlay()
{
	if (m_nPlayerHandle != -1)
	{
		return -1;
	}

	//false: yuv  ;true: rgb32
	int nPlayer = JVSP_CreatePlayer( true );
	if (nPlayer == -1)
	{
		debug("%s: create player false\n", __FUNCTION__);
		return -2;
	}

	m_nPlayerHandle = nPlayer;

	char *pSource = (char*)GetParam("source");
	if (!pSource || strlen(pSource) <= 0)
		return -2;

	if (JVSP_OpenFile(m_nPlayerHandle, pSource))
	{
		debug("%s, open file success\n", __FUNCTION__);
		JVSP_Play(m_nPlayerHandle);
		//JVSP_StepForward(m_nPlayerHandle);
		m_nLastFrameNum = JVSP_GetCurFrameNum(m_nPlayerHandle);

		m_nTotalFrame = JVSP_GetTotalFrames(m_nPlayerHandle);

		JVS_HEADER header;
		JVSP_GetHeaderData(m_nPlayerHandle, &header);
		m_nH = header.i_frame_height;
		m_nW = header.i_frame_width;

		m_s64TotalTime = m_nTotalFrame/25*1000;
	}
	else
	{
		return -3;
	}

	return 0;
}
