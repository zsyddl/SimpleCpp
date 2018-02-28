#include "StdAfx.h"
#include "SmartRender.h"
#include "Thread.h"
#include "Mutex.h"
#include "VideoFramePinBuffer.h"
#include "3dGraphics.h"
#include "3dItemImage.h"
#include "3dItemText2d.h"
#include "PackageParser.h"
#include "Buffer.h"
#include <MMSystem.h>
#include "MediaSdkDef.h"
#include "FfmpegCodec.h"
#include "FileBuffer.h"
#include "FilterPinBuffer.h"

#include "FilterManager.h"

using namespace SimpleCpp;

/*#include "Dwt.h"*/

#pragma  comment(lib, "winmm.lib");

using namespace SimpleCpp;

CSmartRender g_regSmartRender;
CSmartRender::CSmartRender(void)
: m_pThreadRender(0)
/*, m_pDDrawer(0)*/
, m_p3dGraphics(0)
, m_p3dItemImage(0)
, m_nTimeLastFrame(0)
, m_nTimePerFrame(0)
, m_nTimeStatBegin(0)
, m_nFrameCount(0)
, m_p3dItemLogo(NULL)
#ifdef GRAPH_DDRAW
, m_pDDraw(0)
#endif

, m_pLastVideoFrame(NULL)
/*, m_pProcessVideoFrame(NULL)*/
, m_pFilterPinPlayPrevFrame(NULL)
, m_nVideoHeight(0)
, m_nVideoWidth(0)
, m_nDropFrames(0)
, m_pMutexBufferPool(0)

{
#ifdef GRAPH_DDRAW
	m_pDDraw = new CDirectDrawer();
#endif
	m_lst3dItemText2d.resize(MAX_TEXT_ITEMS);
	
	strcpy(m_szName, "smart render");
	strcpy(m_szType, "smart render");
	CFilterManager::RegistFilter(&g_regSmartRender);

	m_pMutexBufferPool = new CMutex;
}

CSmartRender::~CSmartRender(void)
{
#ifdef GRAPH_DDRAW
	if (m_pDDraw)
	{
		delete m_pDDraw;
		m_pDDraw = NULL;
	}
#endif
	m_state = eFilterState_Stop;
	if (m_pThreadRender)
	{
		m_pThreadRender->Stop();
		delete m_pThreadRender;
		m_pThreadRender = NULL;
	}
	if (m_p3dItemImage)
	{
		//m_p3dGraphics->AppendItem(m_p3dItemImage);
		delete m_p3dItemImage;
		m_p3dItemImage = NULL;
	}
	if (m_p3dItemLogo)
	{
		m_p3dGraphics->RemoveItem(m_p3dItemLogo);
		delete m_p3dItemLogo;
		m_p3dItemLogo = NULL;
	}
	if (m_p3dGraphics)
	{
		delete m_p3dGraphics;
		m_p3dGraphics = NULL;
	}
	for (int i=0; i<m_lst3dItemText2d.size(); i++)
	{
		if (m_lst3dItemText2d.at(i) != NULL)
		{
			delete m_lst3dItemText2d.at(i);
		}
	}
	m_lst3dItemText2d.clear();

	//为什么不能 在 基类 删除？, 为什么基类 不包含 该头文件 可以编译通过，确不能正确delete ？
	if (m_p3dItemTextTimeOsd)
	{
		delete m_p3dItemTextTimeOsd;
		m_p3dItemTextTimeOsd = NULL;
	}
	 

	if (m_pFilterPinPlayPrevFrame)
	{
		delete m_pFilterPinPlayPrevFrame;
		m_pFilterPinPlayPrevFrame = NULL;
	}

	if (m_pMutex3DGraphics)
	{
		delete m_pMutex3DGraphics;
		m_pMutex3DGraphics = NULL;
	}

	if (m_pMutexBufferPool)
	{
		delete m_pMutexBufferPool;
	}
}
int CSmartRender::Open(HWND hWnd, int nPlayBuffer)
{
	CRender::Open(hWnd, nPlayBuffer);
	
	m_bpVideoFrames.Create(0);

	int nRet = 0;
#ifdef GRAPH_DDRAW
	if (m_pDDraw)
	{
		delete m_pDDraw;
	}
 	m_pDDraw = new CDirectDrawer();
 	bool bRt = m_pDDraw->InitDirectDraw(hWnd, 20, false, NULL, 0, 1);
	
#else
	if (!m_pFilterPinPlayPrevFrame)
	{
		m_pFilterPinPlayPrevFrame = new CFilterPin(NULL);
		m_pFilterPinPlayPrevFrame->Init(eFilterPinIOType::eFPT_Out, 0);
	}
	//nRt = (bRt==true?0:-1);
	if (m_p3dGraphics)
	{
		delete m_p3dGraphics;
	}

	// z这里 dx 和QWebView冲突。同时使用会崩溃，或者界面无反应
	m_p3dGraphics = new C3dGraphics();
	m_p3dGraphics->Create(hWnd);
	
	m_pMutex3DGraphics = new CMutex();

// 	m_p3dItemImage = new C3dItemImage(m_p3dGraphics);
// 	m_p3dItemImage->Create(1, 1, 1);
// 	m_p3dGraphics->AppendItem(m_p3dItemImage);
/*	C3dItemText2d *p3dItemText2 = new C3dItemText2d(m_p3dGraphics);
	m_lst3dItemText2d.push_back(p3dItemText2);
	p3dItemText2->Create(0, 0.0, 0);
	m_p3dGraphics->AppendItem(p3dItemText2);

	p3dItemText2 = new C3dItemText2d(m_p3dGraphics);
	m_lst3dItemText2d.push_back(p3dItemText2);
	p3dItemText2->Create(0, 0.1, 0);
	m_p3dGraphics->AppendItem(p3dItemText2);
	*/
 	m_p3dItemTextTimeOsd = new C3dItemText2d(m_p3dGraphics);
 	m_p3dItemTextTimeOsd->Create(0, 0, 0);
	//m_p3dItemTextTimeOsd->SetText("osd time:");
	//m_p3dItemTextTimeOsd->SetPos(1.8, 1.8, 0);
 	m_p3dGraphics->AppendItem(m_p3dItemTextTimeOsd);
	
	m_p3dGraphics->Refresh();
#endif


	m_pThreadRender = new CThread();

	nRet = m_pThreadRender->Create(RenderProc, this);

	return nRet;
}

