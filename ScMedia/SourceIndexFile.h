#pragma once
#include "sourcefilter.h"

#include "IndexFile.h"
#include "Buffer.h"

class CSourceIndexFile :
	public CSourceFilter
{
public:
	CSourceIndexFile(void);
	virtual ~CSourceIndexFile(void);

	virtual int Open(char *pSzParam/* =0 */);
	virtual int DoFilter();
	virtual CMediaFilter *CreateObject();
protected:
	int ReadFrame();

	CBuffer *m_pBufferRead;
	sVideoFrameInfo *m_pVideoInfo;

	CIndexFile *m_pFile;
};
