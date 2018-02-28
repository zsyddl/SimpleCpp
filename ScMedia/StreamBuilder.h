#pragma once

#include "MediaSdkDef.h"
#include "MediaSdk.h"

class CMediaStream;

// class CMediaComponentMananger{
// 	
// public:
// 	CMediaComponentMananger();
// 	~CMediaComponentMananger();
// 	
// protected:
// 
// };
class CDecoder;
class CRender;
class CPackageParser;
class CDataFilter;
class CBaseStream;

class VIVSMEDIABOX_API CStreamBuilder
{
public:
	CStreamBuilder(void);
	virtual ~CStreamBuilder(void);

	int  BuildStream(char *pInfo, CMediaStream *pStream, HWND hWnd);
	int  BuildStream(sVivsStreamInfo *pStreamInfo, CMediaStream *pStream, HWND hWnd);
	int  AllocParser(sVivsStreamInfo *pStreamInfo, CPackageParser **pParser);
	int  AllocDecoder(sVivsStreamInfo *pStreamInfo, CDecoder **pDecoder);
	int  AllocRender(sVivsStreamInfo *pStreamInfo, CRender **pRender, HWND hWnd, int nPlayBuffer);

	int  FreeParser(CPackageParser *pParser);
	int  FreeDecoder(CDecoder *pDecoder);
	int  FreeRender(CRender *pRender);
	//
	int AllocFilter(char *pSzKind, char *pSzType, CDataFilter **pFilter);
	int FreeFilter(CDataFilter *pFilter);
	int BuildStream(char *pInfo, CBaseStream *pStream );

	static CStreamBuilder *GetDefaultStreamBuilder();
protected:

};
