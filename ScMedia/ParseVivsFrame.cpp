
/*#ifdef WIN32*/
#include "StdAfx.h"
/*#endif*/

#include "ParseVivsFrame.h"
#include "Buffer.h"
#include <string.h>


#define FTMV_FLAG ('F'<<0|'T'<<8|'M'<<16|'V'<<24)

CStreamFrame::CStreamFrame()
:m_pBufferFrame(NULL)
, m_eFrameType(eVFT_VideoIFrame)
, m_nYear(0)
, m_nMonth(0)
, m_nDay(0)
, m_nHour(0)
, m_nMinute(0)
, m_nSecond(0)
, m_nMiniSecond(0)
{
	m_pBufferFrame = new CBuffer(512*1024, 10*1024*1024);
}
CStreamFrame::~CStreamFrame()
{
	if (m_pBufferFrame)
	{
		delete m_pBufferFrame;
		m_pBufferFrame = NULL;
	}
}
CParseVivsFrame::CParseVivsFrame(void)
:m_pBufferStreamTmp(NULL)
, m_pBufferVideoStream(NULL)
, m_pBufferAudioStream(NULL)
, m_eState(ePVFS_Stop)
, m_nFTMV(FTMV_FLAG)
, m_bParseAvdataEnd(true)
, m_pStreamFrameTmp(NULL)
{
	
}

CParseVivsFrame::~CParseVivsFrame(void)
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
	if (m_pStreamFrameTmp)
	{
		delete m_pStreamFrameTmp;
		m_pStreamFrameTmp = NULL;
	}

}


