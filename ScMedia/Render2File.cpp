#include "StdAfx.h"
#include "Render2File.h"
#include "FfmpegFormat.h"
#include "MediaSdkDef.h"
#include "Buffer.h"
#include "FilterManager.h"

CRender2File g_regRender2File;
CRender2File::CRender2File(void)
: CRender()
, m_pMediaFile(NULL)
, m_pRawFile(NULL)
{
	strcpy(m_szType, FILTER_RENDER_RENDER_TO_FILE);
	strcpy(m_szName, FILTER_RENDER_RENDER_TO_FILE);
	memset(m_szSaveFile, 0, sizeof(m_szSaveFile)) ;
	m_pStreamInfo = new sFfmpegStreamInfo;

	CFilterManager::RegistFilter(&g_regRender2File);
}

CRender2File::~CRender2File(void)
{
	if (m_pRawFile)
	{
		fclose(m_pRawFile);
		m_pRawFile = NULL;
	}
	if (m_pMediaFile)
	{
		m_pMediaFile->Close();
		delete m_pMediaFile;
		m_pMediaFile = NULL;
	}
	if (m_pStreamInfo)
	{
		delete m_pStreamInfo;
		m_pStreamInfo = NULL;
	}
}
int CRender2File::Open(HWND hWnd, int nPlayBuffer)
{

	return 0;
}
int CRender2File::Open(char *pFile2Save, vector<sFfmpegStreamInfo*> &lstStreams)
{	
	if (m_pMediaFile)
		return 1;
	m_pMediaFile = new CFfmpegFormat();

	int nRet = 0;
	nRet = m_pMediaFile->Create("mkv", false);
	if (nRet < 0)
	{
		return -1;
	}

	for (int i=0; i<lstStreams.size(); i++)
	{
		sFfmpegStreamInfo *pStreamInfo = lstStreams.at(i);

		nRet = m_pMediaFile->AddStream(0, *pStreamInfo);

		if (nRet < 0)
		{
			return -2;
		}
	}

	nRet = m_pMediaFile->Open(pFile2Save, AVIO_FLAG_WRITE);
	if (nRet <= 0 )
	{
		return -3;
	}
	return 0;
}
// file=%s video=1 audio=1 
int CRender2File::Open( char *pSzParam )
{

	CMediaFilter::Open(pSzParam);

	//m_pRawFile = fopen("d:\\tmp\\hk2h264.h264", "w+b");

	char szFile[256];
	//sscanf(pSzParam, "file=%s ", szFile);
	
	strcpy(m_szSaveFile, m_mapParamValue["file"].c_str());
	char *pTmp = NULL;
	if ( (pTmp = (char*)GetParam("type")) != NULL)
	{
		strcpy(m_szFileType, pTmp);
	}


	m_pStreamInfo->m_nDataType = AVMEDIA_TYPE_VIDEO;
	m_pStreamInfo->m_videoInfo.m_nBitRate = 2*1024*1024;
	//m_pStreamInfo->m_videoInfo.m_nHeight = 576;
	//m_pStreamInfo->m_videoInfo.m_nWidth = 704;
	//m_pStreamInfo->m_nCodecId = CODEC_ID_H264;
	//m_pStreamInfo->m_videoInfo.m_nFps = 25;
	//m_pStreamInfo->m_videoInfo.m_nGopSize = 25;
	//m_pStreamInfo->m_videoInfo.m_nPixFormat = PIX_FMT_YUV420P;
	//现在不创建。在 收到数据之后 根据 参数创建 
	return 0;
	m_pMediaFile = new CFfmpegFormat();

	int nRet = 0;
	nRet = m_pMediaFile->Create(szFile, false);
	if (nRet < 0)
	{
		return -1;
	}
	
	sFfmpegStreamInfo streamInfo;
	streamInfo.m_nDataType = AVMEDIA_TYPE_VIDEO;
	streamInfo.m_videoInfo.m_nBitRate = 2*1024*1024;
	streamInfo.m_videoInfo.m_nHeight = 576;
	streamInfo.m_videoInfo.m_nWidth = 704;
	streamInfo.m_nCodecId = CODEC_ID_H264;
	streamInfo.m_videoInfo.m_nFps = 25;
	streamInfo.m_videoInfo.m_nGopSize = 25;
	streamInfo.m_videoInfo.m_nPixFormat = PIX_FMT_YUV420P;

	nRet = m_pMediaFile->AddStream(0, streamInfo);

	if (nRet < 0)
	{
		return -2;
	}


	nRet = m_pMediaFile->Open(szFile, AVIO_FLAG_WRITE);
	if (nRet <= 0 )
	{
		return -3;
	}

	return 0;
}

