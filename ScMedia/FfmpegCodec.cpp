#include "StdAfx.h"
#include "FfmpegCodec.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include <libavutil/opt.h> 
#include <libavutil/imgutils.h> 
}
#include "Mutex.h"

#ifndef debug
#define debug printf
#endif
#ifndef AV_NOPTS_VALUE
#define AV_NOPTS_VALUE 0
#endif

//#define AVCodecID CodecID
 
//CMutex CFfmpegCodec::m_mutexFfmpeg;

CFfmpeg g_ffmpeg;
CFfmpeg::CFfmpeg()
:m_pMutexFfmpeg(NULL)
{
	m_pMutexFfmpeg = new CMutex();

	avcodec_register_all();
	av_register_all();
}
CFfmpeg::~CFfmpeg()
{
	if (m_pMutexFfmpeg)
	{
		delete m_pMutexFfmpeg;
		m_pMutexFfmpeg = NULL;
	}
}
void CFfmpeg::Lock()
{
	if (m_pMutexFfmpeg)
	{
		m_pMutexFfmpeg->Lock();
	}
}
void CFfmpeg::Unlock()
{
	if (m_pMutexFfmpeg)
	{
		m_pMutexFfmpeg->Unlock();
	}
}

CFfmpegCodec::CFfmpegCodec(void)
: m_pAvCodec(0)
, m_pAvCodecContext(0)
, m_bDecodeOrEncode(true)
, m_pAvPicture(NULL)
, m_pFileTest(NULL)
, m_bDahuaFlag(false)
, m_bKeyFrame(false)
, m_bNewPicture(false)
//, m_pAvInputPicture(NULL)
{
// 	m_videoInfo.m_nPixFormat = PIX_FMT_YUV420P;
// 	m_videoInfo.m_nWidth = 1280;
// 	m_videoInfo.m_nHeight = 720;
	memset(&m_videoInfo, 0, sizeof(m_videoInfo));
	//avcodec_register_all();

}

