#include "StdAfx.h"
#include "FfmpegDecoderFilter.h"
#include "VideoFramePinBuffer.h"
#include "FfmpegCodec.h"
#include "Buffer.h"
#include "FilterManager.h"

CFfmpegDecoderFilter g_regFfmpegDecoder;
CFfmpegDecoderFilter::CFfmpegDecoderFilter(void)
:CDecoder()
, m_pDecoder(NULL)
, m_s64Timestamp(0)
{
	strcpy(m_szType, FILTER_DECODER_FFMPEG);
	strcpy(m_szName, FILTER_DECODER_FFMPEG);
	
	CFilterManager::RegistFilter(&g_regFfmpegDecoder);
}

CFfmpegDecoderFilter::~CFfmpegDecoderFilter(void)
{
	if (m_pDecoder)
	{
		m_pDecoder->Close();
		delete m_pDecoder;
		m_pDecoder = NULL;
	}
}

CMediaFilter *CFfmpegDecoderFilter::CreateObject()
{
	return new CFfmpegDecoderFilter;
}
int CFfmpegDecoderFilter::Open(char *pParam /*=0 */)
{
	CDecoder::Open(pParam);
	//创建 buffer
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	pPinOut->Clear();

	pPinOut->CreateBuffer<CVideoFramePinBufer>(2);
	if (m_pDecoder)
	{
		return -1; 
	}
	if (m_mapParamValue.find("codec") != m_mapParamValue.end())
	{
		m_pDecoder = new CFfmpegCodec();

		int nCodecId = 0;
		nCodecId = atoi(m_mapParamValue["codec"].c_str());
		int nRet = 0;
		const char *pType = m_mapParamValue["type"].c_str();
		if (strcmp(pType , "video"))
		{
			sFfmpegVideoInfo videoInfo;
			videoInfo.m_nCodecId = nCodecId;
			videoInfo.m_nHeight = atoi(m_mapParamValue["height"].c_str());
			videoInfo.m_nWidth = atoi(m_mapParamValue["width"].c_str());

			m_pDecoder->SetVideoParam(videoInfo);
			nRet = m_pDecoder->Open();
		}
	}
	return 0;
}
int CFfmpegDecoderFilter::Open( char* pParam, bool bHasThread /*=false */ )
{
	CBaseFilter::Open(pParam, bHasThread);
	//创建 buffer
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	pPinOut->Clear();

	pPinOut->CreateBuffer<CVideoFramePinBufer>(2);
	if (m_pDecoder)
	{
		return -1; 
	}
	if (m_mapParamValue.find("codec") != m_mapParamValue.end())
	{
		m_pDecoder = new CFfmpegCodec();

		int nCodecId = 0;
		nCodecId = atoi(m_mapParamValue["codec"].c_str());
		int nRet = 0;
		const char *pType = m_mapParamValue["type"].c_str();
		if (strcmp(pType , "video"))
		{
			sFfmpegVideoInfo videoInfo;
			videoInfo.m_nCodecId = nCodecId;
			videoInfo.m_nHeight = atoi(m_mapParamValue["height"].c_str());
			videoInfo.m_nWidth = atoi(m_mapParamValue["width"].c_str());

			m_pDecoder->SetVideoParam(videoInfo);
			nRet = m_pDecoder->Open();
		}
	}
	return 0;
}
int CFfmpegDecoderFilter::DoFilter()
{
	CFilterPin *pPinIn = m_lstPinIn.at(0);

	CFilterPinBuffer *pBufferCodec = 0;
	pPinIn->GetPreparedBufferConut( );
	if (pPinIn->GetPreparedBufferConut( ) <= 0)
	{
		//debug("%s: no prepared buffer\n", __FUNCTION__);
		//return -1;
	}
#if 1

	//检测 输出 pin 是否 有free buffer
	CFilterPinBuffer *pOutBuffer = NULL;
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (pPinOut->GetFreeBufferCount() <= 0)
	{
		goto ret;
	}
	pPinIn->GetPreparedBuffer(&pBufferCodec);
	if (!pBufferCodec)
	{//仍然 运行 下去。这样给解码器 一次运行的机会
		/*goto ret;*/
	}

	if (  !m_pDecoder && pBufferCodec)
	{

		CBuffer *pBufferEx = pBufferCodec->GetExtData();
		if (!pBufferEx)
		{
			pPinIn->PutFreeBuffer(pBufferCodec);
			goto ret;
		}
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pBufferEx->GetData();
		//创建解码器
		m_streamInfo.m_nDataType = pFrameInfo->m_nDataType;
		m_streamInfo.m_videoInfo = pFrameInfo->m_videoInfo;

		int nRet = 0;
		nRet = CreateDecoder();
		if (nRet < 0)
			goto ret;
	}
	if (!m_pDecoder)
		goto ret;
// 	检测 输出 pin 是否 有free buffer
// 		CFilterPinBuffer *pOutBuffer = NULL;
// 		CFilterPin *pPinOut = m_lstPinOut.at(0);
// 		if (pPinOut->GetFreeBufferCount() <= 0)
// 		{
// 			goto ret;
// 		}
// 		pPinIn->GetPreparedBuffer(&pBufferCodec);
	//以上流程 内存都正常，
	//解码
#ifdef TEST_WRITE_AVI
	static int nTimeStamp = 0;
	nTimeStamp++;
	m_pFfmpegAvi->WriteFrame((char*)pBufferCodec->GetData(), pBufferCodec->GetLength(), nTimeStamp);

#endif

#define debug_decoder
#ifdef debug_decoder
	static FILE *pfileDecoder = 0;
	if (pfileDecoder == NULL)
	{
		pfileDecoder = fopen("F:\\h264-decoder.h264", "w+b");
	}
	if (pfileDecoder && pBufferCodec)
	{
		fwrite(pBufferCodec->GetData(), pBufferCodec->GetLength(), 1, pfileDecoder);
		fflush(pfileDecoder);
	}
#endif
	bool bRt = false;
#ifdef FFMPEG_OLD
	bRt =  m_pDecoder->decode((unsigned char*)pBufferCodec->GetData(), pBufferCodec->GetLength()/*-sizeof(DEF_ST_AV_STREAM_HEADER)*/);
#elif defined(FFMPEG_NEW)
	//0 长度的数据解码 会造成  解码器崩溃
	int nRet = -1;
	if (m_pDecoder)
	{
		if (pBufferCodec)
			nRet = m_pDecoder->Decode((unsigned char*)pBufferCodec->GetData(), pBufferCodec->GetLength());
		else
		{//清空解码器, 这个办法不行。 只能采用 Reset
			//nRet = m_pDecoder->Decode((unsigned char*)NULL, 0);
		}
	}
	if (nRet>=0)
		bRt = true;
#endif

	if ( !bRt)
	{//解码失败
		//debug("%s, decode h264 false, %d\n", __FUNCTION__, pBufferCodec->GetLength());
		goto ret;
	}

	if (!m_pDecoder->HasNewPicture())
	{
		goto ret;
	}
// 	if (m_pDecoder->GetFrameType() != AV_PICTURE_TYPE_I)
// 	{
// 		pPinIn->PutFreeBuffer(pBufferCodec);
// 		return -3;
// 	}
	//debug("%s, decode h264 success, %d\n", __FUNCTION__, pBufferCodec->GetLength());
	CBuffer *pBufferEx = 0;
	if (pBufferCodec )
		pBufferEx = pBufferCodec->GetExtData();
	if (pBufferEx)
	{
		sFfmpegFrameInfo *pFrameInfo = (sFfmpegFrameInfo *)pBufferEx->GetData();
		if (pFrameInfo)
		{
			m_s64Timestamp = pFrameInfo->m_s64Pts;
			m_s64Pts = pFrameInfo->m_s64Pts;
		}
		//debug("%s, pts: %d\n", __FUNCTION__, (int)pFrameInfo->m_s64Pts);
	}
	//获取一个 outpin buff

	pPinOut->GetFreeBuffer(&pOutBuffer);
	if ( !pOutBuffer)
	{
		if (pBufferCodec)
			pPinIn->PutFreeBuffer(pBufferCodec);
		return CDecoder::DoFilter();

	}
	CVideoFramePinBufer *pVideoFrame = dynamic_cast<CVideoFramePinBufer *>(pOutBuffer);
	if (pVideoFrame)
	{//
		AVFrame *pPicture = NULL;
		AVCodecContext *pCodecContext =  NULL;
#ifdef FFMPEG_OLD
		pPicture = m_pDecoder->picture;
		pCodecContext = m_pFfmpeg->c;
#elif defined(FFMPEG_NEW)
		pPicture = m_pDecoder->GetFrame();
		pCodecContext = m_pDecoder->GetCodecContext();
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
		CBuffer *pExtBufferCodecFrame = 0;
		if (pBufferCodec)
			pExtBufferCodecFrame  = pBufferCodec->GetExtData();
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
		if (pOutBuffer)
			pPinOut->PutPreparedBuffer(pOutBuffer);
	}
#endif
	//解码完毕 释放 buffer 
ret:	
	if (pBufferCodec)
	{
		pPinIn->PutFreeBuffer(pBufferCodec);
		pBufferCodec = NULL;
	}
	//
	//debug("%s , in[%d] out: %d\n", __FUNCTION__, pPinIn->GetPreparedBufferConut(), pPinOut->GetPreparedBufferConut());

	return CDecoder::DoFilter();
}
//目前只是支持 YV12 数据, 从 ffmpeg 的 frame copy到显示需要的 frame 
int CFfmpegDecoderFilter::CopyAvFrame2VideoFrame(void *pAvFrame, void *pVideoFrame)
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

	//int nLine
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nDesStrideLine);
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
		memcpy(pDes, pSrc, nDesStrideLine);
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
		memcpy(pDes, pSrc, nDesStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}



	return 0;
}
#define  WIDTH_FLAG_POS 23
int CFfmpegDecoderFilter::CreateDecoder()
{
	 
	if (m_pDecoder)
	{
		return 1;
	}
	int nRet = 0;
	m_pDecoder = new CFfmpegCodec();
	if (m_streamInfo.m_nDataType == AVMEDIA_TYPE_VIDEO)
	{
		m_pDecoder->SetVideoParam(m_streamInfo.m_videoInfo);
		nRet = m_pDecoder->Open();

		if (nRet >= 0)
		{

			if (0&& m_streamInfo.m_videoInfo.m_nCodecId == AV_CODEC_ID_MPEG4)
			{//
				unsigned char exdata[42] = {
					0,0,1,176,245,0,0,1,181,9,  //10
					0,0,1,0,0,0,1,32,0,134,   //10
					196,0,103,12, 0 , 0x10,  0, 81,143,0,  //10
					0,1,178,88,118,105,68,48,48,53,
					48,0};
					//填写 分辨率， 五个字节， 第一个 最低两位， 然后13bit + 1bit 无效+13 bit， （第五字节 取H1 bit）
					//WIDTH_FLAG_POS
					// L2 + 8 + H3 //
					unsigned int nWidth = m_streamInfo.m_videoInfo.m_nWidth;
					unsigned int nHeight = m_streamInfo.m_videoInfo.m_nHeight;
					exdata[WIDTH_FLAG_POS] |= ( (0x1800&nWidth)>>12 ) ; //最高两位
					exdata[WIDTH_FLAG_POS+1] = ((0x7f8&nWidth)>>3 );
					exdata[WIDTH_FLAG_POS +2] |= ((0x7&nWidth)<<5);
					// height; //低四位开始
					exdata[WIDTH_FLAG_POS +2] |= ((0x1e00&nHeight)>>9 );
					exdata[WIDTH_FLAG_POS+3] = ((0x1fe&nHeight)>>1 ) ;
					exdata[WIDTH_FLAG_POS+4] |= ((0x01*nHeight)<<7 );
// 					FILE *fileMpegHeader = fopen("f:\\tmp\\mpeg4.header", "w+b");
// 					if (fileMpegHeader)
// 					{
// 						fwrite(exdata, 1, sizeof(exdata), fileMpegHeader);
// 						fclose(fileMpegHeader);
// 					}
					m_pDecoder->Decode(exdata, 41);
			}

		}
		else
		{
			m_pDecoder->Close();
			delete m_pDecoder;
			m_pDecoder = NULL;
		}
	}


	return nRet;

	int nCodecId = 0;
	nCodecId = atoi(m_mapParamValue["codec"].c_str());
	const char *pType = m_mapParamValue["type"].c_str();
	if (strcmp(pType , "video"))
	{
		sFfmpegVideoInfo videoInfo;
		videoInfo.m_nCodecId = nCodecId;
		videoInfo.m_nHeight = atoi(m_mapParamValue["height"].c_str());
		videoInfo.m_nWidth = atoi(m_mapParamValue["width"].c_str());

		m_pDecoder->SetVideoParam(videoInfo);
		nRet = m_pDecoder->Open();
	}

	return 0;
}

int CFfmpegDecoderFilter::CopyFrame( unsigned char *pImageOut, int nImageLen, int nW, int nH, int pix_fmt )
{
	if (m_pDecoder == NULL)
	{
		return -1;
	}
	int nRet = 0;
	nRet = m_pDecoder->CopyFrame(pImageOut, nImageLen, nW, nH, (AVPixelFormat)pix_fmt);

	return nRet;
}

void CFfmpegDecoderFilter::Release()
{
	delete this;
}

int64 CFfmpegDecoderFilter::GetTimestamp()
{
	return m_s64Timestamp;
}

void CFfmpegDecoderFilter::Reset()
{
	if (m_pDecoder)
	{
		m_pDecoder->Reset();
	}
}


