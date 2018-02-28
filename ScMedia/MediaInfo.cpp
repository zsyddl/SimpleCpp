#include "StdAfx.h"
#include "MediaInfo.h"
#include "FfmpegFormat.h"
#include "DahuaSourceFile.h"
#include "SourceFfmpegFile.h"
#include "FilterManager.h"

CMediaInfo::CMediaInfo(void)
: m_s64TotalTime(0)
, m_nW(0)
, m_nH(0)
, m_bNeedFix(false)
{
	memset(m_szFormat, 0, sizeof(m_szFormat));
	memset(m_szSourceFilter, 0, sizeof(m_szSourceFilter));
	memset(m_szFormatSub, 0, sizeof(m_szFormatSub));
}

CMediaInfo::~CMediaInfo(void)
{
	for (int i=0; i<m_lstStream.size(); i++)
	{
		delete m_lstStream.at(i);
	}
	m_lstStream.clear();
}
int			CMediaInfo::Open(char *pSzFile, int &nPos/*=0*/)
{
	int nRet = -1;
	
	/*CFfmpegFormat *pFfmpegFormat = new CFfmpegFormat;
	if (pFfmpegFormat->Open(pSzFile) >= 0)
	{
		strcpy(m_szFormat, pFfmpegFormat->GetFormatName());
		nRet = 0;
		strcpy(m_szSourceFilter, FILTER_SOURCE_FFMPEG_FILE);
	}
	else
	{
		CDahuaSourceFile *pDahuaSource = new CDahuaSourceFile();
		nRet = pDahuaSource->ProbMediaInfo(pSzFile);
		if (nRet < 0)
		{
		}
		else
		{
			strcpy(m_szFormat, pDahuaSource->GetFormatName());
			strcpy(m_szSourceFilter, FILTER_NAME_SOURCE_DAHUA_FILE);
		}
		delete pDahuaSource;
	}
	delete pFfmpegFormat;
	*/
	vector<CSourceFilter *> lstSource;
	CFilterManager::GetSourceFilter(lstSource);
	//需要把 ffmpeg放在第一
	int nFind = -1;
	for (int i=nPos; i<lstSource.size(); i++)
	{
		CSourceFilter *pSourceFilter = dynamic_cast<CSourceFilter*>(lstSource.at(i));
		if (!pSourceFilter)
			continue;

		nRet = pSourceFilter->ProbMediaInfo(pSzFile);

		if (nRet >= 0)	
		{
			strncpy(m_szFormat, pSourceFilter->GetFormatName(), sizeof(m_szFormat));
			strncpy(m_szFormatSub, pSourceFilter->GetFormatSubName(), sizeof(m_szFormatSub));
			strncpy(m_szSourceFilter, pSourceFilter->GetName(), sizeof(m_szSourceFilter));
			m_bNeedFix = pSourceFilter->IsNeedFix();

			m_nW = pSourceFilter->GetWidth();
			m_nH = pSourceFilter->GetHeight();
			m_s64TotalTime = pSourceFilter->GetTotalTime();

			nFind = i;
			break;
		}
		
	}
	nPos = nFind ;

	return nRet;
}
char *		CMediaInfo::GetFormatName()
{

	return m_szFormat;
}
vector<sFfmpegStreamInfo *> CMediaInfo::GetStreams()
{
	return m_lstStream;
}

char * CMediaInfo::GetSourceFilter()
{
	return m_szSourceFilter;
}

char * CMediaInfo::GetFormatSubName()
{
	return m_szFormatSub;
}
int	CMediaInfo::GetWidth()
{
	return m_nW;
}
int	CMediaInfo::GetHeight()
{

	return m_nH;
}
int64		CMediaInfo::GetTotalTime()
{
	return m_s64TotalTime;
}