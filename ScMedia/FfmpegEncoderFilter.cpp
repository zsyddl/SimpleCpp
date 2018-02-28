#include "StdAfx.h"
#include "FfmpegEncoderFilter.h"
#include "FfmpegCodec.h"
#include "VideoFramePinBuffer.h"
#include "Buffer.h"
#include "FilterManager.h"

CFfmpegEncoderFilter g_regFfmpegEncoder;
CFfmpegEncoderFilter::CFfmpegEncoderFilter(void)
:m_pFfmpegCodec(NULL)
, m_pVideoInfo(0)
, m_pAudioInfo(0)
{
	m_pVideoInfo = new sFfmpegVideoInfo;
	m_pAudioInfo = new sFfmpegAudioInfo;
	memset(m_pVideoInfo, 0, sizeof(sFfmpegVideoInfo));
	memset(m_pAudioInfo, 0, sizeof(sFfmpegAudioInfo));


	strcpy(m_szType, FILTER_ENCODER_FFMPEG);
	strcpy(m_szName, FILTER_ENCODER_FFMPEG);

	m_pFrameInfo = new sFfmpegFrameInfo;
	memset(m_pFrameInfo, 0, sizeof(sFfmpegFrameInfo));

	CFilterManager::RegistFilter(&g_regFfmpegEncoder);

}

CFfmpegEncoderFilter::~CFfmpegEncoderFilter(void)
{
	if (m_pAudioInfo)
	{
		delete m_pAudioInfo;
		m_pAudioInfo = NULL;
	}
	if (m_pVideoInfo)
	{
		delete m_pVideoInfo;
		m_pVideoInfo = NULL;
	}
}

CMediaFilter *CFfmpegEncoderFilter::CreateObject()
{
	return new CFfmpegEncoderFilter;
}
//type=video; codec=28; width=1280; height=720; bps=1001024; fps=25; gop=25; pixformat=0;
int CFfmpegEncoderFilter::Open(char* pSzParam, bool bHasThread/*=false*/)
{
	if (m_pFfmpegCodec)
		return 1;

	CEncoder::Open(pSzParam, bHasThread);
	
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	pPinIn->Clear();
	pPinIn->CreateBuffer<CVideoFramePinBufer>(2);
	//暂时不创见，由 收到数据之后 根据参数创建 
	const char *pTmp = GetParam("codec");
	if (pTmp)
	{
		//m_pVideoInfo->m_nCodecId = CODEC_ID_H264;
		m_pVideoInfo->m_nCodecId = atoi(pTmp);
	}
	pTmp = GetParam("width");
	if (pTmp)
	{
		//m_pVideoInfo->m_nBitRate = 1*CODEC_BPS_M;
		m_pVideoInfo->m_nWidth = atoi(pTmp);
	}
	pTmp = GetParam("height");
	if (pTmp)
	{
		//m_pVideoInfo->m_nBitRate = 1*CODEC_BPS_M;
		m_pVideoInfo->m_nHeight = atoi(pTmp);
	}
	pTmp = GetParam("bps");
	if (pTmp)
	{
		//m_pVideoInfo->m_nBitRate = 1*CODEC_BPS_M;
		m_pVideoInfo->m_nBitRate = atoi(pTmp);
	}
	pTmp = GetParam("gop");
	if (pTmp)
	{
		m_pVideoInfo->m_nGopSize = atoi(pTmp);
	}
	pTmp = GetParam("fps");
	if (pTmp)
	{
		m_pVideoInfo->m_nFps = atoi(pTmp);;
	}
	pTmp = GetParam("pixformat");
	if (pTmp)
	{
		//m_pVideoInfo->m_nPixFormat = PIX_FMT_YUV420P;
		m_pVideoInfo->m_nPixFormat =  atoi(pTmp);;;
	}
	ReCreateCodec();

	return 0;
	int nRet = 0;
	
	m_pFfmpegCodec = new CFfmpegCodec();
	//用于编码, 参数从 pSzParam 解析
	sFfmpegVideoInfo videoParam;
	videoParam.m_nBitRate = 1*CODEC_BPS_M;
	videoParam.m_nFps = 25;
	videoParam.m_nCodecId = CODEC_ID_H264;
	videoParam.m_nGopSize = 10;
	videoParam.m_nHeight = 576;
	videoParam.m_nWidth = 704;
	videoParam.m_nPixFormat = PIX_FMT_YUV420P;

	nRet = m_pFfmpegCodec->SetVideoParam(videoParam);

	nRet = m_pFfmpegCodec->Open(/*CODEC_ID_H264,*/ false);
	if (nRet >= 0)
	{
		debug("%s, create h264 encoder success\n", __FUNCTION__);
	}
	else
	{
		debug("%s, create h264 encoder false\n", __FUNCTION__);
	}

	return nRet;
}

