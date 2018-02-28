#include "StdAfx.h"
#include "SourceFilter.h"
#include "FilterManager.h"

CSourceFilter g_regSourceFilter;
CSourceFilter::CSourceFilter(void)
:CMediaFilter()
, m_bSeek(false)
, m_s64SeekTime(0)
, m_s64TimeStamp(0)
, m_nNeedSleep(10)
, m_bIsStreamEnd(false)
, m_s64CurrTime(0)
, m_s64TotalTime(0)
, m_s64SegmentTimeBegin(0)
, m_s64SegmentTimeEnd(0)
, m_bNeedFix(false)
{
	strcpy(m_szKind, FILTER_KIND_SOURCE);
	strcpy(m_szType, FILTER_NAME_SOURCE);
	strcpy(m_szName, FILTER_NAME_SOURCE);
	strcpy(m_szFormatName, AvFormat_Unknown);
	strcpy(m_szFormatSubName, AvFormat_Unknown);

	CFilterManager::RegistFilter(&g_regSourceFilter);
}

CSourceFilter::~CSourceFilter(void)
{

}
void CSourceFilter::GetTimeBase(int &nNum, int nDen)
{
	nNum = 0;
	nDen = 0;
}
int64 CSourceFilter::TimestampToTime(int64 s64Timestamp)
{
	
	return s64Timestamp;
}
int64 CSourceFilter::TimeToTimestamp(int64 s64Time)
{
	return s64Time;
}
/*
int CSourceFilter::DoFilter()
{
	//

	//
	return CDataFilter::DoFilter();
}*/
int CSourceFilter::Seek(int64_t s64TimeStamp, int nFlag/*=0*/)
{

	return 0;
}

int CSourceFilter::Close()
{
	return 0;
}

CMediaFilter * CSourceFilter::CreateObject()
{
	return new CSourceFilter();
}
int CSourceFilter::CheckFormat(char *pSzFile)
{
	return 0;
}
bool CSourceFilter::IsStreamEnd()
{
	return  m_bIsStreamEnd;
}
int64 CSourceFilter::GetTotalTime()
{
	return m_s64TotalTime;
}
int64 CSourceFilter::GetCurrTime()
{
	return m_s64CurrTime;
}
int CSourceFilter::GetWidth()
{

	return m_nW;
}
int CSourceFilter::GetHeight()
{

	return m_nH;
}
int CSourceFilter::GetPixelFormat()
{
	return m_nPixelFormat;
}

int CSourceFilter::GetPumperSleepTime()
{
	return m_nNeedSleep;
}
int  CSourceFilter::ProbMediaInfo(char *pSzFile)
{

	return -1;
}

char * CSourceFilter::GetFormatSubName()
{
	return m_szFormatSubName;
}
char *CSourceFilter::GetFormatName()
{
	return m_szFormatName;
};

void CSourceFilter::SetSegment( int64_t s64TimeBegin, int64_t s64TimeEnd )
{
	m_s64SegmentTimeBegin = s64TimeBegin;
	m_s64SegmentTimeEnd   = s64TimeEnd;
}
bool  CSourceFilter::IsNeedFix()
{
	return m_bNeedFix;
}
int   CSourceFilter::DoFixFile(int64 &s64Total, int64 &s64Pos)
{
	s64Pos   = 0;
	s64Total = 0;
	return 0;
}