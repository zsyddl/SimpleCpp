#include "StdAfx.h"
#include "FfmpegVideoDecoder.h"
#include "videodecoder.h"

CFfmpegVideoDecoder::CFfmpegVideoDecoder(void)
:m_pFfmpeg(0)
{
}

CFfmpegVideoDecoder::~CFfmpegVideoDecoder(void)
{
	if (m_pFfmpeg)
	{
		delete m_pFfmpeg;
		m_pFfmpeg = NULL;
	}
}
int CFfmpegVideoDecoder::Init(int nCodec)
{
	m_pFfmpeg = new VideoCodec((CodecID)nCodec);
	return 0;
}
int CFfmpegVideoDecoder::DoFilter()
{
	
	return 0;
}