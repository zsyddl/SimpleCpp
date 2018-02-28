#pragma once
#include "PackageParser.h"

class CPackageParser;
class CParserPsH264 :
	public CPackageParser
{
public:
	CParserPsH264(void);
	virtual ~CParserPsH264(void);
	virtual int Open(int nBufferCount=100);

	virtual int InputData(char *pData, int nLen);
protected:
	int ParsePsPackage();
	int ParseH264Nal();

	CFilterPinBuffer *m_pBufferStream;
	CFilterPinBuffer *m_pBufferH264;
};
