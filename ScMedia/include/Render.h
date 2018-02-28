#pragma once
#ifndef  RENDER_H

#define RENDER_H
#include "MediaFilter.h"
//
#define MAX_TEXT_ITEMS 32
//
#ifndef WIN32
#define HWND void*
#else
#include <Windows.h>
#endif

class C3dItemText2d;
class MEDIASDK_API CRender :
	public CMediaFilter
{
public:
	CRender(void);
	virtual ~CRender(void);
	virtual int SetPlayBuffer(int nCount);
	virtual int Open(char *pSzParam=0);
	virtual int Open(HWND hWnd, int nPlayBuffer=5);
	virtual int InitRender(HWND hWnd);
	virtual int DoFilter();
	virtual int SetPlaySpeed(float fSpeed);
	virtual int SetFpsBase(int nFps);

	virtual int SetOsdText(int nIndex, char *pText);
	virtual int SetOsdPos(int nIndex, int nX, int nY);
	virtual int SetOsdFont(int nIndex, char *pSzName, int nSize, long lColor);
	virtual int SetOsdShow(int nIndex, bool bShowOrHide);
	virtual int SnapShot(char *pSaveFile);
	virtual int SnapShot(unsigned char *pBuffer, int &nW, int &nH);

	virtual int SetVisible(bool bVisible);
	virtual bool IsVisible();
	//播放伴音控制，有的没办法控制，所以需要在这里控制伴音
	virtual int PlayAudio();
	virtual int StopAudio();
	virtual int Refresh();
	virtual int HitTest(float fX, float fY);

	virtual int64 GetPts();
	//获取 当前播放的  时间戳 
	//virtual int64_t GetPlayTimeStamp();
protected:
	HWND m_hWndShow;
#ifdef WIN32
	C3dItemText2d *m_p3dItemTextTimeOsd;
#else
	void          *m_p3dItemTextTimeOsd;
#endif
	int m_nPlayBuffer;
	//
	float m_fPlaySpeed;
	//
	int   m_nFps;
	bool  m_bPlayAudio;
	bool  m_bVisible;
	int64		m_s64Pts;
};

#endif