int CSmartRender::Open( char *pParam, bool bHasThread/*=false*/ )
{
	//
	CBaseFilter::Open(pParam);
	HWND hWnd(0);
	int nPlayBuffer;
	const char *pTmp = GetParam("DisplayHandle");
	if (!pTmp)
		return -1;
	string strTmp = pTmp;
	int nStart = strTmp.find("0x");
	if (nStart >= 0)
	{
		strTmp.replace(nStart, 2, "");
	}
	sscanf(strTmp.c_str(), "%p", &hWnd);

	pTmp = GetParam("PlayBuffer");
	if (!pTmp)
		return -1;
	sscanf(pTmp, "%d", &nPlayBuffer);
	//
	CRender::Open(hWnd, nPlayBuffer);
	
	m_bpVideoFrames.Create(0);

	int nRet = 0;
#ifdef GRAPH_DDRAW
	if (m_pDDraw)
	{
		delete m_pDDraw;
	}
 	m_pDDraw = new CDirectDrawer();
 	bool bRt = m_pDDraw->InitDirectDraw(hWnd, 20, false, NULL, 0, 1);
	
#else
	if (!m_pFilterPinPlayPrevFrame)
	{
		m_pFilterPinPlayPrevFrame = new CFilterPin(NULL);
		m_pFilterPinPlayPrevFrame->Init(eFilterPinIOType::eFPT_Out, 0);
	}
	//nRt = (bRt==true?0:-1);
	if (m_p3dGraphics)
	{
		delete m_p3dGraphics;
	}

	// z这里 dx 和QWebView冲突。同时使用会崩溃，或者界面无反应
	// 把 dx 初始化 设置为 FPU方式 
	m_p3dGraphics = new C3dGraphics();
	m_p3dGraphics->Create(hWnd);
	
	m_pMutex3DGraphics = new CMutex();

// 	m_p3dItemImage = new C3dItemImage(m_p3dGraphics);
// 	m_p3dItemImage->Create(1, 1, 1);
// 	m_p3dGraphics->AppendItem(m_p3dItemImage);
/*	C3dItemText2d *p3dItemText2 = new C3dItemText2d(m_p3dGraphics);
	m_lst3dItemText2d.push_back(p3dItemText2);
	p3dItemText2->Create(0, 0.0, 0);
	m_p3dGraphics->AppendItem(p3dItemText2);

	p3dItemText2 = new C3dItemText2d(m_p3dGraphics);
	m_lst3dItemText2d.push_back(p3dItemText2);
	p3dItemText2->Create(0, 0.1, 0);
	m_p3dGraphics->AppendItem(p3dItemText2);
	*/
 	m_p3dItemTextTimeOsd = new C3dItemText2d(m_p3dGraphics);
 	m_p3dItemTextTimeOsd->Create(0, 0, 0);
 	m_p3dGraphics->AppendItem(m_p3dItemTextTimeOsd);
	
	m_p3dGraphics->Refresh();
#endif


	m_pThreadRender = new CThread();

	nRet = m_pThreadRender->Create(RenderProc, this);
	return 0;
}