long CParseVivsFrame::Init(int nBufferSize)
{
	if (m_eState == ePVFS_Run)
	{
		return -1;
	}
	m_pBufferStream = new CBuffer(nBufferSize, nBufferSize);
	m_pBufferStreamTmp = new CBuffer(nBufferSize, nBufferSize);
	m_pBufferVideoStream = new CBuffer(nBufferSize, nBufferSize);
	m_pBufferAudioStream = new CBuffer(nBufferSize, nBufferSize);

	m_pStreamFrameTmp = new CStreamFrame();

	m_eState = ePVFS_Run;
	return 0;
}
long CParseVivsFrame::Close()
{
	m_eState = ePVFS_Stop;
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
	if (m_pBufferStream)
	{
		delete m_pBufferStream ;
		m_pBufferStream = NULL;
	}
	list<CStreamFrame*>::iterator item = m_lstFrame.begin();
	for (item = m_lstFrame.begin(); item!=m_lstFrame.end(); item++)
	{
		CStreamFrame *pFrame = *item;
		if (pFrame)
		{
			delete pFrame;
		}
	}
	m_lstFrame.clear();
	return 0;
}
// ret <0 if false
long CParseVivsFrame::Parse2Std(char *pDataIn, int nLenIn)
{
	if (!m_pBufferStreamTmp)
		return -1;

	int nPassedPos = 0;
	
	m_pBufferStream->Append((unsigned char*)pDataIn, nLenIn);
	 char *pData = (char*)m_pBufferStream->GetData();
	int nLen = m_pBufferStream->GetLength();
	int nAvDataEndType = eADET_NotEnd;
	//解析 FTMV avdataend 
	while (m_eState == ePVFS_Run)
	{
		int nOutLen = 0;
		int nRtPassed = ParsePackage(pData+nPassedPos, nLen-nPassedPos, m_pBufferStream, nOutLen, nAvDataEndType);
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
	m_pBufferStream->Clear(0, nPassedPos);

	if (nAvDataEndType == eADET_NotEnd)
	{

	}
	else if (nAvDataEndType == eADET_Video)
	{//视频数据 结束//解析nal
	
		m_pBufferVideoStream->Append(m_pBufferStreamTmp->GetData(), m_pBufferStreamTmp->GetLength());
		m_pBufferStreamTmp->Clear();
		CStreamFrame *pFrame = NULL;
		bool bHaveNal = true;
		do{
			pFrame = new CStreamFrame();
			pFrame->m_pBufferFrame->Clear();

			ParseH264Nal(m_pBufferVideoStream, pFrame->m_pBufferFrame, pFrame->m_eFrameType);
			if (pFrame->m_pBufferFrame->GetLength()>0)
			{
				m_lstFrame.push_back(pFrame);
				bHaveNal = true;
			}
			else
			{
				bHaveNal = false;
				delete pFrame;
			}

		}while ( (m_eState == ePVFS_Run) && bHaveNal);

	}
	else if (nAvDataEndType == eADET_Audio)
	{//音频数据结束
		//debug("clear audio data: %d\n", m_pBufferStreamTmp->GetLength());
		m_pBufferStreamTmp->Clear();
	}
	else{

	}
	//
	return 0;
}
// @pFrame,  user alloc the memory and pass to this fun. the size must big enough, 512kB eg.
// @nLen,    in & out. the frame data length
// @nFrameType, eVivsFrameType
// @ret,      <0 if false, and nLen <= 0;
// @notes: user must call this fun several times , to get all frames.
long CParseVivsFrame::GetFrame(char *pFrame, int &nLen, int &nFrameType)
{
	if (m_lstFrame.size() <= 0)
	{
		return -1;
	}
	CStreamFrame *pSreamFrame=0;
	pSreamFrame = m_lstFrame.front();
	m_lstFrame.pop_front();
	if (pSreamFrame)
	{
		nFrameType = pSreamFrame->m_eFrameType;
		nLen = pSreamFrame->m_pBufferFrame->GetLength();
		memcpy(pFrame, pSreamFrame->m_pBufferFrame->GetData(), pSreamFrame->m_pBufferFrame->GetLength());
		delete pSreamFrame;
	}
	return 0;
}
long CParseVivsFrame::GetFrame(CStreamFrame *pFrame)
{
	if (m_lstFrame.size() <= 0)
	{
		return -1;
	}
	CStreamFrame *pSreamFrameTmp=0;
	pSreamFrameTmp = m_lstFrame.front();
	m_lstFrame.pop_front();
	if (pSreamFrameTmp)
	{
		pFrame->m_pBufferFrame->CopyFrom(pSreamFrameTmp->m_pBufferFrame);
		pFrame->m_eFrameType = pSreamFrameTmp->m_eFrameType;
		pFrame->m_nYear      = pSreamFrameTmp->m_nYear;
		pFrame->m_nMonth     = pSreamFrameTmp->m_nMonth;
		pFrame->m_nDay       = pSreamFrameTmp->m_nDay;
		pFrame->m_nHour      = pSreamFrameTmp->m_nHour;
		pFrame->m_nMinute    = pSreamFrameTmp->m_nMinute;
		pFrame->m_nSecond    = pSreamFrameTmp->m_nSecond;
		pFrame->m_nMiniSecond= pSreamFrameTmp->m_nMiniSecond;

		delete pSreamFrameTmp;
	}
	else
	{
		return -2;
	}
	return 0;
}
/*流格式：数据被"FTMV"分段为数据小块, 直到遇见sAvDataEnd标记，表示此前数据是可以解码的数据。sAvDataEnd也是由FTMV标记分割。
* sAvDataEnd 表示视频数据包完整结尾,在此之前的数据可以进行解码。
* DEF_ST_AV_STREAM_HEADER用于区分之前的这段数据，是音频还是视频数据，该avdatastream是可选的标记。该标记也是有FTMV标记分割
* FTMV...视频数据/音频数据.......[FTMV DEF_ST_AV_STREAM_HEADER ] FTMV _sAvDataEnd FTMV.....
*
* 返回当前pos
*/
int CParseVivsFrame::ParsePackage(char *pData, int nLen, CBuffer *pBufferOut, int &nOutLen, int &nAvDataEndType)
{
	int nMaxPos = nLen-FTMV_SIZE;
	int nPos0 = -1, nPos1=-1;
	//
	nOutLen = 0;
	// 分拆 FTMV 和 avdata end 
	for (int i=0; i<nMaxPos; i++)
	{
//		int nTmp = *((int*)(pData+i) );
		if (m_nFTMV ==  *((int*)(pData+i)) )
		{// FTMV flag
			if (nPos0==-1)
			{
				nPos0 = i+FTMV_SIZE;
				continue;
			}
			nPos1 = i;
			if (m_bParseAvdataEnd)
			{//移除 avdata end; [stream header]; 
				int nPktLen = nPos1-nPos0;
				if (nPktLen == sizeof(sAvStreamInfo)
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
					//
					break;
				}
				else if (nPos1-nPos0 == sizeof(_sAvDataEnd))
				{//
					char *pAvdataEnd = pData + nPos1 - sizeof(_sAvDataEnd);
					if (memcmp(pAvdataEnd, GW_AV_END_FLAG, GW_AV_END_FLAG_LENGTH) == 0)
					{//找到 avdataend，扔掉。
						nOutLen = 0;
						nPos0 = i+FTMV_SIZE;
						//nPos1 = -1;
						//保存 avdata end
						memcpy(&m_avDataEnd, pAvdataEnd, sizeof(sAvDataEnd));
						nAvDataEndType = eADET_Video;
						
						//
						break;
					}
				}
				//不是 avdata end，
				nOutLen = nPos1-nPos0;
				pBufferOut->Append((unsigned char*)pData+nPos0, nOutLen);
				nPos0 = nPos1+FTMV_SIZE;
			}
			else{
				//
				nOutLen = nPos1-nPos0;
			
				pBufferOut->Append((unsigned char *)pData+nPos0, nOutLen);
				nPos0 = nPos1+FTMV_SIZE;
			}
		}
	}

	//
	if (nPos1 == -1)
	{
		nPos1 = 0;
	}

	return nPos1;
}
int CParseVivsFrame::Split2AvdataPackage(char *pData, int nLen, CBuffer *pBufferOut, int &nOutLen, int &nAvDataEndType)
{
	int nMaxPos = nLen-FTMV_SIZE;
	int nPos0 = -1, nPos1=-1;
	//
	nOutLen = 0;
	// 分拆 FTMV 和 avdata end 
	for (int i=0; i<nMaxPos; i++)
	{
		//		int nTmp = *((int*)(pData+i) );
		if (m_nFTMV ==  *((int*)(pData+i)) )
		{// FTMV flag
			if (nPos0==-1)
			{
				nPos0 = i+FTMV_SIZE;
				continue;
			}
			nPos1 = i;
			if (m_bParseAvdataEnd)
			{//移除 avdata end; [stream header]; 
				int nPktLen = nPos1-nPos0;
				if (nPktLen == sizeof(sAvStreamInfo)
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
					//
					break;
				}
				else if (nPos1-nPos0 == sizeof(_sAvDataEnd))
				{//
					char *pAvdataEnd = pData + nPos1 - sizeof(_sAvDataEnd);
					if (memcmp(pAvdataEnd, GW_AV_END_FLAG, GW_AV_END_FLAG_LENGTH) == 0)
					{//找到 avdataend，扔掉。
						nOutLen = 0;
						nPos0 = i+FTMV_SIZE;
						//nPos1 = -1;
						//保存 avdata end
						memcpy(&m_avDataEnd, pAvdataEnd, sizeof(sAvDataEnd));
						nAvDataEndType = eADET_Video;

						//
						break;
					}
				}
				//不是 avdata end，
				nOutLen = nPos1-nPos0;
				pBufferOut->Append((unsigned char*)pData+nPos0-FTMV_SIZE, nOutLen);
				nPos0 = nPos1+FTMV_SIZE;
			}
			else{
				//
				nOutLen = nPos1-nPos0;

				pBufferOut->Append((unsigned char *)pData+nPos0-FTMV_SIZE, nOutLen);
				nPos0 = nPos1+FTMV_SIZE;
			}
		}
	}

	//
	if (nPos1 == -1)
	{
		nPos1 = 0;
	}

	return nPos1;
}
int CParseVivsFrame::ParseH264Nal(CBuffer *pH264Stream, CBuffer *pBufferNal, int &nFrameType)
{
	int nOutLen = 0;
	int nPos0 = 0;
	//
	int nMaxPos = pH264Stream->GetLength()-4;
	unsigned char *pH264Tmp = pH264Stream->GetData();
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
				if (pH264Tmp[i+4] &0x1f == 1)
				{
					nFrameType = eVFT_VideoPFrame;
				}
				else
				{
					nFrameType = eVFT_VideoIFrame;
				}
			}
			else
			{
				if ( (pH264Tmp[i+4] &0x1f) == 1
					&&(pH264Tmp[i+4] &0x1f) == 5 )
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
				if (pH264Tmp[i+3] &0x1f == 1)
				{
					nFrameType = eVFT_VideoPFrame;
				}
				else
				{
					nFrameType = eVFT_VideoIFrame;
				}
			}
			else
			{
				if ( (pH264Tmp[i+3] &0x1f) == 1 
					|| (pH264Tmp[i+3] &0x1f) == 5 )
				{
					nNalPos1 = i;
					break;
				}
			}
		}
		/*else if( (pH264Tmp[i+1] == 0)
			&& (pH264Tmp[i+2] == 0)
			&& (pH264Tmp[i+3] == 1)
			)
		{
			if (nNalPos0 == -1)
			{
				nNalPos0 = i;
			}
			else
			{
				nNalPos1 = i;
				break;
			}
		}*/
	}

	if (nNalPos1 > 0)
	{//找到一帧 Nal 数据
		pBufferNal->Append(pH264Tmp+nNalPos0, nNalPos1-nNalPos0);
		
		pH264Stream->Clear(0, nNalPos1);
	}
	else if (nNalPos0 > 0)
	{
		pH264Stream->Clear(0, nNalPos0);
	}

	return 0;
}
long CParseVivsFrame::ParseTimeStamp(char *pData, int nLen, unsigned int &u32Second, unsigned int &u32MilliSecond)
{
	if (!pData || nLen<= 0 )
		return -1;

	unsigned int *pTimeStampFlag = (unsigned int*)pData;
	int nPassedPos = -1;
	int nVivsTimeFlagLen = sizeof(sVivsTimeStamp);
	int nMaxPos = nLen-nVivsTimeFlagLen;
	for (int i=0; i<nMaxPos; i++)
	{
		pTimeStampFlag = (unsigned int*)(pData + i);
		if (*pTimeStampFlag == VIVS_TIME_FLAG)
		{
			sVivsTimeStamp *pVivsTimeStamp = (sVivsTimeStamp*)pTimeStampFlag;
			u32Second = pVivsTimeStamp->u32Seconds.time;
			u32MilliSecond = pVivsTimeStamp->u16MilliSecond;
			nPassedPos = i;
			break;
		}
	}

	return nPassedPos;
}

