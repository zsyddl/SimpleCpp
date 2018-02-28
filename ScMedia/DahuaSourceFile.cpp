#include "StdAfx.h"
#include "DahuaSourceFile.h"
#include "dhplay.h"
#include "ScBuffer.h"
#include "FfmpegCodec.h"
#include "VideoFramePinBuffer.h"
#include "DahuaFile.h"
#include "FilterPinBuffer.h"
#include "FilterManager.h"

using namespace SimpleCpp;

#ifdef WIN32
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#else
#include <time.h>
#endif

// 00 00 01 B0 I帧前面的数据
// 00 00 01 B5
// 00 00 01 00
// 00 00 01 20
// 00 00 01 B2
// 00 00 01 B6 //I , b, p

#define MPEG4_VOSH_START		0xB0010000
#define MPEG4_VO_START			0xB5010000
#define MPEG4_VOL_START			0x20010000
#define MPEG4_USER_DATA_START	0xB2010000
#define MPEG4_VOP_START			0xB6010000



CDahuaSourceFile g_regDahuaSourceFile;
CDahuaSourceFile::CDahuaSourceFile(void)
: CSourceFilter()
, m_bConverStarted(false)
, m_lPort(-1)
, m_pFile(NULL)
, m_pFfmpegCodec(NULL)
, m_pFfmpegFileSaveToAvi(NULL)
, m_s64FileCurPos(0)
, m_s64FileLen(0)
, m_bPriv(false)
, m_pDahuaFile(NULL)
{
	strcpy(m_szType, FILTER_NAME_SOURCE_DAHUA_FILE);
	strcpy(m_szName, FILTER_NAME_SOURCE_DAHUA_FILE);

	strcpy(m_szFormatName, AvFormat_Dahua);

	m_buffRead.SetMaxSize(512*1024);
	m_buffRead.SetAllocSize(512*1024);
	memset(&m_streamInfo, 0, sizeof(m_streamInfo) ); 
	m_streamInfo.m_nStreamType = AVMEDIA_TYPE_UNKNOWN;
	CFilterManager::RegistFilter(&g_regDahuaSourceFile);
}

