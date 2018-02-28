#include "StdAfx.h"
#include "H264Decoder.h"

#include "VideoFramePinBuffer.h"
#include "Buffer.h"
#include "PackageParser.h"
#include "FilterManager.h"

#ifdef FFMPEG_OLD
#include "videodecoder.h"
#elif defined (FFMPEG_NEW)
#include "FfmpegCodec.h"
#endif

CH264Decoder g_regDecoderH264;

CH264Decoder::CH264Decoder(void)
:m_pFfmpeg(0)
{
	strcpy(m_szType, "decoder h264");
	strcpy(m_szName, "decoder h264");

	CFilterManager::RegistFilter(&g_regDecoderH264);
	m_nPixOutFormat = PIX_FMT_YUV420P;
#ifdef FFMPEG_OLD
	m_pFfmpeg = new VideoCodec(CODEC_ID_H264);
#else
	
#endif
#ifdef TEST_WRITE_AVI
	m_pFfmpegAvi = new CFfmpegFormat();
	m_pFfmpegAvi->Create("avi", false);
	sFfmpegStreamInfo streamInfo;
	streamInfo.m_nStreamType = eVST_VivsH264;
	streamInfo.m_nDataType = eFDT_Video;
	streamInfo.m_nCodecId = CODEC_ID_H264;
	streamInfo.m_videoInfo.m_nBitRate = 2*1024*1024;
	streamInfo.m_videoInfo.m_nGopSize = 25;
	streamInfo.m_videoInfo.m_nHeight = 576;
	streamInfo.m_videoInfo.m_nWidth = 720;
	streamInfo.m_videoInfo.m_nPixFormat = PIX_FMT_YUV420P;
	streamInfo.m_videoInfo.m_nCodecId = CODEC_ID_H264;
	streamInfo.m_videoInfo.m_nFps = 25;
	int nRet = m_pFfmpegAvi->AddStream(0, streamInfo);
	//
	nRet = m_pFfmpegAvi->Open("f:\\tmp\\test2Avi.avi", "", 0, false);
	if (nRet < 0)
	{
		
	}

#endif
}

