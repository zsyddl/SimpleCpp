#pragma once

#include "Decoder.h"
#include "FfmpegCodec.h"

namespace SimpleCpp{
	class CBuffer;
}
using namespace SimpleCpp;
class CFfmpegCodec;
class CStdDecoder :
	public CDecoder
{
public:
	CStdDecoder(void);
	virtual ~CStdDecoder(void);
	virtual int Open(sFfmpegStreamInfo &mediaInfo);
	virtual int DoFilter();
	//
	int CopyAvFrame2VideoFrame(void *pAvFrame, void *pVideoFrame);
protected:
	CBuffer  *m_pBufferDecodeTmp;
	CFfmpegCodec *m_pFfmpeg;
	sFfmpegStreamInfo m_mediaInfo;
};