CFfmpegCodec::~CFfmpegCodec(void)
{
	if (m_pAvCodecContext)
	{
		g_ffmpeg.Lock();
		avcodec_close(m_pAvCodecContext);
		av_free(m_pAvCodecContext);
		g_ffmpeg.Unlock();
		m_pAvCodecContext = NULL;
		m_pAvCodec = NULL;
	}
	if (m_pAvPicture)
	{
		av_free(m_pAvPicture);
		m_pAvPicture = NULL;
	}
	if (m_pFileTest)
	{
		fclose(m_pFileTest);
		m_pFileTest = NULL;
	}
// 	if (m_pAvInputPicture)
// 	{
// 		avpicture_free(m_pAvInputPicture);
// 		delete m_pAvInputPicture;
// 		m_pAvInputPicture = NULL;
// 	}
}
int CFfmpegCodec::SetVideoParam(sFfmpegVideoInfo &videoInfo)
{
	m_videoInfo = videoInfo;
	return 0;
}
int CFfmpegCodec::SetAudioParam(sFfmpegAudioInfo &audioInfo)
{
	m_audioInfo = audioInfo;
	return 0;
}
sFfmpegVideoInfo &CFfmpegCodec::GetVideoParam()
{
	return m_videoInfo;
}
sFfmpegAudioInfo &CFfmpegCodec::GetAudioParam()
{

	return m_audioInfo;
}
int	CFfmpegCodec::Open(/*int nCodecId, */bool bDecodeOrEncode)
{
	if (m_pAvCodec)
		return 1;

/*	m_nCodecId = nCodecId;*/

	//avcodec_license();
	//avcodec_init();
	//avcodec_register_all();

	//av_register_all();
	g_ffmpeg.Lock();
	if (bDecodeOrEncode)
	{
		m_pAvCodec = avcodec_find_decoder((CodecID)m_videoInfo.m_nCodecId);
	}
	else
	{
		m_pAvCodec = avcodec_find_encoder((CodecID)m_videoInfo.m_nCodecId);
	}
	if (!m_pAvCodec)
	{
		g_ffmpeg.Unlock();
		return -2;
	}

	m_pAvCodecContext = avcodec_alloc_context3(m_pAvCodec);
	//m_pAvCodecContext = avcodec_alloc_context();

	m_pAvCodecContext->codec_id = (AVCodecID)m_videoInfo.m_nCodecId;
	// note 不加这一句 mjpeg 打开会失败 
	if (m_pAvCodecContext->codec_id == CODEC_ID_MJPEG)
	{// 0.8.2 mjpeg 打开失败
		m_pAvCodecContext->strict_std_compliance =  FF_COMPLIANCE_EXPERIMENTAL;
	}
	if (m_pAvCodecContext->codec_id == CODEC_ID_MPEG4)
	{
		m_pAvCodecContext->profile = FF_PROFILE_MPEG4_SIMPLE;//FF_PROFILE_MPEG4_ADVANCED_REAL_TIME;//FF_PROFILE_MPEG4_SIMPLE;
	}

	if (m_pAvCodecContext->codec_id == CODEC_ID_MPEG2VIDEO)
	{
		m_pAvCodecContext->profile = FF_PROFILE_MPEG2_SIMPLE;
	}
	//m_pAvCodecContext->

// 	if (m_pAvCodecContext->codec_id != CODEC_ID_MJPEG
// 		&& m_pAvCodecContext->codec_id != CODEC_ID_JPEG2000
// 		&& m_pAvCodecContext->codec_id != CODEC_ID_PNG)
	if (/*bDecodeOrEncode == false
		&& */m_pAvCodecContext->codec_id == CODEC_ID_H264)
	{//编码不延迟输出 
		//fastdecode
		av_opt_set(m_pAvCodecContext->priv_data, "tune", "zerolatency", 0);
		//slow, medium, fast, veryfast,  superfast, ultrafast
		av_opt_set(m_pAvCodecContext->priv_data, "preset", "superfast", 0);
	}
	if (av_codec_is_encoder(m_pAvCodec))
	{/*
		m_pAvCodecContext->me_range = 16;
		m_pAvCodecContext->max_qdiff = 4;
		m_pAvCodecContext->qmin = 10;
		m_pAvCodecContext->qmax = 51;
		m_pAvCodecContext->qcompress = 0.6; */
		m_pAvCodecContext->time_base.den = 25;
		m_pAvCodecContext->time_base.num = 1;
		m_pAvCodecContext->gop_size = m_videoInfo.m_nGopSize;
		//m_pAvCodecContext->thread_count = 1;
		//m_pAvCodecContext->max_b_frames = 1;
	}

	if (m_pAvCodec->type == AVMEDIA_TYPE_VIDEO)
	{
		m_pAvCodecContext->width = m_videoInfo.m_nWidth;
		m_pAvCodecContext->height = m_videoInfo.m_nHeight;
		m_pAvCodecContext->pix_fmt = (PixelFormat)m_videoInfo.m_nPixFormat;
		m_pAvCodecContext->bit_rate = m_videoInfo.m_nBitRate;
		m_pAvCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
		m_pAvCodecContext->time_base.num = 1;
		m_pAvCodecContext->time_base.den = m_videoInfo.m_nFps;
		m_pAvCodecContext->gop_size = m_videoInfo.m_nGopSize;
	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_AUDIO)
	{
		m_pAvCodecContext->channels =  m_audioInfo.m_nChannels;
		m_pAvCodecContext->sample_fmt = (AVSampleFormat)m_audioInfo.m_nSampleFormat;
		m_pAvCodecContext->sample_rate = m_audioInfo.m_nSampleRate;
		m_pAvCodecContext->bit_rate = m_audioInfo.m_nBitRate;
		m_pAvCodecContext->codec_id = (CodecID)m_videoInfo.m_nCodecId;
		m_pAvCodecContext->codec_type = AVMEDIA_TYPE_AUDIO;
	}
	
	//有这个  写入的文件不能播放
	//m_pAvCodecContext->flags = 0;
	//m_pAvCodecContext->flags |= CODEC_FLAG_GLOBAL_HEADER;
	// 大华的视频 需要这个 标识。 其他的 视频不需要这个标识 
	// 否则都不解码
	if (m_bDahuaFlag)
	{
		if(m_pAvCodec->capabilities&CODEC_CAP_TRUNCATED)
			m_pAvCodecContext->flags|= CODEC_FLAG_TRUNCATED;
	}
	int nRet = 0;
	nRet = avcodec_open2(m_pAvCodecContext, m_pAvCodec, NULL);
	if ( nRet < 0)
	//if (avcodec_open(m_pAvCodecContext, m_pAvCodec) < 0)
	{
		debug("%s, open codec [%d] false: %d\n", 
				__FUNCTION__, 
			  m_pAvCodecContext->codec_id, nRet);
		av_free(m_pAvCodecContext);
		m_pAvCodecContext = NULL;
		g_ffmpeg.Unlock();
		return -3;
	}
 
	if (!m_pAvPicture)
	{
		m_pAvPicture = avcodec_alloc_frame();
		m_pAvPicture->format = m_pAvCodecContext->pix_fmt;
		m_pAvPicture->pts = 0;
		if (bDecodeOrEncode == false)
		{
			//m_pAvPicture = av_image_alloc();
			//nRet = av_image_alloc(m_pAvPicture->data, m_pAvPicture->linesize, m_pAvCodecContext->width, m_pAvCodecContext->height,
			//	m_pAvCodecContext->pix_fmt, 32);
/*			nRet = av_image_alloc(m_pAvPicture->data, m_pAvPicture->linesize, 
				m_pAvCodecContext->width, m_pAvCodecContext->height,
				m_pAvCodecContext->pix_fmt, 32);*/
		}
	}

	g_ffmpeg.Unlock();
	m_bDecodeOrEncode = bDecodeOrEncode;
	return 0;
}
void CFfmpegCodec::Close()
{
	if (m_pAvCodecContext)
	{
		g_ffmpeg.Lock();
		avcodec_close(m_pAvCodecContext);
		av_free(m_pAvCodecContext);
		g_ffmpeg.Unlock();

		m_pAvCodecContext = NULL;
		m_pAvCodec = NULL;
	}
	if (m_pAvPicture)
	{
		av_free(m_pAvPicture);
		m_pAvPicture = NULL;
	}
}
// int CFfmpegVideoCodec::InputData(uint8_t *pData, int nLen)
// {
// 
// 	return 0;
// }
void CFfmpegCodec::Flush()
{
	if (!m_pAvCodecContext )
		return ;

	avcodec_flush_buffers(m_pAvCodecContext);

}
//解码器的数据并不是全部输出。 某些码流中，如果有新的 I帧到来。上一次的最后一个帧似乎还会输出一次。