//这个线程中 不断 获取 输入 pin的 图像数据，并 显示播放
int CSmartRender::RenderProc(void *pParam)
{
	CSmartRender *pRender = (CSmartRender*)pParam;
	if (pRender)
	{
		pRender->RenderProc();
	}
	return 0;
}
int CSmartRender::RenderProc()
{

	while (m_state != eFilterState_Stop
		&& m_state != eFilterState_ToStop)
	{
		if (m_playMode == eFPM_PlayPrevFrame)
		{//放到 do filter里面
			//TryGetVideoFrame();
		}
		if (m_state == eFilterState_Pause)
		{//暂停
			Sleep(10);
			continue;;
		}
		//渲染视频图片
		CFilterPin *pPinIn = m_lstPinIn.at(0);
		if (!pPinIn)
		{
			Sleep(10);
			continue;;
		}
		if (m_nTimePerFrame<=0)
		{
			m_nTimePerFrame = 1000.0/m_nFps;
		}
		//计算 当前实际 的一帧时间
		int nFrameCount = 0;
		long nTimePerFrameNow = m_nTimePerFrame;
		nFrameCount = pPinIn->GetPreparedBufferConut();
		if (nFrameCount <= 0)
		{
			Sleep(10);
			continue;;
		}
		int  nNicePlayBuffer = m_nPlayBuffer/2;
		//计算 当前实际 的一帧时间, 根据帧的多少进行调整

		int nFrameDiff = /*abs*/(nFrameCount - nNicePlayBuffer);// -nNicePlayBuffer ~  nNicePlayBuffer;
		// 太多， 就减少播放一帧的时间，太少 就 增加播放一帧的时间
		float fScale = 1.0;
		if (nFrameDiff <= nNicePlayBuffer/5)
		{//增加播放时间
			fScale =  (float)nNicePlayBuffer/(nFrameCount+nNicePlayBuffer/5);//1+(float)nFrameDiff/nNicePlayBuffer;
		}
		else if (nFrameDiff > nNicePlayBuffer/5)
		{//减少播放时间
			fScale = (float)nNicePlayBuffer/(nFrameCount-nNicePlayBuffer/5)*0.8;
		}
		else
		{

		}
		nTimePerFrameNow = fScale*nTimePerFrameNow;
		//debug("stat. time per frame: %d, cal time per frame: %d, frame: %d:%d\n", m_nTimePerFrame, nTimePerFrameNow, nNicePlayBuffer, nFrameCount );

		// 计算当前时间是否可以播放
		long nTimeNow = timeGetTime();
		long nTimeElapse = nTimeNow - m_nTimeLastFrame;
		nTimePerFrameNow = 0;
		if (nTimeElapse < nTimePerFrameNow-TIMER_PRECISION/2)
		{//时间不到
			Sleep(10);
			continue;;
		}
		

		//取 一个 pinIn 的 papared 数据
		CFilterPinBuffer *pBufferInPapared = 0;
		
		pPinIn->GetPreparedBuffer(&pBufferInPapared);
		//如果是 > 2 倍速 需要丢弃帧，不然速度上不去
		while (m_nDropFrames > 0 && pBufferInPapared)
		{
			pPinIn->PutFreeBuffer(pBufferInPapared);
			pBufferInPapared = NULL;
			pPinIn->GetPreparedBuffer(&pBufferInPapared);

			m_nDropFrames--;
		}
		if (!pBufferInPapared)
			continue;

		CBuffer *pExBuffer = pBufferInPapared->GetExtData();
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pExBuffer->GetData();
		if (pFrameInfo)
		{
			m_s64Pts = pFrameInfo->m_s64Pts;
		}
		//debug("%s, play, pts : %I64d\n", __FUNCTION__, m_s64Pts);
		//
		//渲染。。。
		CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pBufferInPapared);
		bool bWndVisible = false;
		bWndVisible = IsWindowVisible(m_hWndShow);
		if ( /* bWndVisible */
			/*&&*/ pVideoFrame 
			&& pVideoFrame->m_nHeight > 0 
			&& pVideoFrame->m_nWidth>0
			/*&& m_bVisible*/)
		{
			if (m_nVideoHeight != pVideoFrame->m_nHeight
				|| m_nVideoWidth != pVideoFrame->m_nWidth)
			{
				debug("%s: video size changed: [%dx%d]->[%dx%d]\n", 
					__FUNCTION__, m_nVideoWidth, m_nVideoHeight,
					pVideoFrame->m_nWidth, pVideoFrame->m_nHeight);
			}
			m_nVideoHeight = pVideoFrame->m_nHeight;
			m_nVideoWidth = pVideoFrame->m_nWidth;
			//VideoSummary(pVideoFrame, m_pLastVideoFrame);
			//debug("show frame \n");
			//时间到， 可以播放
			//统计帧平均时间
			m_nFrameCount++;
			if (m_nFrameCount>STAT_FRAME && m_nTimeStatBegin>0)
			{
				m_nTimePerFrame = (nTimeNow-m_nTimeStatBegin)/m_nFrameCount;
				if (m_nTimePerFrame > 500)
				{
					m_nTimePerFrame = 500;
				}
				m_nTimeStatBegin = nTimeNow;
				m_nFrameCount = 0;
			}
			m_nTimeLastFrame = nTimeNow;
			//debug("%s, render a frame, time elapse [%d]:need time[%d], nice buffers[%d], now[%d] \n", __FUNCTION__, nTimeElapse, nTimePerFrameNow, nNicePlayBuffer, nFrameCount);
#ifdef GRAPH_DDRAW
			if (m_pDDraw)
			{//显示图片
				PRESENTATION_ITEM pi ={0};

				pi.Y = pVideoFrame->m_pY;
				pi.U = pVideoFrame->m_pU;
				pi.V = pVideoFrame->m_pV;
				pi.i_data_len2 = pVideoFrame->m_nWidth;
				pi.i_other_data0 = 1;//(frame.format == yuv_420) ? 1 : 2;

				pi.i_data_len0 = pVideoFrame->m_nWidth;// frame.width;
				pi.i_data_len1 = pVideoFrame->m_nHeight;//frame.height;

				m_pDDraw->BlitBitmap(&pi);
			}
#else
			if (!m_p3dItemImage)
			{
				m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight, e3dPF_YV420 );
				m_p3dItemImage->Create(0, 0, 0);
				m_p3dItemImage->SetSize(2, 2, 0.01);
				m_p3dGraphics->AddItem2Head(m_p3dItemImage);
				/*
				m_p3dItemLogo = new C3dItemImage(m_p3dGraphics, 128, 128, e3dPF_RGBA );;
				m_p3dItemLogo->Create(0, 0, 0);
				m_p3dItemLogo->SetSize(1, 1, 0.01);
				m_p3dItemLogo->SetAlpha(0.5);
				m_p3dGraphics->AppendItem(m_p3dItemLogo);*/
			}
			if (m_p3dItemImage && m_p3dGraphics)
			{
				//m_pMutex3DGraphics->Lock();
				m_p3dGraphics->SetGraphicsSize(pVideoFrame->m_nWidth/*<<1*/, pVideoFrame->m_nHeight/*<<1*/);

				if (m_p3dItemImage->m_nTextureXPix != pVideoFrame->m_nWidth
					|| m_p3dItemImage->m_nTextureYPix != pVideoFrame->m_nHeight)
				{
					m_p3dGraphics->RemoveItem(m_p3dItemImage);
					delete m_p3dItemImage;
					m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight , e3dPF_YV420 );
					m_p3dItemImage->Create(0, 0, 0);                                                                                                                 
					m_p3dItemImage->SetSize(2, 2, 0.01);
					m_p3dGraphics->AddItem2Head(m_p3dItemImage);
				}
				m_p3dItemImage->SetImageData((char*)pVideoFrame->m_pY, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight, e3dPF_YV420);
				//显示 时间sod
				sAvDataEnd *pAvDataEnd = 0;
				CBuffer *pExtData = pVideoFrame->GetExtData();
				if ( pExtData && pExtData->GetUsedSize() == sizeof(sAvDataEnd))
				{
					pAvDataEnd = (sAvDataEnd *)pExtData->GetData();
					if (pAvDataEnd 
						&& pAvDataEnd->stOsdTime.wYear != 0
						&& pAvDataEnd->stOsdTime.wMonth != 0
						&& pAvDataEnd->stOsdTime.wDay != 0)
					{
						char szOsdTime[255];
						memset(szOsdTime, 0, sizeof(szOsdTime));
						sprintf(szOsdTime,"%d-%.2d-%.2d %.2d:%.2d:%.2d", pAvDataEnd->stOsdTime.wYear, pAvDataEnd->stOsdTime.wMonth, pAvDataEnd->stOsdTime.wDay,
							pAvDataEnd->stOsdTime.wHour, pAvDataEnd->stOsdTime.wMinute, pAvDataEnd->stOsdTime.wSecond);
						//debug("time osd: %s\n", szOsdTime);
						//
						if (m_p3dItemTextTimeOsd)
							m_p3dItemTextTimeOsd->SetText(szOsdTime);
					}
				}
				//
				if (m_bVisible && bWndVisible)
					m_p3dGraphics->Refresh();
				//m_pMutex3DGraphics->Unlock();
			}
