#pragma once
#ifndef PARSE_VIVS_FRAME_H
#define PARSE_VIVS_FRAME_H
#include <list>
#include "VivsMediaDef.h"
#include "VivsMediaDef.h"

#ifndef VIVSMEDIABOX_API
#define  VIVSMEDIABOX_API
#endif

using namespace std;


typedef enum _eVivsFrameType{
	eVFT_Audio,
	eVFT_VideoIFrame,
	eVFT_VideoPFrame,
	eVFT_VideoSFrame,
}eVivsFrameType;



class CBuffer;
//流的一帧
class VIVSMEDIABOX_API CStreamFrame
{
public:
	CStreamFrame();
	~CStreamFrame();
public:
	CBuffer *m_pBufferFrame;
	int     m_eFrameType;
	int m_nW;
	int m_nH;
	//时间戳
	unsigned short m_nYear;
	unsigned short m_nMonth;
	unsigned short m_nDay;
	unsigned short m_nHour;
	unsigned short m_nMinute;
	unsigned short m_nSecond;
	unsigned short m_nMiniSecond;
};
/*
* 非线程安全
*/
class VIVSMEDIABOX_API CParseVivsFrame
{
	typedef enum _eParserVivsFrameState{
		ePVFS_Run,
		ePVFS_Stop,
	}eParserVivsFrameState;
public:
	CParseVivsFrame(void);
	~CParseVivsFrame(void);
	
	long Init(int nBufferSize=1024*1024);
	long Close();
	// ret <0 if false
	//解析为标准 流，无使用信息
	long Parse2Std(char *pData, int nLen);
	// 解析 avdataend， 做分割，以及帧类型，并不剥离数据，
	long ParseAvdataEnd(char *pData, int nLen);
	// @pFrame,  user alloc the memory and pass to this fun. the size must big enough, 512kB eg.
	// @nLen,    in & out. the frame data length
	// @nFrameType, eVivsFrameType
	// @ret,      <0 if false, and nLen <= 0;
	// @notes: user must call this fun several times , to get all frames.
	long GetFrame(char *pFrame, int &nLen, int &nFrameType);
	//
	long GetFrame(CStreamFrame *pFrame);
	//解析媒体时间，如果有就返回时间，没有则为-1, 不缓存数据
	static long ParseTimeStamp(char *pData, int nLen, unsigned int &lSecond, unsigned int &lMilliSecond);
protected:
	//begin parse2h264nal
	//解析 avdata 分包, 剥离是由头，并 分解为 h264 nal 单元
	int ParsePackage(char *pData, int nLen, CBuffer *pOut, int &nOutLen, int &nAvDataEndType);
	int Split2AvdataPackage(char *pData, int nLen, CBuffer *pOut, int &nOutLen, int &nAvDataEndType);
	//解析h264码流
	int ParseH264Nal(CBuffer *pH264Stream, CBuffer *pNal, int &nFrameType);
	//end parse2h264nal

	//码流缓存
	CBuffer *m_pBufferStream;
	CBuffer *m_pBufferStreamTmp;
	CBuffer *m_pBufferVideoStream;
	CBuffer *m_pBufferAudioStream;
	//
	CStreamFrame *m_pStreamFrameTmp;
	//已经解析的 frame
	list<CStreamFrame *> m_lstFrame;
	int m_eState;
	//
	int m_nFTMV;
	bool	m_bParseAvdataEnd;
	//保存 当前的 avdata end 提供给 接下来的 nal分包使用。
	sAvDataEnd m_avDataEnd;
};

/*
* 非线程安全
*/
#ifdef __cplusplus
extern "C" {
#endif
	// must larger than 512kB
	long Vivs_FrameParserInit(int nBufferSize);
	void Vivs_FrameParserClose(long lHandle);

	long Vivs_FrameParserInputData(long lParserHandle, char *pData, int nLen);
	// @pFrame,  user alloc the memory and pass to this fun. the size must big enough, 512kB eg.
	// @nLen,    in & out. the frame data length
	// @nFrameType, eVivsFrameType
	// @ret,      <0 if false, and nLen <= 0;
	// @notes: user must call this fun several times , to get all frames.
	long Vivs_FrameParserGetFrame(long lParserHandle, char *pFrame, int *pLen, int *pFrameType);
#ifdef __cplusplus
}
#endif

#endif //PARSE_VIVS_FRAME_H