int CFfmpegCodec::Decode(uint8_t *pData, int nLen)
{
	m_bNewPicture = false;
	if (!m_pAvCodecContext )
		return -1;

// 	if (m_pFileTest == NULL)
// 	{
// 		m_pFileTest = fopen("f:\\tmp\\vs-264.h264", "w+b");
// 	}
// 	if (m_pFileTest)
// 	{
// // 		char *pszHeader = "zsyddl";
// // 		int nHeaderLen = strlen(pszHeader);
// // 		fwrite(pszHeader, 1, nHeaderLen, m_pFileTest );
// 		fwrite(pData, 1, nLen, m_pFileTest);
// 	}

	int nGotPicture = 0;
	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = pData;
	avPacket.size = nLen;

	if (m_pAvCodec->type == AVMEDIA_TYPE_VIDEO)
	{
		int nRet = 0;
		try{
			
			nRet = avcodec_decode_video2(m_pAvCodecContext, m_pAvPicture, &nGotPicture, &avPacket);
			if (m_pAvPicture->pict_type == AV_PICTURE_TYPE_I)
			{
				//debug("%s decode a I frame \n", __FUNCTION__);
			}
		}
		catch(...){
			debug("%s error\n", __FUNCTION__);
		}
		/*avcodec_decode_video*/
		if (nGotPicture == 0)
		{
			return -2;
		}
		if (nGotPicture >  0)
			m_bNewPicture = true;

		m_videoInfo.m_nWidth = m_pAvPicture->width;
		m_videoInfo.m_nHeight = m_pAvPicture->height;
		m_videoInfo.m_nFps = m_pAvCodecContext->time_base.num;
	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_AUDIO)
	{
		
	}
	 
	return 0;
}
int CFfmpegCodec::Decode(uint8_t *pData, int nLen, uint8_t *pDecodeData, int &nDecodeLen)
{
	if (!m_pAvCodecContext || !m_pAvCodec)
		return -1;
	int nGotPicture = 0;
	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = pData;
	avPacket.size = nLen;
	if (m_pAvCodec->type == AVMEDIA_TYPE_VIDEO)
	{
		avcodec_decode_video2(m_pAvCodecContext, m_pAvPicture, &nGotPicture, &avPacket);
		/*avcodec_decode_video*/
		if (nGotPicture == 0)
		{
			return -2;
		}
		nDecodeLen = avpicture_get_size((PixelFormat)m_videoInfo.m_nPixFormat, m_videoInfo.m_nWidth, m_videoInfo.m_nHeight);
		// read the data to the buffer
		if (pDecodeData)
		{
			avpicture_layout((AVPicture*)m_pAvPicture, (PixelFormat)m_videoInfo.m_nPixFormat, m_videoInfo.m_nWidth, m_videoInfo.m_nHeight, 
			pDecodeData, nDecodeLen);
		}
		
	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_AUDIO)
	{
		
		int nDecodeRet = 0;
		//avcodec_decode_audio4(m_pAvFormatContext->streams[nStreamId]->codec, m_pAvFrame, &nGotFrame, &avPacket);
		nDecodeRet = avcodec_decode_audio3(m_pAvCodecContext, (short*)pDecodeData, &nDecodeLen, &avPacket);
		if (nDecodeRet == 0)
		{
			nDecodeLen = -1;
			return -1;
		}
	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_SUBTITLE)
	{

	}
	else
	{

	}
	return 0;
}
//在编码的时候 才有效
int64_t CFfmpegCodec::GetPts()
{
	if (!m_pAvPicture)
		return 0;
	return m_pAvPicture->pts;
}
int CFfmpegCodec::GetVideoFrameSize()
{
	return avpicture_get_size((PixelFormat)m_videoInfo.m_nPixFormat, m_videoInfo.m_nWidth, m_videoInfo.m_nHeight);
}
int CFfmpegCodec::Encode(uint8_t *pData, int nLen)
{
	if (!m_pAvCodecContext || !m_pAvCodec)
		return -1;
	int nGotPicture = 0;
	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = pData;
	avPacket.size = nLen;
	if (m_pAvCodec->type == AVMEDIA_TYPE_VIDEO)
	{
		//avcodec_encode_video2(m_pAvCodecContext, m_pAvPicture, &nGotPicture, &avPacket);
		/*avcodec_decode_video*/
		if (nGotPicture == 0)
		{
			return -2;
		}
	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_AUDIO)
	{

	}
	return 0;
}
int CFfmpegCodec::Encode(uint8_t *pData, int nLen, uint8_t *pEncodeData, int &nEncodeLen)
{
	
	if (!m_pAvCodecContext || !m_pAvCodec)
	{
		nEncodeLen = 0;
		return -1;
	}

	int nGotPicture = 0;

	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = pEncodeData;
	avPacket.size = nEncodeLen;
	/*nEncodeLen = 0;*/
	if (m_pAvCodec->type == AVMEDIA_TYPE_VIDEO)
	{
		//memset(pData, 0, nLen);
		m_pAvPicture->format = (PixelFormat)m_videoInfo.m_nPixFormat;
		int nSizeFilled = 0;
		nSizeFilled = avpicture_fill((AVPicture*)m_pAvPicture, (uint8_t *)pData,
			(PixelFormat)m_videoInfo.m_nPixFormat, m_videoInfo.m_nWidth, m_videoInfo.m_nHeight);
		m_pAvPicture->height = m_videoInfo.m_nHeight;
		m_pAvPicture->width = m_videoInfo.m_nWidth;
		m_pAvPicture->format = (PixelFormat)m_videoInfo.m_nPixFormat;
		if (m_pAvCodecContext->frame_number%m_videoInfo.m_nGopSize == 0)
		{
			m_pAvPicture->key_frame = 1;
			m_pAvPicture->pict_type = AV_PICTURE_TYPE_I;
			m_bKeyFrame = true;
		}
		else
		{
			m_pAvPicture->key_frame = 0;
			m_pAvPicture->pict_type = AV_PICTURE_TYPE_P;
			m_bKeyFrame = false;
		}
#ifdef WIN32
		m_pAvPicture->pts++;
#else

#endif
		int nRet = 0;
		nRet = avcodec_encode_video2(m_pAvCodecContext, &avPacket, m_pAvPicture, &nGotPicture);
		//nGotPicture = 1;
		//nRet = avcodec_encode_video(m_pAvCodecContext, pEncodeData, nEncodeLen, m_pAvPicture);
		nEncodeLen = avPacket.size;
		if (nRet ==  0)
		{//success
			if (nGotPicture <= 0)
			{
				nEncodeLen = 0;
				return 0;
			}

			nEncodeLen = avPacket.size;
		}
		else
		{
			nEncodeLen = 0;
		}
		//test
// 		if (!m_pFileTest)
// 		{
// 			m_pFileTest = fopen("f:\\tmp\\enc.h264", "wb+");
// 		}
// 		if(m_pFileTest)
// 		{
// 			fwrite(pEncodeData, nEncodeLen, 1, m_pFileTest);
// 		}


	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_AUDIO)
	{
		//m_pAvCodecContext->frame_size = nLen;
		if (m_pAvCodecContext->frame_size<1 )
		{
			nEncodeLen = nLen;
			//frame samples count ;
			if (m_audioInfo.m_nSampleFormat == 1/*SAMPLE_FMT_S16*/)
				m_pAvCodecContext->frame_size = nLen/2;
			else
			{
				m_pAvCodecContext->frame_size = nLen;
			}
		}
		nEncodeLen = avcodec_encode_audio(m_pAvCodecContext, pEncodeData, nEncodeLen, (short*)pData);
		if (nEncodeLen < 0)
		{
			return nEncodeLen;
		}
		//nEncodeLen = m_pAvCodecContext->frame_bits/8;
	}
	else if (m_pAvCodec->type == AVMEDIA_TYPE_SUBTITLE)
	{
		AVSubtitle subTitle;
		nEncodeLen = avcodec_encode_subtitle(m_pAvCodecContext, pEncodeData, nLen, &subTitle);
	}
	else
	{

	}

	return 0;
}
int CFfmpegCodec::GetCodecType()
{
	if (!m_pAvCodecContext || !m_pAvCodec)
	{

		return -1;
	}
	return m_pAvCodec->type ;
}
int CFfmpegCodec::GetCodecType(int nCodecId)
{
	return avcodec_get_type((CodecID)nCodecId);
}
int CFfmpegCodec::GetDecodeData(uint8_t *pData)
{
	if (!m_pAvPicture)
	{
		return -1;
	}

	// Y
	int nSrcStrideLine =0, nDesStrideLine=0;
	unsigned char *pSrc=0, *pDes = 0;
	int nWidth = 0, nHeight=0; 

	pSrc = m_pAvPicture->data[0];
	pDes = pData;
	nSrcStrideLine = m_pAvPicture->linesize[0];
	nDesStrideLine = m_pAvPicture->width;
	nHeight = m_pAvPicture->height;

	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	//U
	pSrc = m_pAvPicture->data[1];
	pDes = pData + m_pAvPicture->width*m_pAvPicture->height;
	nSrcStrideLine = m_pAvPicture->linesize[1];
	nDesStrideLine = m_pAvPicture->width/2;

	nHeight = m_pAvPicture->height/2;
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	//V
	pSrc = m_pAvPicture->data[2];
	pDes = pData + m_pAvPicture->width*m_pAvPicture->height*3/2;
	nSrcStrideLine = m_pAvPicture->linesize[2];
	nDesStrideLine = m_pAvPicture->height/2;

	nHeight = m_pAvPicture->height/2;
	for (int y=0; y<nHeight; y++)
	{
		memcpy(pDes, pSrc, nSrcStrideLine);
		pDes += nDesStrideLine;
		pSrc += nSrcStrideLine;
	}

	return 0;
}
int CFfmpegCodec::GetFameInfo(int &nLen, int &nW, int &nH, int &nType)
{
	if (!m_pAvPicture)
	{
		return -1;
	}
	nW = m_pAvPicture->width;
	nH = m_pAvPicture->height;
	nType = m_pAvPicture->format;
	nLen = nW*nH*3/2;

	return 0;
}
AVFrame *CFfmpegCodec::GetFrame()
{
	return m_pAvPicture;
}
int CFfmpegCodec::CopyFrame(uint8_t *pImageOut, int nImageLen, int nW, int nH, AVPixelFormat pix_fmt)
{
	int nRet = 0;
	if (!m_pAvCodecContext)
		return -1;
// 	if (m_pAvPicture->width <= 0
// 		|| m_pAvPicture->height <= 0
// 		)
// 		return -1;
	//struct SwsContext *img_convert_ctx;
	/*
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
	pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
	if(img_convert_ctx == NULL)
	{
		fprintf(stderr, "Cannot initialize the conversion context!\n");
		exit(1);
	}

	sws_scale(img_convert_ctx,
		pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
		pFrameRGB->data, pFrameRGB->linesize);
		*/
	SwsContext *pSwsContext = 0;
	pSwsContext = sws_getContext(m_pAvCodecContext->width, m_pAvCodecContext->height, m_pAvCodecContext->pix_fmt,
					nW, nH, pix_fmt, SWS_FAST_BILINEAR, NULL,  NULL, NULL);
	if (pSwsContext == NULL)
		return -1;

	//AVPicture *pPicOut = avpicture_alloc()
	uint8_t *pDataOut[4] = {0};
	pDataOut[0] = pImageOut;
	int  nDataOutLen[4] = {0};
	if (pix_fmt == AV_PIX_FMT_ARGB
		|| pix_fmt == AV_PIX_FMT_RGBA
		|| pix_fmt == AV_PIX_FMT_BGRA)
	{
		nDataOutLen[0] = nW*4;
		pDataOut[0] = pImageOut;
		nImageLen = nW*nH*4;
	}
	else if (pix_fmt == AV_PIX_FMT_YUV420P)
	{
		nDataOutLen[0] = nW;
		nDataOutLen[1] = nW/2;
		nDataOutLen[2] = nW/2;

		pDataOut[0] = pImageOut;
		pDataOut[1] = pDataOut[0] + nW*nH;
		pDataOut[2] = pDataOut[1] + nW*nH/4;

		nImageLen = nW*nH*3/2;
	}

	nRet = sws_scale(pSwsContext, m_pAvPicture->data, m_pAvPicture->linesize, 0, m_pAvCodecContext->height,
		     pDataOut, nDataOutLen);
	//
	/*AVPicture out_pic;
	if(avpicture_alloc((AVPicture *)&out_pic, PIX_FMT_RGB24, nW, nH)>=0) 
	{ 
	//	pFrame->data[0] = pFrame->data[0]+pFrame->linesize[0]*(pContext->height-1); 
	//	pFrame->data[1] = pFrame->data[1]+pFrame->linesize[0]*pContext->height/4-1; 
	//	pFrame->data[2] = pFrame->data[2]+pFrame->linesize[0]*pContext->height/4-1; 
	///	pFrame->linesize[0] *= -1; 
	//	pFrame->linesize[1] *= -1; 
	//	pFrame->linesize[2] *= -1; 
		nRet =  sws_scale(pSwsContext, m_pAvPicture->data, m_pAvPicture->linesize, 0, m_pAvPicture->height
			, out_pic.data, out_pic.linesize); 
		avpicture_free((AVPicture *)&out_pic); 
	} */

	sws_freeContext(pSwsContext);
	if (nRet < 0)
		return -1;
	//nRet = avpicture_layout((AVPicture*)m_pAvPicture, pix_fmt, nW, nH,  pImageOut, nImageLen );
	//img_convert(0, 0, 0, 0, 0, 0, );
	return nRet ;
}
int CFfmpegCodec::SetVideoParam( int nWith, int nHeight, int nPixFormat, int nBitRates )
{

	m_videoInfo.m_nBitRate = nBitRates;
	m_videoInfo.m_nHeight = nHeight;
	m_videoInfo.m_nWidth = nWith;
	m_videoInfo.m_nPixFormat = nPixFormat;

	return 0;
}

int CFfmpegCodec::GetTimeBase( int &nTimeBaseNum, int &nTimeBaseDen )
{
	nTimeBaseDen = m_pAvCodecContext->time_base.den;
	nTimeBaseNum = m_pAvCodecContext->time_base.num;

	return 0;
}

int CFfmpegCodec::GetVideoSize( int &nW, int &nH )
{
	nW = m_pAvCodecContext->width;
	nH = m_pAvCodecContext->height;
	return 0;
}

void CFfmpegCodec::SetDahuaFlag()
{
	m_bDahuaFlag = true;
}

bool CFfmpegCodec::IsKeyFrame()
{
	return m_bKeyFrame;
}

int CFfmpegCodec::GetFrameType()
{
	if (!m_pAvPicture)
		return AV_PICTURE_TYPE_NONE;

	return m_pAvPicture->pict_type;
}

bool CFfmpegCodec::HasNewPicture()
{
	return m_bNewPicture;
}

void CFfmpegCodec::Reset()
{
	Close();
	Open(m_bDecodeOrEncode);
}

