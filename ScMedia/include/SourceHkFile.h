#pragma once
#include "sourcefilter.h"

#define FILTER_SOURCE_HK_FILE "HkFile"
#define AvFormat_HK  "AvFormat_HK"

typedef struct  _FRAME_INFO FRAME_INFO;

#define HK_FRAME_READ_LEN (8*1024)

//Êä³ö yuv Êý¾Ý
class MEDIASDK_API CSourceHkFile :
	public CSourceFilter
{
public:
	CSourceHkFile(void);
	virtual ~CSourceHkFile(void);

	virtual int Open(char *pSzParam=0, bool bCreateThread=false);
	//virtual int Open(char *pUrl);
	virtual int Pause();
	virtual int Resume();
	virtual int Seek(int64 nTime, int nFlag=0);
	virtual int SeekToTimestamp(int64 s64Timestamp, int nFlag=0);
	virtual int SetPlaySpeed(float fSpeed);
	virtual int PlayNextFrame();
	virtual int PlayPrevFrame(int64 s64PtsNow=0);
	virtual int SetPlayState(int nState);
	virtual int SetPlayMode(eFilterPlayMode playMode);

	virtual int DoFilter();
	virtual int64 GetCurrTime();
	//virtual int64 GetTotalTime();

	virtual int  ProbMediaInfo(char *pSzFile);
protected:
	int ReadFrame();

	static void CALLBACK HkDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);

protected:
	virtual CMediaFilter *CreateObject();
	//virtual int ProbMediaInfo();

	CBuffer *m_pBufferRead;
	FILE    *m_pFile;
	int64   m_s64TotalLen;
	int64   m_s64CurPos;
	//
	int		m_nHkStreamHandle;
	bool	m_bHkInit;
	int		m_nHkHeaderLen;
	long    m_lWindow;
	//
	long     m_nTimePerFrame;
	long	 m_lTimeLastRead;

	CMutex  *m_pMutexDoFilter;
};
//extern CSourceHkFile g_sourceHkFile;