CDahuaSourceFile::~CDahuaSourceFile(void)
{

	if (m_bConverStarted && m_lPort> 0)
	{
		PLAY_StopAVIConvert(m_lPort);
		m_lPort = 0;
		PLAY_Stop(m_lPort);
		PLAY_CloseFile(m_lPort);
	}
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pFfmpegCodec)
	{
		m_pFfmpegCodec->Close();
		delete m_pFfmpegCodec;
		m_pFfmpegCodec = NULL;
	}
	if (m_pFfmpegFileSaveToAvi)
	{
		m_pFfmpegFileSaveToAvi->Close();
		delete m_pFfmpegFileSaveToAvi;
		m_pFfmpegFileSaveToAvi = NULL;
	}
	if (m_pDahuaFile)
	{
		m_pDahuaFile->Close();
		delete m_pDahuaFile;
		m_pDahuaFile = NULL;
	}
}
CMediaFilter *CDahuaSourceFile::CreateObject()
{
	return new CDahuaSourceFile();
}
void CALLBACK  CDahuaSourceFile::DahuaDecodeCb(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long nReserved2)
{
	if (pFrameInfo->nType == 0/*FRAME_TYPE_AUDIO*/ )
	{
		return ;
	}

	//debug("%s\n", __FUNCTION__);
	CDahuaSourceFile *pDhFile = (CDahuaSourceFile*) nUser;
	pDhFile->m_nTimePerFrame = 1000/pFrameInfo->nFrameRate;

	if (pDhFile->GetState() != eFilterState_Run)
		return ;

	CFilterPin *pPinOut = pDhFile->m_lstPinOut.at(0);
	if (!pPinOut)
		return;
	while (pPinOut->GetFreeBufferCount() <= 0
		&& pDhFile->GetState() == eFilterState_Run)
	{
		Sleep(2);
	};

	CFilterPinBuffer  *pPinBuffer = NULL;
	pPinOut->GetFreeBuffer(&pPinBuffer);
	if (!pPinBuffer)
		return ;

	CVideoFramePinBufer *pVideoFrame = 0;
	pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pPinBuffer);
	if (!pVideoFrame)
	{
		pPinOut->PutFreeBuffer(pPinBuffer);
		return ;
	}
	pPinOut->SetAutoFreeMemory(0);
	// copy yuv to video frame;
	pVideoFrame->SetFrameSize(pFrameInfo->nWidth, pFrameInfo->nHeight);
	pVideoFrame->SetUsedSize(pFrameInfo->nWidth*pFrameInfo->nHeight*3);

	CBuffer *pBufferExt = pVideoFrame->GetExtData();
	if (pBufferExt->GetAllocSize() < sizeof(sFfmpegFrameInfo))
	{
		pBufferExt->SetAllocSize(sizeof(sFfmpegFrameInfo));
		pBufferExt->SetUsed(sizeof(sFfmpegFrameInfo));

	}
	sFfmpegFrameInfo *pFfmpegFrameInfo = (sFfmpegFrameInfo*)pBufferExt->GetData();
	memset(pFfmpegFrameInfo, 0, sizeof(sFfmpegFrameInfo));
	//这个是毫秒，需要转换到 时间戳，时间戳 是 timebase 的一个计数

	pFfmpegFrameInfo->m_nTimeDen = pFrameInfo->nFrameRate;
	if (pFfmpegFrameInfo->m_nTimeDen <= 0)
		pFfmpegFrameInfo->m_nTimeDen = 25;
	pFfmpegFrameInfo->m_nTimeNum = 1;;
	pFfmpegFrameInfo->m_s64Pts = pFrameInfo->nStamp/(1000*pFfmpegFrameInfo->m_nTimeNum/pFfmpegFrameInfo->m_nTimeDen);//PlayM4_GetPlayedTime(pHkFile->m_nHkStreamHandle);
	//debug("%s, timestamp: time %d,->%d\n ", __FUNCTION__,  pFrameInfo->nStamp, pFfmpegFrameInfo->m_s64Pts);

	pDhFile->m_s64CurrTime = pFrameInfo->nStamp;

	pFfmpegFrameInfo->m_videoInfo.m_nWidth = pFrameInfo->nWidth;
	pFfmpegFrameInfo->m_videoInfo.m_nHeight = pFrameInfo->nHeight;
	pFfmpegFrameInfo->m_s64Bts = 0;
	pFfmpegFrameInfo->m_videoInfo.m_nFps = pFrameInfo->nFrameRate;
	pBufferExt->SetUsed(sizeof(sFfmpegFrameInfo));

	//memcpy(pVideoFrame->m_pY, (char *)pFrameDecodeInfo->pVideoData[0], pFrameInfo->nWidth * pFrameInfo->nHeight);
	//memcpy(pVideoFrame->m_pV, (char *)(pFrameDecodeInfo->pVideoData[1]), pFrameInfo->nWidth * pFrameInfo->nHeight/4);
	//memcpy(pVideoFrame->m_pU, (char *)(pFrameDecodeInfo->pVideoData[2]), pFrameInfo->nWidth * pFrameInfo->nHeight/4);


	pPinOut->PutPreparedBuffer(pVideoFrame);
}
int CDahuaSourceFile::Open(char *pSzParam/* =0 */, bool bCreateThread)
{
	CSourceFilter::Open(pSzParam);

	char *pSource = const_cast<char*>(GetParam("source"));
	if (!pSource)
		return -1;
#ifdef DH_SELF
	if (!PLAY_GetFreePort(&m_lPort) )
	{
		return -2;
	}
	if (!PLAY_OpenFile(m_lPort, pSource) )
		return -3;

	char *pSaveTo = const_cast<char*>(GetParam("saveto"));
#else
	if (m_pFile == NULL)
	{
		m_pFile = fopen(pSource, "r+b");
	}
	if (!m_pFfmpegCodec)
	{
		m_pFfmpegCodec = new CFfmpegCodec();
	}
	int nRet = 0;
	nRet = ProbMediaInfo(pSource);
	if (nRet >= 0 )
	{
		if (!m_bPriv)
		{//大华 mpeg4 流
			if (!m_pDahuaFile)
			{
				m_pDahuaFile = new CDahuaFile();

				nRet = m_pDahuaFile->Open(pSource, AVIO_FLAG_READ);
				if (nRet < 0)
				{
					debug("%s, open dahua file false\n", __FUNCTION__);
				}
				else
				{
					debug("%s, open dahua file success\n", __FUNCTION__);
				}
				m_bNeedFix = m_pDahuaFile->IsNeedFix();
				if (!m_bNeedFix)
				{
					m_s64TotalTime = m_pDahuaFile->GetTotalTime();
				}
				else
				{//需要修复
					//m_pDahuaFile->SetTimebase(m_streamInfo.m_nTimebaseNum, m_streamInfo.m_nTimebaseDen);
					m_pDahuaFile->SetVideoWidth(m_nW);
					m_pDahuaFile->SetVideoHeight(m_nH);
				}
			}
		}
		else
		{//dahua 私有的
			if (!PLAY_GetFreePort(&m_lPort) )
			{
				return -2;
			}
			if (!PLAY_OpenFile(m_lPort, pSource) )
				return -3;
			
			PLAY_SetDecCallBackEx(m_lPort, DahuaDecodeCb, (long)this);

			if (!PLAY_Play(m_lPort, (HWND)0))
			{
				debug("%s: PlayM4_Play false\n", __FUNCTION__);
				return -1;
			}
			//输出是 解码的yuv
			CFilterPin *pPinOut = m_lstPinOut.at(0);
			if (!pPinOut)
				return -1;

			pPinOut->Clear();

			pPinOut->CreateBuffer<CVideoFramePinBufer>(4);
		}
	}



	sFfmpegStreamInfo streamInfo;
	streamInfo.m_nDataType = AVMEDIA_TYPE_VIDEO;
	streamInfo.m_videoInfo.m_nBitRate = 200*1024;
	streamInfo.m_videoInfo.m_nHeight = m_streamInfo.m_videoInfo.m_nHeight;
	streamInfo.m_videoInfo.m_nWidth = m_streamInfo.m_videoInfo.m_nWidth;
	streamInfo.m_nCodecId = CODEC_ID_MPEG4;
	streamInfo.m_videoInfo.m_nFps = m_streamInfo.m_videoInfo.m_nFps;
	streamInfo.m_videoInfo.m_nGopSize = m_streamInfo.m_videoInfo.m_nGopSize;
	streamInfo.m_videoInfo.m_nPixFormat = m_streamInfo.m_videoInfo.m_nPixFormat;
	streamInfo.m_nTimebaseNum = m_streamInfo.m_nTimebaseNum;
	streamInfo.m_nTimebaseDen = m_streamInfo.m_nTimebaseDen;
	//m_pFfmpegFileSrc->GetStreamInfo(streamInfo, 0);
	char *pSaveTo = const_cast<char*>(GetParam("saveto"));
	if (pSaveTo && strlen(pSaveTo) > 0)
	{
		m_pFfmpegFileSaveToAvi = new CFfmpegFormat;
		nRet = m_pFfmpegFileSaveToAvi->Create("avi", false);
		if (nRet < 0)
		{
			goto fail_ret;
		}

		nRet = m_pFfmpegFileSaveToAvi->AddStream(0, streamInfo/**m_pStreamInfo*/);

		if (nRet < 0)
		{
			goto fail_ret;
		}

		nRet = m_pFfmpegFileSaveToAvi->Open(pSaveTo, AVIO_FLAG_READ_WRITE);
		if (nRet < 0 )
		{
			goto fail_ret;
		}
	}
	m_s64Timestamp = 0;
	m_s64TotalTime = m_pDahuaFile->GetTotalTime();
#endif
fail_ret:

	return 0;
}

