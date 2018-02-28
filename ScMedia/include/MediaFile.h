#pragma once
#include "MediaFilter.h"

class CMediaFile 
{
public:
	CMediaFile(void);
	virtual ~CMediaFile(void);

	virtual int Open(char *pSzFile, int nOpenMode);
	virtual int Close();

	virtual int Write(char *pData, int nDataLen);
	virtual int Read(char *pData, int &nBufferLen);

	virtual int Seek(int64 s64Time, int nSeekFlag);
	virtual int SeekToTimestamp(int64 s64Time, int nSeekFlag);
	int64		TimeToTimestamp(int64 s64Timestamp);
	int64		TimestampToTime(int64 s64Time);
	int64		GetTotalTime();
	int64		GetCurrTime();
	void		GetTimebase(int &nNum, int &nDen);
	void		SetTimebase(int nNum, int nDen);
	void		SetFrameRate(int nNum, int nDen);
	void		GetFrameRate(int &nNum, int &nDen);

	virtual int GetVideoWidth();
	virtual int GetVIdeoHeight();
	virtual int SetVideoWidth(int nW);
	virtual int SetVideoHeight(int nH);
	
	bool		IsNeedFix();
	virtual int DoFixFile();
protected:
	int		m_nTimebaseNum;
	int		m_nTimebaseDen;
	int		m_nFps;
	int		m_nFpsNum;
	int		m_nFpsDen;
	int64	m_s64TotalTime;
	int64	m_s64CurrTimestamp;
	int64	m_s64StartTimestamp;
	bool    m_bNeedFix;
	int		m_nWidth;
	int		m_nHeight;
};
