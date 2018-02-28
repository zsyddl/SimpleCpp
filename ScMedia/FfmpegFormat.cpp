#include "StdAfx.h"
#include "FfmpegFormat.h"
#include "libavutil/dict.h"
#include "stdio.h"
#include "stdlib.h"
#include "Charset.h"
#include <sys/stat.h>
#include "H264File.h"
// #include <fcntl.h>
// #include <io.h>

/*
* sqlite内存数据库rc = sqlite3_open("file:memdb1?mode=memory&cache=shared", &db);
*/

#define CODEC_ID_VIVS_HK 0x20002
CFfmpegFormat::CFfmpegFormat(void)
: m_pAvOutFormat(NULL)
, m_pAvInFormat(NULL)
, m_pAvFormatContext(NULL)
, m_s64VideoTimeStamp(0) 
, m_u32FrameFlag(0)
, m_s64TimeStampLastKeyFrame(0)
, m_bSeek(false)
//, m_pH264PpsSps(NULL)
//, m_nH264InfoSize(0)
, m_bMp4H264(false)
, m_pMp4H264Ctx(NULL)
, m_s64ReadSize(0)
, m_s64FileSize(0)
, m_nSeekFlag(0)
, m_s64AudioTimeStamp(0)
, format_opts(0)
, m_pH264File(0)
{
	//非 0.8.2版本 的 release 模式 下会 异常 vs设置：连接选项，保留未引用信息 OPT:NOREF
	//av_register_all();
	//avcodec_register_all();
	// ffmpegcodec.h .cpp里面自动全局类 完成该功能
	debug("%s, av register\n", __FUNCTION__);
}