#endif
			//计算需要丢弃的帧数
			if (m_fPlaySpeed >= 3 && m_nDropFrames==0)
			{
				m_nDropFrames = m_fPlaySpeed - 2;
			}
		}	
		else{
			//将数据 直接放回 不需要处理

			//
		}
		//放回 到 pin in 的 free 数据队列
		if (pBufferInPapared)
			pPinIn->PutFreeBuffer(pBufferInPapared);
		Sleep(0);

	}
	m_state = eFilterState_Stop;

	return 0;
}
int CSmartRender::SetPlaySpeed(float fSpeed)
{
	m_fPlaySpeed = fSpeed;

	return 0;
}
//取数据，播放，在另一个播放线程中完成播放
int CSmartRender::DoFilter()
{
	if (m_playMode == eFPM_PlayPrevFrame)
	{
		TryGetVideoFrame();
	}
	CRender::DoFilter();
	return 0;
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return -1;
	if (m_nTimePerFrame<=0)
	{
		m_nTimePerFrame = 1000.0/m_nFps;
	}
	//计算 当前实际 的一帧时间
	int nFrameCount = 0;
	long nTimePerFrameNow = m_nTimePerFrame;
	nFrameCount = pPinIn->GetPreparedBufferConut();
	if (nFrameCount <= 0)
		return -2;
	int  nNicePlayBuffer = m_nPlayBuffer/2;
	//计算 当前实际 的一帧时间, 根据帧的多少进行调整

	int nFrameDiff = /*abs*/(nFrameCount - nNicePlayBuffer);// -nNicePlayBuffer ~  nNicePlayBuffer;
	// 太多， 就减少播放一帧的时间，太少 就 增加播放一帧的时间
	float fScale = 1.0;
	if (nFrameDiff <= nNicePlayBuffer/5)
	{//增加播放时间
		fScale =  (float)nNicePlayBuffer/(nFrameCount+nNicePlayBuffer/5);//1+(float)nFrameDiff/nNicePlayBuffer;
	}
	else if (nFrameDiff > nNicePlayBuffer/5)
	{//减少播放时间
		fScale = (float)nNicePlayBuffer/(nFrameCount-nNicePlayBuffer/5)*0.8;
	}
	else
	{
	
	}
	nTimePerFrameNow = 40;//fScale*nTimePerFrameNow;
	//debug("stat. time per frame: %d, cal time per frame: %d, frame: %d:%d\n", m_nTimePerFrame, nTimePerFrameNow, nNicePlayBuffer, nFrameCount );

	// 计算当前时间是否可以播放
	long nTimeNow = timeGetTime();
	long nTimeElapse = nTimeNow - m_nTimeLastFrame;

	if (nTimeElapse < nTimePerFrameNow-TIMER_PRECISION/2)
	{//时间不到
		return -3;
	}


	//
	
	//取 一个 pinIn 的 papared 数据
	CFilterPinBuffer *pBufferInPapared = 0;
	
	pPinIn->GetPreparedBuffer(&pBufferInPapared);
	//渲染。。。
	CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pBufferInPapared);
	bool bWndVisible = false;
	bWndVisible = IsWindowVisible(m_hWndShow);
	if (bWndVisible 
		&& pVideoFrame 
		&& pVideoFrame->m_nHeight > 0 
		&& pVideoFrame->m_nWidth>0)
	{
		//时间到， 可以播放
		//统计帧平均时间
		m_nFrameCount++;
		if (m_nFrameCount>STAT_FRAME && m_nTimeStatBegin>0)
		{
			m_nTimePerFrame = (nTimeNow-m_nTimeStatBegin)/m_nFrameCount;
			if (m_nTimePerFrame > 500)
			{
				m_nTimePerFrame = 500;
			}
			m_nTimeStatBegin = nTimeNow;
			m_nFrameCount = 0;
		}
		m_nTimeLastFrame = nTimeNow;
		//debug("%s, render a frame, time elapse [%d]:need time[%d], nice buffers[%d], now[%d] \n", __FUNCTION__, nTimeElapse, nTimePerFrameNow, nNicePlayBuffer, nFrameCount);
#ifdef GRAPH_DDRAW
		if (m_pDDraw)
		{//显示图片
			PRESENTATION_ITEM pi ={0};

			pi.Y = pVideoFrame->m_pY;
			pi.U = pVideoFrame->m_pU;
			pi.V = pVideoFrame->m_pV;
			pi.i_data_len2 = pVideoFrame->m_nWidth;
			pi.i_other_data0 = 1;//(frame.format == yuv_420) ? 1 : 2;

			pi.i_data_len0 = pVideoFrame->m_nWidth;// frame.width;
			pi.i_data_len1 = pVideoFrame->m_nHeight;//frame.height;

			m_pDDraw->BlitBitmap(&pi);
		}
#else
		if (!m_p3dItemImage)
		{
			m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight );
			m_p3dItemImage->Create(0, 0, 0);
			m_p3dItemImage->SetSize(2, 2, 0.01);
			m_p3dGraphics->AddItem2Head(m_p3dItemImage);
		}
		if (m_p3dItemImage && m_p3dGraphics)
		{
			if (m_p3dItemImage->m_nTextureXPix != pVideoFrame->m_nWidth
				|| m_p3dItemImage->m_nTextureYPix != pVideoFrame->m_nHeight)
			{
				m_p3dGraphics->RemoveItem(m_p3dItemImage);
				delete m_p3dItemImage;
				m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight );
				m_p3dItemImage->Create(0, 0, 0);                                                                                                                 
				m_p3dItemImage->SetSize(2, 2, 0.01);
				m_p3dGraphics->AddItem2Head(m_p3dItemImage);
			}
			m_p3dItemImage->SetImageData((char*)pVideoFrame->m_pY, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight, e3dPF_YV420);
			//显示 时间sod
			sAvDataEnd *pAvDataEnd = 0;
			CBuffer *pExtData = pVideoFrame->GetExtData();
			if ( pExtData)
			{
				pAvDataEnd = (sAvDataEnd *)pExtData->GetData();
				if (pAvDataEnd 
					&& pAvDataEnd->stOsdTime.wYear != 0
					&& pAvDataEnd->stOsdTime.wMonth != 0
					&& pAvDataEnd->stOsdTime.wDay != 0)
				{
					char szOsdTime[255];
					memset(szOsdTime, 0, sizeof(szOsdTime));
					sprintf(szOsdTime,"%d-%.2d-%.2d %.2d:%.2d:%.2d", pAvDataEnd->stOsdTime.wYear, pAvDataEnd->stOsdTime.wMonth, pAvDataEnd->stOsdTime.wDay,
						 pAvDataEnd->stOsdTime.wHour, pAvDataEnd->stOsdTime.wMinute, pAvDataEnd->stOsdTime.wSecond);
					//debug("time osd: %s\n", szOsdTime);
					//
					if (m_p3dItemTextTimeOsd)
						m_p3dItemTextTimeOsd->SetText(szOsdTime);
				}
			}
			//
			m_p3dGraphics->Refresh();
		}