void CALLBACK  DH_AVIConvertCallback(LONG nPort, LONG lMediaChangeType, void* pUserData, BOOL *pbIfContinue, char *sNewFileName)
{
	debug("%s: ", __FUNCTION__);
}
 
int CDahuaSourceFile::Close()
{
	if (m_bConverStarted && m_lPort> 0)
	{
		PLAY_StopAVIConvert(m_lPort);
		m_lPort = 0;
		PLAY_CloseFile(m_lPort);
	}
	if (m_pDahuaFile)
	{
		m_pDahuaFile->Close();
		delete m_pDahuaFile;
		m_pDahuaFile = NULL;
	}

	return 0;
}

int CDahuaSourceFile::DoFilter()
{

#ifdef DH_SELF
	char *pSaveTo = const_cast<char*>( GetParam("saveto") );
	//
	if (pSaveTo)
	{//转码模式
		if (!m_bConverStarted)
		{

			char *pWindow = const_cast<char*>( GetParam("window") );
			HWND hWnd = 0;
			if (pWindow)
			{
				hWnd = (HWND)atol(pWindow);
			}

			if (!PLAY_Play(m_lPort, hWnd))
			{
				debug("%s: PLAY_Play  false\n", __FUNCTION__);

			}

			bool bRet = true;
			//bRet = PLAY_StartAVIConvert(m_lPort, pSaveTo, DH_AVIConvertCallback, this);
			if (!bRet)
			{
				debug("%s: start avi convert false \n", __FUNCTION__);
			}
			else
			{
				debug("%s: start avi convert success \n", __FUNCTION__);
				m_bConverStarted = true;
			}

		}
		if (!m_bConverStarted)
			return 0;

		int nTotalFrames = 0;
		nTotalFrames = PLAY_GetFileTotalFrames(m_lPort);
		int nCurFramePos = 0;
		nCurFramePos = PLAY_GetCurrentFrameNum(m_lPort);
		if (m_cbProgress)
		{
			m_cbProgress(m_fSpeed, nCurFramePos, nTotalFrames,  m_pCbProgressUserData, m_pCbProgressReversed);
		}
	}
	else
	{

	}
#else
	long lTimeNow = timeGetTime();
	if (m_fSpeed > 16)
	{//不再控制速度
		m_nNeedSleep = 0;
	}
	else if (lTimeNow - m_lTimeLastRead < m_nTimePerFrame/m_fSpeed)
	{//不到时间
		Sleep(5);
		return CSourceFilter::DoFilter();
	}
	CFilterPin *pPinOut =  m_lstPinOut.at(0);
	if (pPinOut->GetFreeBufferCount() <= 0)
	{
		return CSourceFilter::DoFilter();
	}

	char *pSaveTo = const_cast<char*>( GetParam("saveto") );
	CBuffer buffFrame;
	
	if (pSaveTo && strlen(pSaveTo) > 0)
	{//需要转码的时候不检查输出pin。

	}
	else
	{
		if (!pPinOut && pPinOut->GetFreeBufferCount() <= 0)
			goto ret;
	}
	
// 	if (pPinOut->GetFreeBufferCount() <= 0)
// 		goto ret;
	
	int nFrameType;
	int64 s64Timestamp = 0;
	int   nStreamId = 0;
	if (!m_bPriv)
	{
		if (m_pDahuaFile)
		{	
			int nReadLen = 0;
			buffFrame.SetAllocSize(1024*1024);
			int nMaxLen = buffFrame.GetAllocSize();
			buffFrame.SetUsed(0);
			nReadLen = m_pDahuaFile->Read((char*)buffFrame.GetData(), nMaxLen, s64Timestamp, nStreamId, nFrameType);
			
			m_s64CurrTime = m_pDahuaFile->GetCurrTime();
			m_s64TotalTime = m_pDahuaFile->GetTotalTime();

			if (nReadLen >= 0)
			{
				buffFrame.SetUsed(nReadLen);
			}
			else
			{
				m_bIsStreamEnd = true;
			}
		}else{
			ReadFrame(buffFrame, nFrameType);
		}
	}
	
	m_s64FileCurPos = _ftelli64(m_pFile);

	m_lTimeLastRead = lTimeNow;
	if (buffFrame.GetUsedSize() > 0)
	{
		int nRet = 0;
		//
		if (m_pFfmpegFileSaveToAvi)
		{
			nRet = m_pFfmpegFileSaveToAvi->WriteFrame((char*)buffFrame.GetData(), buffFrame.GetUsedSize(),
				m_s64Timestamp, 0/*, 0*/);
			if (nRet < 0)
			{
				debug("%s: write frame false\n", __FUNCTION__);
			}
			m_s64Timestamp++;
		}

		//
		CFilterPinBuffer *pPinBuffer(NULL);
		
		pPinOut->GetFreeBuffer(&pPinBuffer);
		pPinBuffer->Clear();
		pPinBuffer->Append((char*)buffFrame.GetData(), buffFrame.GetUsedSize());
		
		CBuffer *pBufferEx = pPinBuffer->GetExtData();
		if (!pBufferEx)
		{
			pPinOut->PutFreeBuffer(pPinBuffer);
			goto ret;
		}
		pBufferEx->SetUsed(sizeof(sFfmpegFrameInfo));
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pBufferEx->GetData();
		//创建解码器
		pFrameInfo->m_nDataType = AVMEDIA_TYPE_VIDEO;
		pFrameInfo->m_videoInfo.m_nCodecId = m_streamInfo.m_videoInfo.m_nCodecId;
		pFrameInfo->m_videoInfo.m_nWidth = m_streamInfo.m_videoInfo.m_nWidth;
		pFrameInfo->m_videoInfo.m_nHeight = m_streamInfo.m_videoInfo.m_nHeight;
		pFrameInfo->m_videoInfo.m_nFps = 25;
		pFrameInfo->m_videoInfo.m_nGopSize = 25;
		pFrameInfo->m_videoInfo.m_nPixFormat = AV_PIX_FMT_YUV420P;
		if (m_pDahuaFile)
		{
			//pFrameInfo->m_nTimeNum = 1;
			//pFrameInfo->m_nTimeDen = 25;
			m_pDahuaFile->GetTimebase(pFrameInfo->m_nTimeNum, pFrameInfo->m_nTimeDen);
		}
		else
		{
			pFrameInfo->m_nTimeNum = 1;
			pFrameInfo->m_nTimeDen = 25;
		}
		pFrameInfo->m_s64Pts = s64Timestamp;
		pFrameInfo->m_s64Dts = s64Timestamp;
		pFrameInfo->m_s64Bts = 0;
	
		if (m_pDahuaFile)
		{
		}else{
			pFrameInfo->m_nFrameType = CheckFrameType((char*)buffFrame.GetData(), buffFrame.GetUsedSize());
		}
		 
		pPinOut->PutPreparedBuffer(pPinBuffer);
		

		if (m_cbProgress)
		{
			if (m_pDahuaFile)
			{
				m_cbProgress(m_fSpeed, m_pDahuaFile->GetCurrTime(), m_pDahuaFile->GetTotalTime(), m_pCbProgressUserData, m_pCbProgressReversed);
			}else{
				m_cbProgress(m_fSpeed, m_s64FileCurPos, m_s64FileLen, m_pCbProgressUserData, m_pCbProgressReversed);
			}
		}
	}
	else 
	{
		if (m_bIsStreamEnd)
		{//结束了

			if (m_cbFinished)
			{
				m_cbFinished(m_pCbFinishedUserData, m_pCbFinishedReversed);
			}
		}
	}
#endif

ret:

	return CSourceFilter::DoFilter();
}
// 00 00 01 B0 I帧前面的数据
// 00 00 01 B5
// 00 00 01 00
// 00 00 01 20
// 00 00 01 B2

