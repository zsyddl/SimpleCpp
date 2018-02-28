#pragma once
#include "sourcefilter.h"

//暂时不需要解码器 
class CVideoSummaryFile;
class CVideoSummarySource :
	public CSourceFilter
{
public:
	CVideoSummarySource(void);
	virtual ~CVideoSummarySource(void);

	virtual int Init(char *pSzParam=0);
	virtual int DoFilter();

protected:
	virtual CDataFilter* CreateObject();

	CVideoSummaryFile *m_pFile;
};