#endif
	}	
	else{
		//将数据 直接放回 不需要处理

		//
	}
	//放回 到 pin in 的 free 数据队列
	pPinIn->PutFreeBuffer(pBufferInPapared);

	return 0;
}
int CSmartRender::SetOsdText(int nIndex, char *pText)
{
	if (nIndex<0 || nIndex >= MAX_TEXT_ITEMS)
	{
		return -1;
	}
	if (m_lst3dItemText2d.at(nIndex) == NULL)
	{
		C3dItemText2d *p3dItemText = new C3dItemText2d(m_p3dGraphics);
		p3dItemText->Create(0, 0, 0);
		m_lst3dItemText2d.at(nIndex) = p3dItemText;
		m_p3dGraphics->AppendItem(p3dItemText);
		
	}
	m_lst3dItemText2d.at(nIndex)->SetText(pText);

	return 0;
}
int CSmartRender::SetOsdPos(int nIndex, int nX, int nY)
{
	
	return 0;
}
int CSmartRender::SnapShot(char *pSaveFile)
{
	int nRt  = 0;
	if (m_p3dGraphics)
	{
		nRt = m_p3dGraphics->SnapShot(pSaveFile);
	}
#ifdef GRAPH_DDRAW
	else if (m_pDDraw)
	{
		m_pDDraw->Snapshot(pSaveFile);
	}
#endif

	return nRt;
}
int CSmartRender::SnapShot(unsigned char *pBuffer, int &nW, int &nH)
{
	int nRt  = 0;
	if (m_p3dGraphics)
	{
		nRt = m_p3dGraphics->SnapShot(pBuffer, nW, nH);
	}
	return 0;
}
int CSmartRender::SnapShotVideoPicture(unsigned char *pBuffer, int &nW, int &nH)
{
	if (!m_p3dItemImage)
	{
		nW = 0;
		nH = 0;
		return -1;
	}
	m_p3dItemImage->GetImageData((char*)pBuffer, nW, nH);


	return 0;
}
int CSmartRender::SnapShotVideoPicture(char *pSaveFile)
{
	if (!m_p3dItemImage)
	{
		return -1;
	}
	m_p3dItemImage->GetImageData(pSaveFile);

	return 0;
}
int CSmartRender::Refresh()
{
	
	int nRt = 0;
	if (m_p3dGraphics)
	{
		//m_pMutex3DGraphics->Lock();
		nRt = m_p3dGraphics->Refresh();
		//m_pMutex3DGraphics->Unlock();
	}
#ifdef GRAPH_DDRAW
	else if (m_pDDraw)
	{
		//m_pDDraw->Refresh();
		m_pDDraw->DDrawOnPaint();
	}
#endif
	return nRt;
}
int CSmartRender::SetOsdFont( int nIndex, char *pSzName, int nSize, long lColor )
{
	if (nIndex<0 || nIndex >=m_lst3dItemText2d.size())
	{
		return -1;
	}
	C3dItemText2d *pTextItem = m_lst3dItemText2d.at(nIndex);
	if (!pTextItem)
		return 0;
	pTextItem->SetFont(pSzName, nSize, lColor);

	return 0;
}