// 00 00 01 B6 //I , b, p
// 
int CDahuaSourceFile::ReadFrame( CBuffer &buffer, int &nFrameType, FILE *pFile )
{
	if (!pFile)
	{
		pFile = m_pFile;
	}
	if (pFile == NULL)
		return -1;

	int nPos0(-1), nPos1(-1);
	uint8_t *pBuffer = NULL;
	int nBufferLen = 0;
	int nMaxPos = 0;
	bool bFileEnd = false;
	do{
		pBuffer = m_buffRead.GetData();
		nBufferLen = m_buffRead.GetUsedSize();
		nMaxPos = nBufferLen-4;

		unsigned int *pTmp = NULL;
		for (int i=0; i<nMaxPos; i++, pBuffer++)
		{
			pTmp = (unsigned int*)pBuffer;
			if ( *pTmp == MPEG4_VOSH_START
				|| *pTmp == MPEG4_VOP_START)
			{
				nPos0 = i;
				nFrameType = *pTmp;
				break;
			}
		}
		if (nPos0 == -1)
		{
			m_buffRead.Clear();
			int nRead = fread(m_buffRead.GetData(), 1, m_buffRead.GetAllocSize(),  pFile);
			
			if (nRead <= 0)
			{
				//debug("%s: read end of file\n", __FUNCTION__);
				m_buffRead.Clear();
				return -1;
			}
			m_buffRead.SetUsed(nRead);
		}
	}while (nPos0 == -1);

	do 
	{
		pBuffer = m_buffRead.GetData() + nPos0  + 4;;
		nMaxPos = m_buffRead.GetUsedSize()-4;
		for (int i=nPos0+4 ; i<nMaxPos; i++, pBuffer++)
		{
			unsigned int *pTmp = (unsigned int*)pBuffer;
			if ( *pTmp == MPEG4_VOSH_START
				|| *pTmp == MPEG4_VOP_START)
			{
				nPos1 = i;
				break;
			}
		}
		if (nPos1 == -1)
		{
			int nFreeSize = m_buffRead.GetAllocSize() - m_buffRead.GetUsedSize();
			int nRead = fread(m_buffRead.GetData()+m_buffRead.GetUsedSize(), 1, nFreeSize,  pFile);

			if (nRead <= 0)
			{
				//debug("%s: read end of file\n", __FUNCTION__);
				buffer.Clear();
				buffer.Append(m_buffRead.GetData() , m_buffRead.GetUsedSize());
				m_buffRead.Clear();
				return -1;
			}
			m_buffRead.SetUsed(nRead+m_buffRead.GetUsedSize());
		}
	} while ( nPos1 == -1 );

	if (nPos0 != -1
		&& nPos1 != -1 )
	{
		buffer.Clear();
		buffer.Append(m_buffRead.GetData() + nPos0, nPos1-nPos0);
		m_buffRead.Clear(0, nPos1);
	}
	return 0;
}
 

