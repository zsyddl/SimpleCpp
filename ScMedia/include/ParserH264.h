#pragma once
#include "PackageParser.h"

class CPackageParser;
class CParserH264 :
	public CPackageParser
{
public:
	CParserH264(void);
	virtual ~CParserH264(void);

	virtual int Open(int nBufferCount=100);

	virtual int InputData(char *pData, int nLen);
protected:
	int ParseH264Frame(CBuffer *pBufferH264, CBuffer *pBufferFrame,int &nFrameType);
	//
	CBuffer *m_pBufferStream;
};
