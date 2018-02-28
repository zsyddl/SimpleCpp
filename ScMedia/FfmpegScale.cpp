#include "StdAfx.h"
#include "FfmpegScale.h"
/*#include <QDebug>*/
extern "C"{
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
};

CFfmpegScale::CFfmpegScale(void)
: m_pSwsContext(NULL)
, m_nWSrc(0)
, m_nHSrc(0)
, m_nWDes(0)
, m_nHDes(0)
, m_nPixFmtSrc(0)
, m_nPixFmtDes(0)
, m_pAvPictureSrc(0)
, m_pAvPictureDes(0)
, m_pAvPictureBuffer(NULL)
, m_nFilterType(SWS_FAST_BILINEAR)
{
	m_pAvPictureSrc = new AVPicture;
	memset(m_pAvPictureSrc, 0, sizeof(AVPicture));
	m_pAvPictureDes = new AVPicture;
	memset(m_pAvPictureDes, 0, sizeof(AVPicture));
}

CFfmpegScale::~CFfmpegScale(void)
{
	if (m_pSwsContext)
	{
		sws_freeContext(m_pSwsContext);
		m_pSwsContext = NULL;
	}
	if (m_pAvPictureDes)
	{
		avpicture_free(m_pAvPictureDes);

		delete m_pAvPictureDes;
		m_pAvPictureDes = NULL;
	}
	if (m_pAvPictureSrc)
	{
		//不能释放
		//avpicture_free(m_pAvPictureSrc);

		delete m_pAvPictureSrc;
		m_pAvPictureSrc = NULL;
	}
	if (m_pAvPictureBuffer)
	{
		av_free(m_pAvPictureBuffer);
		m_pAvPictureBuffer = NULL;
	}
}

int CFfmpegScale::Scale( uint8 *pSrc, int nWSrc, int nHSrc, int nPixFmtSrc, uint8 *pDes, int nWDes, int nHDes, int nPixFmtDes )
{
	if (! m_pSwsContext 
		|| m_nHSrc != nHSrc
		|| m_nWSrc != nWSrc
		|| m_nWDes != nWDes
		|| m_nHDes != nHDes
		|| m_nPixFmtSrc != nPixFmtSrc
		|| m_nPixFmtDes != nPixFmtDes)
	{
		if (m_pSwsContext)
		{
			sws_freeContext(m_pSwsContext);
			m_pSwsContext = NULL;
		}
		debug("%s, %d\n", __FUNCTION__, __LINE__);
		m_pSwsContext = sws_getContext(nWSrc, nHSrc, (AVPixelFormat)nPixFmtSrc,  
			nWDes, nHDes, (AVPixelFormat)nPixFmtDes,  
			m_nFilterType,   
			NULL, NULL, NULL);  
		debug("%s, %d\n", __FUNCTION__, __LINE__);
		m_nHSrc = nHSrc;
		m_nWSrc = nWSrc;
		m_nHDes = nHDes;
		m_nWDes = nWDes;

		m_nPixFmtSrc = nPixFmtSrc;
		m_nPixFmtDes = nPixFmtDes;

		if (m_pAvPictureDes)
		{
			avpicture_free(m_pAvPictureDes);
			//m_pAvPictureDes = NULL;
		}
		if (m_pAvPictureSrc)
		{
			avpicture_free(m_pAvPictureSrc);
			//m_pAvPictureSrc = NULL;
		}
		if (m_pAvPictureBuffer)
		{
			av_free(m_pAvPictureBuffer);
			m_pAvPictureBuffer = NULL;
			av_malloc(m_nHSrc*m_nWSrc*4);
		}
		//avpicture_alloc(m_pAvPictureSrc, (AVPixelFormat)nPixFmtSrc, m_nWSrc+32, m_nHSrc);
		//avpicture_fill(m_pAvPictureSrc, m_pAvPictureBuffer, (AVPixelFormat)nPixFmtSrc, m_nWSrc+32, m_nHSrc);
		avpicture_alloc(m_pAvPictureDes, (AVPixelFormat)nPixFmtDes, m_nWDes, m_nHDes);
	}

	/*
	int nSrcStride[1];
	uint8_t *pSrcBuff[3] = {0, 0, 0};
	if (nPixFmtSrc == AV_PIX_FMT_YUV420P)
	{
		nSrcStride[0] = nWSrc;
		nSrcStride[1] = nWSrc/2;
		nSrcStride[2] = nWSrc/2;

		pSrcBuff[0] = pSrc;
		pSrcBuff[1] = pSrc+nWSrc*nHSrc;
		pSrcBuff[2] = pSrc+nWSrc*nHSrc*5/4;
	}
	else if(nPixFmtSrc == AV_PIX_FMT_GRAY8)
	{
		nSrcStride[0] = nWSrc;
		pSrcBuff[0] = pSrc;
	}
	else if(nPixFmtSrc == AV_PIX_FMT_YUYV422)
	{

	}
	*/

	
	//填充图片
	//导致程序崩溃？
	
	int nSrcSize = 0;
	nSrcSize = avpicture_get_size((AVPixelFormat)nPixFmtSrc, m_nWSrc, m_nHSrc);
	//memcpy(m_pAvPictureBuffer, pSrc, nSrcSize);

	//	avpicture_fill(m_pAvPictureSrc, (const uint8_t*)m_pAvPictureBuffer, (AVPixelFormat)nPixFmtSrc, m_nWSrc, m_nHSrc);
	//不需要提前分配内存，该函数会使用 pSrc 内存填充AVPicture 的内存指针
	avpicture_fill(m_pAvPictureSrc, pSrc, (AVPixelFormat)nPixFmtSrc, m_nWSrc, m_nHSrc);

	//return 0;

// 	int nDstStride[1];
// 	uint8_t *pDesBuff[3] = {0, 0, 0};
// 
// 	nDstStride[0] = nWDes;
// 	pDesBuff[0] = pDes;
	//qDebug("%s, %d", __FUNCTION__, __LINE__);
	/*sws_scale(m_pSwsContext, pSrcBuff,  
		nSrcStride, 0, nHSrc,  
		pDesBuff, nDstStride);  */

	sws_scale(m_pSwsContext, m_pAvPictureSrc->data, m_pAvPictureSrc->linesize,
		0, nHSrc,
		m_pAvPictureDes->data, m_pAvPictureDes->linesize);
	
 
	int nDesSize = 0;
	nDesSize = avpicture_get_size((AVPixelFormat)nPixFmtDes, m_nWDes, m_nHDes);
	//复制出去
	avpicture_layout((AVPicture*)m_pAvPictureDes,
		(PixelFormat)nPixFmtDes, m_nWDes, m_nHDes, 
		pDes, nDesSize);
	//qDebug("%s, %d", __FUNCTION__, __LINE__);
 
	 
	return nDesSize;
}

int CFfmpegScale::GetPictureSize(int nW, int nH, int nPixFmt)
{
	int nDesSize = 0;
	nDesSize = avpicture_get_size((AVPixelFormat)nPixFmt, nW, nH);

	return nDesSize;
}