CFfmpegFormat::~CFfmpegFormat(void)
{
// 	if (m_pH264PpsSps)
// 	{
// 		free(m_pH264PpsSps);
// 		m_pH264PpsSps = NULL;
// 	}
	if (m_pMp4H264Ctx)
	{
		delete m_pMp4H264Ctx;
		m_pMp4H264Ctx = NULL;
	}
	if (m_pH264File)
	{
		m_pH264File->Close();
		delete m_pH264File;
		m_pH264File = NULL;
	}
}
long CFfmpegFormat::Create(char *pFormat, bool bReadOrWrite)
{
 
	if (m_pAvOutFormat)
	{
		return 2;
	}
	 
	if (!bReadOrWrite)
	{//write
		char szFile[256] = "1.";

		if (strstr(pFormat, ".") != NULL)
		{//完整文件路径
			strcpy(szFile, pFormat);
		}
		else
		{//只有格式： avi, mkv
			strcpy(szFile + strlen("1."), pFormat);
		}
		/*
#ifdef FFMPEG_52
		m_pAvOutFormat = guess_format(NULL, szFile, NULL);
#elif  defined(FFMPEG_80)
		m_pAvOutFormat = av_guess_format(NULL, szFile , NULL);
#endif
		if (!m_pAvOutFormat)
			return -1;

		//strcpy(m_pAvOutFormat->long_name, "vivs-video");
#ifdef FFMPEG_52
		m_pAvFormatContext = av_alloc_format_context();
#elif  defined(FFMPEG_80)
		m_pAvFormatContext = avformat_alloc_context();
#endif
		*/
		avformat_alloc_output_context2(&m_pAvFormatContext, NULL, pFormat, szFile);
		//
		m_pAvOutFormat = m_pAvFormatContext->oformat;
		m_pAvFormatContext->start_time = 0000;
		m_pAvFormatContext->duration = 2000000000;
		m_pAvFormatContext->bit_rate = 2*1024*1024;
		/*m_pAvFormatContext->*/
#ifdef WIN32
		_snprintf(m_pAvFormatContext->filename, sizeof(m_pAvFormatContext->filename), "%s", szFile);
#else
        snprintf(m_pAvFormatContext->filename, sizeof(m_pAvFormatContext->filename), "%s", szFile);
#endif
	}
	else
	{// read 
	}
	m_nOpenFlag = bReadOrWrite;

	return 0;
}
/*
* mp4 的 h264 不是 原始 h264流，需要取得 pps sps， 同时第五个字节才是264数据，重新添加为 00 00 00 01 xx
*/
int CFfmpegFormat::Open(char *pSzFile,  int nFlag)
{
	 
	m_nOpenFlag = nFlag;
	if ( m_nOpenFlag&AVIO_FLAG_WRITE && !m_pAvOutFormat)
	{
		return -1;
	}
	else if (m_nOpenFlag==AVIO_FLAG_READ && m_pAvFormatContext)
	{
		return -2;
	}

	char szFileUtf8[1024];
	memset(szFileUtf8, 0, sizeof(szFileUtf8));
	CCharset charset;
	charset.From(pSzFile, "gb18030");
	int nRet = 0;
	nRet = charset.To(szFileUtf8, "utf-8");
	//strcpy(szFileUtf8, pSzFile);
	if (nRet < 0)
		return -3;


	if (m_nOpenFlag&AVIO_FLAG_WRITE)
	{
#ifdef WIN32
		_snprintf(m_pAvFormatContext->filename, sizeof(m_pAvFormatContext->filename), "%s", szFileUtf8);
#else
        snprintf(m_pAvFormatContext->filename, sizeof(m_pAvFormatContext->filename), "%s", szFileUtf8);
#endif
	}
	
	if (m_nOpenFlag&AVIO_FLAG_WRITE)
	{//write
		
#ifdef FFMPEG_52
		if (url_fopen(&m_pAvFormatContext->pb, szFileUtf8, URL_RDWR) < 0)
		{
			return -2;
		}
#elif defined(FFMPEG_80)
		g_ffmpeg.Lock();
		//if (!(m_pAvOutFormat->flags & AVFMT_NOFILE)) {
			if (avio_open(&m_pAvFormatContext->pb, szFileUtf8, m_nOpenFlag) < 0)
			{
				g_ffmpeg.Unlock();
				return -2;
			}
			g_ffmpeg.Unlock();
		//}
		//av_dump_format(m_pAvFormatContext, 0, pSzFile, 1);
#endif
// 
// 		if (avformat_set_parameters(this->outputContext, NULL) < 0)
// 		{
// 			throw runtime_error("Invalid output format parameters.");
// 		}
#ifdef FFMPEG_52
		if (av_set_parameters(m_pAvFormatContext, NULL) < 0)
		{
				//throw runtime_error("Invalid output format parameters.");
			return -3;
		}
		av_write_header(m_pAvFormatContext);
#elif defined(FFMPEG_80)
		AVDictionary *pAvDict = 0;
		

		//if (m_nOpenFlag ==AVIO_FLAG_WRITE )
		{//纯写入新的文件
			nRet = avformat_write_header(m_pAvFormatContext, NULL);
		}
		//else
		{//打开现有的文件

		}
#endif
	}
	else
	{// read , 
		//m_pAvInFormat = av_guess_format(NULL, pSzFile, NULL);
		//if (!m_pAvInFormat)
		//	return -1;
		g_ffmpeg.Lock();
		int nCodecTag = ('H'<<24|'K'<<16|'0'<<8|'0');
#ifdef FFMPEG_52
		if (av_open_input_file(&m_pAvFormatContext, szFileUtf8, NULL, 10*1024, NULL) != 0)
		{
			return -2;
		}
#elif defined(FFMPEG_80)
		int nRet = 0;
		nRet = avformat_open_input(&m_pAvFormatContext, szFileUtf8, NULL, NULL);
		//nRet = avformat_open_input(&m_pAvFormatContext, szFileUtf8, /*m_pAvFormatContext->iformat*/, &format_opts);
		if ( nRet  != 0)
		{
			g_ffmpeg.Unlock();
			return -2;
		}
#endif
		//avi 文件 如果第一个不是I帧， ffmpeg 找不到streaminfo
		//mpeg 之内的文件可以找到
		if (av_find_stream_info(m_pAvFormatContext) < 0)
		{
			g_ffmpeg.Unlock();
			return -3;
		}

		//m_pVideoStream = m_pAvFormatContext->streams[0];
		//m_pParStream = m_pAvFormatContext->streams[1];
		m_s64VideoTimeStamp = m_pAvFormatContext->streams[0]->start_time;
		//mp4 的h264文件 需要特殊处理
		if (strstr(m_pAvFormatContext->iformat->name, "mp4")
			&& m_pAvFormatContext->streams[0]->codec->codec_id == AV_CODEC_ID_H264)
		{
			m_bMp4H264 = true;
// 			if (ParseMp4H264Info()< 0)
// 			{
// 				debug("%s, parse mp4 h264 info pps sps fasle", __FUNCTION__);
// 				nRet =  -4;
// 			}
			m_pMp4H264Ctx = new H264BSFContext;
			memset( m_pMp4H264Ctx, 0, sizeof(H264BSFContext) );
		}

		g_ffmpeg.Unlock();

		if (m_pAvFormatContext->duration < 0 /*&& 0*/
			&& strstr(m_pAvFormatContext->iformat->long_name, "raw H.264"))
		{//处理 h264 裸文件
			m_pH264File = new CH264File();
			int nNum(1), nDen(25);
			GetTimeBase(nNum, nDen);
			m_pH264File->SetTimebase(nNum, nDen);
			int nFpsNum(1), nFpsDen(25);
			GetFrameRate(nFpsNum, nFpsDen);

			m_pH264File->SetFrameRate(nFpsNum, nFpsDen);

			nRet = m_pH264File->Open(pSzFile, AVIO_FLAG_READ);
			// 	CH264File file;
			// 	nRet = file.Open(pSzFile, AVIO_FLAG_READ);
			if (nRet >= 0)
			{
				debug("%s, open H264 file success\n", __FUNCTION__);
			}
			else
			{
				debug("%s, open H264 file false\n", __FUNCTION__);
			}
		}
		//

	}

	m_s64FileSize = avio_size(m_pAvFormatContext->pb);
// 	struct _stat64 buf;
// 
// 	if (_stati64(pSzFile, &buf) == 0)
// 	{
// 		m_s64FileSize =  buf.st_size; 
// 	}
// 	else
// 	{
// 
// 	}
 
	return nRet;
}
int CFfmpegFormat::AddStream(int nStreamId)
{
	if (!m_pAvFormatContext)
	{
		return -1;
	}
	if (nStreamId <0)
		return -2;
	//必须创建流 
	AVStream *pStream = av_new_stream(m_pAvFormatContext, nStreamId);
	if (!pStream)
	{
		return -3;
	}
	pStream->start_time = 000;
	pStream->duration = 2000000000000;

	AVCodecContext *pAvCodecContext = pStream->codec;
	pAvCodecContext->codec_id = (CodecID)CODEC_ID_RAWVIDEO;

#ifdef FFMPEG_52
	pAvCodecContext->codec_type = CODEC_TYPE_VIDEO;
#elif defined(FFMPEG_80)
	pAvCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
#endif

	//这个标记用于 标记前端 类型 
	pAvCodecContext->codec_tag = ('H'<<24|'K'<<16|'0'<<8|'0');
	/*pAvCodecContext->stream_codec_tag = ('V'<<24|'I'<<16|'V'<<8|'S');*/
	//pAvCodecContext->flags |=  CODEC_FLAG_GLOBAL_HEADER;
	pAvCodecContext->bit_rate = 10;
	pAvCodecContext->width = 352;
	pAvCodecContext->height= 288;
	pAvCodecContext->time_base.den = 1000;// 该数据 会 在第二个帧， 被 增加 num/den 的时间刻度 
	pAvCodecContext->time_base.num = 1;

	pAvCodecContext->gop_size = 0;
	pAvCodecContext->pix_fmt = PIX_FMT_BGR24;

	m_lstStream.push_back(pStream);

	return 0;
}
int CFfmpegFormat::AddStream(int nStreamId, sFfmpegStreamInfo &streamInfo)
{
	if (!m_pAvFormatContext)
	{
		return -1;
	}
	if (nStreamId <0)
		return -2;
	if (m_nOpenFlag == AVIO_FLAG_READ )
	{
		return 0;
	}
	//必须创建流 
	//AVStream *pStream = av_new_stream(m_pAvFormatContext, nStreamId);
	AVStream *pStream = NULL;
	AVCodec *pEncCodec = avcodec_find_encoder((CodecID)streamInfo.m_nCodecId);
	if(pEncCodec== NULL)
		return -1;
	pStream = avformat_new_stream(m_pAvFormatContext, pEncCodec);
	pStream->id = m_pAvFormatContext->nb_streams-1;
	pStream->start_time = 0;
	pStream->duration = 200000000;
	pStream->time_base.den = streamInfo.m_nTimebaseDen;//streamInfo.m_videoInfo.m_nFps;;
	pStream->time_base.num = streamInfo.m_nTimebaseNum;//1;//
	pStream->r_frame_rate.num = streamInfo.m_videoInfo.m_nFps;
	pStream->r_frame_rate.den = 1;
	//av_free(pStream->codec);
	AVCodecContext *pAvCodecContext = pStream->codec;

	avcodec_get_context_defaults3(pAvCodecContext, pEncCodec);
	//pStream->codec = pAvCodecContext; 
	pAvCodecContext->codec_id = (CodecID)streamInfo.m_nCodecId;


	if (streamInfo.m_nDataType == eFDT_Video)
	{
#ifdef FFMPEG_52
	pAvCodecContext->codec_type = CODEC_TYPE_VIDEO;
#elif defined(FFMPEG_80)
		pAvCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
#endif
		//这个标记用于 标记前端 类型 
		//pAvCodecContext->codec_tag = ('H'<<24|'K'<<16|'0'<<8|'0');
		/*pAvCodecContext->stream_codec_tag = ('V'<<24|'I'<<16|'V'<<8|'S');*/
		//pAvCodecContext->flags |=  CODEC_FLAG_GLOBAL_HEADER;
		pAvCodecContext->bit_rate = streamInfo.m_videoInfo.m_nBitRate;
		pAvCodecContext->width = streamInfo.m_videoInfo.m_nWidth;
		pAvCodecContext->height= streamInfo.m_videoInfo.m_nHeight;
		pAvCodecContext->time_base.den = streamInfo.m_nTimebaseDen;//streamInfo.m_videoInfo.m_nFps;// 该数据 会 在第二个帧， 被 增加 num/den 的时间刻度 
		pAvCodecContext->time_base.num = streamInfo.m_nTimebaseNum;//1;
		
		pAvCodecContext->gop_size = streamInfo.m_videoInfo.m_nGopSize;
		pAvCodecContext->pix_fmt = (PixelFormat) ( (streamInfo.m_videoInfo.m_nPixFormat==-1)?PIX_FMT_RGB24:streamInfo.m_videoInfo.m_nPixFormat );

		m_pAvFormatContext->video_codec_id = pAvCodecContext->codec_id;
	}
	else if (streamInfo.m_nDataType == eFDT_Audio)
	{

	}
	else if (streamInfo.m_nDataType == eFDT_Text)
	{

	}
	else{

	}
	//dump_format(m_pAvFormatContext, 0, "f:\\tmp\\testWrite.avi", 1);  
	//AVCodecContext *pAvCodecContext1 = avcodec_alloc_context();
	if (m_pAvFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
	{
		m_pAvFormatContext->flags |= CODEC_FLAG_GLOBAL_HEADER; 
	}

	//open it
	//不用打开，直接写 编码好的数据就好了
	int nRet = 0;
 /*	nRet = avcodec_open2(pAvCodecContext, pEncCodec, NULL) ;
 	if( nRet < 0)
	{

		nRet = -1;
	}
*/	

	m_lstStream.push_back(pStream);

	return nRet;
}
AVStream *CFfmpegFormat::GetStream(int nIndex)
{
	if (!m_pAvFormatContext)
	{
		return NULL;
	}
	if (nIndex < 0|| nIndex> m_pAvFormatContext->nb_streams-1)
	{
		return NULL;
	}
	

	return m_pAvFormatContext->streams[nIndex];
}
void CFfmpegFormat::GetStreamInfo(sFfmpegStreamInfo &streamInfo, int nIndex)
{
	streamInfo.m_nStreamType = AVMEDIA_TYPE_UNKNOWN;
	if (!m_pAvFormatContext)
	{
		return  ;
	}
	if (nIndex < 0|| nIndex> m_pAvFormatContext->nb_streams-1)
	{
		return  ;
	}
	AVStream *pStream = m_pAvFormatContext->streams[nIndex];
	AVCodecContext *pCodec = pStream->codec;
	if (!pCodec)
	{
		return ;
	}
	streamInfo.m_nStreamType = pCodec->codec_type;
	streamInfo.m_nCodecId = pCodec->codec_id;
	if (pCodec->codec_type == AVMEDIA_TYPE_VIDEO)
	{
		streamInfo.m_videoInfo.m_nBitRate = pCodec->bit_rate;
		if (streamInfo.m_videoInfo.m_nBitRate <= 0)
		{
			streamInfo.m_videoInfo.m_nBitRate = m_pAvFormatContext->bit_rate;
		}
		 if (pStream->r_frame_rate.den == 0
			 || pStream->r_frame_rate.num == 0
			|| pStream->r_frame_rate.num/pStream->r_frame_rate.den > 60)
		{
			streamInfo.m_videoInfo.m_nFps = 25;
		}
		else
		{
			streamInfo.m_videoInfo.m_nFps = pStream->r_frame_rate.num/pStream->r_frame_rate.den;
		}
		streamInfo.m_videoInfo.m_nGopSize = pCodec->gop_size;
		streamInfo.m_videoInfo.m_nHeight = pCodec->height;
		streamInfo.m_videoInfo.m_nWidth = pCodec->width;
		streamInfo.m_videoInfo.m_nCodecId = pCodec->codec_id;
		//这个很重要。不然会导致 解码器 不可预测的崩溃
		streamInfo.m_videoInfo.m_nPixFormat = pCodec->pix_fmt;

		streamInfo.m_s64StartTimestamp = pStream->start_time;
		streamInfo.m_nTimebaseDen = pStream->time_base.den;
		streamInfo.m_nTimebaseNum = pStream->time_base.num;
	}
	else if (pCodec->codec_type == AVMEDIA_TYPE_AUDIO)
	{

	}
	else
	{

	}
}
void CFfmpegFormat::GetFrameRate(int &nNum, int &nDen)
{
	if (!m_pAvFormatContext)
	{
		return  ;
	}
	
	AVStream *pStream = GetStreamByType(AVMEDIA_TYPE_VIDEO);
	if (!pStream)
		return ;
	AVCodecContext *pCodec = pStream->codec;
	if (!pCodec)
	{
		return ;
	}

	nNum = pStream->r_frame_rate.num;
	nDen = pStream->r_frame_rate.den;
}
int CFfmpegFormat::GetStreamCount()
{
	if (!m_pAvFormatContext)
	{
		return 0;
	}
	return m_pAvFormatContext->nb_streams;
}
void CFfmpegFormat::Close()
{
	if (m_pAvFormatContext)
	{
		if (m_nOpenFlag&=AVIO_FLAG_WRITE)
		{// write
			//m_pAvFormatContext->start_time_realtime = 100;
			//m_pVideoStream->start_time = 100;
			//m_pVideoStream->duration = 100;
#ifdef FFMPEG_52
			av_write_header(m_pAvFormatContext);
			av_write_trailer(m_pAvFormatContext);

			url_fclose(m_pAvFormatContext->pb);
			//avio_close(m_pAvFormatContext->pb);
			av_free(m_pAvFormatContext);
			
			m_pAvFormatContext = NULL;
#elif	defined(FFMPEG_80)
			//avformat_write_header(m_pAvFormatContext, NULL);
			av_write_trailer(m_pAvFormatContext);

			avio_close(m_pAvFormatContext->pb);

			avformat_free_context(m_pAvFormatContext);
			m_pAvFormatContext = NULL;
#endif
		}
		else
		{// read 
#ifdef FFMPEG_52
			av_close_input_file(m_pAvFormatContext);
			m_pAvFormatContext = NULL;
#elif	defined(FFMPEG_80)
			avformat_close_input(&m_pAvFormatContext);
			m_pAvFormatContext = NULL;
#endif
		}
	}
	if (m_pH264File)
	{
		m_pH264File->Close();
		delete m_pH264File;
		m_pH264File = NULL;
	}
}
int CFfmpegFormat::ReadFrame(char *pBuffer, int &nLen, int64_t &nTimeStamp, int &nStreamId)
{
	if (m_pH264File)
	{
		int nFrameType = eFFT_I;
		int nRead = 0;
		nRead = m_pH264File->Read(pBuffer, nLen, nTimeStamp, nStreamId, nFrameType);
		if (nFrameType == eFFT_I)
		{
			m_u32FrameFlag = AV_PKT_FLAG_KEY;
		}
		m_s64VideoTimeStamp = nTimeStamp;
		return nRead;
	}
	if (!m_pAvFormatContext)
		return -1;
	if (m_nOpenFlag==AVIO_FLAG_WRITE)
		return -2;
	
// 	if (m_pH264PpsSps && m_s64TimeStamp == 0)
// 	{//mp4 h264 需要添加 pps sps
// 		memcpy(pBuffer, m_pH264PpsSps, m_nH264InfoSize);
// 		pBuffer += m_nH264InfoSize;
// 	}

	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = (uint8_t*)pBuffer;
	avPacket.size = nLen;
	avPacket.stream_index = nStreamId;
	avPacket.pts = AV_NOPTS_VALUE;
	avPacket.dts = AV_NOPTS_VALUE;
	/*m_pAvFormatContext->streams[0]->cur_dts = 02;*/
	//TRACE("to read frame, %d\n", m_pAvFormatContext);
 
	int nRt = av_read_frame(m_pAvFormatContext, &avPacket);
	if (nRt != 0)
		return -2;

	if (avPacket.flags & AV_PKT_FLAG_KEY)
	{
		//debug("%s: frame pkg flag: %d, I frame\n", __FUNCTION__, avPacket.flags);
	}

	nStreamId = avPacket.stream_index;
	nTimeStamp = avPacket.dts;
	//nLen = avPacket.size;
	/*
	if (0 && m_bSeek && !(m_nSeekFlag&AVSEEK_FLAG_ANY) )
	{
		int nTries = 1000;
		while (!( avPacket.flags&AV_PKT_FLAG_KEY) 
			&& nTries--)
		{//
			nRt = av_read_frame(m_pAvFormatContext, &avPacket);
			if (nRt != 0)
				return -2;
			debug("%s, after seek, first read, need I frame, not a I frame. %d \n", __FUNCTION__, avPacket.dts);
		}
		if(avPacket.flags&AV_PKT_FLAG_KEY)
		{
			m_bSeek = false;
		}
	}
	*/
	
//	m_bMp4H264 = false;
	int nFrameType =  (avPacket.data[4] & 0x0f);
	if (m_s64VideoTimeStamp == m_pAvFormatContext->streams[0]->start_time
		&& m_bMp4H264)
	{//还没有第一个I帧，不输出. 海康的视频类型 不能通过这个判断。 
		if ( nFrameType == 5
			//|| nFrameType == 9
			|| nFrameType == 6
			|| nFrameType == 7)
		{//分割符号
			debug("i frame \n");
		}
		else
		{
			//return 1;
		}
	}
	if (m_bMp4H264 
		&& 
		(nFrameType == 5
		//|| nFrameType == 9
		|| nFrameType == 6
		|| nFrameType == 7
		))
	{//分割符号
		//avPacket.flags = 1;
	}
	else
	{
		//avPacket.flags = 0;
	}

	if (m_bMp4H264)
	{
		nLen = 0;
		if (m_pMp4H264Ctx)
		{
			uint8_t *pOut = NULL;
			int nOutLen;
			AVCodecContext *pAvCodec = m_pAvFormatContext->streams[0]->codec;
			CAvFromatMp4::Filter(m_pMp4H264Ctx, m_pAvFormatContext->streams[0]->codec,
				NULL, &pOut, &nOutLen,
				avPacket.data, avPacket.size, 1);
			//memcpy(pBuffer, pAvCodec->extradata, pAvCodec->extradata_size);
			memcpy(pBuffer, m_pMp4H264Ctx->sps_pps_data, m_pMp4H264Ctx->size);
			nLen = m_pMp4H264Ctx->size;

		}
		avPacket.data[0] = 00;
		avPacket.data[1] = 00;
		avPacket.data[2] = 00;
		avPacket.data[3] = 01;

		memcpy(pBuffer+nLen, avPacket.data, avPacket.size);
		nLen += avPacket.size;
	}
	else
	{
		
		//这个不需要拷贝，会有问题。 同一个内存。
		//有些视频里面最后一个包特别大。
		if (nLen >= avPacket.size)
		{
			memmove(pBuffer, avPacket.data, avPacket.size);
			nLen = avPacket.size;
		}
		else{
			nLen = 1;
		}
	}
	//debug("%s: pts: %I64d, dts: %I64d, stream: %d\n", __FUNCTION__, (int)avPacket.pts, avPacket.dts, avPacket.stream_index);
	if (avPacket.pts != AV_NOPTS_VALUE)
		nTimeStamp = avPacket.pts;
	else
	{
		nTimeStamp = avPacket.dts;
	}
	if (nTimeStamp < m_s64VideoTimeStamp && m_bSeek == false)
	{
//		debug("error ts\n");
	}
	else if(avPacket.stream_index == 0)
	{//取 stream 0 作为计算时间戳 的 流单位
		m_s64VideoTimeStamp = nTimeStamp;
		m_bSeek = false;
	}
	nStreamId = avPacket.stream_index;
	
	m_u32FrameFlag = avPacket.flags;
	if (avPacket.flags != 0)
	{
		//debug("%s: frame pkg flag: %d\n", __FUNCTION__, avPacket.flags);
	}
	else
	{
		//debug("%s: frame pkg flag: %d\n", __FUNCTION__, avPacket.flags);
	}
	if (m_u32FrameFlag&AV_PKT_FLAG_KEY)
	{
		//debug("%s: is key frame: %d \n", __FUNCTION__, m_u32FrameFlag&AV_PKT_FLAG_KEY);
		m_s64TimeStampLastKeyFrame = m_s64VideoTimeStamp;
	}

	if (nTimeStamp == AV_NOPTS_VALUE
		|| nTimeStamp < m_pAvFormatContext->streams[0]->start_time)
	{
		nTimeStamp = 0;
	}
	else
	{
		nTimeStamp -= m_pAvFormatContext->streams[0]->start_time;
	}

	av_free_packet(&avPacket);

	//debug("%s: timestamp: %I64d\n", __FUNCTION__, m_s64TimeStamp);
	 
	return nLen;
}
int CFfmpegFormat::WriteFrame(char *pBuffer, int nLen,  int64_t nTimeStamp, int nFlag/*=0*/,int nStreamId )
{
	if (!m_pAvFormatContext)
		return -1;
	if (m_nOpenFlag == AVIO_FLAG_READ)
		return -2;
	if (nTimeStamp == AV_NOPTS_VALUE)
	{
		nTimeStamp = 0;
	}
	if (nTimeStamp <= 0)
		nTimeStamp = 1;
	if (nTimeStamp <= m_s64VideoTimeStamp )
	{//
		nTimeStamp = m_s64VideoTimeStamp + 1;
	}
	AVPacket avPacket;
	av_init_packet(&avPacket);
	avPacket.data = (uint8_t*)pBuffer;
	avPacket.size = nLen;
	avPacket.stream_index = nStreamId;
	avPacket.flags = nFlag;

	if (nTimeStamp != -1)
	{
		avPacket.pts = nTimeStamp;
		avPacket.dts = nTimeStamp;
	}
	else
	{
		avPacket.pts = 1;
		avPacket.dts = 1;
	}
#ifdef FFMPEG_52
	avPacket.flags |= 	PKT_FLAG_KEY;
#elif defined(FFMPEG_80)
	//if (avPacket.pts <= 1)
		//avPacket.flags |= AV_PKT_FLAG_KEY;
#endif

	avPacket.duration = 1;
	avPacket.convergence_duration = avPacket.pts;
	if (avPacket.convergence_duration <= 0)
		avPacket.convergence_duration = 1;

	int nRt = 0;
	nRt = av_write_frame(m_pAvFormatContext, &avPacket);
	//nRt = av_interleaved_write_frame(m_pAvFormatContext, &avPacket);
	if (nRt != 0)
	{
		TRACE("write frame false: %d\n", (int)avPacket.pts);
		av_free_packet(&avPacket);
		return -2;
	}
	else
	{
		//TRACE("write frame ok: %d\n", (int)avPacket.pts);
	}
	int64_t dts = 0;
	int64_t absTime = 0;
	m_s64VideoTimeStamp = nTimeStamp;
	//av_get_output_timestamp(m_pAvFormatContext, 0, &dts, &absTime);
	//TRACE("write frame ok: %d, get time: %d, %d \n", (int)avPacket.pts, (int)dts, (int)absTime);
	av_free_packet(&avPacket);

	return 0;
}
int CFfmpegFormat::SeekToTimestamp(int64_t s64Timestamp, int nFlag/*=AVSEEK_FLAG_ANY*/, int nStreamId/*=0*/)
{
	if (m_pH264File)
	{
		m_pH264File->SeekToTimestamp(s64Timestamp, nFlag);
		return 0;
	}

	if (m_pAvFormatContext == NULL)
		return -1;

	m_nSeekFlag = nFlag;
	m_bSeek = true;
	int nRt =0;
	int64 nTime = 0;
	int64_t timestamp = 0;
	AVRational timeBaseQ /*= AV_TIME_BASE_Q*/;
	timeBaseQ.num = 1;
	timeBaseQ.den = AV_TIME_BASE;
//	timestamp = av_rescale_q(nTime*AV_TIME_BASE, timeBaseQ, m_pAvFormatContext->streams[0]->time_base);

	//float fMsPerFrame = (float )pStream->time_base.num*1000 / pStream->time_base.den;
	//
	//return m_s64TimeStamp*fMsPerFrame;
// 	AVRational time_base = m_pAvFormatContext->streams[0]->time_base;
// 	timestamp = nTime*time_base.den/(time_base.num*1000);

	timestamp = s64Timestamp;
	timestamp += m_pAvFormatContext->streams[0]->start_time;

	//debug("%s, timestamp: %I64d\n", __FUNCTION__, timestamp);
	//timestamp = av_rescale_q(nTimeStamp, AV_TIME_BASE_Q, m_pAvFormatContext->streams[0]->time_base);
	/*int nFlag = 0;*/
	if (nTime < m_s64VideoTimeStamp)
	{
		nFlag |= AVSEEK_FLAG_BACKWARD;
	}
	if (timestamp < m_pAvFormatContext->streams[0]->first_dts)
	{//
		timestamp = m_pAvFormatContext->streams[0]->first_dts;
	}

	nRt = av_seek_frame(m_pAvFormatContext, nStreamId, timestamp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);
	if (nRt < 0)
	{
		nFlag |= AVSEEK_FLAG_ANY;
		nRt = av_seek_frame(m_pAvFormatContext, nStreamId, timestamp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);
		m_nSeekFlag = nFlag;
	}
	if (nRt >= 0)
	{
		m_s64VideoTimeStamp = timestamp;
		m_bSeek = true;

		char *pBuffer  = new char[256*1024];
		int nLen = 256*1024;
		AVPacket avPacket;
		av_init_packet(&avPacket);
		avPacket.data = (uint8_t*)pBuffer;
		avPacket.size = nLen;
		avPacket.stream_index = nStreamId;
		avPacket.pts = AV_NOPTS_VALUE;
		avPacket.dts = AV_NOPTS_VALUE;
		/*m_pAvFormatContext->streams[0]->cur_dts = 02;*/
		//TRACE("to read frame, %d\n", m_pAvFormatContext);
		int nTries = 100;
		do
		{//找到对应的 stream 才行
			int nRt = av_read_frame(m_pAvFormatContext, &avPacket);

		}while(avPacket.stream_index != nStreamId
			&& nTries-- >= 0);
		bool bModifyTimestamp = false;
		// I帧索引和实际 package 异常的情况 需要修正 一次
		if ( !(nFlag&AVSEEK_FLAG_ANY) && avPacket.flags==0)
		{
			int64 s64TimestampTmp = timestamp-1;
			int nTries = 1000;
			while (nRt==0
				&& nTries--)
			{

				nRt = av_seek_frame(m_pAvFormatContext, nStreamId, s64TimestampTmp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);
				nRt = av_read_frame(m_pAvFormatContext, &avPacket);
				if (avPacket.pts != AV_NOPTS_VALUE)
					s64TimestampTmp = avPacket.pts;
				else
				{
					s64TimestampTmp = avPacket.dts;
				}
				s64TimestampTmp -= 1;
				if (avPacket.flags != 0 
					&& avPacket.stream_index == nStreamId
					)
				{
					debug("%s: I frame false, find new I frame,  \n", __FUNCTION__);
					bModifyTimestamp = true;
					break;
				}
			}
			//修正 需要定位的 时间戳
			timestamp = s64TimestampTmp;
			debug("%s, find I frame, tries: %d\n", __FUNCTION__, nTries);

		}
		//debug("%s, read pkt flag: %d\n", __FUNCTION__, avPacket.flags);
		delete pBuffer;
		if (nRt != 0)
			return -2;
		nLen = avPacket.size;
		debug("%s, seek: %I64d, result pts: %I64d, dts: %I64d, stream: %d\n", __FUNCTION__,\
			timestamp,\
			avPacket.pts, avPacket.dts, avPacket.stream_index);

		if (bModifyTimestamp)
		{
			nFlag = AVSEEK_FLAG_ANY;
		}
		else
		{

		}
		//重新seek ，修正后的 I帧位置
		nRt = av_seek_frame(m_pAvFormatContext, nStreamId, timestamp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);

		if (avPacket.pts != AV_NOPTS_VALUE)
			timestamp = avPacket.pts;
		else
		{
			timestamp = avPacket.dts;
		}
		//debug("%s, modify timestamp: %I64d\n", __FUNCTION__, timestamp);
		m_s64VideoTimeStamp = timestamp;
		/*nTime = TimestampToTime(timestamp);*/
	}

	//nRt = av_seek_frame(m_pAvFormatContext, 0, nTimeStamp, /*AVSEEK_FLAG_ANY|*/AVSEEK_FLAG_BACKWARD/*|AVSEEK_FLAG_BYTE*/);
	//nRt = avformat_seek_file(m_pAvFormatContext, 0, 0, nTimeStamp, 1000, AVSEEK_FLAG_ANY|AVSEEK_FLAG_BACKWARD);

	//seek_start=av_rescale_q(target_start, bqTimebase, timebase);
	//AVRational avTimeBase = m_pAvFormatContext->streams[0]->time_base;
	//nPts = av_rescale_q(nTimeStamp, avTimeBase, avTimeBase );
	//nRt = av_index_search_timestamp(m_pAvFormatContext->streams[0], nPts, AVSEEK_FLAG_ANY|AVSEEK_FLAG_BACKWARD);
	return 0;
}
int CFfmpegFormat::SeekTo(int64_t &nTime, int nFlag/*=AVSEEK_FLAG_ANY*/, int nStreamId)
{
	if (m_pH264File)
	{
		m_pH264File->Seek(nTime, nFlag);
		return 0;
	}
	if (m_pAvFormatContext == NULL)
		return -1;

	m_nSeekFlag = nFlag;
	m_bSeek = true;
	int nRt =0;
	int64_t timestamp = 0;
	AVRational timeBaseQ /*= AV_TIME_BASE_Q*/;
	timeBaseQ.num = 1;
	timeBaseQ.den = AV_TIME_BASE;
	timestamp = av_rescale_q(nTime*AV_TIME_BASE, timeBaseQ, m_pAvFormatContext->streams[0]->time_base);

	//float fMsPerFrame = (float )pStream->time_base.num*1000 / pStream->time_base.den;
	//
	//return m_s64TimeStamp*fMsPerFrame;
	AVRational time_base = m_pAvFormatContext->streams[0]->time_base;
	timestamp = nTime*time_base.den/(time_base.num*1000);
	timestamp += m_pAvFormatContext->streams[0]->start_time;

	debug("%s, timestamp: %I64d\n", __FUNCTION__, timestamp);
	//timestamp = av_rescale_q(nTimeStamp, AV_TIME_BASE_Q, m_pAvFormatContext->streams[0]->time_base);
	/*int nFlag = 0;*/
	if (nTime < m_s64VideoTimeStamp)
	{
		nFlag |= AVSEEK_FLAG_BACKWARD;
	}
	if (timestamp < m_pAvFormatContext->streams[0]->first_dts)
	{//
		timestamp = m_pAvFormatContext->streams[0]->first_dts;
	}

	nRt = av_seek_frame(m_pAvFormatContext, nStreamId, timestamp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);
	if (nRt < 0)
	{
		nFlag |= AVSEEK_FLAG_ANY;
		nRt = av_seek_frame(m_pAvFormatContext, nStreamId, timestamp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);
		m_nSeekFlag = nFlag;
	}
	if (nRt >= 0)
	{
		m_s64VideoTimeStamp = timestamp;
		m_bSeek = true;

		char *pBuffer  = new char[256*1024];
		int nLen = 256*1024;
		AVPacket avPacket;
		av_init_packet(&avPacket);
		avPacket.data = (uint8_t*)pBuffer;
		avPacket.size = nLen;
		avPacket.stream_index = nStreamId;
		avPacket.pts = AV_NOPTS_VALUE;
		avPacket.dts = AV_NOPTS_VALUE;
		/*m_pAvFormatContext->streams[0]->cur_dts = 02;*/
		//TRACE("to read frame, %d\n", m_pAvFormatContext);
		int nTries = 100;
		do
		{//找到对应的 stream 才行
			int nRt = av_read_frame(m_pAvFormatContext, &avPacket);
		}while(avPacket.stream_index != nStreamId
			&& nTries-- >= 0);

		bool bModifyTimestamp = false;
		// I帧索引和实际 package 异常的情况 需要修正 一次
		if ( !(nFlag&AVSEEK_FLAG_ANY) && avPacket.flags==0)
		{
			int64 s64TimestampTmp = timestamp-1;
			int nTries = 1000;
			while (/*avPacket.flags == 0 */
				nRt==0
				&& nTries--
				/*&& avPacket.stream_index != nStreamId*/)
			{
				
				nRt = av_seek_frame(m_pAvFormatContext, nStreamId, s64TimestampTmp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);
				nRt = av_read_frame(m_pAvFormatContext, &avPacket);
				if (avPacket.pts != AV_NOPTS_VALUE)
					s64TimestampTmp = avPacket.pts;
				else
				{
					s64TimestampTmp = avPacket.dts;
				}
				if (avPacket.flags != 0
					&& avPacket.stream_index == nStreamId)
				{
					debug("%s: I frame false, find new I frame,  \n", __FUNCTION__);
					bModifyTimestamp = true;
					break;
				}
				s64TimestampTmp -= 1;
			}
			//修正 需要定位的 时间戳
			timestamp = s64TimestampTmp;
			debug("%s, find I frame, tries: %d\n", __FUNCTION__, nTries);
			
		}
		debug("%s, read pkt flag: %d\n", __FUNCTION__, avPacket.flags);
		delete pBuffer;
		if (nRt != 0)
			return -2;
		nLen = avPacket.size;
		debug("%s, seek: %I64d, result pts: %I64d, dts: %I64d, stream: %d\n", __FUNCTION__,
			timestamp,
			avPacket.pts, avPacket.dts, avPacket.stream_index);

		
		//重新seek ，修正后的 I帧位置
		if (bModifyTimestamp)
		{
			nFlag = AVSEEK_FLAG_ANY;
		}
		else
		{

		}
		nRt = av_seek_frame(m_pAvFormatContext, nStreamId, timestamp, nFlag/*|AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_BYTE*/);

		if (avPacket.pts != AV_NOPTS_VALUE)
			timestamp = avPacket.pts;
		else
		{
			timestamp = avPacket.dts;
		}
		m_s64VideoTimeStamp = timestamp;
		nTime = TimestampToTime(timestamp-GetStartTimestamp());
	}
	
	//nRt = av_seek_frame(m_pAvFormatContext, 0, nTimeStamp, /*AVSEEK_FLAG_ANY|*/AVSEEK_FLAG_BACKWARD/*|AVSEEK_FLAG_BYTE*/);
	//nRt = avformat_seek_file(m_pAvFormatContext, 0, 0, nTimeStamp, 1000, AVSEEK_FLAG_ANY|AVSEEK_FLAG_BACKWARD);
	
	//seek_start=av_rescale_q(target_start, bqTimebase, timebase);
	//AVRational avTimeBase = m_pAvFormatContext->streams[0]->time_base;
	//nPts = av_rescale_q(nTimeStamp, avTimeBase, avTimeBase );
	//nRt = av_index_search_timestamp(m_pAvFormatContext->streams[0], nPts, AVSEEK_FLAG_ANY|AVSEEK_FLAG_BACKWARD);
	return 0;
}
int CFfmpegFormat::SeekTo(int nFrameIndex)
{

	return 0;
}
bool CFfmpegFormat::IsKeyFrame()
{
	return (m_u32FrameFlag&AV_PKT_FLAG_KEY);
}
void CFfmpegFormat::GetTimeBase(int &nNum, int &nDen)
{
	if (!m_pAvFormatContext)
		return ;
	if (m_pAvFormatContext->nb_streams <= 0)
		return ;

	AVStream *pStream = m_pAvFormatContext->streams[0];
	nNum = pStream->time_base.num;
	nDen = pStream->time_base.den;
	//
}
int64 CFfmpegFormat::TimeToTimestamp(int64 s64MilliSeconds)
{
	int64 s64Timestamp = 0;
	if (!m_pAvFormatContext)
		return 0;
	if (m_pAvFormatContext->nb_streams <= 0)
		return 0;
	AVStream *pStream = m_pAvFormatContext->streams[0];
	
	s64Timestamp  = s64MilliSeconds*pStream->time_base.den/(pStream->time_base.num*1000);
	return s64Timestamp/* + pStream->start_time*/;
}
int64 CFfmpegFormat::TimestampToTime(int64 s64Timestamp)
{
	int64 s64Time = 0;
	if (!m_pAvFormatContext)
		return 0;
	if (m_pAvFormatContext->nb_streams <= 0)
		return 0;
	AVStream *pStream = 0;// m_pAvFormatContext->streams[0];
	pStream = GetStreamByType(AVMEDIA_TYPE_VIDEO);
	if (!pStream)
		return 0;
	//不需要时间。 这个对外提供的都是相对其实时间
	//s64Timestamp += pStream->start_time;
	if (s64Timestamp < 0)
		s64Timestamp = 0;
	int nTimebaseNum = 1, nTimebaseDen = 25;
	if ( 1 || m_pAvFormatContext->duration > 0)
	{
		nTimebaseNum = pStream->time_base.num;
		nTimebaseDen = pStream->time_base.den;
	}
	else
	{
		nTimebaseDen = 1;
		nTimebaseDen = 25;
	}
// 	int nFpsNum(25), nFpsDen(1);
// 	GetFrameRate(nFpsNum, nFpsDen);

	s64Time  = s64Timestamp*nTimebaseNum*1000 / nTimebaseDen;

	return s64Time;
}
//时间
int64_t CFfmpegFormat::GetCurrentTime()
{
	if (m_pH264File)
	{
		return m_pH264File->GetCurrTime();
	}
	if (!m_pAvFormatContext)
		return 0;
	if (m_pAvFormatContext->nb_streams <= 0)
		return 0;
	//v
	AVStream *pStream = m_pAvFormatContext->streams[0];
	int hours, mins, secs, us;
	float fMsPerFrame = (float )pStream->time_base.num*1000 / pStream->time_base.den;
	//文件里面的相对时间
	int64 s64TimeInFile = 0;
	s64TimeInFile = m_s64VideoTimeStamp - pStream->start_time;
	if (s64TimeInFile < 0 )
	{
		s64TimeInFile = 0;
	}
	//
	return s64TimeInFile*fMsPerFrame;
}
int64_t CFfmpegFormat::GetTotalTime()
{
	if (m_pH264File)
	{
		int64 s64TotalTime = 0;
		s64TotalTime = m_pH264File->GetTotalTime();
		return s64TotalTime;
	}
	if (!m_pAvFormatContext)
		return 0;
	int64 s64TotalTime = 0;
	int hours, mins, secs, us;
	if (m_pAvFormatContext->duration == AV_NOPTS_VALUE)
	{
		if (strstr(m_pAvFormatContext->iformat->name, "h264") != NULL)
		{
			s64TotalTime = -1;
		}
		else
		{
			s64TotalTime = -1;
		}
	}
	else
	{
		secs = m_pAvFormatContext->duration / AV_TIME_BASE;
		us = m_pAvFormatContext->duration % AV_TIME_BASE;
		s64TotalTime = secs*1000+us/1000;
	}
	//debug("total time: %d second\n", secs);
	
	
	
// 	AVStream *pStream = m_pAvFormatContext->streams[0];
// 	
// 	int64 s64TotalTime1 = 0;
// 	s64TotalTime1 = pStream->duration*pStream->time_base.num*1000/pStream->time_base.den;
// 	
// 	s64TotalTime1 /= 1000;
	if (s64TotalTime <  0)
	{//不够一秒都视为无效
		s64TotalTime = -1;
	}
	return s64TotalTime;
}
int64_t CFfmpegFormat::GetStartTime()
{
	if (!m_pAvFormatContext)
		return 0;
	int hours, mins, secs, us;
	secs = m_pAvFormatContext->start_time / AV_TIME_BASE;
	us = m_pAvFormatContext->start_time % AV_TIME_BASE;
	//debug("total time: %d second\n", secs);
	return secs*1000+us/1000;
}
int CFfmpegFormat::SeekToNextKeyFrame()
{

	return 0;
}
int CFfmpegFormat::SeekToPrevKeyFrame(int64 nTimestamp/*=-1*/)
{
	int64 s64TimeStampSeek = 0;
	s64TimeStampSeek = m_s64VideoTimeStamp;

	//s64TimeStamp -= 1;
	int nLen = 500*1024;
	char *pFrameTmp = new char[nLen];
	int nRead = 0;
	int64 s64TimeStampTmp = 0;
	int nStreamId = 0;
 
	//定位 ， 回退 seek 每次 会跳转到一个 最近的 I帧 
	if (nTimestamp != -1)
	{
		s64TimeStampSeek = nTimestamp;
	}
	else
	{
		s64TimeStampSeek = m_s64VideoTimeStamp;
		s64TimeStampSeek -= m_pAvFormatContext->streams[0]->start_time;
	}

	SeekToTimestamp(s64TimeStampSeek - 10, AVSEEK_FLAG_BACKWARD, 0);
	
	//读取一次，以获取当前的 dts 数值
/*	while (1)
	{
		nRead = ReadFrame(pFrameTmp, nLen, s64TimeStampTmp, nStreamId);
		if (nRead <= 0)
		{
			break;
		}
		sFfmpegStreamInfo streamInfo;
		GetStreamInfo(streamInfo, nStreamId);
		if (streamInfo.m_nStreamType == AVMEDIA_TYPE_VIDEO)
		{
			break;
		}
		else
		{
			SeekToTimestamp(s64TimeStampTmp - 2, AVSEEK_FLAG_BACKWARD, 0);
		}
	}
	SeekToTimestamp(s64TimeStampTmp, AV_PKT_FLAG_KEY|AVSEEK_FLAG_BACKWARD);
    */
	delete pFrameTmp;
	//m_s64TimeStamp = s64TimeStampTmp-2;

	return 0;
}
int CFfmpegFormat::GetVideoWidth()
{
	if (m_pH264File)
	{
		;
	}
	if (!m_pAvFormatContext)
		return 0;
	if (m_pAvFormatContext->nb_streams <= 0)
		return 0;
	AVStream *pVideoStream = 0;
	for (int i=0; i<m_pAvFormatContext->nb_streams; i++)
	{
		AVStream *pStream = m_pAvFormatContext->streams[i];
		if (pStream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			pVideoStream = pStream;
			break;
		}
	}
	if (!pVideoStream)
		return 0;
	
	return pVideoStream->codec->width;
	return 0;
}
int CFfmpegFormat::GetVideoHeight()
{
	if (!m_pAvFormatContext)
		return 0;
	if (m_pAvFormatContext->nb_streams <= 0)
		return 0;
	AVStream *pVideoStream = 0;
	for (int i=0; i<m_pAvFormatContext->nb_streams; i++)
	{
		AVStream *pStream = m_pAvFormatContext->streams[i];
		if (pStream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			pVideoStream = pStream;
			break;
		}
	}
	if (!pVideoStream)
		return 0;

	return pVideoStream->codec->height;
}

int CFfmpegFormat::ParseMp4H264Info()
{
	return 0;
	if (!m_pAvFormatContext
		|| m_pAvFormatContext->nb_streams <= 0)
		return 0;

	int dummy_size;
	AVBitStreamFilterContext* bsfc =  av_bitstream_filter_init("h264_mp4toannexb");

	if(bsfc == NULL)
	{
		return -1;
	}


	//av_bitstream_filter_filter(
	//	bsfc, format_ctx_->streams[stream_id]->codec, NULL, &dummy, &dummy_size, NULL, 0, 0);
// 	av_bitstream_filter_filter(
// 		bsfc, m_pAvFormatContext->streams[0]->codec, NULL, (uint8_t**)&m_pH264PpsSps, &m_nH264InfoSize, NULL, 0, 0);

	av_bitstream_filter_close(bsfc);


	return 0;
}

int64 CFfmpegFormat::GetFilePos()
{
	if (!m_pAvFormatContext)
		return -1;

	return avio_tell(m_pAvFormatContext->pb);
}

int64_t CFfmpegFormat::GetStartTimestamp()
{
	if (!m_pAvFormatContext)
	{
		return 0 ;
	}
	AVStream *pStream;
	pStream = m_pAvFormatContext->streams[0];
	//if ()
	return pStream->start_time;
}

int64_t CFfmpegFormat::GetCurrentTimestamp()
{
	if (!m_pAvFormatContext)
	{
		return 0 ;
	}
	AVStream *pStream;
	pStream = m_pAvFormatContext->streams[0];
	if (!pStream)
	{
		return 0;
	}
	return m_s64VideoTimeStamp - pStream->start_time;
}

AVStream * CFfmpegFormat::GetStreamByType( int nType/*=AVMEDIA_TYPE_VIDEO*/ )
{
	if (!m_pAvFormatContext)
	{
		return NULL;
	}
	AVStream *pStream = 0;
	for (int i=0; i<m_pAvFormatContext->nb_streams; i++)
	{
		AVStream *pStreamTmp= m_pAvFormatContext->streams[i];
		if (pStreamTmp->codec->codec_type == nType)
		{
			pStream = pStreamTmp;
			break;
		}
	}


	return pStream;
}

int CFfmpegFormat::GetStreamType( int nIndex )
{

	if (!m_pAvFormatContext)
	{
		return NULL;
	}
	if (nIndex < 0|| nIndex> m_pAvFormatContext->nb_streams-1)
	{
		return NULL;
	}


	return m_pAvFormatContext->streams[nIndex]->codec->codec_type;
}	
bool	CFfmpegFormat::IsNeedFix()
{
	bool bNeedFix = false;
	if (m_pAvFormatContext->duration > 0)
		return false;
	if (m_pH264File)
	{
		bNeedFix = m_pH264File->IsNeedFix();
	}

	return bNeedFix;
}
int     CFfmpegFormat::DoFixFile(int64 &s64Total, int64 &s64Pos)
{

	if (!m_pH264File)
	{
		return -1;
	}

	int nRet = 0;
	nRet = m_pH264File->DoFixFile(s64Total, s64Pos);

	return nRet;
}