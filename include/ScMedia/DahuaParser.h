#pragma once
#include "PackageParser.h"
#include "MediaSdkDef.h"

#define DAHUA_PKG_MIN_LEN  (16*1024)
#define DAHUA_PKG_MAX_LEN  (4*1024*1024)
namespace SimpleCpp{
class CBuffer;
}
using namespace SimpleCpp;

class CDahuaParser :
	public CPackageParser
{
public:
	CDahuaParser(void);
	virtual ~CDahuaParser(void);

	virtual int Open(int nBufferCount=100);

	virtual int InputData(char *pData, int nLen);

	void SetParseAvdataEnd(bool bParse=false);
protected:
	virtual int ParsePackage(char *pData, int nLen, CFilterPinBuffer *pOut, int &nOutLen, int &nAvDataEndType);

	bool	m_bParseAvdataEnd;
	int		m_nFTMV;
	//保存 当前的 avdata end 提供给 接下来的 nal分包使用。
	sAvDataEnd m_avDataEnd;
	//用于保存 网络分包 去除 ftmv 和 avdataend 的 裸流
	CFilterPinBuffer *m_pBufferStreamTmp; 

	CBuffer *m_pBufferStream;
	//最小包, 太小了 不发送出去,
	int m_nMinLen;
};
