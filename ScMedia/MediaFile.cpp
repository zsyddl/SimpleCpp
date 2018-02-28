#include "StdAfx.h"
#include "MediaFile.h"

CMediaFile::CMediaFile(void)
: m_nTimebaseNum(1)
, m_nTimebaseDen(25)
, m_nFps(25)
, m_s64TotalTime(0)
, m_s64CurrTimestamp(0)
, m_s64StartTimestamp(0)
, m_bNeedFix(false)
, m_nWidth(0)
, m_nHeight(0)
{

}

CMediaFile::~CMediaFile(void)
{
}
int CMediaFile::Open(char *pSzFile, int nOpenMode)
{
	return 0;
}
 int CMediaFile::Close()
 {
	 return 0;
 }

 int CMediaFile::Write(char *pData, int nDataLen)
 {
	 return 0;
 }
 int CMediaFile::Read(char *pData, int &nBufferLen)
 {
	 return 0;
 }
 int CMediaFile::Seek(int64 s64Time, int nSeekFlag)
 {
	 return 0;
 }
 int CMediaFile::SeekToTimestamp(int64 s64Time, int nSeekFlag)
 {
	 return 0;
 }
int64 CMediaFile::TimeToTimestamp(int64 s64Time)
{
	int64 s64Timestamp = 0;

	s64Timestamp = s64Time*m_nTimebaseDen/1000.0/m_nTimebaseNum;

	return s64Timestamp;

}
int64 CMediaFile::TimestampToTime(int64 s64Timestamp)
{
	int64 s64Time=0;

	s64Time = s64Timestamp*1000*m_nTimebaseNum/m_nTimebaseDen;

	return s64Time;
}
int64 CMediaFile::GetTotalTime()
{
	
	return (m_s64TotalTime);
}
int64 CMediaFile::GetCurrTime()
{
	return TimestampToTime(m_s64CurrTimestamp);
}
void  CMediaFile::GetTimebase(int &nNum, int &nDen)
{
	 nNum = m_nTimebaseNum;
	 nDen = m_nTimebaseDen;
}
void  CMediaFile::SetTimebase(int nNum, int nDen)
{
	 m_nTimebaseDen = nDen;
	 m_nTimebaseNum = nNum;
}
void		CMediaFile::SetFrameRate(int nNum, int nDen)
{
	m_nFpsNum = nNum;
	m_nFpsDen = nDen;

	m_nFps = nNum/nDen;
}
void		CMediaFile::GetFrameRate(int &nNum, int &nDen)
{
	nNum = m_nFpsNum;
	nDen = m_nFpsDen;
}
bool		CMediaFile::IsNeedFix()
{

	return m_bNeedFix;
}
int CMediaFile::DoFixFile()
{
	return 0;
}

int CMediaFile::GetVideoWidth()
{
	return m_nWidth;
}

int CMediaFile::GetVIdeoHeight()
{
	return m_nHeight;
}

int CMediaFile::SetVideoWidth( int nW )
{
	m_nWidth = nW;
	return 0;
}

int CMediaFile::SetVideoHeight( int nH )
{
	m_nHeight = nH;
	return 0;
}
