#pragma once
#ifndef DECODER_H
#define DECODER_H

#include "MediaFilter.h"

class MEDIASDK_API CDecoder :
	public CMediaFilter
{
public:
	CDecoder(void);
	virtual ~CDecoder(void);

	virtual int Open(char *pParam=0);
	virtual int DoFilter();

	void   SetPixOutFormat(int nPixFormat);
	//复制到对应的 缓存，图片的 pix format nw nH
	virtual int CopyFrame(unsigned char *pImageOut, int nImageLen, int nW, int nH, int pix_fmt);

	virtual int64 GetTimestamp();
protected:
	int m_nPixOutFormat;
};
#endif
