#include "StdAfx.h"
#include "VoiceIntercom.h"
#include "audioin.h"
#include "audioout.h"
#include "Buffer.h"
#include "FfmpegCodec.h"
extern "C"{
#include "libavcodec\avcodec.h"
};

CVoiceIntercom::CVoiceIntercom(void)
: m_pAudioIn(NULL)
, m_pAudioOut(NULL)
, m_pAudioEncode(NULL)
, m_pAudioDecode(NULL)
, m_cbAudioInData(NULL)
, m_pCbAudioInData(0)
, m_pCbAudioInReversed(0)
, m_pBufferEncodeTmp(NULL)
, m_pBufferDecodeTmp(NULL)
{
	
}

CVoiceIntercom::~CVoiceIntercom(void)
{
	if(m_pAudioIn)
	{
		m_pAudioIn->Close();
		delete m_pAudioIn;
		m_pAudioIn = NULL;
	}
	if (m_pAudioOut)
	{
		m_pAudioOut->Close();
		delete m_pAudioOut;
		m_pAudioOut = NULL;
	}
	if (m_pBufferDecodeTmp)
	{
		delete m_pBufferDecodeTmp;
		m_pBufferDecodeTmp = NULL;
	}
	if (m_pBufferEncodeTmp)
	{
		delete m_pBufferEncodeTmp;
		m_pBufferEncodeTmp = NULL;
	}
	if (m_pAudioEncode)
	{
		m_pAudioEncode->Close();
		delete m_pAudioEncode;
		m_pAudioEncode = NULL;
	}
	if (m_pAudioDecode)
	{
		m_pAudioDecode->Close();
		delete m_pAudioDecode;
		m_pAudioDecode = NULL;
	}
}
// audioin devices;
void CVoiceIntercom::OnAudioInData(char *pData, int nLength, void *pParam, void *pReversed)
{
	CVoiceIntercom *pVoiceIntercom = (CVoiceIntercom*)pParam;
	if (!pVoiceIntercom)
	{
		return ;
	}
	int nEncodeLen = 0;
	//压缩数据
	if (pVoiceIntercom->m_pAudioEncode)
	{
		pVoiceIntercom->m_pBufferEncodeTmp->Resize(nLength);
		pVoiceIntercom->m_pBufferEncodeTmp->SetUsed(nLength);
		nEncodeLen = nLength;
		pVoiceIntercom->m_pAudioEncode->Encode((uint8_t*)pData, nLength, pVoiceIntercom->m_pBufferEncodeTmp->GetData(), nEncodeLen);
		pVoiceIntercom->m_pBufferEncodeTmp->SetUsed(nEncodeLen);
	}
	//
	if (pVoiceIntercom->m_cbAudioInData)
	{//数据回调通知外部
		pVoiceIntercom->m_cbAudioInData((char*)pVoiceIntercom->m_pBufferEncodeTmp->GetData(), pVoiceIntercom->m_pBufferEncodeTmp->GetLength(), 
			pVoiceIntercom->m_pCbAudioInData, pVoiceIntercom->m_pCbAudioInReversed);
	}
}
int CVoiceIntercom::Open( int nCodec, int nChannels, int nSampleBitDepth, int nSampleRate, int nBitRate )
{
	if (m_pAudioIn)
		return -1;
	if (m_pAudioOut)
		return -1;
	if (!m_pBufferDecodeTmp)
	{
		m_pBufferDecodeTmp = new CBuffer(1024*1024, 10*1024*1024);
		m_pBufferDecodeTmp->Resize(MAX_AUDIO_FRAME_LEN);
	}
	if (!m_pBufferEncodeTmp)
	{
		m_pBufferEncodeTmp = new CBuffer(1024*1024, 10*1024*1024);
		m_pBufferEncodeTmp->Resize(MAX_AUDIO_FRAME_LEN);
	}
	int nRet = 0;

	m_pAudioIn = new CAudioIn(nChannels, nSampleBitDepth, nSampleRate);
	m_pAudioOut = new CAudioOut(nChannels, nSampleBitDepth, nSampleRate);

	m_pAudioIn->SetOnDataCB(OnAudioInData, this);
	nRet = m_pAudioIn->Open();
	if (nRet < 0)
	{
		debug("open audio in device false\n");
	}
	nRet = m_pAudioIn->Start();
	if (nRet < 0)
	{
		debug("start audio in device false\n");
	}
	nRet = m_pAudioOut->Open(nChannels, nSampleBitDepth, nSampleRate);
	if (nRet < 0)
	{
		debug("start audio out device false\n");
	}

	sFfmpegAudioInfo audioInfo;
	audioInfo.m_nSampleRate = nSampleRate;
	if (nSampleBitDepth == 8)
		audioInfo.m_nSampleFormat = 0;//SAMPLE_FMT_U8;
	else if (nSampleBitDepth == 16)
	{
		audioInfo.m_nSampleFormat = 1;//SAMPLE_FMT_S16;
	}
	audioInfo.m_nChannels = nChannels;
	audioInfo.m_nBitRate = nBitRate;


	m_pAudioDecode = new CFfmpegCodec;
	m_pAudioDecode->SetAudioParam(audioInfo);
	nRet = m_pAudioDecode->Open(/*nCodec,*/ true);
	if (nRet < 0)
	{
		debug("open decode false\n");
	}

	m_pAudioEncode = new CFfmpegCodec;
	m_pAudioEncode->SetAudioParam(audioInfo);
	nRet = m_pAudioEncode->Open(/*nCodec,*/ false);
	if (nRet < 0)
	{
		debug("open encode false\n");
	}

	m_nRunState = eRS_Run;

	return 0;
}

int CVoiceIntercom::Start()
{

	return 0;
}

void CVoiceIntercom::Close()
{
	m_nRunState = eRS_Stop;
	
	if(m_pAudioIn)
	{
		m_pAudioIn->Close();
		delete m_pAudioIn;
		m_pAudioIn = NULL;
	}
	if (m_pAudioOut)
	{
		m_pAudioOut->Close();
		delete m_pAudioOut;
		m_pAudioOut = NULL;
	}
	/*return 0;*/
}

int CVoiceIntercom::InputData( char *pData, int nLen )
{
	if (m_nRunState != eRS_Run)
		return -1;
	if (!m_pAudioOut)
	{
		return -1;
	}
	//解码
	if (!m_pAudioDecode)
	{
		return -2;
	}
	//char szTmp[]
	//m_pBufferDecodeTmp->Resize(MAX_AUDIO_FRAME_LEN);
	int nDecodeLen = nLen*2;
	m_pBufferDecodeTmp->Resize(nLen*2);
	m_pAudioDecode->Decode((uint8_t*)pData, nLen, m_pBufferDecodeTmp->GetData(), nDecodeLen);
	//播放
	m_pAudioOut->FillData((char*)m_pBufferDecodeTmp->GetData(), nDecodeLen);

	return 0;
}

int CVoiceIntercom::SetAudioInDataCb( Cb_OnAudioIn cbAudioIn, void *pUserData, void *pReversed )
{
	m_cbAudioInData = cbAudioIn;
	m_pCbAudioInData = pUserData;
	m_pCbAudioInReversed = pReversed;
	
	return 0;
}