int CFfmpegEncoderFilter::DoFilter()
{
	//取 pin的数据， 压缩， 然后 放入 到输出pin
	
	//
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
	{
		return -1;
	}
	CFilterPinBuffer *pOutBuffer = 0;
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut || pPinOut->GetFreeBufferCount() <= 0 )
	{//输出 没有内存 
		CEncoder::DoFilter();
		return -4;
	}
	pPinOut->GetFreeBuffer(&pOutBuffer);
	if (!pOutBuffer)
	{
		return -5;
	}
	//取 一个 pinIn 的 papared 数据
	CFilterPinBuffer *pBufferInPapared = 0;

	pPinIn->GetPreparedBuffer(&pBufferInPapared);
	if (pBufferInPapared == NULL)
	{
		pPinOut->PutFreeBuffer(pOutBuffer);
		return -2;
	}
	//取数据压缩
	CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pBufferInPapared);

	pOutBuffer->SetAllocSize(CODEC_BPS_K * 100);
	int nEncodeSize =  pOutBuffer->GetAllocSize();
	sFfmpegFrameInfo *pFrameInfoIn = (sFfmpegFrameInfo *)pVideoFrame->GetExtData()->GetData();
	int nExtDataSize = pVideoFrame->GetExtData()->GetUsedSize();
	//检测 分辨率变化，  重新 初始化编码器 
	sFfmpegVideoInfo videoInfo;
	
	sFfmpegAudioInfo audioInfo;
	if (pFrameInfoIn &&
		nExtDataSize == sizeof(sFfmpegFrameInfo)
		&& CheckStreamChanged(pFrameInfoIn->m_videoInfo, pFrameInfoIn->m_audioInfo))
	{
		if (pFrameInfoIn)
		{
			m_pVideoInfo->m_nFps = pFrameInfoIn->m_videoInfo.m_nFps;
			m_pVideoInfo->m_nGopSize = pFrameInfoIn->m_videoInfo.m_nGopSize;
			m_pVideoInfo->m_nHeight = pFrameInfoIn->m_videoInfo.m_nHeight;
			m_pVideoInfo->m_nWidth = pFrameInfoIn->m_videoInfo.m_nWidth;
			if (pFrameInfoIn->m_videoInfo.m_nBitRate > 0) 
			{
				m_pVideoInfo->m_nBitRate = pFrameInfoIn->m_videoInfo.m_nBitRate;
			}
			if (pFrameInfoIn->m_videoInfo.m_nCodecId > 0)
			{
				m_pVideoInfo->m_nCodecId = pFrameInfoIn->m_videoInfo.m_nCodecId;
			}
		}
		if (m_pVideoInfo->m_nFps <= 0)
			m_pVideoInfo->m_nFps = 25;
		if (m_pVideoInfo->m_nGopSize <=0)
			m_pVideoInfo->m_nGopSize = 25;

		ReCreateCodec();
	}
	//
	if (m_pFfmpegCodec == NULL)
	{//没有 编码器，直接返回数据
		pPinIn->PutFreeBuffer(pBufferInPapared);
		pPinOut->PutFreeBuffer(pOutBuffer);
		return -3;
	}
	if (pOutBuffer->GetAllocSize() < 1*1024*1024)
	{
		pOutBuffer->SetAllocSize(1*1024*1024);
	}
	//
	//
	m_pFfmpegCodec->Encode((unsigned char*)pVideoFrame->GetData(), pVideoFrame->GetLength(),
							(unsigned char*)pOutBuffer->GetData(), nEncodeSize);
	pOutBuffer->SetUsedSize(nEncodeSize);
	if (nEncodeSize > 0)
	{
		CBuffer *pBufferEx = pOutBuffer->GetExtData();
		if (!pBufferEx)
		{
			debug("%s error\n", __FUNCTION__);
		}
		if (pBufferEx->GetAllocSize() < sizeof(sFfmpegFrameInfo))
		{
			pBufferEx->SetAllocSize(sizeof(sFfmpegFrameInfo));

		}
		pBufferEx->SetUsed(sizeof(sFfmpegFrameInfo));
		sFfmpegFrameInfo *pVideoFrameInfo = (sFfmpegFrameInfo *)pBufferEx->GetData();
// 		pVideoFrameInfo->nHeight	= m_pVideoInfo->m_nHeight;
// 		pVideoFrameInfo->nWidth		= m_pVideoInfo->m_nWidth;
// 		pVideoFrameInfo->nCodecId	= m_pVideoInfo->m_nCodecId;
// 		pVideoFrameInfo->nGop		= m_pVideoInfo->m_nGopSize;
// 		pVideoFrameInfo->nFps		= m_pVideoInfo->m_nFps;
// 		pVideoFrameInfo->nBps		= m_pVideoInfo->m_nBitRate;
// 		pVideoFrameInfo->nPixFormat = m_pVideoInfo->m_nPixFormat;
// 		pVideoFrameInfo->nPts		= m_pFfmpegCodec->GetPts();
		pVideoFrameInfo->m_videoInfo = *m_pVideoInfo;
		pVideoFrameInfo->m_nDataType = AVMEDIA_TYPE_VIDEO;
		pVideoFrameInfo->m_nTimeDen = 25;
		pVideoFrameInfo->m_nTimeNum = 1;
		pVideoFrameInfo->m_s64Pts = m_pFfmpegCodec->GetPts();
		if (m_pFfmpegCodec->IsKeyFrame())
		{
			pVideoFrameInfo->m_nFrameType = eFFT_I;
		}
		else
		{	
			pVideoFrameInfo->m_nFrameType = eFFT_P;
		}
		
		pPinOut->PutPreparedBuffer(pOutBuffer);
	}
	else{
		pPinOut->PutFreeBuffer(pOutBuffer);
	}
	//
	pPinIn->PutFreeBuffer(pBufferInPapared);
	//
	return CEncoder::DoFilter();
}
bool CFfmpegEncoderFilter::CheckStreamChanged(sFfmpegVideoInfo &videoInfo, sFfmpegAudioInfo &audioInfo )
{
	bool bRet = false;
	if (!m_pFfmpegCodec)
	{//没有，则直接创见
	
		return true;
	}
	else
	{
		if (m_pVideoInfo->m_nHeight != videoInfo.m_nHeight
			|| m_pVideoInfo->m_nWidth != videoInfo.m_nWidth)
		{
			bRet = true;
		}
	}
	return bRet;
}
int  CFfmpegEncoderFilter::ReCreateCodec()
{
	int nRet = 0;
	if (m_pFfmpegCodec)
	{
		m_pFfmpegCodec->Close();
		delete m_pFfmpegCodec;
		m_pFfmpegCodec = NULL;
	}
	if (m_pVideoInfo->m_nBitRate <= 0)
	{
		if (m_pVideoInfo->m_nWidth > 1280)
		{//> 1280, 1080 ....
			m_pVideoInfo->m_nBitRate = 4*1024*1024;
		}// > D1, 720P
		else if (m_pVideoInfo->m_nWidth > 720)
		{
			m_pVideoInfo->m_nBitRate = 2*1024*1024;
		}// > cif, D1
		else if (m_pVideoInfo->m_nWidth > 352)
		{
			m_pVideoInfo->m_nBitRate = 1*1024*1024;
		}
		// <=cif
		else 
		{
			m_pVideoInfo->m_nBitRate = 512*1024;
		}
	}
	if (m_pVideoInfo->m_nCodecId == AV_CODEC_ID_NONE)
	{
		m_pVideoInfo->m_nCodecId = AV_CODEC_ID_H264;
	}
	m_pFfmpegCodec = new CFfmpegCodec;
	m_pFfmpegCodec->SetVideoParam(*m_pVideoInfo);
	nRet = m_pFfmpegCodec->Open(/*CODEC_ID_H264,*/ false);
	if (nRet >= 0)
	{
		debug("%s, create %d encoder success\n", __FUNCTION__, m_pVideoInfo->m_nCodecId);
	}
	else
	{
		delete m_pFfmpegCodec;
		m_pFfmpegCodec = NULL;
		debug("%s, create %d encoder false\n", __FUNCTION__, m_pVideoInfo->m_nCodecId);
	}

	return 0;
}
