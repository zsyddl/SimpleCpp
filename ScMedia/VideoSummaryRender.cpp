#include "StdAfx.h"
#include "VideoSummaryRender.h"
#include "Thread.h"
#include "Mutex.h"
#include "FfmpegCodec.h"
#include "3dGraphics.h"
#include "3dItemImage.h"
#include "3dItemVideoSummaryPicture.h"
#include "Buffer.h"
#include "FilterPinBuffer.h"
#include "FilterManager.h"

#include <sys/types.h>
#include <sys/stat.h>

CVideoSummaryRender m_regVideoSummaryRender;
CVideoSummaryRender::CVideoSummaryRender(void)
:CRender()
, m_pThreadRender(NULL)
, m_pImgBkg(NULL)
, m_p3dGraphics(NULL)
, m_nTimeBaseNum(1)
, m_nTimeBaseDen(1)
, m_s64StartTimestamp(0)
, m_pMutexPicPbject(NULL)
{
	strcpy(m_szType, "VideoSummaryRender");
	strcpy(m_szName, "VideoSummaryRender");

	CFilterManager::RegistFilter(&m_regVideoSummaryRender);
}

CVideoSummaryRender::~CVideoSummaryRender(void)
{
	if (m_pThreadRender)
	{
		m_pThreadRender->Stop();
		delete m_pThreadRender;
		m_pThreadRender = NULL;
	}
	ClearObjectItem();

	if (m_pMutexPicPbject)
	{
		delete m_pMutexPicPbject;
		m_pMutexPicPbject = NULL;
	}
	if (m_p3dGraphics)
	{
		delete m_p3dGraphics;
		m_p3dGraphics = NULL;
	}
	if (m_pImgBkg)
	{
		delete m_pImgBkg;
		m_pImgBkg = NULL;
	}
}
int CVideoSummaryRender::Open(HWND hWnd, int nPlayBuffer)
{
	CRender::Open(hWnd, nPlayBuffer);


	int nRet = 0;

	m_p3dGraphics = new C3dGraphics();
	m_p3dGraphics->Create(hWnd);

	m_pImgBkg = new C3dItemImage(m_p3dGraphics);


	m_pMutexPicPbject = new CMutex();
	m_pThreadRender = new CThread();


	nRet = m_pThreadRender->Create(RenderProc, this);

	return nRet;
}
//filter的执行 函数 
int CVideoSummaryRender::DoFilter()
{

	return CRender::DoFilter();
}
int CVideoSummaryRender::Stop()
{
	if (m_state == eFilterState_Stop)
	{
		return -1;
	}
	m_state = eFilterState_ToStop;
	debug("%s ++ \n", __FUNCTION__);
	int nTries = 0;
	while (m_state != eFilterState_Stop)
	{
		nTries++;
		Sleep(10);
	}
	debug("%s -- , tries: %d times\n", __FUNCTION__, nTries);

	return 0;
}

