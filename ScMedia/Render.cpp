#include "StdAfx.h"
#include "Render.h"
#include "VideoFramePinBuffer.h"
#include <math.h>
#include <stdio.h>
#include "D3dPresenter.h"

#ifndef QT_OPENGL
#include "3dItemText2d.h"
#endif

CRender::CRender()
: m_hWndShow(0)
#ifndef QT_OPENGL
, m_p3dItemTextTimeOsd(0)
#endif
, m_nPlayBuffer(10)
, m_fPlaySpeed(1.0)
, m_nFps(25)
, m_bPlayAudio(false)
, m_bVisible(true)
{

	strcpy(m_szKind, FILTER_KIND_RENDER);
	strcpy(m_szName, FILTER_KIND_RENDER);
	strcpy(m_szType, FILTER_KIND_RENDER);
}

CRender::~CRender(void)
{
	// 	for (int i=0; i<m_lst3dItemText2d.size(); i++)
	// 	{
	// 		C3dItemText2d *p3dItemText2 = m_lst3dItemText2d.at(i);
	// 		if (p3dItemText2)
	// 			delete p3dItemText2;
	// 	}
	// 	m_lst3dItemText2d.clear();
	// 
#ifndef QT_OPENGL
	if (m_p3dItemTextTimeOsd)
	{
		delete m_p3dItemTextTimeOsd;
		m_p3dItemTextTimeOsd = NULL;
	}
#endif


}
int CRender::SetPlayBuffer(int nCount)
{
	m_nPlayBuffer = nCount;

	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return -1;

	pPinIn->ResizeBuffer<CVideoFramePinBufer>(nCount);

	return 0;
}
int CRender::InitRender(HWND hWnd)
{
	m_hWndShow = hWnd;
	return 0;
}
int CRender::SetFpsBase(int nFps)
{
	m_nFps = nFps;
	return 0;
}
int CRender::SetPlaySpeed(float fSpeed)
{
    if ( fSpeed<0.1 )
		fSpeed = 0.125;

	m_fPlaySpeed = fSpeed;

	return 0;
}
/*
filter init
*/
int CRender::Open(HWND hWnd, int nPlayBuffer)
{
	CMediaFilter::Open();
	//创建 buffer
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return -1;
	m_nPlayBuffer = nPlayBuffer;
	pPinIn->Clear();
	pPinIn->CreateBuffer<CVideoFramePinBufer>(m_nPlayBuffer);
	m_hWndShow = hWnd;
	pPinIn->SetAutoFreeMemory(0);

	return 0;
}
int CRender::Open(char *pSzParam)
{

	CMediaFilter::Open(pSzParam);
	//创建 buffer
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return -1;
	//m_nPlayBuffer = nPlayBuffer;
	pPinIn->Clear();
	pPinIn->CreateBuffer<CVideoFramePinBufer>(m_nPlayBuffer);
	//m_hWndShow = hWnd;
	pPinIn->SetAutoFreeMemory(0);

	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (pPinOut)
	{
		pPinOut->Clear();
		pPinOut->CreateBuffer<CVideoFramePinBufer>(2);
	}

	return 0;
}
int CRender::HitTest(float fX, float fY)
{

	return 0;
}
int CRender::DoFilter()
{

	//取 一个 pinIn 的 papared 数据
	/*CFilterPinBuffer *pBufferInPapared = 0;
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	pPinIn->GetPreparedBuffer(&pBufferInPapared);
	//渲染。。。
	if (pBufferInPapared)
	{
		debug("%s, render a frame\n", __FUNCTION__);
		//

		//放回 到 pin in 的 free 数据队列
		pPinIn->PutFreeBuffer(pBufferInPapared);
	}
	*/

	return CMediaFilter::DoFilter();
}
int CRender::SetOsdText(int nIndex, char *pText)
{
	if (nIndex<=0 || nIndex > MAX_TEXT_ITEMS)
	{
		return -1;
	}

	return 0;
}
int CRender::SetOsdShow(int nIndex, bool bShowOrHide)
{
	if (nIndex<=0 || nIndex > MAX_TEXT_ITEMS)
	{
		return -1;
	}

	return 0;
}
int CRender::SetOsdPos(int nIndex, int nX, int nY)
{

	return 0;
}
int CRender::SnapShot(char *pSaveFile)
{
	return 0;
}

int CRender::SnapShot(unsigned char *pBuffer, int &nW, int &nH)
{

	return 0;
}


int CRender::SetOsdFont( int nIndex, char *pSzName, int nSize, long lColor )
{
	
	return 0;
}

int CRender::PlayAudio()
{
	m_bPlayAudio = true;
	return 0;
}


int CRender::StopAudio()
{

	m_bPlayAudio = false;
	return 0;
}
int CRender::Refresh()
{

	return 0;
}
 int CRender::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
	return 0;
}
bool CRender::IsVisible()
{
	return m_bVisible;
}
int64 CRender::GetPts()
{
	return m_s64Pts;
}
