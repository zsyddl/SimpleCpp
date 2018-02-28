#pragma once
#include "render.h"
#include <map>

/*
*接收 输入的 一组 图片名字 作为一帧，通过 ffmpeg解码， 然后显示
*
*/
namespace SimpleCpp{
	class CThread;
	class CMutex;
};
/*class SThread::CThread;*/
using namespace SimpleCpp;
using namespace std;

class C3dGraphics;
class C3dItemImage;
class C3dItemVideoSummaryPicture;
class CVideoSummaryRender :
	public CRender
{
public:
	CVideoSummaryRender(void);
	virtual ~CVideoSummaryRender(void);

	virtual int Open(HWND hWnd, int nPlayBuffer);
	//filter的执行 函数 
	virtual int DoFilter();

	int CopyAvFrame2Buffer(void *pAvFrame, char *pVideoFrame, int nW, int nH);

	virtual int Refresh();
	virtual int Stop();
	virtual int HitTest(float fX, float fY);
	//获取时间。 是时间。不是 pts 
	virtual int HitTest(float fX, float fY, int64 &s64Time );
	
	virtual int PlayNextFrame();
	virtual int PlayPrevFrame();

protected:
	virtual CMediaFilter *CreateObject();

	bool DecodeJpegFile(char *pSzFile, int &nW, int &nH,  char *pImg);

	static int __stdcall  RenderProc(void *pParam);
	int RenderProc();
	CThread *m_pThreadRender;
	
	void ClearObjectItem();
	CMutex *m_pMutexPicPbject;

	int m_nTimePerFrame;
	int m_nTimeBaseNum;
	int m_nTimeBaseDen;

	C3dGraphics  *m_p3dGraphics;

	char m_szBkgFile[256];
	C3dItemImage *m_pImgBkg;
	int64 m_s64StartTimestamp;
	list<C3dItemVideoSummaryPicture*> m_lstObjectPic;
};
