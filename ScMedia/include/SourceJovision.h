#pragma once
#include "sourcefilter.h"


#define FILTER_SOURCE_JOVISION_FILE "SourceJovision"
#define AvFormat_JOVISION "AvFormat_Jovison"

class CBuffer;
class CFfmpegScale;
class CSourceJovision :
	public CSourceFilter
{
public:
	CSourceJovision(void);
	virtual ~CSourceJovision(void);

	virtual int Init(char *pSzParam=0);
	virtual int  ProbMediaInfo(char *pSzFile);

	virtual int DoFilter();
protected:
	virtual CMediaFilter *CreateObject();
	int OpenPlay();

	static bool m_bInitSdk;
	int    m_nPlayerHandle;
	int    m_nLastFrameNum;
	int    m_nTotalFrame;
	CBuffer *m_pBufferDecode;
	CFfmpegScale *m_pFfmpegScale;
};
