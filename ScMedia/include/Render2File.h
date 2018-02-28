#pragma once
#include "render.h"
#include <vector>
#include "MediaSdkDef.h"

using namespace std;
//渲染到文件
#define FILTER_RENDER_RENDER_TO_FILE "Render2File"
//一般是压缩数据 写文件 
class CFfmpegFormat;
typedef struct _sFfmpegFrameInfo sFfmpegFrameInfo;
typedef struct _sFfmpegStreamInfo sFfmpegStreamInfo;
class CRender2File :
	public CRender
{
public:
	CRender2File(void);
	virtual ~CRender2File(void);

	virtual int Open(HWND hWnd, int nPlayBuffer);
	virtual int Open(char *pFile2Save, vector<sFfmpegStreamInfo*> &lstStreams);
	virtual int Open(char *pSzParam);

	virtual int DoFilter();
protected:
	virtual CMediaFilter *CreateObject();

	bool  CheckStreamChanged(sFfmpegFrameInfo *videoFrameInfo);
	int   ReCreate();

protected:
	CFfmpegFormat *m_pMediaFile;
	FILE *m_pRawFile;
	char m_szSaveFile[256];
	char m_szFileType[16];

	sFfmpegStreamInfo *m_pStreamInfo;
};
