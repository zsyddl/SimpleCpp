#pragma once
#ifndef MEDIA_INFO_H
#define MEDIA_INFO_H

#include <vector>

using namespace std;
typedef long long int64;


typedef struct _sFfmpegStreamInfo sFfmpegStreamInfo;

class MEDIASDK_API CMediaInfo
{
public:
	CMediaInfo(void);
	~CMediaInfo(void);

	int			Open(char *pSzFile, int &nPos);
	char *		GetFormatName();
	char *		GetFormatSubName();
	char *      GetSourceFilter();
	int			GetWidth();
	int			GetHeight();
	int64		GetTotalTime();

	vector<sFfmpegStreamInfo *> GetStreams();

protected:
	char m_szFormat[32];
	char m_szFormatSub[32];
	char m_szSourceFilter[64];
	vector<sFfmpegStreamInfo *> m_lstStream;
	int  m_nW;
	int  m_nH;
	int64 m_s64TotalTime;
	bool	m_bNeedFix;
};


#endif
