#pragma once
#include "sourcefilter.h"

class CVideoSummaryFile;
class CSourceVideoSummaryFile :
	public CSourceFilter
{
public:
	CSourceVideoSummaryFile(void);
	virtual ~CSourceVideoSummaryFile(void);
	virtual int Init(char *pSzParam=0);
	virtual int DoFilter();

	virtual int PlayNextFrame();
protected:
	virtual CMediaFilter* CreateObject();

	CVideoSummaryFile *m_pFile;
	int  m_nTimePerFrame;
};