CMediaFilter *CVideoSummaryRender::CreateObject()
{

	return new CVideoSummaryRender();
}
int CVideoSummaryRender::RenderProc(void *pParam)
{
	CVideoSummaryRender *pRender = (CVideoSummaryRender*)pParam;
	if (pRender)
	{
		pRender->RenderProc();
	}
	return 0;
}
int CVideoSummaryRender::RenderProc()
{
	CFfmpegCodec *pCodec = NULL;
	char *pImageTmp = NULL;
	int nFrames = 0;
	while (m_state != eFilterState_Stop
		&& m_state != eFilterState_ToStop)
	{
		if (m_state == eFilterState_Pause)
		{//暂停
			Sleep(10);
			continue;;
		}
		//渲染视频图片
		CFilterPin *pPinIn = m_lstPinIn.at(0);
		if (!pPinIn)
		{
			Sleep(10);
			continue;;
		}
		if (m_nTimePerFrame<=0)
		{
			m_nTimePerFrame = 1000.0/m_nFps;
		}
		//计算 当前实际 的一帧时间
		int nFrameCount = 0;
		long nTimePerFrameNow = m_nTimePerFrame;
		nFrameCount = pPinIn->GetPreparedBufferConut();
		if (nFrameCount <= 0)
		{
			Sleep(10);
			continue;;
		}

		nFrames++;

		//取 一个 pinIn 的 papared 数据
		CFilterPinBuffer *pBufferInPapared = 0;

		pPinIn->GetPreparedBuffer(&pBufferInPapared);
		if (!pBufferInPapared)
		{
			continue;
		}

		CBuffer *pBufferEx =pBufferInPapared->GetExtData();
		if (pBufferEx)
		{//获取信息 
			sFfmpegFrameInfo * pFrameInfo  = (sFfmpegFrameInfo*) pBufferEx->GetData();
			m_nTimeBaseDen = pFrameInfo->m_nTimeDen;
			m_nTimeBaseNum = pFrameInfo->m_nTimeNum;
		}
		//显示图片
		//1 解析 输入的 图片 参数
		//首先解析 背景 
		int nPos = 0, nPosFind(0);
		char *pData = pBufferInPapared->GetData();
		char *pFind = pData;
		
		pFind = strstr(pData, "\n");
		map<string, string> mapParam;
		int nX(0), nY(0), nW(0), nH(0);
		if (pFind)
		{
			ParseParam(pData, pFind-pData, mapParam );
			//debug("bkg=%s;w=%s;h=%s\n", mapParam["bkg"].c_str(), mapParam["w"].c_str(), mapParam["h"].c_str());
			nPos += pFind-pData + 1;
			nW = atoi( mapParam["w"].c_str());
			nH = atoi( mapParam["h"].c_str());
			if (pImageTmp == NULL)
				pImageTmp = new char[nW*nH*3];
			char szStartTimestamp[64];
			memset(szStartTimestamp, 0, sizeof(szStartTimestamp));
			strcpy(szStartTimestamp, mapParam["StartTimestamp"].c_str());
			sscanf(szStartTimestamp, "%I64d", &m_s64StartTimestamp);
			//判断是否解码背景图片放入显示。
			if (strcmp(m_szBkgFile, mapParam["bkg"].c_str()) != 0 )
			{//背景更新
				//
				if (m_pImgBkg->m_nTextureXPix != nW
					|| m_pImgBkg->m_nTextureYPix != nH)
				{//
					m_pImgBkg->Release();
					delete m_pImgBkg;
					m_p3dGraphics->RemoveItem(m_pImgBkg);
					m_pImgBkg = new C3dItemImage(m_p3dGraphics, nW, nH, e3dPF_YV420);
					m_pImgBkg->Create(0, 0, 0);                                                                                                                 
					m_pImgBkg->SetSize(2, 2, 0.01);
					m_p3dGraphics->AddItem2Head(m_pImgBkg);
				}
				/*char *pPic = new char[nW*nH*3];*/
				
				char szFile[256];
				strcpy(szFile, mapParam["bkg"].c_str());
				DecodeJpegFile(szFile, nW, nH, pImageTmp);
				
				m_pImgBkg->SetImageData(pImageTmp, nW, nH, e3dPF_YV420);
				//
				/*delete pPic;*/
			}
		}
		if (0)
		{

			goto next;
		}
		//解析图片
		//该函数带锁
		ClearObjectItem();

		m_pMutexPicPbject->Lock();
		while (m_state != eFilterState_Stop
			&& m_state != eFilterState_ToStop
			&& nPos < pBufferInPapared->GetUsedSize())
		{
			pFind = strstr(pData+nPos, "\n");
			if (pFind == NULL)
			{
				break;
			}
			mapParam.clear();
			ParseParam(pData+nPos, pFind-pData-nPos, mapParam );
			if (mapParam.size() <= 0)
			{
				debug("empty picture\n");
			}
			//debug("pic=%s;x:%s;y=%s;w=%s;h=%s\n", 
			//	mapParam["pic"].c_str(), mapParam["x"].c_str(),mapParam["y"].c_str(), mapParam["w"].c_str(), mapParam["h"].c_str());
			//解码图片，放入显示 
			nW = atoi( mapParam["w"].c_str());
			nH = atoi( mapParam["h"].c_str());
			nX = atoi( mapParam["x"].c_str());
			nY = atoi( mapParam["y"].c_str());

			char szFile[256];
			strcpy(szFile, mapParam["pic"].c_str());
			DecodeJpegFile(szFile, nW, nH, pImageTmp);
			C3dItemVideoSummaryPicture *pNewImageItem = NULL;
			pNewImageItem = new C3dItemVideoSummaryPicture(m_p3dGraphics, nW, nH, e3dPF_YV420);
			pNewImageItem->Create(0, 0, 0);  

			float fX, fY, fW, fH;
			fX = (float)nX*2/m_pImgBkg->m_nTextureXPix;
			fY = (float)nY*2/m_pImgBkg->m_nTextureYPix;
			fW = (float)nW*2/m_pImgBkg->m_nTextureXPix;
			fH = (float)nH*2/m_pImgBkg->m_nTextureYPix;
			pNewImageItem->SetSize(fW, fH, 0.01);
			pNewImageItem->SetPos(fX, fY, 0.0);
			pNewImageItem->SetAlpha(0.8);
			//debug("pic [%d]: x=%.2f, y=%.2f; w=%.2f, h=%.2f\n", nCount, fX, fY, fW, fH);

			pNewImageItem->SetImageData(pImageTmp, nW, nH, e3dPF_YV420);
			pNewImageItem->SetTimeBase(m_nTimeBaseNum, m_nTimeBaseDen);
			pNewImageItem->SetStartTimestamp(m_s64StartTimestamp);

			int64 s64PTS  = 0 ;
			s64PTS = _atoi64( mapParam["pts"].c_str());
			pNewImageItem->SetTimeStamp(s64PTS);

			m_lstObjectPic.push_back(pNewImageItem);

			m_p3dGraphics->AppendItem(pNewImageItem);

			nPos = pFind-pData + 1;
		}
		m_pMutexPicPbject->Unlock();
		//2 解码图片
		

		//3 显示到窗口
		
		//
next:
		m_p3dGraphics->Refresh();
		pPinIn->PutFreeBuffer(pBufferInPapared);
	}
	
	if (pImageTmp )
	{
		delete pImageTmp;
		pImageTmp = NULL;
	}
	if (pCodec)
	{
		pCodec->Close();
		delete pCodec;
		pCodec = NULL;
	}

	m_state = eFilterState_Stop;

	return 0;
}
int CVideoSummaryRender::Refresh()
{
	if (!m_p3dGraphics)
		return -1;
	
	m_p3dGraphics->Refresh();

	return 0;
}
int CVideoSummaryRender::HitTest(float fX, float fY)
{
	if (!m_p3dGraphics)
		return -1;

	m_p3dGraphics->HitTest(fX, fY);

	return 0;
}
//目前只能获取最上面一个 目标
int CVideoSummaryRender::HitTest(float fX, float fY, int64 &s64Time )
{
	if (!m_p3dGraphics)
		return -1;

	bool bFind = false;
	int64 s64Ts = 0;
	list<C3dItemVideoSummaryPicture*>::iterator item;

	m_pMutexPicPbject->Lock();
	for (item = m_lstObjectPic.begin(); item != m_lstObjectPic.end(); item++)
	{
		C3dItemVideoSummaryPicture *pItemImage = *item;
		if (!pItemImage || pItemImage == m_pImgBkg)
		{
			continue;
		}
		if (!pItemImage->HitTest(fX, fY) )
			continue;

		s64Ts = pItemImage->GetTimeStamp();
		bFind = true;
		break;

	}
	m_pMutexPicPbject->Unlock();
	// ms ;
	if (bFind)
	{//使用时间戳对应 才计算简单。 时间对应 计算比较复杂
		s64Time =  s64Ts;;//s64Ts* 1000*m_nTimeBaseNum/m_nTimeBaseDen;
		return 0;
	}
	else
	{
		return -1;
	}
	return 0;
}
bool CVideoSummaryRender::DecodeJpegFile(char *pSzFile, int &nW, int &nH,  char *pImg)
{

	FILE *pFile = NULL;
	pFile = fopen(pSzFile, "rb");
	if (!pFile)
		return false;
		
	int nRet = 0;
	struct   _stat64   stat;    
	if (_fstat64(fileno(pFile), &stat) != 0)
	{
		fclose(pFile);
		return false;
	}

	int64   iFileSize   =   stat.st_size;     //获取文件大小   
	int nJpegLen = 256*1024;
	char *pJpegData = new char[iFileSize];
	nJpegLen = fread(pJpegData, 1, iFileSize, pFile);
	if (nJpegLen <= 0)
	{
		fclose(pFile);
		delete pJpegData;
		return false;
	}
	fclose(pFile);


	CFfmpegCodec *pCodec = new CFfmpegCodec();
	//解码图片
	sFfmpegVideoInfo videoInfo;
	videoInfo.m_nBitRate = 40*1024*8;
	videoInfo.m_nCodecId = CODEC_ID_MJPEG;
	videoInfo.m_nGopSize = 1;
	videoInfo.m_nPixFormat = PIX_FMT_YUV420P;
	videoInfo.m_nHeight = nH;
	videoInfo.m_nWidth = nW;

	pCodec->SetVideoParam(videoInfo);

	nRet = pCodec->Open(true);


	//pCodec->Decode(pData, videoInfo.m_nWidth*videoInfo.m_nHeight*3/2, 
	//	(uint8*)pJpeg, nJpegLen);
	int nDecodeLen = 0;
	nRet = pCodec->Decode((uint8_t* )pJpegData, nJpegLen/*, pImg, nDecodeLen*/);
	bool bRt = false;
	if (nRet>=0)
		bRt = true;
	else
		goto ret;
	AVFrame *pPicture = NULL;
	AVCodecContext *pCodecContext =  NULL;
	pPicture = pCodec->GetFrame();
	pCodecContext = pCodec->GetCodecContext();

	/*
	CVideoFramePinBufer *pVideoFrame;
	//
	//copy 数据
	pVideoFrame->m_nPixFormat = ePF_YV420;
	pVideoFrame->SetFrameSize(pCodecContext->width, pCodecContext->height);
	pVideoFrame->SetUsedSize(pCodecContext->width* pCodecContext->height*3);
	*/
	//
	//CopyAvFrame2VideoFrame(pPicture, pVideoFrame);
	CopyAvFrame2Buffer(pPicture, pImg, nW, nH);

ret:
	delete pCodec;
	delete pJpegData;

	return true;
}
int CVideoSummaryRender::CopyAvFrame2Buffer(void *pAvFrame, char *pVideoFrame, int nW, int nH)
{
	AVFrame *pAvFrame1 = (AVFrame*)pAvFrame;
	if (!pAvFrame1 || !pVideoFrame)
		return -1;
	// Y
	int nSrcStrideLine =0, nDesStrideLine=0;
	unsigned char *pSrc=0, *pDes = 0;
	int nWidth = 0, nHeight=0; 

	pSrc = pAvFrame1->data[0];
	pDes = (unsigned char*)pVideoFrame;
	nSrcStrideLine = pAvFrame1->linesize[0];
	nDesStrideLine = nW;
	nHeight = nH;

	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	//U
	pSrc = pAvFrame1->data[1];
	pDes = (unsigned char*)pVideoFrame + nW*nH;
	nSrcStrideLine = pAvFrame1->linesize[1];
	nDesStrideLine = nW/2;

	nHeight = nH/2;
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	//V
	pSrc = pAvFrame1->data[2];
	pDes = (unsigned char*)pVideoFrame + nW*nH*5/4;
	nSrcStrideLine = pAvFrame1->linesize[2];
	nDesStrideLine = nW/2;

	nHeight = nH/2;
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	return 0;
}
void CVideoSummaryRender::ClearObjectItem()
{
	list<C3dItemVideoSummaryPicture*>::iterator item;
	if (m_pMutexPicPbject)
		m_pMutexPicPbject->Lock();
	for (item = m_lstObjectPic.begin(); item != m_lstObjectPic.end(); item++)
	{
		C3dItemVideoSummaryPicture *pItemImage = *item;
		m_p3dGraphics->RemoveItem(pItemImage);
		pItemImage->Release();
		delete pItemImage;
	}
	m_lstObjectPic.clear();
	if (m_pMutexPicPbject)
		m_pMutexPicPbject->Unlock();
}

