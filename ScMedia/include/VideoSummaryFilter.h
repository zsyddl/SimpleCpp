#pragma once
/*
*ÊÓÆµÕªÒª
*
*/
#include "datafilter.h"

class CVideoSummaryFilter :
	public CDataFilter
{
public:
	CVideoSummaryFilter(void);
	virtual ~CVideoSummaryFilter(void);

	virtual int Init(char *pParam);
	virtual int DoFilter();

protected:
	virtual CDataFilter *CreateObject();
};