int CRender2File::DoFilter()
{

	CFilterPin *pPinIn = NULL;
	pPinIn = m_lstPinIn.at(0);
	CFilterPinBuffer *pBufferIn = NULL;
	if (pPinIn->GetPreparedBufferConut() <= 0)
		return -2;
	pPinIn->GetPreparedBuffer(&pBufferIn);
	if (!pBufferIn)
		return -3;

	if(m_pRawFile != NULL)
	{
		fwrite(pBufferIn->GetData(), pBufferIn->GetLength(), 1, m_pRawFile);
	}

	sFfmpegFrameInfo *pVideoFrameInfo = NULL;
	CBuffer *pBufferEx = pBufferIn->GetExtData();
	pVideoFrameInfo = (sFfmpegFrameInfo *) pBufferEx->GetData();
	if (pBufferEx || pBufferEx->GetLength() != sizeof(sVideoFrameInfo))
	{
		if (CheckStreamChanged(pVideoFrameInfo))
		{
			ReCreate();
		}
	}

	if (!m_pMediaFile)
	{
		pPinIn->PutFreeBuffer(pBufferIn);

		return -1;
	}
	int nRet = 0;
	if (m_pMediaFile && pVideoFrameInfo)
	{
		//key frame
		int nFrameFlag = 0;
		if (pVideoFrameInfo && pVideoFrameInfo->m_nFrameType == eFFT_I)
		{
			nFrameFlag = AV_PKT_FLAG_KEY;
		}
		nRet = m_pMediaFile->WriteFrame(pBufferIn->GetData(), pBufferIn->GetLength(), 
			pVideoFrameInfo->m_s64Pts,	nFrameFlag);
		if (nRet < 0)
		{
			debug("%s: writeframe false, data len[%d]\n", __FUNCTION__, pBufferIn->GetLength());
		}
	}

	pPinIn->PutFreeBuffer(pBufferIn);

	return CRender::DoFilter();
}

CMediaFilter * CRender2File::CreateObject()
{
	return new CRender2File();
}

bool  CRender2File::CheckStreamChanged(sFfmpegFrameInfo *videoFrameInfo)
{
	if (!m_pMediaFile)
	{

		m_pStreamInfo->m_videoInfo = videoFrameInfo->m_videoInfo;
		m_pStreamInfo->m_nCodecId = videoFrameInfo->m_videoInfo.m_nCodecId;
		if (videoFrameInfo->m_nTimeNum == 0)
		{
			m_pStreamInfo->m_nTimebaseDen = videoFrameInfo->m_videoInfo.m_nFps;
			m_pStreamInfo->m_nTimebaseNum = 1;
		}
		else if (videoFrameInfo->m_nTimeDen/videoFrameInfo->m_nTimeNum > 100
			|| videoFrameInfo->m_nTimeDen/videoFrameInfo->m_nTimeNum < 5)
		{
			m_pStreamInfo->m_nTimebaseDen = videoFrameInfo->m_videoInfo.m_nFps;
			m_pStreamInfo->m_nTimebaseNum = 1;
		}
		else
		{
			m_pStreamInfo->m_nTimebaseDen = videoFrameInfo->m_nTimeDen;
			m_pStreamInfo->m_nTimebaseNum = videoFrameInfo->m_nTimeNum;
		}
		m_pStreamInfo->m_nDataType = videoFrameInfo->m_nDataType;
		return true;
	}
	return false;
}
int   CRender2File::ReCreate()
{
	if (m_pMediaFile)
	{
		m_pMediaFile->Close();
	}
	m_pMediaFile = new CFfmpegFormat;
	int nRet = 0;

	sFfmpegStreamInfo streamInfo;
	streamInfo.m_nDataType = AVMEDIA_TYPE_VIDEO;
	streamInfo.m_videoInfo.m_nBitRate = 2*1024*1024;
	streamInfo.m_videoInfo.m_nHeight = 576;
	streamInfo.m_videoInfo.m_nWidth = 704;
	streamInfo.m_nCodecId = CODEC_ID_H264;
	streamInfo.m_videoInfo.m_nFps = 25;
	streamInfo.m_videoInfo.m_nGopSize = 25;
	streamInfo.m_videoInfo.m_nPixFormat = PIX_FMT_YUV420P;
	streamInfo.m_nTimebaseDen = 25;
	streamInfo.m_nTimebaseNum = 1;


	nRet = m_pMediaFile->Create(m_szFileType, false);
	if (nRet < 0)
	{
		delete m_pMediaFile;
		m_pMediaFile = NULL;
		return -1;
	}

	nRet = m_pMediaFile->AddStream(0,  *m_pStreamInfo);

	if (nRet < 0)
	{
		delete m_pMediaFile;
		m_pMediaFile = NULL;
		return -2;
	}


	nRet = m_pMediaFile->Open(m_szSaveFile, AVIO_FLAG_READ_WRITE);
	if (nRet < 0 )
	{
		delete m_pMediaFile;
		m_pMediaFile = NULL;
		return -3;
	}

	return 0;
}