int CSmartRender::SetOsdShow(int nIndex, bool bShowOrHide )
{
	if (nIndex<0 || nIndex >=m_lst3dItemText2d.size())
	{
		return -1;
	}
	C3dItemText2d *pTextItem = m_lst3dItemText2d.at(nIndex);
	if (!pTextItem)
		return 0;
	pTextItem->SetVisible(bShowOrHide);

	return 0;
}

int CSmartRender::Stop()
{
	if (m_state == eFilterState_Stop)
	{
		return -1;
	}
	m_state = eFilterState_ToStop;

	int nTries =0 ;
	while (m_state != eFilterState_Stop
		/*&& nTries < 100*/)
	{
		nTries++;
		Sleep(10);
	}
	if (nTries >= 100)
	{
		return -2;
	}

	return 0;
}

CMediaFilter * CSmartRender::CreateObject()
{
	return new CSmartRender();
}
int CSmartRender::Pause()
{
	//m_bpVideoFrames.ClearBusyBuffer();

	return CMediaFilter::Pause();
}
int CSmartRender::Resume()
{
	m_bpVideoFrames.ClearBusyBuffer();
	return CMediaFilter::Resume();
}
int CSmartRender::Flush()
{
	//只有在 刚进入 单帧倒放的时候 才 需要清空 缓存
	if (m_playMode != eFPM_PlayPrevFrame)
	{
		CFilterPin *pPinIn = m_lstPinIn.at(0);
		if (!pPinIn)
		{
			return 0;
		}
		int nFrameCount = pPinIn->GetPreparedBufferConut();
		if (nFrameCount <= 0)
		{
			return -1;;
		}
		//取 一个 pinIn 的 papared 数据
		CFilterPinBuffer *pBufferInPapared = 0;
		do{ 
			pPinIn->GetPreparedBuffer(&pBufferInPapared);
			if (pBufferInPapared == NULL)
			{
				continue;
			}
			//放回 到 pin in 的 free 数据队列
			pPinIn->PutFreeBuffer(pBufferInPapared);
		}while(pPinIn->GetPreparedBufferConut()>0);

		m_playMode = eFPM_PlayPrevFrame;
	}
	return CRender::Flush();
}
int CSmartRender::PlayNextFrame()
{
	//debug("%s ++\n", __FUNCTION__);
	//清空 反向播放的缓存
	m_bpVideoFrames.ClearBusyBuffer();
	//渲染视频图片
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
	{
		return 0;
	}

	int nFrameCount = pPinIn->GetPreparedBufferConut();
	if (nFrameCount <= 0)
	{

		return -1;;
	}
	 
	//取 一个 pinIn 的 papared 数据
	CFilterPinBuffer *pBufferInPapared = 0;

	pPinIn->GetPreparedBuffer(&pBufferInPapared);
	if (pBufferInPapared == NULL)
		return -1;
	CBuffer *pExBuffer = pBufferInPapared->GetExtData();
	if (pExBuffer)
	{	
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pExBuffer->GetData();
		if (pFrameInfo)
		{
			m_s64Pts = pFrameInfo->m_s64Pts;
		}
	}
	//渲染。。。
	CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pBufferInPapared);
	//bool bWndVisible = false;
	//bWndVisible = IsWindowVisible(m_hWndShow);
	if (pVideoFrame 
		&& pVideoFrame->m_nHeight > 0 
		&& pVideoFrame->m_nWidth>0
		/*&& m_bVisible*/)
	{

		//VideoSummary(pVideoFrame, m_pLastVideoFrame);
				//debug("%s, render a frame, time elapse [%d]:need time[%d], nice buffers[%d], now[%d] \n", __FUNCTION__, nTimeElapse, nTimePerFrameNow, nNicePlayBuffer, nFrameCount);
#ifdef GRAPH_DDRAW
		if (m_pDDraw)
		{//显示图片
			PRESENTATION_ITEM pi ={0};

			pi.Y = pVideoFrame->m_pY;
			pi.U = pVideoFrame->m_pU;
			pi.V = pVideoFrame->m_pV;
			pi.i_data_len2 = pVideoFrame->m_nWidth;
			pi.i_other_data0 = 1;//(frame.format == yuv_420) ? 1 : 2;

			pi.i_data_len0 = pVideoFrame->m_nWidth;// frame.width;
			pi.i_data_len1 = pVideoFrame->m_nHeight;//frame.height;

			m_pDDraw->BlitBitmap(&pi);
		}
#else
		if (!m_p3dItemImage)
		{
			m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight );
			m_p3dItemImage->Create(0, 0, 0);
			m_p3dItemImage->SetSize(2, 2, 0.01);
			m_p3dGraphics->AddItem2Head(m_p3dItemImage);
		}
		if (m_p3dItemImage && m_p3dGraphics)
		{
			if (m_p3dItemImage->m_nTextureXPix != pVideoFrame->m_nWidth
				|| m_p3dItemImage->m_nTextureYPix != pVideoFrame->m_nHeight)
			{
				m_p3dGraphics->RemoveItem(m_p3dItemImage);
				delete m_p3dItemImage;
				m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight );
				m_p3dItemImage->Create(0, 0, 0);                                                                                                                 
				m_p3dItemImage->SetSize(2, 2, 0.01);
				m_p3dGraphics->AddItem2Head(m_p3dItemImage);
			}
			m_p3dItemImage->SetImageData((char*)pVideoFrame->m_pY, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight, e3dPF_YV420);
			//显示 时间sod
			sAvDataEnd *pAvDataEnd = 0;
			CBuffer *pExtData = pVideoFrame->GetExtData();
			if ( pExtData)
			{
				pAvDataEnd = (sAvDataEnd *)pExtData->GetData();
				if (pAvDataEnd 
					&& pAvDataEnd->stOsdTime.wYear != 0
					&& pAvDataEnd->stOsdTime.wMonth != 0
					&& pAvDataEnd->stOsdTime.wDay != 0)
				{
					char szOsdTime[255];
					memset(szOsdTime, 0, sizeof(szOsdTime));
					sprintf(szOsdTime,"%d-%.2d-%.2d %.2d:%.2d:%.2d", pAvDataEnd->stOsdTime.wYear, pAvDataEnd->stOsdTime.wMonth, pAvDataEnd->stOsdTime.wDay,
						pAvDataEnd->stOsdTime.wHour, pAvDataEnd->stOsdTime.wMinute, pAvDataEnd->stOsdTime.wSecond);
					//debug("time osd: %s\n", szOsdTime);
					//
					if (m_p3dItemTextTimeOsd)
						m_p3dItemTextTimeOsd->SetText(szOsdTime);
				}
			}
			//
			if (m_bVisible  )
				m_p3dGraphics->Refresh();
		}
