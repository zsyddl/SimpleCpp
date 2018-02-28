#pragma once
#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H
#include "MediaFilter.h"
#include "MediaSdkDef.h"
#include "FilterPinBuffer.h"
using namespace SimpleCpp;

typedef enum _eMediaBoxPixFormat{
	ePF_YV420 = 0,
	ePF_RGB ,
}eMediaBoxPixFormat;
class MEDIASDK_API CVideoFramePinBufer :
	public CFilterPinBuffer
{
public:
	CVideoFramePinBufer(void);
	virtual ~CVideoFramePinBufer(void);
	virtual int CopyFrom(CFilterPinBuffer *pBuffer);

	virtual int SetFrameSize(int nW, int nH);

	virtual void Clear(int nReversedMemoryLen=0);

	unsigned char *m_pY;
	unsigned char *m_pU;
	unsigned char *m_pV;

	int m_nWidth;
	int m_nHeight;
	int m_nPixFormat;
	int m_nFps;
	int m_nGop;
	int64_t m_nPts;
};
#endif
