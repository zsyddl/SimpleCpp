#include "StdAfx.h"
#include "VideoSummaryDecoder.h"
#include "FilterManager.h"

CVideoSummaryDecoder m_regVideoSummaryDecoder;
CVideoSummaryDecoder::CVideoSummaryDecoder(void)
:CDecoder()
{
	strcpy(m_szType, "CVideoSummaryDecoder");
	strcpy(m_szName, "CVideoSummaryDecoder");

	CFilterManager::RegistFilter(&m_regVideoSummaryDecoder);
}

CVideoSummaryDecoder::~CVideoSummaryDecoder(void)
{

}
