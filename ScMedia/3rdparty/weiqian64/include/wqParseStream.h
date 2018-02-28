#ifndef _WQPARSESTREAM_H
#define _WQPARSESTREAM_H

#ifdef WIN32
#define WQPARSESTREAM_API __declspec(dllexport)
#define CALLMETHOD __stdcall
#else
#define __stdcall
#define CALLMETHOD
#define WQPARSESTREAM_API
#endif

#define SAMPLE_FREQ_4000	1
#define SAMPLE_FREQ_8000	2
#define SAMPLE_FREQ_11025	3
#define SAMPLE_FREQ_16000	4
#define SAMPLE_FREQ_20000	5
#define SAMPLE_FREQ_22050	6
#define SAMPLE_FREQ_32000	7
#define SAMPLE_FREQ_44100	8
#define SAMPLE_FREQ_48000	9

// Frame Type and SubType
#define WQ_FRAME_TYPE_UNKNOWN		0
#define WQ_FRAME_TYPE_VIDEO			1
#define WQ_FRAME_TYPE_AUDIO			2
#define WQ_FRAME_TYPE_DATA			3

#define WQ_FRAME_TYPE_VIDEO_I_FRAME	0
#define WQ_FRAME_TYPE_VIDEO_P_FRAME	1
#define WQ_FRAME_TYPE_VIDEO_B_FRAME	2
#define WQ_FRAME_TYPE_VIDEO_S_FRAME	3
#define WQ_FRAME_TYPE_DATA_TEXT     5
#define WQ_FRAME_TYPE_DATA_INTL     6

// Encode type:
#define WQ_ENCODE_UNKNOWN				0
#define WQ_ENCODE_VIDEO_MPEG4			1
#define WQ_ENCODE_VIDEO_HI_H264			2
#define WQ_ENCODE_VIDEO_JPEG            3
#define WQ_ENCODE_VIDEO_H264			4
#define WQ_ENCODE_AUDIO_PCM8			7	// 8BITS,8K
#define WQ_ENCODE_AUDIO_G729			8
#define WQ_ENCODE_AUDIO_IMA				9
#define WQ_ENCODE_AUDIO_PCM_MULAW		10
#define WQ_ENCODE_AUDIO_G721			11
#define WQ_ENCODE_AUDIO_PCM8_VWIS		12	// 16BITS,8K
#define WQ_ENCODE_AUDIO_ADPCM			13	// 16BITS,8K/16K
#define WQ_ENCODE_AUDIO_G711A			14	// 16BITS,8K
#define WQ_ENCODE_AUDIO_TALK            30

#define ENCODE_VIDEO_SVAC            5
#define ENCODE_VIDEO_SVAC_NEW        6

typedef struct 
{
	unsigned char* pHeader;  //包含头的数据指针
	unsigned char* pContent; //真正数据的偏移
	unsigned long nLength; //数据长度(包括头部)
	unsigned long nFrameLength; //数据长度.
	
	unsigned int nType; // VIDEO, AUDIO, DATA
	unsigned int nSubType; // I-FRAME, P-FRAME, etc.
	
	unsigned int nEncodeType; // MPEG4/H264, PCM, MSADPCM, etc.
	// 只有I帧才有的数据
	unsigned long nYear;
	unsigned long nMonth;
	unsigned long nDay;
	unsigned long nHour;
	unsigned long nMinute;
	unsigned long nSecond;
	unsigned long nTimeStamp;
	
	unsigned int  nFrameRate;	//帧率
	int			  nWidth;  //图象宽度
	int			  nHeight; //图象高度
	unsigned long nRequence; //序列号
	// 音频才有的数据
	unsigned int nChannels;
	unsigned int nBitsPerSample;
	unsigned int nSamplesPerSecond;
	
	unsigned long nParam1;		// 扩展用
	unsigned long nParam2;		// 扩展用
	
} WQ_FRAME_INFO;

typedef void* PARSERHANDLE;
	
WQPARSESTREAM_API PARSERHANDLE CALLMETHOD
WQSP_Init();

WQPARSESTREAM_API int CALLMETHOD
WQSP_Free(PARSERHANDLE hHandle);

WQPARSESTREAM_API int CALLMETHOD
WQSP_InputData(PARSERHANDLE hHandle, unsigned char *byData, unsigned long dwLength);

WQPARSESTREAM_API WQ_FRAME_INFO * CALLMETHOD
WQSP_GetNextFrame(PARSERHANDLE hHandle);

#endif // INCLUDED_DHSTREAMPARSE_H