int CDahuaSourceFile::ParseMediaInfo(char *pSzFile)
{
	CFfmpegCodec *pFfmpegCodec(0);
	if (!pFfmpegCodec)
	{
		pFfmpegCodec = new CFfmpegCodec();
	}
	FILE *pFile(0);
	if (!pFile)
	{
		pFile = fopen(pSzFile, "r+b");
	}
	if (!pFile)
	{
		delete pFfmpegCodec;
		return -1;
	}
	
	_fseeki64(pFile, 0, SEEK_END );
	m_s64FileLen = _ftelli64(pFile);
	_fseeki64(pFile, 0, SEEK_SET );

 	m_streamInfo.m_videoInfo.m_nCodecId = AV_CODEC_ID_MPEG4;
// 	m_streamInfo.m_videoInfo.m_nHeight = 576;
// 	m_streamInfo.m_videoInfo.m_nWidth = 720;
	pFfmpegCodec->SetVideoParam(m_streamInfo.m_videoInfo);
	pFfmpegCodec->SetDahuaFlag();
	int nRet = pFfmpegCodec->Open();
	if (nRet < 0)
	{
		return -2;
	}
	m_buffRead.Clear();
	CBuffer buffFrame;
	int nRetDecode = 0;
	int nTries = 100;
	int nWidth = 0, nHeight=0;
#define  WIDTH_FLAG_POS 23
	do 
	{
		buffFrame.Clear();
		int nFrameType;
		ReadFrame(buffFrame, nFrameType, pFile);
		if (nFrameType == MPEG4_VOSH_START)
		{
			unsigned char *exdata = buffFrame.GetData();
			
			int nTmp = 0;
			nTmp = ((exdata[WIDTH_FLAG_POS] & 0x03) << 12 );
			nWidth = nTmp;
			nTmp =(exdata[WIDTH_FLAG_POS+1]<<3);
			nWidth += nTmp;
			nTmp =  ((exdata[WIDTH_FLAG_POS+2]&0xE0) >> 5);
			//exdata[WIDTH_FLAG_POS] |= ( (0x1800&nWidth)>>12 ) ; //最高两位
			//exdata[WIDTH_FLAG_POS+1] = ((0x7f8&nWidth)>>3 );
			//exdata[WIDTH_FLAG_POS +2] |= ((0x7&nWidth)<<5);
			// height; //低四位开始
			//exdata[WIDTH_FLAG_POS +2] |= ((0x1e00&nHeight)>>9 );
			//exdata[WIDTH_FLAG_POS+3] = ((0x1fe&nHeight)>>1 ) ;
			//exdata[WIDTH_FLAG_POS+4] |= ((0x01*nHeight)<<7 );
		}
		nRetDecode = pFfmpegCodec->Decode(buffFrame.GetData(), buffFrame.GetUsedSize());
		if (nRetDecode < 0)
		{
			
		}
		else
		{
			break;
		}
	} while (nTries--);
	
	fseek(pFile, 0, SEEK_SET);
	m_buffRead.Clear();
	m_streamInfo.m_videoInfo.m_nCodecId = AV_CODEC_ID_MPEG4;
	m_streamInfo.m_nCodecId = AV_CODEC_ID_MPEG4;
	//m_streamInfo.m_videoInfo.m_nWidth = m_pFfmpegCodec->GetVideoFrameSize();
	AVCodecContext *pCodecContext = pFfmpegCodec->GetCodecContext();
	pFfmpegCodec->GetVideoSize(m_streamInfo.m_videoInfo.m_nWidth, m_streamInfo.m_videoInfo.m_nHeight);
	m_streamInfo.m_videoInfo.m_nWidth = pCodecContext->width;
	m_streamInfo.m_videoInfo.m_nHeight = pCodecContext->height;
	m_streamInfo.m_videoInfo.m_nGopSize = pCodecContext->gop_size;
	m_streamInfo.m_videoInfo.m_nFps = pCodecContext->time_base.num;
	if (m_streamInfo.m_videoInfo.m_nFps == 0)
	{
		m_streamInfo.m_videoInfo.m_nFps = 1;
	}
	m_streamInfo.m_nTimebaseDen = pCodecContext->time_base.den;
	m_streamInfo.m_nTimebaseNum = pCodecContext->time_base.num;
	m_streamInfo.m_videoInfo.m_nPixFormat = pCodecContext->pix_fmt;

	m_nTimePerFrame = 1000*m_streamInfo.m_nTimebaseNum/m_streamInfo.m_nTimebaseDen;
	m_nW = m_streamInfo.m_videoInfo.m_nWidth ;
	m_nH = m_streamInfo.m_videoInfo.m_nHeight;

	if (pFfmpegCodec)
	{
		pFfmpegCodec->Close();
		delete pFfmpegCodec;
		pFfmpegCodec = NULL;
	}
	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}

	return 0;
}

