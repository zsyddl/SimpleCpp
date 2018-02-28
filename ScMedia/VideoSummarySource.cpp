#include "StdAfx.h"
#include "VideoSummarySource.h"

CVideoSummarySource m_regVideoSummarySource;
CVideoSummarySource::CVideoSummarySource(void)
:CSourceFilter()
{
	strcpy(m_szType, "CVideoSummarySource");
	strcpy(m_szName, "CVideoSummarySource");

	CFilterManager::RegistFilter(&m_regVideoSummarySource);
}

CVideoSummarySource::~CVideoSummarySource(void)
{
}
int CVideoSummarySource::Init(char *pSzParam)
{
	
}
int CVideoSummarySource::DoFilter()
{

	return CSourceFilter::DoFilter();
}

CDataFilter* CVideoSummarySource::CreateObject()
{

	return new CVideoSummarySource;
}