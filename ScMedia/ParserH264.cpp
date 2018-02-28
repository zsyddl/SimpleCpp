#include "StdAfx.h"
#include "ParserH264.h"
#include "Buffer.h"
#include "FilterPinBuffer.h"

CParserH264::CParserH264(void)
: m_pBufferStream(0)
{
	m_pBufferStream = new CBuffer(512*1024, 5*1024*1024);
}

CParserH264::~CParserH264(void)
{
	if (m_pBufferStream)
	{
		delete m_pBufferStream;
		m_pBufferStream = NULL;
	}
}

int CParserH264::Open(int nBufferCount)
{
	
	return CPackageParser::Open(nBufferCount);
}
//返回 长度， 
int CParserH264::InputData(char *pData, int nLen)
{
	if (!pData)
	{
		return -1;
	}
	m_pBufferStream->Append((unsigned char*)pData, nLen);
	//
	CBuffer bufferFrame;
	int nFrameType = 0;
	int nRt = nLen;
	do{
		bufferFrame.Clear();
		nFrameType = -1;
		ParseH264Frame(m_pBufferStream, &bufferFrame, nFrameType);
		if (bufferFrame.GetLength() <= 0)
			break;
		CFilterPin *pPinOut = m_lstPinOut.at(0);
		if (!pPinOut)
		{
			continue;
		}
		CFilterPinBuffer *pPinBufferFrame = 0;
		pPinOut->GetFreeBuffer(&pPinBufferFrame);
		if (!pPinBufferFrame)
		{
			continue;
		}
		pPinBufferFrame->Clear();
		pPinBufferFrame->Append((char*)bufferFrame.GetData(), bufferFrame.GetLength());
		pPinOut->PutPreparedBuffer(pPinBufferFrame);
	}while (m_state == eFilterState_Run);
	//

	return nRt;
}
//h264 分帧
//每次调用 只能解析一帧数据，需要反复调用, 5 7 8 都为 I帧 1 为 P帧。P帧之前的 为一个I帧,或者为一个P帧
int CParserH264::ParseH264Frame(CBuffer *pBufferH264, CBuffer *pBufferFrame,int &nFrameType)
{
	int nPos0=-1, nPos1=-1;
	int nMaxPos = pBufferH264->GetLength()-4;
	unsigned char *pPos = pBufferH264->GetData();
	for (int i=0; i<nMaxPos; i++)
	{
		if ( pPos[i] == 0
			&& pPos[i+1] == 0
			&& pPos[i+2] == 0
			&& pPos[i+3] == 1)
		{//
			if (nPos0 == -1)
			{
				nPos0 = i;

				if ((pPos[i+4]&0x1f) == 5
					|| (pPos[i+4]&0x1f) == 7
					/*|| (pPos[i+4]&0x1f) == 8*/)
				{
					nFrameType = eFT_I;
				}
				else
					nFrameType = eFT_P;
				i+=3;
				continue;
			}
			if ( nFrameType == eFT_I )
			{//当前 是I帧。则遇见p frame, 一个完整的帧 结束
				if ( (pPos[i+4]&0x1f) == 1 )
				{
					nPos1 = i;
					break;
				}
			}
			else
			{//p帧， 遇见一个 000001 都结束
				nPos1 = i;
				break;
			}
		}
		else if (pPos[i] == 0
			&& pPos[i+1] == 0
			&& pPos[i+2] == 1
			)
		{
			if (nPos0 == -1)
			{
				nPos0 = i;

				if ((pPos[i+3]&0x1f) == 5
					|| (pPos[i+3]&0x1f) == 7
					/*|| (pPos[i+3]&0x1f) == 8*/)
				{
					nFrameType = eFT_I;
				}
				else
					nFrameType = eFT_P;	
				i+=2;
				continue;
			}
			if ( nFrameType == eFT_I )
			{//当前 是I帧。则遇见p frame, 一个完整的帧 结束
				if ( (pPos[i+3]&0x1f) == 1 )
				{
					nPos1 = i;
					break;
				}
			}
			else
			{//p帧， 遇见一个 000001 都结束
				nPos1 = i;
				break;
			}
		}
	}
	int nRtPos = 0;
	if (nPos0 != -1
		&& nPos1 != -1)
	{
		//h264 AUD: 00 00 00 01 09 30 00 00 
		/*unsigned char szAUD[8];
		memset(szAUD, 0, 8);
		szAUD[3] = 1;
		szAUD[4] = 9;
		szAUD[5] = 30;
		pBufferFrame->Append(szAUD, sizeof(szAUD));
		*/
		pBufferFrame->Append(pPos+nPos0, nPos1-nPos0);
		pBufferH264->Clear(0, nPos1);
		nRtPos = nPos1;
	}
	else if (nPos0 != -1)
	{
		pBufferH264->Clear(0, nPos0);
		nRtPos = nPos0;
		return -1;
	}
	return 0;
}