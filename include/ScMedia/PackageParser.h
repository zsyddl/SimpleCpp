#pragma once

#ifndef PACKAGEPARSER_H
#define PACKAGEPARSER_H
#include "MediaFilter.h"
#include "MediaSdkDef.h"


class MEDIASDK_API CPackageParser :
	public CMediaFilter
{
public:
	CPackageParser(void);
	virtual ~CPackageParser(void);

	virtual int Open(int nBufferCount=50);

	virtual int InputData(char *pData, int nLen);

protected:
	virtual CMediaFilter *CreateObject();
};
extern CPackageParser g_parser;

#endif
