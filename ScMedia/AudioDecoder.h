#pragma once
#include "decoder.h"

class VideoCodec;
class CFfmpegCodec;
class CAudioDecoder :
	public CDecoder
{
public:
	CAudioDecoder(void);
	virtual ~CAudioDecoder(void);
	virtual int Open(int nCodec);
	virtual int DoFilter();
protected:
	//VideoCodec *m_pFfmpeg;
	CFfmpegCodec *m_pFfmpeg;
};
