#pragma once
#include "decoder.h"
#include "dhplay.h"
class CDhDecoder :
	public CDecoder
{
public:
	CDhDecoder(void);
	virtual ~CDhDecoder(void);

	friend static void CALLBACK DhDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);;

	virtual int Open();
	virtual int DoFilter();
protected:
	static void CALLBACK DhDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2);
protected:
	int		m_nDhStreamHandle;
	bool	m_bHkInit;
	int		m_nHkHeaderLen;
};