#endif
	}	
	else{
		//将数据 直接放回 不需要处理

		//
	}
	//放回 到 pin in 的 free 数据队列
	pPinIn->PutFreeBuffer(pBufferInPapared);

	//debug("%s --, pts: %I64d\n", __FUNCTION__, m_s64Pts);
	return 0;
}
int CSmartRender::PlayPrevFrame(int64 s64PtsNow/*=0*/)
{
	return -1;
#define  FILE_BUFFER
#ifdef FILE_BUFFER
	;
	debug("%s, file buffer :%d\n", __FUNCTION__, m_bpVideoFrames.GetBusyBufferCount());
	CFileBuffer *pFileBuffer = m_bpVideoFrames.GetBusyBuffer(false);
	if (!pFileBuffer)
		return -1;

	int nW(0), nH(0);
	int nPos, nLen;
	int64 s64Pts = 0;
	CVideoFramePinBufer *pVideoFrame = 0;
	nPos = 0; nLen = 4;
	pFileBuffer->GetData(nPos, (char*)&nW, nLen);
	nPos += 4;
	pFileBuffer->GetData(nPos, (char*)&nH, nLen);
	nPos += 4; nLen = 8;
	pFileBuffer->GetData(nPos, (char*)&s64Pts, nLen);
	nPos += 8;
	if (s64Pts > m_s64Pts)
	{//时间戳异常
		goto ret;
	}
	debug("%s: %I64d\n", __FUNCTION__, s64Pts);
	m_s64Pts = s64Pts;

	pVideoFrame = new CVideoFramePinBufer();

	pVideoFrame->SetAllocSize(nW*nH*3);
	pVideoFrame->SetFrameSize(nW, nH);
	
	nLen = nW*nH*3/2;
	pFileBuffer->GetData(nPos, (char*)pVideoFrame->m_pY, nLen);
#else
	//渲染视频图片
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
	{
		return 0;
	}
	debug("%s ++, frames: %d\n", __FUNCTION__, pPinIn->GetPreparedBufferConut());

	int nFrameCount = pPinIn->GetPreparedBufferConut();
	if (nFrameCount <= 0)
	{

		return -1;;
	}
	//将所有 数据 取 到

	//取 一个 pinIn 的 papared 数据
	CFilterPinBuffer *pBufferInPapared = 0;

	pPinIn->GetPreparedBuffer(&pBufferInPapared, false);
	//渲染。。。
	CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pBufferInPapared);
#endif

	//bool bWndVisible = false;
	//bWndVisible = IsWindowVisible(m_hWndShow);
	if (pVideoFrame 
		&& pVideoFrame->m_nHeight > 0 
		&& pVideoFrame->m_nWidth>0)
	{
		//debug("%s, render a frame, time elapse [%d]:need time[%d], nice buffers[%d], now[%d] \n", __FUNCTION__, nTimeElapse, nTimePerFrameNow, nNicePlayBuffer, nFrameCount);
#ifdef GRAPH_DDRAW
		if (m_pDDraw)
		{//显示图片
			PRESENTATION_ITEM pi ={0};

			pi.Y = pVideoFrame->m_pY;
			pi.U = pVideoFrame->m_pU;
			pi.V = pVideoFrame->m_pV;
			pi.i_data_len2 = pVideoFrame->m_nWidth;
			pi.i_other_data0 = 1;//(frame.format == yuv_420) ? 1 : 2;

			pi.i_data_len0 = pVideoFrame->m_nWidth;// frame.width;
			pi.i_data_len1 = pVideoFrame->m_nHeight;//frame.height;

			m_pDDraw->BlitBitmap(&pi);
		}
#else
		if (!m_p3dItemImage)
		{
			m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight );
			m_p3dItemImage->Create(0, 0, 0);
			m_p3dItemImage->SetSize(2, 2, 0.01);
			m_p3dGraphics->AddItem2Head(m_p3dItemImage);
		}
		if (m_p3dItemImage && m_p3dGraphics)
		{
			if (m_p3dItemImage->m_nTextureXPix != pVideoFrame->m_nWidth
				|| m_p3dItemImage->m_nTextureYPix != pVideoFrame->m_nHeight)
			{
				m_p3dGraphics->RemoveItem(m_p3dItemImage);
				delete m_p3dItemImage;
				m_p3dItemImage = new C3dItemImage(m_p3dGraphics, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight );
				m_p3dItemImage->Create(0, 0, 0);                                                                                                                 
				m_p3dItemImage->SetSize(2, 2, 0.01);
				m_p3dGraphics->AddItem2Head(m_p3dItemImage);
			}
			m_p3dItemImage->SetImageData((char*)pVideoFrame->m_pY, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight, e3dPF_YV420);
			//显示 时间sod
			sAvDataEnd *pAvDataEnd = 0;
			CBuffer *pExtData = pVideoFrame->GetExtData();
			if ( pExtData)
			{
				pAvDataEnd = (sAvDataEnd *)pExtData->GetData();
				if (pAvDataEnd 
					&& pAvDataEnd->stOsdTime.wYear != 0
					&& pAvDataEnd->stOsdTime.wMonth != 0
					&& pAvDataEnd->stOsdTime.wDay != 0)
				{
					char szOsdTime[255];
					memset(szOsdTime, 0, sizeof(szOsdTime));
					sprintf(szOsdTime,"%d-%.2d-%.2d %.2d:%.2d:%.2d", pAvDataEnd->stOsdTime.wYear, pAvDataEnd->stOsdTime.wMonth, pAvDataEnd->stOsdTime.wDay,
						pAvDataEnd->stOsdTime.wHour, pAvDataEnd->stOsdTime.wMinute, pAvDataEnd->stOsdTime.wSecond);
					//debug("time osd: %s\n", szOsdTime);
					//
					if (m_p3dItemTextTimeOsd)
						m_p3dItemTextTimeOsd->SetText(szOsdTime);
				}
			}
			//
			m_p3dGraphics->Refresh();
		}