long CParseVivsFrame::ParseAvdataEnd(char *pDataIn, int nLenIn)
{
	if (!m_pBufferStreamTmp)
		return -1;

	int nPassedPos = 0;

	m_pBufferStream->Append((unsigned char*)pDataIn, nLenIn);
	char *pData = (char*)m_pBufferStream->GetData();
	int nLen = m_pBufferStream->GetLength();
	int nAvDataEndType = eADET_NotEnd;
	//搜索 pDataIn， 如果 有 avdataend 标记，表示一帧结束。
	int nPosEnd = 0;
	int nFrameType = eVFT_VideoIFrame;

	int i=0;
	
	while (m_eState == ePVFS_Run && i<nLen-FTMV_SIZE)
	{
       // printf("%s, while in, i %d: len %d\n", __FUNCTION__, i, nLen);
		for (; i<nLen-FTMV_SIZE; i++)
		{

			if (m_nFTMV !=  *((int*)(pData+i)) )
			{// FTMV flag
				continue;
			}
           // printf("%s, for in, got ftmv, i %d: len %d\n", __FUNCTION__, i, nLen);
			//查看 是否 是 avdataend
			char *pAvdataTmp = pData + i+FTMV_SIZE;
			if (i<nLen-sizeof(sAvStreamInfo)
				&& 0 == memcmp(pAvdataTmp, GW_AV_STREAM_END_FLAG, GW_MAX_AV_STREAM_END_FLAG_LEN))
			{	// stream end;
				sAvStreamInfo *pStreamInfo = (sAvStreamInfo *)pAvdataTmp;
				nAvDataEndType = pStreamInfo->ucMediaType;
				m_pStreamFrameTmp->m_eFrameType = nAvDataEndType;
			}
			else if ( i<nLen-sizeof(sAvDataEnd)
				&& 0 == memcmp(pAvdataTmp, GW_AV_END_FLAG, GW_AV_END_FLAG_LENGTH))
			{	
				nPosEnd = i+FTMV_SIZE+sizeof(sAvDataEnd);
				nFrameType = 0;//?
				nAvDataEndType = eADET_Video;
				sAvDataEnd *pAvdataEnd = (sAvDataEnd *)pAvdataTmp;
				m_pStreamFrameTmp->m_nYear = pAvdataEnd->stOsdTime.wYear;
				m_pStreamFrameTmp->m_nMonth = pAvdataEnd->stOsdTime.wMonth;
				m_pStreamFrameTmp->m_nDay = pAvdataEnd->stOsdTime.wDay;
				m_pStreamFrameTmp->m_nHour = pAvdataEnd->stOsdTime.wHour;
				m_pStreamFrameTmp->m_nMinute = pAvdataEnd->stOsdTime.wMinute;
				m_pStreamFrameTmp->m_nSecond = pAvdataEnd->stOsdTime.wSecond;

				break;
			}
			//
		}
        if (nAvDataEndType != eADET_NotEnd && nPosEnd > 0)
		{//一帧结束，保存一帧数据。
			//m_pBufferStreamTmp->Append(pDataIn, nPosEnd);
			//m_pStreamFrameTmp->m_pBufferFrame->Append(pDataIn, nPosEnd);
			//
            CStreamFrame *pFrame = new CStreamFrame();
			pFrame->m_pBufferFrame->Append(m_pBufferStream->GetData(), nPosEnd);
			pFrame->m_eFrameType	= m_pStreamFrameTmp->m_eFrameType;
			 
			pFrame->m_nYear			= m_pStreamFrameTmp->m_nYear  ;
			pFrame->m_nMonth		= m_pStreamFrameTmp->m_nMonth  ;
			pFrame->m_nDay			= m_pStreamFrameTmp->m_nDay  ;
			pFrame->m_nHour			= m_pStreamFrameTmp->m_nHour  ;
			pFrame->m_nMinute		= m_pStreamFrameTmp->m_nMinute;
			pFrame->m_nSecond		= m_pStreamFrameTmp->m_nSecond ;
			pFrame->m_nMiniSecond	= 0;

			m_lstFrame.push_back(pFrame);

			m_pStreamFrameTmp->m_pBufferFrame->Clear();
			//
			m_pBufferStream->Clear(0, nPosEnd);
	
            i = 0;
			nLen = m_pBufferStream->GetLength();
			pData = (char*)m_pBufferStream->GetData();
			nAvDataEndType = eADET_NotEnd;
            nPosEnd = 0;
            //printf("%s, for out if in, i %d: len %d\n", __FUNCTION__, i, nLen);
		}
		else
		{//没有找到帧结束，继续缓存数据
			break;
		}
	}
    printf("%s,end\n", __FUNCTION__);
	return 0;

}
//
//>=512k
long Vivs_FrameParserInit(int nBufferSize)
{
	if (nBufferSize < 512*1024)
	{
		nBufferSize = 512*1024;
	}
	CParseVivsFrame *pParser = new CParseVivsFrame();
	pParser->Init(nBufferSize);

	return (long)pParser;
}
void Vivs_FrameParserClose(long lParserHandle)
{
	if (!lParserHandle)
	{
		return ;
	}
	CParseVivsFrame *pParser = (CParseVivsFrame *)lParserHandle;
	pParser->Close();

	delete pParser;
}
long Vivs_FrameParserInputData(long lParserHandle, char *pData, int nLen)
{
	if (!lParserHandle)
		return -1;
	CParseVivsFrame *pParser = (CParseVivsFrame *)lParserHandle;
	pParser->ParseAvdataEnd(pData, nLen);

	return 0;
}
long Vivs_FrameParserGetFrame(long lParserHandle, char *pFrame, int *pLen, int *pFrameType)
{
	if (!lParserHandle)
		return -1;
	CParseVivsFrame *pParser = (CParseVivsFrame *)lParserHandle;

	int nRt = pParser->GetFrame(pFrame, *pLen, *pFrameType);

	
	return nRt;
}