int MemFind(unsigned char *pMem, int nLen, unsigned char *pTarget, int nTargetLen )
{
	int nPos = -1;
	
	for (int i=0; i<nLen; i++)
	{
		unsigned char *pMemTmp = pMem+i;
		bool bMatch = true;
		for (int j=0; j<nTargetLen; j++)
		{
			if (pTarget[j] != pMemTmp[j])
			{
				bMatch = false;
			}
		}
		if (bMatch)
		{
			nPos = i;
			break;
		}
	}

	return nPos;
}

int CDahuaSourceFile::ProbMediaInfo( char *pSzFile )
{
	m_nW = 0;
	m_nH = 0;
	m_s64TotalTime = 0;
	memset(&m_streamInfo, 0, sizeof(m_streamInfo));


	int nRet = -1;
	FILE *pFile = fopen(pSzFile, "r+b");
	if (!pFile)
		return -1;

	CBuffer bufferRead(512*1024);
	int nRead = 0;
	nRead = fread(bufferRead.GetData(), 1, bufferRead.GetAllocSize(), pFile);
	bufferRead.SetUsed(nRead);
	int nFilePathLen = 0;
	nFilePathLen = strlen(pSzFile);

	CDahuaFile dahuaFile;
	if (MemFind(bufferRead.GetData(), bufferRead.GetUsedSize(), (unsigned char*)DahuaFileFlag1, strlen(DahuaFileFlag1)) >= 0)
	{
		debug("%s: dahua file\n", __FUNCTION__);
	}
	else
	{
		nRet = -2;
		goto ret;
	}

	unsigned char *pBuffer = bufferRead.GetData();
	int nMaxPos = bufferRead.GetUsedSize()-4;
	bool bFindMpeg4Flag = false;
	for (int i=0; i<nMaxPos; i++)
	{
		unsigned int *pTemp = (unsigned int*)pBuffer+i;
		if (*pTemp == MPEG4_VOP_START
			|| *pTemp == MPEG4_VOSH_START)
		{
			nRet = 0;
			bFindMpeg4Flag = true;
			break;
		}
	}
	if (bFindMpeg4Flag)
	{//大华 mpeg4 文件
		strcpy(m_szFormatSubName, "mpeg4");
	}
	else
	{//大华私有文件, 暂时没找到 这个情况
		strcpy(m_szFormatSubName, "dahua_priv");
		m_bPriv = true;
	}
	

	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	// 大华 mp4 格式 
	//  
	CFfmpegFormat *pFfmpegFormat = new CFfmpegFormat;
	if (pFfmpegFormat->Open(pSzFile) >= 0)
	{
		nRet = 0;
		//
		AVStream *pStream;
		int nStreams = 0;
		nStreams = pFfmpegFormat->GetStreamCount();
		for (int i=0; i<nStreams; i++)
		{
			pStream = pFfmpegFormat->GetStream(i);
			if (pStream->codec->codec_type != AVMEDIA_TYPE_VIDEO)
				continue;

			m_nW = pStream->codec->width;
			m_nH = pStream->codec->height;
			break;
		}
		m_s64TotalTime = pFfmpegFormat->GetTotalTime();

		AVFormatContext *pFormatContext = pFfmpegFormat->GetFormatContext();
		if (pFormatContext)
		{
			strcpy(m_szFormatSubName, pFormatContext->iformat->name );
		}
		else
		{
			nRet = -1;
		}
	}
	pFfmpegFormat->Close();
	delete pFfmpegFormat;

	nRet = ParseMediaInfo(pSzFile);
	if (nRet < 0)
	{
		debug("parse media info false");
	}
	else
	{
		debug("%s, parse media info success, wxh: %dx%d\n", __FUNCTION__, m_nW, m_nH);
	}

	
	nRet = dahuaFile.Open(pSzFile, AVIO_FLAG_READ);
	if (nRet >= 0)
	{
		debug("%s, dahua file open success\n", __FUNCTION__);
		m_s64TotalTime = dahuaFile.GetTotalTime();
	}
	else{
		debug("%s, dahua file open false\n", __FUNCTION__);
	}

ret:
	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}

	return nRet;
}