#endif
	}	
	else{
		//将数据 直接放回 不需要处理

		//
	}
	//放回 到 pin in 的 free 数据队列
#ifdef FILE_BUFFER
ret:
	if (pVideoFrame)
	{
		delete pVideoFrame;
	}
	if (pFileBuffer)
	{
		pFileBuffer->Clear();
		m_bpVideoFrames.PutFreeBuffer(pFileBuffer);
	}
#else
	pPinIn->PutFreeBuffer(pBufferInPapared);
#endif
	//debug("%s --\n", __FUNCTION__);
	return 0;
}

int CSmartRender::SetImage(int nLevel, uint8 *pImage, int nW, int nH
							, float fPosX, float fPosY
							, float fSizeX, float fSizeY)
{
	if (nLevel < 0 || nLevel >= m_lstUserImage.size())
	{
		return -1;
	}
	C3dItemImage *pItemImage(NULL);
 
	pItemImage = m_lstUserImage.at(nLevel);
	if (!pItemImage)
	{
		return -2;
	}
	pItemImage->SetImageData((char*)pImage, nW, nH);

	return 0;
}

int CSmartRender::AppendImage( uint8 *pImage, int nW, int nH, float fPosX, float fPosY, float fSizeX, float fSizeY )
{

	C3dItemImage *pItemImage(NULL);

	pItemImage = new C3dItemImage(m_p3dGraphics, nW, nH);
	m_lstUserImage.push_back(pItemImage);
	m_p3dGraphics->AppendItem(pItemImage);

	pItemImage->Create(fPosX, fPosY, 0);
	pItemImage->SetSize(fSizeX, fSizeY, 0.01);

 
	pItemImage->SetImageData((char*)pImage, nW, nH);

	return m_lstUserImage.size()-1;
}

C3dItemImage * CSmartRender::GetImage( int nIndex )
{
	if (nIndex < 0 || nIndex >= m_lstUserImage.size())
	{
		return NULL;
	}

	return m_lstUserImage.at(nIndex);
}

int CSmartRender::GetImageCount()
{

	return m_lstUserImage.size();
}

void CSmartRender::SetImageVisible( bool bVisible )
{
	for (int i=0; i<m_lstUserImage.size(); i++)
	{
		C3dItemImage *pItemImage = m_lstUserImage.at(i);
		if (pItemImage)
		{
			pItemImage->SetVisible(bVisible);
		}
	}
}

int CSmartRender::GetVideoWidth()
{
	return m_nVideoWidth;
}
int CSmartRender::GetVideoHeight()
{

	return m_nVideoHeight;
}

void CSmartRender::TryGetVideoFrame()
{
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
	{
		return ;
	}

	int nFrameCount = 0;
	
	nFrameCount = pPinIn->GetPreparedBufferConut();

	while (nFrameCount > 0)
	{

		CFilterPinBuffer *pPinInBuffer = 0;
		pPinIn->GetPreparedBuffer(&pPinInBuffer);


		CFileBuffer *pFileBuffer = m_bpVideoFrames.GetFreeBuffer();
		if (!pFileBuffer)
		{
			debug("%s: no file buffer\n", __FUNCTION__);
			return ;
		}
		CBuffer *pExBuffer = pPinInBuffer->GetExtData();
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pExBuffer->GetData();
		if (pFrameInfo)
		{
			//m_s64Pts = pFrameInfo->m_s64Pts;
		}

		CVideoFramePinBufer *pVideoBuffer = dynamic_cast<CVideoFramePinBufer*> (pPinInBuffer);
		pFileBuffer->Append((char*)&pVideoBuffer->m_nWidth, 4);
		pFileBuffer->Append((char*)&pVideoBuffer->m_nHeight, 4);
		pFileBuffer->Append((char*)&pFrameInfo->m_s64Pts,  8);
		pFileBuffer->Append((char*)pVideoBuffer->m_pY, pVideoBuffer->m_nWidth*pVideoBuffer->m_nHeight*3/2);

		m_bpVideoFrames.PutBusyBuffer(pFileBuffer);

		pPinIn->PutFreeBuffer(pPinInBuffer);

		nFrameCount = pPinIn->GetPreparedBufferConut();
	}

	//debug("%s, video frames:%d\n", __FUNCTION__, m_bpVideoFrames.GetBusyBufferCount());
}

/*
int CSmartRender::SnapShot( char *pBuffer, int &nLen )
{

 
	return 0;
}
*/