int CVideoSummaryRender::PlayNextFrame()
{
	debug("%s ++\n", __FUNCTION__);
	if (m_lstPinIn.size()<=0)
		return -1;
	CFilterPin *pPinIn = NULL;
	int nFrameCount = 0;
	long nTimePerFrameNow = m_nTimePerFrame;
	pPinIn = m_lstPinIn.at(0);
	nFrameCount = pPinIn->GetPreparedBufferConut();
	if (nFrameCount <= 0)
	{
		return -1;
	}

	
	//取 一个 pinIn 的 papared 数据
	CFilterPinBuffer *pBufferInPapared = 0;

	pPinIn->GetPreparedBuffer(&pBufferInPapared);
	if (!pBufferInPapared)
	{
		return -1;
	}

	CBuffer *pBufferEx =pBufferInPapared->GetExtData();
	if (pBufferEx)
	{//获取信息 
		sFfmpegFrameInfo * pFrameInfo  = (sFfmpegFrameInfo*) pBufferEx->GetData();
		m_nTimeBaseDen = pFrameInfo->m_nTimeDen;
		m_nTimeBaseNum = pFrameInfo->m_nTimeNum;
	}
	//显示图片
	//1 解析 输入的 图片 参数
	//首先解析 背景 
	int nPos = 0, nPosFind(0);
	char *pData = pBufferInPapared->GetData();
	char *pFind = pData;

	pFind = strstr(pData, "\n");
	map<string, string> mapParam;
	int nX(0), nY(0), nW(0), nH(0);
	char *pImageTmp = NULL;
	if (pFind)
	{
		ParseParam(pData, pFind-pData, mapParam );
		//debug("bkg=%s;w=%s;h=%s\n", mapParam["bkg"].c_str(), mapParam["w"].c_str(), mapParam["h"].c_str());
		nPos += pFind-pData + 1;
		nW = atoi( mapParam["w"].c_str());
		nH = atoi( mapParam["h"].c_str());
		if (pImageTmp == NULL)
			pImageTmp = new char[nW*nH*3];
		char szStartTimestamp[64];
		memset(szStartTimestamp, 0, sizeof(szStartTimestamp));
		strcpy(szStartTimestamp, mapParam["StartTimestamp"].c_str());
		sscanf(szStartTimestamp, "%I64d", &m_s64StartTimestamp);
		//判断是否解码背景图片放入显示。
		if (strcmp(m_szBkgFile, mapParam["bkg"].c_str()) != 0 )
		{//背景更新
			//
			if (m_pImgBkg->m_nTextureXPix != nW
				|| m_pImgBkg->m_nTextureYPix != nH)
			{//
				m_pImgBkg->Release();
				delete m_pImgBkg;
				m_pImgBkg = new C3dItemImage(m_p3dGraphics, nW, nH, e3dPF_YV420);
				m_pImgBkg->Create(0, 0, 0);                                                                                                                 
				m_pImgBkg->SetSize(2, 2, 0.01);
				m_p3dGraphics->AddItem2Head(m_pImgBkg);
			}
			/*char *pPic = new char[nW*nH*3];*/

			char szFile[256];
			strcpy(szFile, mapParam["bkg"].c_str());
			DecodeJpegFile(szFile, nW, nH, pImageTmp);

			m_pImgBkg->SetImageData(pImageTmp, nW, nH, e3dPF_YV420);
			//
			/*delete pPic;*/
		}
	}
	if (0)
	{

		goto next;
	}
	//解析图片
	ClearObjectItem();

	while (m_state != eFilterState_Stop
		&& m_state != eFilterState_ToStop
		&& nPos < pBufferInPapared->GetUsedSize())
	{
		pFind = strstr(pData+nPos, "\n");
		if (pFind == NULL)
		{
			break;
		}
		mapParam.clear();
		ParseParam(pData+nPos, pFind-pData-nPos, mapParam );
		if (mapParam.size() <= 0)
		{
			debug("empty picture\n");
		}
		//debug("pic=%s;x:%s;y=%s;w=%s;h=%s\n", 
		//	mapParam["pic"].c_str(), mapParam["x"].c_str(),mapParam["y"].c_str(), mapParam["w"].c_str(), mapParam["h"].c_str());
		//解码图片，放入显示 
		nW = atoi( mapParam["w"].c_str());
		nH = atoi( mapParam["h"].c_str());
		nX = atoi( mapParam["x"].c_str());
		nY = atoi( mapParam["y"].c_str());

		char szFile[256];
		strcpy(szFile, mapParam["pic"].c_str());
		DecodeJpegFile(szFile, nW, nH, pImageTmp);
		C3dItemVideoSummaryPicture *pNewImageItem = NULL;
		pNewImageItem = new C3dItemVideoSummaryPicture(m_p3dGraphics, nW, nH, e3dPF_YV420);
		pNewImageItem->Create(0, 0, 0);  

		float fX, fY, fW, fH;
		fX = (float)nX*2/m_pImgBkg->m_nTextureXPix;
		fY = (float)nY*2/m_pImgBkg->m_nTextureYPix;
		fW = (float)nW*2/m_pImgBkg->m_nTextureXPix;
		fH = (float)nH*2/m_pImgBkg->m_nTextureYPix;
		pNewImageItem->SetSize(fW, fH, 0.01);
		pNewImageItem->SetPos(fX, fY, 0.0);
		pNewImageItem->SetAlpha(0.8);
		//debug("pic [%d]: x=%.2f, y=%.2f; w=%.2f, h=%.2f\n", nCount, fX, fY, fW, fH);

		pNewImageItem->SetImageData(pImageTmp, nW, nH, e3dPF_YV420);
		pNewImageItem->SetTimeBase(m_nTimeBaseNum, m_nTimeBaseDen);
		pNewImageItem->SetStartTimestamp(m_s64StartTimestamp);

		int64 s64PTS  = 0 ;
		s64PTS = _atoi64( mapParam["pts"].c_str());
		pNewImageItem->SetTimeStamp(s64PTS);

		m_lstObjectPic.push_back(pNewImageItem);

		m_p3dGraphics->AppendItem(pNewImageItem);

		nPos = pFind-pData + 1;
	}
	//2 解码图片


	//3 显示到窗口

	//
next:
	m_p3dGraphics->Refresh();
	pPinIn->PutFreeBuffer(pBufferInPapared);

	if (pImageTmp)
	{
		delete pImageTmp;
		pImageTmp = NULL;
	}
	debug("%s --\n", __FUNCTION__);
	return 0;
}
int CVideoSummaryRender::PlayPrevFrame()
{
	debug("%s ++\n", __FUNCTION__);
	if (m_lstPinIn.size()<=0)
		return -1;
	CFilterPin *pPinIn = NULL;
	int nFrameCount = 0;
	long nTimePerFrameNow = m_nTimePerFrame;
	pPinIn = m_lstPinIn.at(0);
	nFrameCount = pPinIn->GetPreparedBufferConut();
	if (nFrameCount <= 0)
	{
		return -1;
	}


	//取 一个 pinIn 的 papared 数据
	CFilterPinBuffer *pBufferInPapared = 0;

	pPinIn->GetPreparedBuffer(&pBufferInPapared, false);
	if (!pBufferInPapared)
	{
		return -1;
	}

	CBuffer *pBufferEx =pBufferInPapared->GetExtData();
	if (pBufferEx)
	{//获取信息 
		sFfmpegFrameInfo * pFrameInfo  = (sFfmpegFrameInfo*) pBufferEx->GetData();
		m_nTimeBaseDen = pFrameInfo->m_nTimeDen;
		m_nTimeBaseNum = pFrameInfo->m_nTimeNum;
	}
	//显示图片
	//1 解析 输入的 图片 参数
	//首先解析 背景 
	int nPos = 0, nPosFind(0);
	char *pData = pBufferInPapared->GetData();
	char *pFind = pData;

	pFind = strstr(pData, "\n");
	map<string, string> mapParam;
	int nX(0), nY(0), nW(0), nH(0);
	char *pImageTmp = NULL;
	if (pFind)
	{
		ParseParam(pData, pFind-pData, mapParam );
		//debug("bkg=%s;w=%s;h=%s\n", mapParam["bkg"].c_str(), mapParam["w"].c_str(), mapParam["h"].c_str());
		nPos += pFind-pData + 1;
		nW = atoi( mapParam["w"].c_str());
		nH = atoi( mapParam["h"].c_str());
		if (pImageTmp == NULL)
			pImageTmp = new char[nW*nH*3];
		char szStartTimestamp[64];
		memset(szStartTimestamp, 0, sizeof(szStartTimestamp));
		strcpy(szStartTimestamp, mapParam["StartTimestamp"].c_str());
		sscanf(szStartTimestamp, "%I64d", &m_s64StartTimestamp);
		//判断是否解码背景图片放入显示。
		if (strcmp(m_szBkgFile, mapParam["bkg"].c_str()) != 0 )
		{//背景更新
			//
			if (m_pImgBkg->m_nTextureXPix != nW
				|| m_pImgBkg->m_nTextureYPix != nH)
			{//
				m_pImgBkg->Release();
				delete m_pImgBkg;
				m_pImgBkg = new C3dItemImage(m_p3dGraphics, nW, nH, e3dPF_YV420);
				m_pImgBkg->Create(0, 0, 0);                                                                                                                 
				m_pImgBkg->SetSize(2, 2, 0.01);
				m_p3dGraphics->AddItem2Head(m_pImgBkg);
			}
			/*char *pPic = new char[nW*nH*3];*/

			char szFile[256];
			strcpy(szFile, mapParam["bkg"].c_str());
			DecodeJpegFile(szFile, nW, nH, pImageTmp);

			m_pImgBkg->SetImageData(pImageTmp, nW, nH, e3dPF_YV420);
			//
			/*delete pPic;*/
		}
	}
	if (0)
	{

		goto next;
	}
	//解析图片
	ClearObjectItem();

	while (m_state != eFilterState_Stop
		&& m_state != eFilterState_ToStop
		&& nPos < pBufferInPapared->GetUsedSize())
	{
		pFind = strstr(pData+nPos, "\n");
		if (pFind == NULL)
		{
			break;
		}
		mapParam.clear();
		ParseParam(pData+nPos, pFind-pData-nPos, mapParam );
		if (mapParam.size() <= 0)
		{
			debug("empty picture\n");
		}
		//debug("pic=%s;x:%s;y=%s;w=%s;h=%s\n", 
		//	mapParam["pic"].c_str(), mapParam["x"].c_str(),mapParam["y"].c_str(), mapParam["w"].c_str(), mapParam["h"].c_str());
		//解码图片，放入显示 
		nW = atoi( mapParam["w"].c_str());
		nH = atoi( mapParam["h"].c_str());
		nX = atoi( mapParam["x"].c_str());
		nY = atoi( mapParam["y"].c_str());

		char szFile[256];
		strcpy(szFile, mapParam["pic"].c_str());
		DecodeJpegFile(szFile, nW, nH, pImageTmp);
		C3dItemVideoSummaryPicture *pNewImageItem = NULL;
		pNewImageItem = new C3dItemVideoSummaryPicture(m_p3dGraphics, nW, nH, e3dPF_YV420);
		pNewImageItem->Create(0, 0, 0);  

		float fX, fY, fW, fH;
		fX = (float)nX*2/m_pImgBkg->m_nTextureXPix;
		fY = (float)nY*2/m_pImgBkg->m_nTextureYPix;
		fW = (float)nW*2/m_pImgBkg->m_nTextureXPix;
		fH = (float)nH*2/m_pImgBkg->m_nTextureYPix;
		pNewImageItem->SetSize(fW, fH, 0.01);
		pNewImageItem->SetPos(fX, fY, 0.0);
		pNewImageItem->SetAlpha(0.8);
		//debug("pic [%d]: x=%.2f, y=%.2f; w=%.2f, h=%.2f\n", nCount, fX, fY, fW, fH);

		pNewImageItem->SetImageData(pImageTmp, nW, nH, e3dPF_YV420);
		pNewImageItem->SetTimeBase(m_nTimeBaseNum, m_nTimeBaseDen);
		pNewImageItem->SetStartTimestamp(m_s64StartTimestamp);

		int64 s64PTS  = 0 ;
		s64PTS = _atoi64( mapParam["pts"].c_str());
		pNewImageItem->SetTimeStamp(s64PTS);

		m_lstObjectPic.push_back(pNewImageItem);

		m_p3dGraphics->AppendItem(pNewImageItem);

		nPos = pFind-pData + 1;
	}
	//2 解码图片


	//3 显示到窗口

	//
next:
	m_p3dGraphics->Refresh();
	pPinIn->PutFreeBuffer(pBufferInPapared);

	if (pImageTmp)
	{
		delete pImageTmp;
		pImageTmp = NULL;
	}
	debug("%s --\n", __FUNCTION__);
	return 0;
}
