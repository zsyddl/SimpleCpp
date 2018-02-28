#include "StdAfx.h"
#include "AudioDecoder.h"
#include "FfmpegCodec.h"

CAudioDecoder::CAudioDecoder(void)
:m_pFfmpeg(0)
{
}
 
CAudioDecoder::~CAudioDecoder(void)
{
	if (m_pFfmpeg)
	{
		delete m_pFfmpeg;
		m_pFfmpeg = NULL;
	}
}
int CAudioDecoder::Open(int nCodec)
{
	m_pFfmpeg = new CFfmpegCodec( /*(CodecID)nCodec*/);//CODEC_ID_H264
	
	return 0;
}
int CAudioDecoder::DoFilter()
{
	//取一个 pin in的 prepared buffer
	
	//解码，将解码数据放到 pin out 的 prepared buffer
	
	//将pin in 的 buffer 放回到 free buffer。

	return 0;
}