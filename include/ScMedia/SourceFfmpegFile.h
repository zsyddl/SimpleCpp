#pragma once
#include "sourcefilter.h"
#include "FfmpegFormat.h"

#define FILTER_SOURCE_FFMPEG_FILE	"SourceFfmpegFile"

class CFfmpegFormat ;
class CMediaFileIndex;
class CH264File;
class MEDIASDK_API CSourceFfmpegFile :
	public CSourceFilter
{
public:
	CSourceFfmpegFile(void);
	virtual ~CSourceFfmpegFile(void);

	virtual int Open(char *pSzParam=0);
	virtual int Open(char *pUrl, bool bHasThread=false);
	
	//virtual int	Start();
	virtual int Stop();
	virtual int DoFilter();
	virtual int PlayNextFrame();
	virtual int PlayPrevFrame(int64 s64PtsNow=0);
	virtual int Resume();
	virtual int Seek(int64 s64Time, int nFlag=AVSEEK_FLAG_ANY );
	virtual int SeekToTimestamp(int64 s64Timestamp, int nFlag=AVSEEK_FLAG_ANY);

	virtual int64 GetTotalTime();
	virtual int64 GetCurrTime();
	virtual void GetTimeBase(int &nNum, int nDen);
	virtual int64 TimestampToTime(int64 s64Timestamp);
	virtual int64 TimeToTimestamp(int64 s64Time);
	virtual void  SetSegment(int64_t s64TimeBegin, int64_t s64TimeEnd);

	virtual int   DoFixFile(int64 &s64Total, int64 &s64Pos);

	virtual int  ProbMediaInfo(char *pSzFile);
	//
	virtual	int   RunFilter();
protected:
	virtual CMediaFilter* CreateObject();

	CFfmpegFormat *m_pFfmpegFile;
	CH264File     *m_pH264File;
	CBuffer *m_pReadBuffer;

	long m_lTimeLastRead;
	long m_lTimeInc;
	long m_nTimePerFrame; //每个帧需要等待的时间, ms
	//最后一次读的 位置， 保存，用于在 倒转单帧播放的时候，纪录
	int64_t m_s64LastReadFrame;
	long m_lLastProgressTime;
	//最后一次时间
	int64_t m_s64LastMediaTimestamp;
	long    m_lMediaTimeInc;
	//int64_t m_s64StartMediaTime;

	int  m_nFinishedCount;
};
