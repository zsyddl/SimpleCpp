
#include "StdAfx.h"
#include "QtGlRender.h"
#ifdef WIN32
#include <MMSystem.h>
#endif
#include "VideoFramePinBuffer.h"
#include "MediaSdkDef.h"
#include <stdio.h>


#ifdef QT_OPENGL
#include <QHBoxLayout>

#define  TIMER_PRECISION 10
#define STAT_FRAME 200
CQtGlRender::CQtGlRender(void)
:m_pGlWidget(0)
{
}

CQtGlRender::~CQtGlRender(void)
{
	QWidget *pParent = m_pGlWidget->parentWidget();
	if (pParent)
	{
		QHBoxLayout *pLayout = (QHBoxLayout*)pParent->layout();
		pLayout->removeWidget(m_pGlWidget);
		m_pGlWidget->hide();
		delete m_pGlWidget;
	}
}

int CQtGlRender::Open( HWND pParent, int nPlayBuffer/*=5*/ )
{
	if (m_pGlWidget)
		return 1;
	if (!pParent)
		return -1;
	
	m_pGlWidget = new CGlWindow((QWidget*)pParent);
	QLayout *pOldLayout = ((QWidget*)pParent)->layout();
	if (pOldLayout)
	{
		delete pOldLayout;
	}	
	QHBoxLayout *pLayout = new QHBoxLayout();
	pLayout->setContentsMargins(0, 0, 0 ,0);
	pLayout->addWidget(m_pGlWidget);
	m_pGlWidget->show();

	((QWidget*)pParent)->setLayout(pLayout);

	return CRender::Open(0, nPlayBuffer);
}

int CQtGlRender::DoFilter()
{
	
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
	nTimePerFrameNow = fScale*nTimePerFrameNow;
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
	bWndVisible = m_pGlWidget->isVisible();
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
		
		m_pGlWidget->SetPicture( pVideoFrame->m_pY, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight);
	/*	if (!m_p3dItemImage)
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
			m_p3dItemImage->SetImageData((char*)pVideoFrame->m_pY, pVideoFrame->m_nWidth, pVideoFrame->m_nHeight, ImgPF_YV420);
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
					//为什么 内存泄露？
					if (m_p3dItemTextTimeOsd)
						m_p3dItemTextTimeOsd->SetText(szOsdTime);
				}
			}
			//
			m_p3dGraphics->Refresh();
		}*/
	}	
	else{
		//将数据 直接放回 不需要处理

		//
	}
	//放回 到 pin in 的 free 数据队列
	pPinIn->PutFreeBuffer(pBufferInPapared);

	return 0;
}


#endif
