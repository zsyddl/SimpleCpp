#pragma once
#include "Render.h"
#include "IndexFile.h"
#include "MediaSdkDef.h"

class MEDIASDK_API CRender2IndexFile :
	public CRender
{
public:
	CRender2IndexFile(void);
	virtual ~CRender2IndexFile(void);

	virtual int Open(char *pParam);
	virtual int DoFilter();
	virtual CMediaFilter *CreateObject();

protected:
	bool  CheckStreamChanged(sVideoFrameInfo &videoFrameInfo);
	int   ReCreate();

	sVideoFrameInfo *m_pVideoFrameInfo;
	CIndexFile *m_pFile;
};
