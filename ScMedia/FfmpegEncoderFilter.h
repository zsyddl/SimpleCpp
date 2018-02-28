#pragma once
#include "MediaSdk.h"
#include "encoder.h"



class CFfmpegCodec;

typedef struct _sFfmpegVideoInfo sFfmpegVideoInfo;
typedef struct _sFfmpegAudioInfo sFfmpegAudioInfo;
typedef struct _sFfmpegFrameInfo sFfmpegFrameInfo;
class MEDIASDK_API CFfmpegEncoderFilter :
	public CEncoder
{
public:
	CFfmpegEncoderFilter(void);
	virtual ~CFfmpegEncoderFilter(void);

	virtual int Open(char* pSzParam, bool bHasThread=false);
	virtual int DoFilter();
	
protected:
	virtual CMediaFilter *CreateObject();
	//检查 视频的 参数 是否变化 需要 重新初始化 解码器 
	bool CheckStreamChanged(sFfmpegVideoInfo &videoInfo, sFfmpegAudioInfo &audioInfo );
	int  ReCreateCodec();

protected:
	CFfmpegCodec *m_pFfmpegCodec;
	sFfmpegVideoInfo *m_pVideoInfo;
	sFfmpegAudioInfo *m_pAudioInfo;
	sFfmpegFrameInfo *m_pFrameInfo;
};
