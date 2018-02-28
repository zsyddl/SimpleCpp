#pragma once
#include "decoder.h"
#include "MediaSdkDef.h"
/*
typedef struct{
	long nWidth;
	long nHeight;
	long nStamp;
	long nType;
	long nFrameRate;
	DWORD dwFrameNum;
}FRAME_INFO;*/
typedef struct _FRAME_INFO FRAME_INFO;

#define FILTER_DECODER_HK "HkDecoder"

#define HK_SOURCE_BUFFER_LEN (600*1024)
class MEDIASDK_API CHkDecoder :
	public CDecoder
{
	friend static void CALLBACK HkDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);;
public:
	CHkDecoder(void);
	virtual ~CHkDecoder(void);

	virtual int Open(char *pParam=0);
	virtual int DoFilter();
	void SetWnd(HWND hWnd);
protected:
	static void CALLBACK HkDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);
protected:
	virtual CMediaFilter *CreateObject();
	int		m_nHkStreamHandle;
	bool	m_bHkInit;
	int		m_nHkHeaderLen;
	HWND    m_hWnd;
};

//extern CHkDecoder g_decoderHk;