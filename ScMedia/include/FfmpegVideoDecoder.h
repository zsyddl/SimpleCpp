#pragma once
#include "decoder.h"

class VideoCodec;
class CFfmpegVideoDecoder :
	public CDecoder
{
public:
	CFfmpegVideoDecoder(void);
	virtual ~CFfmpegVideoDecoder(void);
	virtual int Init(int nCodec);
	virtual int DoFilter();
	//
protected:
	VideoCodec *m_pFfmpeg;
};
