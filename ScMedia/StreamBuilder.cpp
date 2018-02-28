#include "StdAfx.h"
#include "StreamBuilder.h"
#include "MediaStream.h"
#include "PackageParser.h"
#include "H264Decoder.h"
#include "ParserH264.h"
#include "ParserPsH264.h"
#include "ParserVivsH264.h"


#ifdef QT_OPENGL
#include "QtGlRender.h"
#else

#endif

#ifdef OS_WIN
#include "DahuaParser.h"
#include "DahuaRender.h"
#include "HkDecoder.h"
#include "HkParser.h"
#include "AudioRender.h"
#include "AudioDecoder.h"
#include "StdDecoder.h"
#include "SmartRender.h"
#endif

//static CStreamBuilder g_streamBuilderDefault;
CStreamBuilder::CStreamBuilder(void)
{
}

CStreamBuilder::~CStreamBuilder(void)
{
}
CStreamBuilder *CStreamBuilder::GetDefaultStreamBuilder()
{
	return NULL;
}
int  CStreamBuilder::BuildStream(char *pInfo, CMediaStream *pStream, HWND hWnd)
{
	if (!pStream)
		return -1;
	CPackageParser *pParser = 0;
	CDecoder *pDecoder = 0;
	CRender *pRender = 0;
	//CRender *pAudioRender=0;
	CDecoder *pAudioDecoder = 0;
	if (strcmp(pInfo, VIVS_CODEC_H264) == 0 )
	{// vivs h264
		/*CPackageParser **/pParser = new CParserVivsH264();
		pParser->Init();
		pDecoder = new CH264Decoder();
		pDecoder->Init();
#ifdef QT_OPENGL
        pRender = new CQtGlRender();
#else
		pRender = new CSmartRender();
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
#endif
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);

#ifdef OS_WIN
		CStdDecoder *pStdAudioDecoder = new CStdDecoder();
		sFfmpegStreamInfo audioInfo;
		audioInfo.m_nCodecId = CODEC_ID_PCM_ALAW;
		audioInfo.m_audioInfo.m_nChannels = 1;
		audioInfo.m_audioInfo.m_nSampleFormat = SAMPLE_FMT_S16;//16;
		audioInfo.m_audioInfo.m_nSampleRate = 8000;
		audioInfo.m_audioInfo.m_nBitRate = 10;

		pStdAudioDecoder->Init(audioInfo);
		pStream->AttachAudioDecoder(pStdAudioDecoder);
		CAudioRender *pAudioRender = new CAudioRender();
		pAudioRender->Init(1, 16, 8000);
		pStream->AttachAudioRender(pAudioRender);
#endif
		
	}
	else if (strcmp(pInfo, VIVS_CODEC_H264_STD) == 0 )
	{// ffmpeg h264
		/*CParserH264 **/
		pParser = new CParserH264();
		pParser->Init();
		pDecoder = new CH264Decoder();
		pDecoder->Init();
#ifdef QT_OPENGL
        pRender = new CQtGlRender();
#else
        pRender = new CSmartRender();
#endif
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);/**/
	}
	else if (strcmp(pInfo, VIVS_CODEC_H264_PS) == 0 )
	{// ffmpeg h264
		/*CParserPsH264**/ pParser = new CParserPsH264();
		pParser->Init();
		pDecoder = new CH264Decoder();
		pDecoder->Init();
#ifdef QT_OPENGL
        pRender = new CQtGlRender();
#else
        pRender = new CSmartRender();
#endif
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
#ifdef OS_WIN
	else if ( strcmp(pInfo, VIVS_CODEC_DH) == 0)
	{// dh
		/*CParserPsH264**/ pParser = (CPackageParser*)new CDahuaParser();
		pParser->Init();
		pDecoder = new CDecoder();
		pDecoder->Init();
		pRender = new CDahuaRender();
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
	else if ( strcmp(pInfo, VIVS_CODEC_HK) == 0)
	{// dh
		CHkParser *pParser = new CHkParser();
		pParser->SetParseAvdataEnd(true);
		pParser->Init();
		CHkDecoder *pDecoder = new CHkDecoder();
		pDecoder->Init();
		//pDecoder->SetWnd(hWnd);
		pRender = new CSmartRender();
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
#endif
	else
	{
		
	}

	return 0;
}

int CStreamBuilder::BuildStream( sVivsStreamInfo *pStreamInfo, CMediaStream *pStream, HWND hWnd )
{
	CPackageParser *pParser = 0;
	CDecoder *pDecoder = 0;
	CRender *pRender = 0;
	//CRender *pAudioRender=0;
	CDecoder *pAudioDecoder = 0;
	if (pStreamInfo->m_nStreamType == eVST_VivsH264/*strcmp(pInfo, VIVS_CODEC_H264) == 0 */)
	{// vivs h264
		/*CPackageParser **/pParser = new CParserVivsH264();
		pParser->Init(50);
		pDecoder = new CH264Decoder();
		pDecoder->Init();
		
#ifdef QT_OPENGL
		pRender = new CQtGlRender();
#else
		pRender = new CSmartRender();
#endif
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);

#ifdef OS_WIN
		CStdDecoder *pStdAudioDecoder = new CStdDecoder();
		sFfmpegStreamInfo audioInfo;
		audioInfo.m_nCodecId = CODEC_ID_PCM_ALAW;
		audioInfo.m_audioInfo.m_nChannels = 1;
		audioInfo.m_audioInfo.m_nSampleFormat = SAMPLE_FMT_S16;//16;
		audioInfo.m_audioInfo.m_nSampleRate = 8000;
		audioInfo.m_audioInfo.m_nBitRate = 10;

		pStdAudioDecoder->Init(audioInfo);
		pStream->AttachAudioDecoder(pStdAudioDecoder);
		CAudioRender *pAudioRender = new CAudioRender();
		pAudioRender->Init(1, 16, 8000);
		pStream->AttachAudioRender(pAudioRender);
#endif

	}
	else if (pStreamInfo->m_nStreamType == eVST_StdH264/*strcmp(pInfo, CODEC_H264_STD) == 0 */)
	{// ffmpeg h264
		/*CParserH264 **/pParser = new CParserH264();
		pParser->Init();
		pDecoder = new CH264Decoder();
		pDecoder->Init();
#ifdef QT_OPENGL
        pRender = new CQtGlRender();
#else
        pRender = new CSmartRender();
#endif
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
	else if (pStreamInfo->m_nStreamType == eVST_VivsPSH264/*strcmp(pInfo, H264_PS) == 0*/ )
	{// ffmpeg h264
		/*CParserPsH264**/ pParser = new CParserPsH264();
		pParser->Init();
		pDecoder = new CH264Decoder();
		pDecoder->Init();
#ifdef QT_OPENGL
        pRender = new CQtGlRender();
#else
        pRender = new CSmartRender();
#endif
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
#ifdef OS_WIN
	else if ( pStreamInfo->m_nStreamType == eVST_DH /*strcmp(pInfo, VIVS_CODEC_DH) == 0*/)
	{// dh
		/*CParserPsH264**/ pParser = (CPackageParser*)new CDahuaParser();
		pParser->Init();
		pDecoder = new CDecoder();
		pDecoder->Init();
		pRender = new CDahuaRender();
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
	else if ( pStreamInfo->m_nStreamType == eVST_HK/*strcmp(pInfo, VIVS_CODEC_HK) == 0*/)
	{// dh
		CHkParser *pParser = new CHkParser();
		pParser->SetParseAvdataEnd(true);
		pParser->Init();
		CHkDecoder *pDecoder = new CHkDecoder();
		pDecoder->Init();
		//pDecoder->SetWnd(hWnd);
		pRender = new CSmartRender();
		pRender->Init(hWnd, pStream->m_nPlayBuffer);
		pStream->AttachParser(pParser);
		pStream->AttachDecoder(pDecoder);
		pStream->AttachRender(pRender);
	}
#endif
	else
	{
		return -2;
	}

	return 0;
}
int  CStreamBuilder::AllocParser(sVivsStreamInfo *pStreamInfo, CPackageParser **pParser)
{
	CPackageParser *pNewParser = NULL;
	if (pStreamInfo->m_nStreamType == eVST_VivsH264)
	{
		pNewParser = new CParserVivsH264();
		pNewParser->Init();
	}
	else if (pStreamInfo->m_nStreamType == eVST_StdH264)
	{

	}
	else if (pStreamInfo->m_nStreamType == eVST_HK)
	{

	}
	else if (pStreamInfo->m_nStreamType == eVST_DH)
	{

	}
	else
	{

	}
	*pParser = pNewParser;
	return 0;
}
int  CStreamBuilder::AllocDecoder(sVivsStreamInfo *pStreamInfo, CDecoder **pDecoder)
{
	CDecoder *pNewDecoder = NULL;
	if (pStreamInfo->m_nStreamType == eVST_VivsH264)
	{
		pNewDecoder = new CH264Decoder();
		pNewDecoder->Init();
	}
	else if (pStreamInfo->m_nStreamType == eVST_StdH264)
	{
		pNewDecoder = new CH264Decoder();
		pNewDecoder->Init();
	}
	else if (pStreamInfo->m_nStreamType == eVST_HK)
	{

	}
	else if (pStreamInfo->m_nStreamType == eVST_DH)
	{

	}
	else
	{

	}
	*pDecoder = pNewDecoder;
	return 0;
}
int  CStreamBuilder::AllocRender(sVivsStreamInfo *pStreamInfo, CRender **pRender, HWND hWnd, int nPlayBuffer)
{
	CRender *pNewRender = NULL;
	if (pStreamInfo->m_nStreamType == eVST_VivsH264)
	{
#ifdef QT_OPENGL
        pNewRender = new CQtGlRender();
#else
        pNewRender = new CSmartRender();
#endif
		pNewRender->Init(hWnd, nPlayBuffer);
	}
	else if (pStreamInfo->m_nStreamType == eVST_StdH264)
	{
#ifdef QT_OPENGL
        pNewRender = new CQtGlRender();
#else
        pNewRender = new CSmartRender();
#endif
		pNewRender->Init(hWnd, nPlayBuffer);
	}
	else if (pStreamInfo->m_nStreamType == eVST_HK)
	{

	}
	else if (pStreamInfo->m_nStreamType == eVST_DH)
	{

	}
	else
	{

	}
	*pRender = pNewRender;

	return 0;
}
int  CStreamBuilder::FreeParser(CPackageParser *pParser)
{
	delete pParser;
	return 0;
}
int  CStreamBuilder::FreeDecoder(CDecoder *pDecoder)
{
	delete pDecoder;
	return 0;
}
int  FreeRender(CRender *pRender)
{
	delete pRender;
	return 0;
}
