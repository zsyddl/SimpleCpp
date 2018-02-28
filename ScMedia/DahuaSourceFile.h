#pragma once
#include "sourcefilter.h"

#include "Buffer.h"
#include "FfmpegCodec.h"
#include "FfmpegFormat.h"
#define  FILTER_NAME_SOURCE_DAHUA_FILE "DahuaSourceFile"

#define  AvFormat_Dahua "AvFormat_Dahua"
#define  DahuaFileFlag "DHPT"
#define  DahuaFileFlag1 "Dahua_ZH"
//目前是用于 dahua file 转换到 avi格式

typedef unsigned char uint8_t;
struct FRAME_DECODE_INFO;
struct FRAME_INFO_EX;
typedef  struct _FRAME_INFO FRAME_INFO;

namespace SimpleCpp{
class CBuffer;
}
using namespace SimpleCpp;
class CDahuaFile;
class CDahuaSourceFile :
	public CSourceFilter
{
public:
	CDahuaSourceFile(void);
	virtual ~CDahuaSourceFile(void);

	virtual int Open(char *pSzParam/* =0 */, bool bCreateThread=false);
	//virtual int Open(char *pUrl);
	virtual int Close();
	virtual int Resume();
	
	virtual int DoFilter();
	virtual int PlayNextFrame();
	virtual int Seek(int64 s64Time, int nFlag=AVSEEK_FLAG_ANY );
	virtual int SeekToTimestamp(int64 s64Timestamp, int nFlag=AVSEEK_FLAG_ANY);
	virtual int64 TimestampToTime(int64 s64Timestamp);
	virtual int64 TimeToTimestamp(int64 s64Time);

	virtual int   DoFixFile(int64 &s64Total, int64 &s64Pos);

	virtual char *GetFormatName(){return AvFormat_Dahua;};
	virtual int  ProbMediaInfo(char *pSzFile);
protected:
	virtual CMediaFilter *CreateObject();
	int ReadFrame(CBuffer &buffer, int &nFrameType, FILE *pFile=0);
	int CheckFrameType(char *pFrame, int nLen);
	int ParseMediaInfo(char *pFile);
	//LONG nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2
	//static void CALLBACK  DahuaDecodeCb(long nPort, FRAME_DECODE_INFO* pFrameDecodeInfo, FRAME_INFO* pFrameInfo, void* pUser);
	static void CALLBACK  DahuaDecodeCb(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2);
	long    m_lPort;
	bool    m_bConverStarted;
	FILE	*m_pFile;
	CBuffer m_buffRead;
	CBuffer m_buffVol;

	long	m_lTimeLastRead;
	long	m_nTimePerFrame; //每个帧需要等待的时间, ms
	int64   m_s64FileLen;
	int64   m_s64FileCurPos;

	sFfmpegStreamInfo m_streamInfo;
	//用于探测 数据信息，宽度，高度，帧率等
	CFfmpegCodec *m_pFfmpegCodec;
	//用于输出的 avi文件
	CFfmpegFormat *m_pFfmpegFileSaveToAvi;
	//
	CDahuaFile		*m_pDahuaFile;
	//
	int64    m_s64Timestamp;
	//dahua private 
	bool	m_bPriv;
};
