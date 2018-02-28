#pragma once

#include "stdafx.h"
//缩放，不知道为什么，在外面直接用ffmpeg，release会崩溃

struct SwsContext;
struct AVPicture;
typedef unsigned char uint8;
class MEDIASDK_API CFfmpegScale
{
public:
	CFfmpegScale(void);
	~CFfmpegScale(void);
	int GetFilterType() const { return m_nFilterType; }
	void SetFilterType(int val) { m_nFilterType = val; }

	int Scale(uint8 *pSrc, int nWSrc, int nHSrc, int nPixFmtSrc, uint8 *pDes, int nWDes, int nHDes, int nPixFmtDes);

	static int GetPictureSize(int nW, int nH, int nPixFmt);
protected:
	int FillImage(uint8 *pSliceData[], int nLineStride[]);

	int m_nWSrc;
	int m_nHSrc;
	int m_nWDes;
	int m_nHDes;
	int m_nPixFmtSrc;
	int m_nPixFmtDes;

	SwsContext *m_pSwsContext;

	AVPicture *m_pAvPictureSrc;
	AVPicture *m_pAvPictureDes;
	void      *m_pAvPictureBuffer;
	int			m_nFilterType;

};
