#pragma once
#include "Decoder.h"
/*#include "FfmpegFormat.h"*/
/*#define FFMPEG_OLD*/
#define FFMPEG_NEW
//#define  TEST_WRITE_AVI

class CFfmpegCodec;
class VideoCodec;
class CFfmpegFormat;
class MEDIASDK_API CH264Decoder :
	public CDecoder
{
public:
	CH264Decoder(void);
	virtual ~CH264Decoder(void);

	virtual int Open(char *pParam=0);
	virtual int DoFilter();
	virtual int Flush();

	int CopyFrame(int nWidth, int nHeight, char *pVideoFrame, int &nLen, int nOutFormat);
	
	//复制到对应的 缓存，读音的 pix format nw nH
	virtual int CopyFrame(unsigned char *pImageOut, int nImageLen, int nW, int nH, int pix_fmt);


	static int CopyAvFrame2VideoFrame(void *pAvFrame, void *pVideoFrame);
protected:
	virtual CMediaFilter *CreateObject();
#ifdef FFMPEG_OLD
	VideoCodec *m_pFfmpeg;
#else
	CFfmpegCodec *m_pFfmpeg;
#endif
#ifdef TEST_WRITE_AVI
	//测试写 avi文件
	CFfmpegFormat *m_pFfmpegAvi;
	//
#endif
};

//extern CH264Decoder g_decoderH264;