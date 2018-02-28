#include "StdAfx.h"

#include "ParserVivsH264.h"
#include "PackageParser.h"
#include "Buffer.h"
#include <string.h>

CParserVivsH264 g_parserVivsH264;
CParserVivsH264::CParserVivsH264(void)
: m_bParseAvdataEnd(true)
, m_nFTMV(FTMV_FLAG)
, m_bFrameEnd(false)
{
	strcpy(m_szName, "parser vivs-h264");
	strcpy(m_szType, "parser vivs-h264");
	CFilterManager::RegistFilter(&g_parserVivsH264);

	m_pBufferStreamTmp = new CFilterPinBuffer(512*1024, 2*1024*1024);
	m_pBufferAudioStream = new CFilterPinBuffer();
	m_pBufferVideoStream = new CFilterPinBuffer(512*1024, 2*1024*1024);
	memset(&m_avDataEnd, 0, sizeof(sAvDataEnd));
}

CParserVivsH264::~CParserVivsH264(void)
{
	if (m_pBufferStreamTmp)
	{
		delete m_pBufferStreamTmp;
		m_pBufferStreamTmp = NULL;
	}
	if (m_pBufferAudioStream)
	{
		delete m_pBufferAudioStream;
		m_pBufferAudioStream = NULL;
	}
	if (m_pBufferVideoStream)
	{
		delete m_pBufferVideoStream;
		m_pBufferVideoStream = NULL;
	}
	if (m_pBufferInputTmp)
	{
		delete m_pBufferInputTmp;

		m_pBufferInputTmp = NULL;
	}
}
CDataFilter *CParserVivsH264::CreateObject()
{
	return new CParserVivsH264();
}
int CParserVivsH264::Init(int nBufferCount)
{
	CDataFilter::Init();

	m_pBufferInputTmp = new CFilterPinBuffer();

	CFilterPin *pInOut = m_lstPinOut.at(0);
	if (pInOut)
	{
		pInOut->Clear();
		pInOut->CreateBuffer<CFilterPinBuffer>(nBufferCount);
	}
	return 0;
}
/*
//
*FTMV ... FTMV avdata end FTMV ... FTMV
*正常数据 是1024 字节 一个 FTMV 
*需要 按照 264 的 方式分包  
* 解析到一个 avdataend 或者 avdatastream 会 结束 
* 如果是 录像 或者下载文件，在FTMV结束前有一个 zxel数据包
*/
int CParserVivsH264::InputData(char *pData, int nLen)
{
	// 取一个 outpin的 free buffer
	
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;
	int nCurPos = 0;
	CFilterPinBuffer *pBuffer = NULL;
	int nPassedPos = 0;

	int nAvDataEndType = eADET_NotEnd;
	//解析 FTMV avdataend 
	while (m_state == eFS_Run)
	{
		int nOutLen = 0;
		int nRtPassed = ParsePackage(pData+nPassedPos, nLen-nPassedPos, m_pBufferStreamTmp, nOutLen, nAvDataEndType);
		if (nAvDataEndType != eADET_NotEnd)
		{
			nPassedPos += nRtPassed;
			break;
		}
		if ( (nRtPassed > 0 && nRtPassed <=nLen)
			)
		{
			nPassedPos += nRtPassed;
		}
		else
		{
			break;
		}
	}
	
	if (nAvDataEndType == eADET_NotEnd)
	{
	}
	else if (nAvDataEndType == eADET_Video)
	{//视频数据 结束//解析nal
		CFilterPinBuffer *pBufferNal = NULL;
		int nAppend = m_pBufferVideoStream->Append(m_pBufferStreamTmp->GetData(), m_pBufferStreamTmp->GetLength());
		if (nAppend < m_pBufferStreamTmp->GetLength())
		{
			debug("%s: error apend\n", __FUNCTION__);
		}
/*#define debug_video*/
#ifdef debug_video
		static FILE *fileVideo = NULL;
		if (!fileVideo)
		{

			fileVideo = fopen("F:\\tmp\\h264-video.vd", "w+b");

		}

		if (fileVideo)
		{
			fwrite(m_pBufferStreamTmp->GetData(), m_pBufferStreamTmp->GetLength(), 1, fileVideo);
			fflush(fileVideo);
		}
#endif
		m_pBufferStreamTmp->Clear();
		do{
			pBufferNal = NULL;
			pPinOut->GetFreeBuffer(&pBufferNal);
			if (!pBufferNal)
				break;
			pBufferNal->Clear();
			ParseH264Nal(m_pBufferVideoStream, pBufferNal);
			if (pBufferNal->GetLength()>0)
			{
				pBufferNal->m_nDataType = eFT_Video;
				

#ifdef debug_pased_nal
				static FILE *pfileParsedNal = 0;
				if (pfileParsedNal == NULL)
				{
					pfileParsedNal = fopen("F:\\tmp\\h264-parsed_nal.vd", "w+b");
				}
				if (pfileParsedNal)
				{
					fwrite(pBufferNal->GetData(), pBufferNal->GetLength(), 1, pfileParsedNal);
					//debug("write %d \n", pBufferNal->GetLength());
					fflush(pfileParsedNal);
				}
#endif
				pPinOut->PutPreparedBuffer(pBufferNal);
			}
			else
			{
				pPinOut->PutFreeBuffer(pBufferNal);
			}

		}while ( (m_state == eFS_Run) && pBufferNal->GetLength()>0);

	}
	else if (0 && nAvDataEndType == eADET_Audio)
	{//音频数据结束
		//debug("clear audio data: %d\n", m_pBufferStreamTmp->GetLength());
		CFilterPinBuffer *pBufferAudio = 0;
		pPinOut->GetFreeBuffer(&pBufferAudio);
		if (pBufferAudio)
		{
			pBufferAudio->Clear();
			pBufferAudio->m_nDataType = eFT_Audio;
			pBufferAudio->Append(m_pBufferStreamTmp->GetData(), m_pBufferStreamTmp->GetLength());
			pPinOut->PutPreparedBuffer(pBufferAudio);
		}
#ifdef debug_audio
		static FILE *fileAudio = NULL;
		if (!fileAudio)
		{

			fileAudio = fopen("F:\\tmp\\audio.vd", "w+b");

		}

		if (fileAudio)
		{
			fwrite(m_pBufferStreamTmp->GetData(), m_pBufferStreamTmp->GetLength(), 1, fileAudio);
			fflush(fileAudio);
		}
#endif
		m_pBufferStreamTmp->Clear();
	}
	else{
		
	}
	//
	return nPassedPos;
}
/*流格式：数据被"FTMV"分段为数据小块, 直到遇见sAvDataEnd标记，表示此前数据是可以解码的数据。sAvDataEnd也是由FTMV标记分割。
* sAvDataEnd 表示视频数据包完整结尾,在此之前的数据可以进行解码。
* DEF_ST_AV_STREAM_HEADER用于区分之前的这段数据，是音频还是视频数据，该avdatastream是可选的标记。该标记也是有FTMV标记分割
* FTMV...视频数据/音频数据.......[FTMV DEF_ST_AV_STREAM_HEADER ] FTMV _sAvDataEnd FTMV.....
* 如果是录像 或者下载数据，在某些 ftmv前面有 "zxel" 时间戳
* 返回当前pos
*/
int CParserVivsH264::ParsePackage(char *pData, int nLen, CFilterPinBuffer *pBufferOut, int &nOutLen, int &nAvDataEndType)
{
	int nMaxPos = nLen-FTMV_SIZE;
	int nPos0 = -1, nPos1=-1;
	//
	nOutLen = 0;
	int nPassedPos = 0;
	// 分拆 FTMV 和 avdata end 
	for (int i=0; i<nMaxPos; i++)
	{
//		int nTmp = *((int*)(pData+i) );
		if (m_nFTMV ==  *((int*)(pData+i)) )
		{// FTMV flag
			if (nPos0==-1)
			{
				nPos0 = i+FTMV_SIZE;
				nPassedPos = i;
				continue;
			}
			nPos1 = i;
			//看 是 avdata 或者 avstream
			if (m_bParseAvdataEnd) 
			{//移除 avdata end; [stream header]; 
				int nPktLen = nPos1-nPos0;
				if ((nPktLen == sizeof(sAvStreamInfo) 
					|| nPktLen == sizeof(sAvStreamInfo)+sizeof(sVivsTimeStamp) )
					&& 0 == memcmp(pData+nPos0, GW_AV_STREAM_END_FLAG, GW_MAX_AV_STREAM_END_FLAG_LEN) )
				{// stream header 

					sAvStreamInfo *pStreamHeader = (sAvStreamInfo *)(pData + nPos0);
					if (GW_AV_DATA_TYPE_AUDIO == pStreamHeader->ucMediaType)
					{
						nAvDataEndType = eADET_Audio;
					}
					else
					{
						nAvDataEndType = eADET_Video;
					}
					nPassedPos = i;
					//
					break;
				}
				else if (nPktLen == sizeof(_sAvDataEnd)
					|| nPktLen== sizeof(_sAvDataEnd)+sizeof(sVivsTimeStamp) )
				{//
					char *pAvdataEnd = pData + nPos0;
					if (memcmp(pAvdataEnd, GW_AV_END_FLAG, GW_AV_END_FLAG_LENGTH) == 0)
					{//找到 avdataend，扔掉。
						nOutLen = 0;
						nPos0 = i+FTMV_SIZE;
						//nPos1 = -1;
						//保存 avdata end，其中有播放的时间osd信息
						memcpy(&m_avDataEnd, pAvdataEnd, sizeof(sAvDataEnd));
						nAvDataEndType = eADET_Video;
						//
						nPassedPos = i;

						break;
					}
				}
				else
				{//不是 avdata end，
					// 不是 avdata end avdatastreamend ， 查看是否有 zxel 标记 
					if ( nPos1-nPos0>sizeof(sVivsTimeStamp)
						&& VIVS_TIME_FLAG == *((unsigned int*)(pData+i- sizeof(sVivsTimeStamp))) 
						)
					{//查看 是 
						nOutLen = nPos1-nPos0-sizeof(sVivsTimeStamp);
						pBufferOut->Append(pData+nPos0, nOutLen);
						nPos0 = nPos1+FTMV_SIZE;
					}
					else
					{//
						nOutLen = nPos1-nPos0;
						nPassedPos = i;
						pBufferOut->Append(pData+nPos0, nOutLen);
/*#define debug_h264*/
#ifdef debug_h264
						static FILE *pfileH264NoNal = 0;
						if (pfileH264NoNal == NULL)
						{
							pfileH264NoNal = fopen("F:\\tmp\\vivs1_h264-no nal.vd", "w+b");
						}
						if (pfileH264NoNal)
						{
							fwrite(pData+nPos0, nOutLen, 1, pfileH264NoNal);
							fflush(pfileH264NoNal);
						}
#endif


						nPos0 = nPos1+FTMV_SIZE;
					}
				}
			}
			else{
				// 不是 avdata end avdatastreamend ， 查看是否有 zxel 标记 
				if ( nPos1-nPos0>sizeof(sVivsTimeStamp)
					&& VIVS_TIME_FLAG == *((unsigned int*)(pData+i- sizeof(sVivsTimeStamp))) 
					)
				{//查看 是 
					nOutLen = nPos1-nPos0-sizeof(sVivsTimeStamp);
					pBufferOut->Append(pData+nPos0, nOutLen);
					nPos0 = nPos1+FTMV_SIZE;
				}
				else
				{//
					nOutLen = nPos1-nPos0;

					pBufferOut->Append(pData+nPos0, nOutLen);
					nPos0 = nPos1+FTMV_SIZE;
				}
			}
		}
	}


	return nPassedPos;
}
//解析有问题。
int CParserVivsH264::ParseH264Nal(CFilterPinBuffer *pH264Stream, CFilterPinBuffer *pBufferNal)
{
	int nOutLen = 0;
	int nPos0 = 0;
	//
	int nMaxPos = pH264Stream->GetLength()-4;
	char *pH264Tmp = pH264Stream->GetData();
	int nNalPos0 = -1;
	int nNalPos1 = -1;
	for (int i=0; i<nMaxPos; i++)
	{// 分拆  nal 单元

		if( (pH264Tmp[i+0] == 0)
			&& (pH264Tmp[i+1] == 0)
			&& (pH264Tmp[i+2] == 0)
			&& (pH264Tmp[i+3] == 1)
			)
		{
			if (nNalPos0 == -1)
			{
				nNalPos0 = i;
				i += 3;
			}
			else
			{
				if ( (pH264Tmp[i+4] &0x1f) == 1 )
				{
					nNalPos1 = i;
					break;
				}
			}
		}
		else if( (pH264Tmp[i+0] == 0)
			&& (pH264Tmp[i+1] == 0)
			&& (pH264Tmp[i+2] == 1)
			)
		{
			if (nNalPos0 == -1)
			{
				nNalPos0 = i;
				i += 2;
			}
			else
			{
				if ( (pH264Tmp[i+3] &0x1f) == 1 )
				{
					nNalPos1 = i;
					break;
				}
			}
		}
	}

	if (nNalPos1 > 0)
	{//找到一帧 Nal 数据
		if (pBufferNal->GetMaxSize() < nNalPos1-nNalPos0)
		{
			pBufferNal->SetMaxSize(nNalPos1-nNalPos0);
		}
		int nAppend = pBufferNal->Append(pH264Tmp+nNalPos0, nNalPos1-nNalPos0);
		if (nAppend <  nNalPos1-nNalPos0)
		{
			debug("%s: error append\n", __FUNCTION__);
		}
		CBuffer *pExtBuffer = pBufferNal->GetExtData();
		if (pBufferNal)
		{//用 ext buffer 保存 avdata end 数据主要是时间数据
			pExtBuffer->Resize(sizeof(sAvDataEnd));
			pExtBuffer->Clear();
			pExtBuffer->Append((unsigned char *)&m_avDataEnd, sizeof(sAvDataEnd));
		}
		pH264Stream->Clear(0, nNalPos1);
	}
	else if (nNalPos0 > 0)
	{
		pH264Stream->Clear(0, nNalPos0);
	}

	return 0;
}
int CParserVivsH264:: DoFilter()
{
	//取 输入pin的 数据
	CFilterPinBuffer *pInPinBuffer = 0;
	CFilterPin *pInPin = 0;
	pInPin = m_lstPinIn.at(0);
	if (pInPin)
	{
		pInPin->GetPreparedBuffer(&pInPinBuffer);
		if (!pInPinBuffer)
		{
			debug("%s: no prepared buffer\n", __FUNCTION__);
		}
		while (pInPinBuffer)
		{
			int nAppendLen = m_pBufferInputTmp->Append(pInPinBuffer);
			if (nAppendLen<=0)
			{//error
				debug("%s: append error\n", __FUNCTION__);
			}
			//应该放入到 之前的准备好的 队列。。。
			//目前 是直接丢弃
			pInPin->PutFreeBuffer(pInPinBuffer);
			pInPinBuffer = NULL;
			pInPin->GetPreparedBuffer(&pInPinBuffer);
		}

	}
	//解析

	int nParsedLen = 0;
	//nParsedLen = InputData(m_pBufferInputTmp->GetData(), m_pBufferInputTmp->GetLength());
	while (m_state != eFS_Stop 
		&& m_pBufferInputTmp->GetLength()>0)
	{//网络解包
		int nPassedPos = 0;
		nPassedPos = InputData(m_pBufferInputTmp->GetData(), m_pBufferInputTmp->GetLength());
		if (nPassedPos<=0)
			break;

		m_pBufferInputTmp->Clear(0, nPassedPos);
		//debug("%s, parse passed %d, bytes, remain %d \n", __FUNCTION__, nPassedPos, pStream->m_pInputDataBuffer->GetLength());		
	}
	CFilterPin *pOutPin = m_lstPinOut.at(0);
	//debug("%s: out: busy %d, free %d\n", __FUNCTION__, pOutPin->GetPreparedBufferConut(), pOutPin->GetFreeBufferCount());

	//放入到输出pin

	return CPackageParser::DoFilter();
}