#include "StdAfx.h"
#include "VideoSummaryFilter.h"

CVideoSummaryFilter g_regVideoSummary;
CVideoSummaryFilter::CVideoSummaryFilter(void)
:CDataFilter()
{
	strcpy(m_szKind, "VideoAnalyse");
	strcpy(m_szType, "VideoSummary");
	strcpy(m_szName, "VideoSummary");

	CFilterManager::RegistFilter(&g_regVideoSummary);
}

CVideoSummaryFilter::~CVideoSummaryFilter(void)
{
}
CDataFilter * CVideoSummaryFilter::CreateObject()
{
	return new CVideoSummaryFilter();
}
int CVideoSummaryFilter::Init( char *pParam )
{

	CDataFilter::Init(pParam);


	return 0;
}

int CVideoSummaryFilter::DoFilter()
{
	//取数据，分析， 然后 将结果 可以显示出来。

	return CDataFilter::DoFilter();
}