CH264Decoder::~CH264Decoder(void)
{
	if (m_pFfmpeg)
	{
		delete m_pFfmpeg;
		m_pFfmpeg = NULL;
	}

#ifdef TEST_WRITE_AVI

	m_pFfmpegAvi->Close();
#endif
	
}
CMediaFilter *CH264Decoder::CreateObject()
{
	return new CH264Decoder();
}
int CH264Decoder::Open(char *pParam)
{
	if (m_pFfmpeg)
		return 1;
	CDecoder::Open();
	//创建 buffer
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	pPinOut->Clear();

	pPinOut->CreateBuffer<CVideoFramePinBufer>(4);

	m_pFfmpeg = new CFfmpegCodec();
	sFfmpegVideoInfo videoParam;
	videoParam.m_nBitRate = 1*CODEC_BPS_M;
	videoParam.m_nFps = 25;
	videoParam.m_nCodecId = CODEC_ID_H264;
	videoParam.m_nGopSize = 10;
	videoParam.m_nHeight = 576;
	videoParam.m_nWidth = 704;
	videoParam.m_nPixFormat = PIX_FMT_YUV420P;

	int nRet = 0;
	nRet = m_pFfmpeg->SetVideoParam(videoParam);

	if (m_pFfmpeg->Open(/*CODEC_ID_H264*/) >= 0)
	{
		debug("%s, open h264 codec new, ok\n", __FUNCTION__);
	}
	else
	{
		debug("%s, open h264 codec new, false\n", __FUNCTION__);
	}
	//
	return 0;
}
int CH264Decoder::DoFilter()
{
	//
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	
	CFilterPinBuffer *pBufferCodec = 0;
	pPinIn->GetPreparedBufferConut( );
	if (pPinIn->GetPreparedBufferConut( ) <= 0)
	{
		//debug("%s: no prepared buffer\n", __FUNCTION__);
		return -1;
	}
#if 1
	if (  !m_pFfmpeg )
	{
		//pPinIn->PutFreeBuffer(pBufferCodec);
		return -2;
	}
	//检测 输出 pin 是否 有free buffer
	CFilterPinBuffer *pOutBuffer = NULL;
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (pPinOut->GetFreeBufferCount() <= 0)
	{
		goto ret;
	}
	pPinIn->GetPreparedBuffer(&pBufferCodec);
	//以上流程 内存都正常，
	//解码
#ifdef TEST_WRITE_AVI
	static int nTimeStamp = 0;
	nTimeStamp++;
	m_pFfmpegAvi->WriteFrame((char*)pBufferCodec->GetData(), pBufferCodec->GetLength(), nTimeStamp);
	
#endif

#ifdef debug_decoder
	static FILE *pfileDecoder = 0;
	if (pfileDecoder == NULL)
	{
		pfileDecoder = fopen("F:\\tmp\\h264-decoder.vd", "w+b");
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
#elif defined(FFMPEG_NEW)
	//0 长度的数据解码 会造成  解码器崩溃
	int nRet = m_pFfmpeg->Decode((unsigned char*)pBufferCodec->GetData(), pBufferCodec->GetLength());
	if (nRet>=0)
		bRt = true;
#endif

	if ( !bRt)
	{//解码失败
		//debug("%s, decode h264 false, %d\n", __FUNCTION__, pBufferCodec->GetLength());
		pPinIn->PutFreeBuffer(pBufferCodec);
		return -2;
	}
	//debug("%s, decode h264 success, %d\n", __FUNCTION__, pBufferCodec->GetLength());

	//获取一个 outpin buff

	pPinOut->GetFreeBuffer(&pOutBuffer);
	if ( !pOutBuffer)
	{
		pPinIn->PutFreeBuffer(pBufferCodec);
		return CDecoder::DoFilter();

	}
	CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer *>(pOutBuffer);
	if (pVideoFrame)
	{//
		AVFrame *pPicture = NULL;
		AVCodecContext *pCodecContext =  NULL;
#ifdef FFMPEG_OLD
		pPicture = m_pFfmpeg->picture;
		pCodecContext = m_pFfmpeg->c;
#elif defined(FFMPEG_NEW)
		pPicture = m_pFfmpeg->GetFrame();
		pCodecContext = m_pFfmpeg->GetCodecContext();
#endif
		//
		//debug("%s, decode h264 ok %d ,[%d x %d]\n", __FUNCTION__, pBufferCodec->GetLength(), pCodecContext->width, pCodecContext->height);
		//copy 数据
		pVideoFrame->m_nPixFormat = ePF_YV420;
		pVideoFrame->SetFrameSize(pCodecContext->width, pCodecContext->height);
		pVideoFrame->SetUsedSize(pCodecContext->width* pCodecContext->height*3);
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
	{
		pPinOut->PutPreparedBuffer(pOutBuffer);
	}
#endif
	//解码完毕 释放 buffer 
	pPinIn->PutFreeBuffer(pBufferCodec);
	//

ret:
	return CDecoder::DoFilter();
}
//目前只是支持 YV12 数据, 从 ffmpeg 的 frame copy到显示需要的 frame 
int CH264Decoder::CopyAvFrame2VideoFrame(void *pAvFrame, void *pVideoFrame)
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
//颜色转换 缩放等操作 
int CH264Decoder::CopyFrame( int nWidth, int nHeight, char *pVideoFrame, int &nLen, int nOutFormat )
{

	return 0;
}
int CH264Decoder::Flush()
{
	if (m_pFfmpeg)
	{
		m_pFfmpeg->Flush();
	}
	return CMediaFilter::Flush();
}
int CH264Decoder::CopyFrame(unsigned char *pImageOut, int nImageLen, int nW, int nH, int pix_fmt)
{
	if (m_pFfmpeg == NULL)
	{
		return -1;
	}
	m_pFfmpeg->CopyFrame(pImageOut, nImageLen, nW, nH, (AVPixelFormat)pix_fmt);

	return 0;
}