int CDahuaSourceFile::CheckFrameType( char *pFrame, int nLen )
{
	int nFrameType = eFFT_I;
	unsigned char nFlag = pFrame[4];
	switch(nFlag & (BYTE)0xc0)
	{
	case 0x00:
		//I Frame
		nFrameType = eFFT_I;
		break;
	case 0x40:
		//P Frame
		nFrameType = eFFT_P;
		break;
	case 0x80:
		//B Frame
		nFrameType = eFFT_B;
		break;
	default:
		break;
	}
	
	return nFrameType;
}
int CDahuaSourceFile::Seek(int64 s64Time, int nFlag/*=0*/ )
{
	 if (m_pDahuaFile)
	 {
		 m_pDahuaFile->Seek(s64Time, nFlag);
	 }
	 m_bIsStreamEnd = false;
	 m_lTimeLastRead = timeGetTime();

	return CSourceFilter::Seek(s64Time);
}
//外面一律使用相对视频的 其实时间戳
int CDahuaSourceFile::SeekToTimestamp(int64 s64Timestamp, int nFlag/*=0*/ )
{
	
	if (m_pDahuaFile)
	{
		m_pDahuaFile->SeekToTimestamp(s64Timestamp, nFlag);
	}
	m_bIsStreamEnd = false;
	m_lTimeLastRead = timeGetTime();

	return CSourceFilter::SeekToTimestamp(s64Timestamp);
}
 int64 CDahuaSourceFile::TimestampToTime(int64 s64Timestamp)
{
	int64 s64Time = 0;
	if (m_pDahuaFile)
	{
		s64Time = m_pDahuaFile->TimestampToTime(s64Timestamp);
	}
	return s64Time;
}
int64 CDahuaSourceFile::TimeToTimestamp(int64 s64Time)
{
	int64 s64Timestamp = 0;
	if (m_pDahuaFile)
	{
		s64Timestamp = m_pDahuaFile->TimeToTimestamp(s64Time);
	}
	return s64Timestamp;
}

