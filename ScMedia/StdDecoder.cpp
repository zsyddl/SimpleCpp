#include "StdAfx.h"
#include "StdDecoder.h"
#include "FfmpegCodec.h"
#include "Buffer.h"
#include "VideoFramePinBuffer.h"

CStdDecoder::CStdDecoder(void)
:m_pFfmpeg(0)
, m_pBufferDecodeTmp(0)
{
}

CStdDecoder::~CStdDecoder(void)
{
	if (m_pFfmpeg)
	{
		delete m_pFfmpeg;
		m_pFfmpeg = NULL;
	}
	if (m_pBufferDecodeTmp)
	{
		delete m_pBufferDecodeTmp;
		m_pBufferDecodeTmp = NULL;
	}
}
int CStdDecoder::Open(sFfmpegStreamInfo &mediaInfo)
{
	//m_pFfmpeg = new VideoCodec((CodecID)nCodec);
	CDecoder::Open();

	m_mediaInfo = mediaInfo;
	m_pFfmpeg = new CFfmpegCodec();
	m_pFfmpeg->SetAudioParam(mediaInfo.m_audioInfo);
	m_pFfmpeg->SetVideoParam(mediaInfo.m_videoInfo);
	if (m_pFfmpeg->Open(/*mediaInfo.m_nCodecId*/) < 0)
	{
		return -1;
	}
	int nFrameSize = 0;
	if (m_pFfmpeg->GetCodecType() == AVMEDIA_TYPE_VIDEO)
	{
		nFrameSize = mediaInfo.m_videoInfo.m_nWidth*mediaInfo.m_videoInfo.m_nHeight*4;
		if (nFrameSize < 512*1024)
		{
			nFrameSize = 512*1024;
		}
		CFilterPin *pPinOut = m_lstPinOut.at(0);
		if (!pPinOut)
			return -1;
		pPinOut->Clear();
		pPinOut->CreateBuffer<CVideoFramePinBufer>(1);
	}
	else
	{
		nFrameSize = 1024*1024;
	}
	m_pBufferDecodeTmp = new CBuffer(nFrameSize);

	return 0;
}

