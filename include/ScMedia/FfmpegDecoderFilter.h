#pragma once
#include "Decoder.h"

#include "FfmpegCodec.h"



class CFfmpegCodec;

class MEDIASDK_API  CFfmpegDecoderFilter :
	public CDecoder
{
public:
	CFfmpegDecoderFilter(void);
	virtual ~CFfmpegDecoderFilter(void);
	//type=audio;codec=%d;bitrate=%d;fps=%d;gop=%d;width=%d;height%d;
	virtual int Open(char *pParam =0 );
	virtual int Open(char* pParam, bool bHasThread =false );
	virtual int DoFilter();

	virtual int CopyFrame(unsigned char *pImageOut, int nImageLen, int nW, int nH, int pix_fmt);
	virtual void Release();
	virtual void Reset();

	virtual int64 GetTimestamp();
protected:
	virtual CMediaFilter *CreateObject();

	int CopyAvFrame2VideoFrame(void *pAvFrame, void *pVideoFrame);
	int CreateDecoder();

	CFfmpegCodec *m_pDecoder;
	sFfmpegStreamInfo m_streamInfo;
	int64	m_s64Timestamp;
};
