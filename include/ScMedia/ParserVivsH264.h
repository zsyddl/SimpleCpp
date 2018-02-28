#pragma once
#include "PackageParser.h"
#include "MediaSdkDef.h"

#ifdef WIN32
#ifdef VIVSMEDIABOX_EXPORTS
#define MEDIASDK_API __declspec(dllexport)
#else
#define MEDIASDK_API __declspec(dllimport)
#endif
#else
#define MEDIASDK_API
#define __stdcall
#endif

class CDataFilter;
class CMediaFilter;
class MEDIASDK_API CParserVivsH264 :
	public CPackageParser
{
public:
	CParserVivsH264(void);
	virtual ~CParserVivsH264(void);

	virtual int Init(int nBufferCount=10);

	virtual int InputData(char *pData, int nLen);
	virtual int DoFilter();
protected:	
	virtual CMediaFilter *CreateObject();
	virtual int ParsePackage(char *pData, int nLen, CFilterPinBuffer *pOut, int &nOutLen, int &nAvDataEndType);
	int ParseH264Nal(CFilterPinBuffer *pH264Stream, CFilterPinBuffer *pNal);

	//
	//
	bool	m_bParseAvdataEnd;
	int		m_nFTMV;
	//缓存 输入流的buffer
	CFilterPinBuffer *m_pBufferInputTmp;

	//用于保存 网络分包 去除 ftmv 和 avdataend 的 裸流
	CFilterPinBuffer *m_pBufferStreamTmp; 
	//同上， 音频流
	CFilterPinBuffer *m_pBufferAudioStream;
	CFilterPinBuffer *m_pBufferVideoStream;
	bool    m_bFrameEnd;
	//保存 当前的 avdata end 提供给 接下来的 nal分包使用。
	sAvDataEnd m_avDataEnd;
};

extern CParserVivsH264 g_parserVivsH264;