int CStdDecoder::DoFilter()
{
	CFilterPin *pPinIn = m_lstPinIn.at(0);

	CFilterPinBuffer *pBufferCodec = 0;
	pPinIn->GetPreparedBuffer(&pBufferCodec);
	if ( !m_pFfmpeg || !pBufferCodec)
	{
		pPinIn->PutFreeBuffer(pBufferCodec);
		return -1;
	}
	//以上流程 内存都正常，
	//解码
#ifdef debug_std_decoder
	static FILE *pfileDecoder = 0;
	if (pfileDecoder == NULL)
	{
		pfileDecoder = fopen("F:\\tmp\\audio.alaw", "w+b");
	}
	if (pfileDecoder)
	{
		fwrite(pBufferCodec->GetData(), pBufferCodec->GetLength(), 1, pfileDecoder);
		fflush(pfileDecoder);
	}
#endif
	bool bRt = false;
#ifdef FFMPEG_OLD
	bRt =  m_pFfmpeg->decode((unsigned char*)pBufferCodec->GetData(), pBufferCodec->GetLength()/*-sizeof(DEF_ST_AV_STREAM_HEADER)*/);
#else
	int nRet = 0;
	int nDecodeLen = 0;
	if  (m_pFfmpeg->GetCodecType() == AVMEDIA_TYPE_VIDEO)
	{
		nRet = m_pFfmpeg->Decode((unsigned char*)pBufferCodec->GetData(), pBufferCodec->GetLength(), m_pBufferDecodeTmp->GetData(), nDecodeLen);
		if (nRet>=0)
		{
			bRt = true;
			m_pBufferDecodeTmp->SetUsed(nRet);
		}
	}
	else if (m_pFfmpeg->GetCodecType() == AVMEDIA_TYPE_AUDIO)
	{//audio
		m_pBufferDecodeTmp->Resize(pBufferCodec->GetLength()*10);
		nDecodeLen = pBufferCodec->GetLength()*10;
		nRet = m_pFfmpeg->Decode((unsigned char*)pBufferCodec->GetData(), pBufferCodec->GetLength(), m_pBufferDecodeTmp->GetData(), nDecodeLen);
		if (nRet>=0 && nDecodeLen > 0)
		{
			bRt = true;
			m_pBufferDecodeTmp->SetUsed(nDecodeLen);
		}
		else{
			bRt = false;
		}
		
	}

#endif
	//////////////////////////////////////////////////////////////////////////
	if ( !bRt)
	{//解码失败
		debug("%s, std decode  false, %d\n", __FUNCTION__, pBufferCodec->GetLength());
		pPinIn->PutFreeBuffer(pBufferCodec);
		return -2;
	}

	//获取一个 outpin buff
	CFilterPinBuffer *pOutBuffer = NULL;
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	pPinOut->GetFreeBuffer(&pOutBuffer);
	
	if (pOutBuffer == NULL)	
	{
		pPinIn->PutFreeBuffer(pBufferCodec);
		return -1;
	}
	
	if (m_pFfmpeg->GetCodecType() == AVMEDIA_TYPE_VIDEO)
	{//
		CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer *>(pOutBuffer);
		AVFrame *pPicture = NULL;
		AVCodecContext *pCodecContext = NULL;
#ifdef FFMPEG_OLD
		pPicture		= m_pFfmpeg->picture;
		pCodecContext	= m_pFfmpeg->c;
#else
		pPicture		= m_pFfmpeg->GetFrame();
		pCodecContext	= m_pFfmpeg->GetCodecContext();

#endif
		//
		//debug("%s, decode h264 ok %d ,[%d x %d]\n", __FUNCTION__, pBufferCodec->GetLength(), pCodecContext->width, pCodecContext->height);
		//copy 数据
		pVideoFrame->m_nPixFormat = ePF_YV420;
		pVideoFrame->SetFrameSize(pCodecContext->width, pCodecContext->height);
		//
		CopyAvFrame2VideoFrame(pPicture, pVideoFrame);
		// b保存 传入的　ext data
		CBuffer *pExtBufferVideoFrame = pVideoFrame->GetExtData();
		CBuffer *pExtBufferCodecFrame = pBufferCodec->GetExtData();
		if (pExtBufferCodecFrame 
			&&pExtBufferVideoFrame)
		{
			pExtBufferVideoFrame->Clear();
			pExtBufferVideoFrame->Resize(pExtBufferCodecFrame->GetLength());
			pExtBufferVideoFrame->Append(pExtBufferCodecFrame->GetData(), pExtBufferCodecFrame->GetLength());
		}
		//
		pPinOut->PutPreparedBuffer(pOutBuffer);
	}
	else
	{//音频
		pOutBuffer->Clear();
		pOutBuffer->Append((char*)m_pBufferDecodeTmp->GetData(), m_pBufferDecodeTmp->GetLength());
		pPinOut->PutPreparedBuffer(pOutBuffer);
	}

	//解码完毕 释放 buffer 
	pPinIn->PutFreeBuffer(pBufferCodec);
	//
	return 0;

}
//目前只是支持 YV12 数据, 从 ffmpeg 的 frame copy到显示需要的 frame 
int CStdDecoder::CopyAvFrame2VideoFrame(void *pAvFrame, void *pVideoFrame)
{
	AVFrame *pAvFrame1 = (AVFrame*)pAvFrame;
	CVideoFramePinBufer *pVideoFrame1 = (CVideoFramePinBufer*) pVideoFrame;
	if (!pAvFrame1 || !pVideoFrame1)
		return -1;
	// Y
	int nSrcStrideLine =0, nDesStrideLine=0;
	unsigned char *pSrc=0, *pDes = 0;
	int nWidth = 0, nHeight=0; 

	pSrc = pAvFrame1->data[0];
	pDes = pVideoFrame1->m_pY;
	nSrcStrideLine = pAvFrame1->linesize[0];
	nDesStrideLine = pVideoFrame1->m_nWidth;
	nHeight = pVideoFrame1->m_nHeight;

	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	//U
	pSrc = pAvFrame1->data[1];
	pDes = pVideoFrame1->m_pU;
	nSrcStrideLine = pAvFrame1->linesize[1];
	nDesStrideLine = pVideoFrame1->m_nWidth/2;

	nHeight = pVideoFrame1->m_nHeight/2;
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	//V
	pSrc = pAvFrame1->data[2];
	pDes = pVideoFrame1->m_pV;
	nSrcStrideLine = pAvFrame1->linesize[2];
	nDesStrideLine = pVideoFrame1->m_nWidth/2;

	nHeight = pVideoFrame1->m_nHeight/2;
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}


	return 0;
}