int CDahuaSourceFile::PlayNextFrame()
{
	CFilterPin *pPinOut = 0;
	pPinOut = m_lstPinOut.at(0);
	if (pPinOut->GetFreeBufferCount() <= 0)
		return -1;

	CBuffer  buffFrame;
	int nFrameType;
	int64 s64Timestamp = 0;
	int   nStreamId = 0;
	if (!m_bPriv)
	{
		if (m_pDahuaFile)
		{	
			int nReadLen = 0;
			buffFrame.SetAllocSize(1024*1024);
			int nMaxLen = buffFrame.GetAllocSize();
			nReadLen = m_pDahuaFile->Read((char*)buffFrame.GetData(), nMaxLen, s64Timestamp, nStreamId, nFrameType);
			buffFrame.SetUsed(nReadLen);
		}else{
			ReadFrame(buffFrame, nFrameType);
		}
	}
	
	m_s64FileCurPos = _ftelli64(m_pFile);

	m_lTimeLastRead = timeGetTime();
	if (buffFrame.GetUsedSize() > 0)
	{
		int nRet = 0;
		//
		if (m_pFfmpegFileSaveToAvi)
		{
			nRet = m_pFfmpegFileSaveToAvi->WriteFrame((char*)buffFrame.GetData(), buffFrame.GetUsedSize(),
				m_s64Timestamp, 0/*, 0*/);
			if (nRet < 0)
			{
				debug("%s: write frame false\n", __FUNCTION__);
			}
			m_s64Timestamp++;
		}

		//
		CFilterPinBuffer *pPinBuffer(NULL);
		
		pPinOut->GetFreeBuffer(&pPinBuffer);
		pPinBuffer->Clear();
		pPinBuffer->Append((char*)buffFrame.GetData(), buffFrame.GetUsedSize());
		
		CBuffer *pBufferEx = pPinBuffer->GetExtData();
		if (!pBufferEx)
		{
			pPinOut->PutFreeBuffer(pPinBuffer);
			goto ret;
		}
		pBufferEx->SetUsed(sizeof(sFfmpegFrameInfo));
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pBufferEx->GetData();
		//创建解码器
		pFrameInfo->m_nDataType = AVMEDIA_TYPE_VIDEO;
		pFrameInfo->m_videoInfo.m_nCodecId = m_streamInfo.m_videoInfo.m_nCodecId;
		pFrameInfo->m_videoInfo.m_nWidth = m_streamInfo.m_videoInfo.m_nWidth;
		pFrameInfo->m_videoInfo.m_nHeight = m_streamInfo.m_videoInfo.m_nHeight;
		pFrameInfo->m_videoInfo.m_nFps = 25;
		pFrameInfo->m_videoInfo.m_nGopSize = 25;
		pFrameInfo->m_videoInfo.m_nPixFormat = AV_PIX_FMT_YUV420P;
		pFrameInfo->m_nTimeNum = 1;
		pFrameInfo->m_nTimeDen = 25;
		pFrameInfo->m_s64Pts = s64Timestamp;
		pFrameInfo->m_s64Dts = s64Timestamp;
		pFrameInfo->m_s64Bts = 0;
		
		if (m_pDahuaFile)
		{
		}else{
			pFrameInfo->m_nFrameType = CheckFrameType((char*)buffFrame.GetData(), buffFrame.GetUsedSize());
		}
		 
		pPinOut->PutPreparedBuffer(pPinBuffer);

		if (m_cbProgress)
		{
			if (m_pDahuaFile)
			{
				m_cbProgress(m_fSpeed, m_pDahuaFile->GetCurrTime(), m_pDahuaFile->GetTotalTime(), m_pCbProgressUserData, m_pCbProgressReversed);
			}else{
				m_cbProgress(m_fSpeed, m_s64FileCurPos, m_s64FileLen, m_pCbProgressUserData, m_pCbProgressReversed);
			}
		}
	}
	else 
	{
		if (m_s64FileLen == m_s64FileCurPos
			&& m_buffRead.GetUsedSize() == 0)
		{//结束了

			if (m_cbFinished)
			{
				m_cbFinished(m_pCbFinishedUserData, m_pCbFinishedReversed);
			}
		}
	}
	CSourceFilter::DoFilter(); 
ret:
	return 0;
}

int CDahuaSourceFile::DoFixFile( int64 &s64Total, int64 &s64Pos )
{
	if (!m_pDahuaFile)
	{
		return -1;
	}
	m_pDahuaFile->DoFixFile(s64Total, s64Pos);

	return 0;
}

int CDahuaSourceFile::Resume()
{
	CSourceFilter::Resume();

	m_lTimeLastRead = timeGetTime();

	
	return 0;
}
 