#pragma once

#include <vector>
#include <map>
#include "MediaSdkDef.h"
//#include "MediaFilter.h"
#include "FfmpegFormat.h"
#include "BasePipeLine.h"
#include "BaseFilter.h"
//#include "BaseStream.h"
/*
#ifdef WIN32
#ifdef VIVSPLAYSDK_EXPORTS
#define VIVSPLAYSDK_API __declspec(dllexport)
#else
#define VIVSPLAYSDK_API __declspec(dllimport)
#endif
#else
#define VIVSPLAYSDK_API
#endif

#define VIVSPLAYSDK_API  
*/

using namespace std;
using namespace SimpleCpp;

// eFS_Run = 0,
// eFS_ToStop,
// eFS_Stop,
// eFS_Pause,
typedef	enum _eStreamState{
	eSS_Run = 0,
	eSS_ToStop = 1,
	eSS_Stop ,
	eSS_Pause,
}eStreamState;

typedef	enum _eStreamControlState{
	eSPM_None = 0,
	eSPM_Play ,
	eSPM_PlayNextFrame,
	eSPM_PlayPrevFrame,
}eStreamPlayMode;

typedef enum _eStreamDataMode{
	eSDM_Push = 0,
	eSDM_Pull,
}eStreamDataMode;

typedef enum _sStreamSeekFlag{
	eSSF_IFrame = 0,
	eSSF_Any = AVSEEK_FLAG_ANY,
	eSSF_Backward = AVSEEK_FLAG_BACKWARD,
	eSSF_UntilDecode = 8,
	eSSF_UntilDisplay = 16,
}sStreamSeekFlag;

class CMediaFilter;
class CSourceFilter;
namespace SimpleCpp{
class CThread;
class CMutex;
}
 
using namespace SimpleCpp;
class MEDIASDK_API CMediaStream
	:public CBasePipeLine
{
public:
	CMediaStream(void);
	virtual ~CMediaStream(void);
	
	//void TestStdAllocator(vector<int> &lstValue ) ;
// 	virtual int Start();
// 	virtual int Stop();
// 	virtual int Pause();
	virtual int Resume();
	//移动到一个位置，但是不等待解码
	virtual int Seek(int64 s63Time, int nFlag=AVSEEK_FLAG_ANY);
	virtual int SeekToTimestamp(int64 s63Timestamp, int nFlag=AVSEEK_FLAG_ANY);
	virtual int PlayNextFrame();
	virtual int PlayPrevFrame();
	virtual int SetParam(char *pName, char *pValue);
	virtual int GetParam(char *pSzName, char *pSzValue);

	//控制命令
	virtual int Control(char *pSzName, char *pSzValue);
	
	virtual int SetPlaySpeed(float fSpeed);
	virtual float GetPlaySpeed();
	bool IsPaused();
	bool IsStoped();
	bool IsStreamEnd();

	//设置 图像需要显示的区域，将 设定的区域显示在整个窗口中。类似于缩放。
	virtual int SetShowRect(int nX, int nY, int nW, int nH);
	//在窗口需要刷新的时候调用该接口
	virtual int Refresh();
	//
	virtual int Snapshot(char *pSzSaveFile);
	//保存指定时间的一张截图。该接口会导致视频播放的位置移动
	//
	virtual int Snapshot(char *pSzSaveFile, int64 s64Time);
	virtual int Snapshot(char *pImage, int &nLen, int &nW, int &nH, int64 s64Time);
	//该接口只是对 内存 输入数据模式有效，对文件 等内部自动模式无效
	virtual int FillData(char *pData, int nLen);
	CMediaFilter *FindFilterByKind(char *szKind);

	//virtual int AppendFilter(CMediaFilter *pFilter);
	int SetProgressCb(FilterCB_Progress cbProgress, void *pUserData, void *pReversed);
	int SetFinishedCb(FilterCB_Finished cbFinished, void *pUserData, void *pReversed);
	int SetEventCb(FilterCB_Event cbEvent, void *pUserData, void *pReversed);
	CSourceFilter *GetSourceFilter();
protected:
	CMediaFilter *FindFilterByKindPriv(char *szKind);
	//向前播放一帧。 无锁
	int PlayNextFramePriv();
	int SetPlayStatePriv(int nState);
	int SetPlayModePriv(int nMode);
	//所有连接的 filter
	//vector<CMediaFilter *> m_lstFilter;
	//
	
	CThread         *m_pThrStream;
	static int __stdcall ThrStreamProc(void *pParam);
	virtual int ThrStreamProc();
	//eStreamState m_state;
	//
	bool m_bIsStreamEnd;
	//单帧播放等控制
	eStreamPlayMode m_playMode;
	int m_nIndexInGop; //在一个 I帧 之间的  序号。 用于控制单帧播放之后， 继续播放 
	//
	float m_fSpeed;
	//数据模式 推、拉
	eStreamDataMode m_dataMode;
	//
	FilterCB_Progress	m_cbProgress;
	void *m_pCbProgressUserData;
	void *m_pCbProgressReversed;
	FilterCB_Finished	m_cbFinished;
	void *m_pCbFinishedUserData;
	void *m_pCbFinishedReversed;
	FilterCB_Event		m_cbEvent;
	void *m_pCbEventUserData;
	void *m_pCbEventReversed;
};
//
class CPlayLiveStream{

};
class CPlayFileStream{

};
class CTransFileStream{

};
class CWriteFileStream{

};


