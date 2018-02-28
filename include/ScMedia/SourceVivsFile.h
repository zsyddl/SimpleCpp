#pragma once
#include "sourcefilter.h"

class CVivsMediaFile;
class MEDIASDK_API CSourceVivsFile :
	public CSourceFilter
{
public:
	CSourceVivsFile(void);
	virtual ~CSourceVivsFile(void);
	virtual int Init();
	virtual int Open(char *pUrl);
	virtual int Close();
	virtual int DoFilter();
protected:
	int ReadFrame();
	int ReadOneFrame();
protected:
	CVivsMediaFile	*m_pVivsMediaFile;
	int64_t			m_s64TimeLastRead;
	int64_t			m_s64TimeLastTimeStamp;
	bool			m_bEndOfFile;
	int64_t			m_s64imeNeedToWait;
	
	CBuffer			*m_pBufferRead;

};
