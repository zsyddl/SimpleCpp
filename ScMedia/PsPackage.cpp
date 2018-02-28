#include "StdAfx.h"
#include "PsPackage.h"
#include "Buffer.h"

#define printf
#ifndef TRACE
#define TRACE
#endif

#define MAX_STREAM_BUFFER (2*1024*1024)
CPsPackage::CPsPackage(int nW, int nH, int nFPS, int nBitRate)
:m_pBufferH264(NULL)
, m_nTimeStamp(0)
, m_nW(nW)
, m_nH(nH)
, m_nFPS(nFPS)
, m_nBitRate(nBitRate)
{
	m_pBufferH264 = new CBuffer(MAX_STREAM_BUFFER);
}

CPsPackage::~CPsPackage(void)
{
	if (m_pBufferH264)
	{
		delete m_pBufferH264;
		m_pBufferH264 = NULL;
	}
}
//对 ps流解包.输出为 标准 h264  
int CPsPackage::UnPackage(unsigned char *pPsStream, int nLen, unsigned char *pH264Stream, int &nLenOut)
{
	m_pBufferH264->Append(pPsStream, nLen);
	//
	bool bFindPsHeader = false;
	char *pPos = (char*)m_pBufferH264->GetData();
	int nMaxPos = m_pBufferH264->GetLength() - PS_HEADER_FLAG_LEN; 
	nLenOut = 0;
	int nPesLen = 0;
	int nValidPos = 0;
	for (int i=0; i<nMaxPos; /*i++*/)
	{
		//
		pPos = (char*)m_pBufferH264->GetData() + i;
		unsigned int *pFlag  = (unsigned int *)pPos;
		//判断 是否是 ps 头标记. sys, psm, pes 
		if (*pFlag == PS_HEADER_FLAG)
		{//ps header
			if (nMaxPos-i < PS_HEADER_FLAG_LEN)
			{//数据长度不够
				break;
			}
			PS_HEADER_LEN;
			i += PS_HEADER_LEN;
			nPesLen = 0;
		}else if (*pFlag == PS_SYS_FLAG)
		{// sys header
			if (nMaxPos-i < PS_SYS_LEN)
			{//数据长度不够
				break;
			}
			i += PS_SYS_LEN;
			nPesLen = 0;
		}else if (*pFlag == PS_PSM_FLAG)
		{//psm header
			if (nMaxPos-i < PS_PSM_FLAG)
			{//数据长度不够
				break;
			}
			i += PS_PSM_LEN;
			nPesLen = 0;
		}else if ( (*pFlag&0xff)>>8 == PS_PES_HEADER_FLAG )
		{// pes flag len
			int nPesHeaderStart = i;
			//数据长度不够
			if (nMaxPos-i < 16)
			{//数据不够
				break;
			}
			i += 3;
			int nPesHeahderLen = 0;
			// streamid 1bytes
			i += 1;
			// 5,6 pes package len 
			i += 2;
			nPesLen =  *(pPos+4)<<8|*(pPos+5);
			// 7  optional PES HEADER的第一个字节。目前是写公定值，以后应该扩展成可配置的。
			i += 1;
			//header len
			char *pPesHeaderLen = pPos+i;
			nPesHeahderLen = *pPesHeaderLen;
			i += 1;
			// 8 ps ts
			unsigned int *pPsTsFlag = (unsigned int*)(pPos + 7);
			i += 1;
			if (*pPsTsFlag & OPTIONAL_FIELDS_PTS_DTS)
			{
				i+=10;
			}
			else if (*pPsTsFlag & OPTIONAL_FIELDS_ONLY_PTS)
			{
				i+=5;
			}else{
				i+=5;
			}
			//就是headerlen 16 (字节)
			i = nPesHeaderStart + nPesHeahderLen/* - (i-)*/;
			// pes 数据
			//nPesLen - nPesHeahderLen;
			if (nMaxPos-i < nPesLen)
			{//数据不够
				break;
			}
			//
			memcpy(pH264Stream, m_pBufferH264->GetData()+i, nPesLen-nPesHeahderLen );
			nLenOut += nPesLen-nPesHeahderLen;
			//
			i += nPesLen-nPesHeahderLen;
			//记录有效数据位置,用于最后的删除 使用过的数据
			nValidPos = i;
		}
		else
		{//不是标记,是数据
			i++;
		}
		//
	}
	m_pBufferH264->Clear(0, nValidPos);


	return 0;
}
int CPsPackage::UnPackage(unsigned char *pPsStream, int nLen, CBuffer *pBufferH264)
{
	if (m_pBufferH264->GetLength() > MAX_STREAM_BUFFER)
	{
		m_pBufferH264->Clear();
	}
	m_pBufferH264->Append(pPsStream, nLen);
	//
	bool bFindPsHeader = false;
	char *pPos = (char*)m_pBufferH264->GetData();
	int nMaxPos = m_pBufferH264->GetLength() - PS_HEADER_FLAG_LEN; 
	
	int nPesLen = 0;
	int nPassedPos = 0;
	for (int i=0; i<nMaxPos; /*i++*/)
	{
		//
		pPos = (char*)m_pBufferH264->GetData() + i;
		unsigned int *pFlag  = (unsigned int *)pPos;
		//判断 是否是 ps 头标记. sys, psm, pes 
		if (*pFlag == PS_HEADER_FLAG)
		{//ps header
			if (nMaxPos-i < PS_HEADER_FLAG_LEN)
			{//数据长度不够
				break;
			}
			PS_HEADER_LEN;
			i += PS_HEADER_LEN;
			nPesLen = 0;
		}else if (*pFlag == PS_SYS_FLAG)
		{// sys header
			if (nMaxPos-i < PS_SYS_LEN)
			{//数据长度不够
				break;
			}
			i += PS_SYS_LEN;
			nPesLen = 0;
		}else if (*pFlag == PS_PSM_FLAG)
		{//psm header
			if (nMaxPos-i < PS_PSM_LEN)
			{//数据长度不够
				break;
			}
			i += PS_PSM_LEN;
			nPesLen = 0;
 		}
//			else if ()
// 		{//跳过 h264 的 代码 00 00 00 1 
// 
// 		}
		else if ( (*pFlag&0x00ff0000)<<8 == PS_PES_HEADER_FLAG )
		{// pes flag len
			int nPesHeaderStart = i;
			unsigned char *pPesStart = (unsigned char *)pFlag;
			int nPesPos = 0;
			//数据长度不够
			if (nMaxPos-i < 16)
			{//数据不够
				break;
			}
			nPesPos += 3;
			int nPesHeaderDataLen = 0;
			// streamid 1bytes
			nPesPos += 1;
			// 5,6 pes package len 
			nPesPos += 2;
			nPesLen = (*((unsigned char *)pPesStart+ 4)<<8|*((unsigned char *)pPesStart+ 5))&0xffff;
			// 7  optional PES HEADER的第一个字节。目前是写公定值，以后应该扩展成可配置的。
			nPesPos += 1;
			//pes header  data len, 从这个 字节 之后的数据长度, ps ts + ff 标记,  9 字节 之后的.
			//9, header data len, 就是headerlen 16 (字节)
			unsigned char *pPesHeaderLen = (unsigned char*)pPesStart + 8;
			nPesHeaderDataLen = *pPesHeaderLen;
			nPesPos += 1;
			// 9 ps ts
			unsigned int *pPsTsFlag = (unsigned int*)(pPesStart + 9);
			nPesPos += 1;
			if (*pPsTsFlag & OPTIONAL_FIELDS_PTS_DTS)
			{
				i+=10;
			}
			else if (*pPsTsFlag & OPTIONAL_FIELDS_ONLY_PTS)
			{
				i+=5;
			}else{
				i+=5;
			}
			i = nPesHeaderStart + nPesHeaderDataLen + 9/* - (i-)*/;
			// pes 数据
			//nPesLen - nPesHeahderLen;
			if (nMaxPos-nPesHeaderStart < nPesLen-3)
			{//数据不够
				break;
			}
			//
			int nPesDataLen = nPesLen-nPesHeaderDataLen-3;
			if (nPesDataLen > 0 )
			{
				pBufferH264->Append(m_pBufferH264->GetData()+i, nPesDataLen);
				TRACE("pes: start [%d]-[%d], pes data len[%d], pes len[%d]\n", 
					nPesHeaderStart, nPesHeaderStart+nPesLen, nPesDataLen, nPesLen);
				//
				i += nPesDataLen; //;nPesLen-nPesHeaderDataLen-9;
				//记录有效数据位置,用于最后的删除 使用过的数据
				nPassedPos = i;
			}
		}
		else
		{//不是标记,是数据
			i++;
		}
		//
	}
	m_pBufferH264->Clear(0, nPassedPos);

	return 0;
}
int CPsPackage::Package(unsigned char *pH264, int nLen, unsigned char *pPsStream, int &nLenOut)
{
	m_pBufferH264->Append(pH264, nLen);
	

	
	return 0;

}
int CPsPackage::Package(unsigned char *pH264, int nLen, CBuffer *pBufferPsPkg)
{
	m_pBufferH264->Append(pH264, nLen);
	CBuffer bufferFrame;
	int nFrameType = 0;
	do{
		bufferFrame.Clear();
		nFrameType = -1;
		ParseH264Frame(m_pBufferH264, &bufferFrame, nFrameType);
		if (bufferFrame.GetLength() <= 0)
			return -1;
		//AUD
		unsigned char szAUD[8];
		memset(szAUD, 0, 8);
		szAUD[3] = 1;
		szAUD[4] = 9;
		szAUD[5] = 30;
// 		CBuffer bufferAud;
// 		bufferAud.Append(szAUD, sizeof(szAUD));
		//PackageH2642Ps(&bufferAud, pBufferPsPkg, eFT_P);
		//
		int nPsLen = PackageH2642Ps(&bufferFrame, pBufferPsPkg, nFrameType);
	}while (1);
	
	return 0;
}
int CPsPackage::PackageFrame(unsigned char *pH264Frame, int nLen, int nFrameType, CBuffer *pBufferPsPkg)
{

	int nPsLen = PackageH2642Ps(pH264Frame,  nLen, nFrameType, pBufferPsPkg);

	return nPsLen;
}
//h264 分帧
//每次调用 只能解析一帧数据，需要反复调用, 5 7 8 都为 I帧 1 为 P帧。P帧之前的 为一个I帧,或者为一个P帧
int CPsPackage::ParseH264Frame(CBuffer *pBufferH264, CBuffer *pBufferFrame,int &nFrameType)
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
	}
	return 0;
}
/*
*他与 stPSInof.u64SCR 是与时间戳是300倍的关系pstMediaInfo->u32TimeStamp += 3600 25帧的情况下 pstMediaInfo->u32TimeStamp是个递增 3600从你的时间芯片获取时间的哦协议 规定 1秒刻度 是 90000你刚才 那个3000是凑巧你该下 你获得的pstMediaInfo->u32TimeStamp值的累加
*
*/
int CPsPackage::PackageH2642Ps(CBuffer *pBufferFrame, CBuffer *pBufferPsPkg, int nFrameType)
{

	StMediaInfo mediaInfo;
	if (nFrameType == eFT_I)
		mediaInfo.s32MediaType = 0x01;
	else
	{
		mediaInfo.s32MediaType = 0x02;
	}
	mediaInfo.s32VideoHeight = m_nH;
	mediaInfo.s32VideoWidth = m_nW;
	mediaInfo.s32VideoFPS = m_nFPS;

	mediaInfo.u32TimeStamp = m_nTimeStamp;
	m_nTimeStamp += 90000/m_nFPS;/*3000*/;//???

	StMediaInfo *pstMediaInfo = &mediaInfo;
	StPSInfo stPSInof;
	memset(&stPSInof, 0, sizeof(StPSInfo));
	stPSInof.u64SCR = pstMediaInfo->u32TimeStamp * 300;
	stPSInof.u32MuxRate = m_nBitRate/50.0;//188*8.0/3.0;	//应该与 码率/50 相对应。这里暂时写死，以后要跟码率联系起来。
	stPSInof.s32MediaType = pstMediaInfo->s32MediaType;
	//stPSInof.s32Flag |= (pstMediaInfo->s32LaunchFlag & 0x01) ? 0x01 :0x00;
	stPSInof.s32Flag = (mediaInfo.s32MediaType==0x01)? 0x01:0x00;

	int nOutLen = 0;
	char szPsTmp[MAX_PS_SIZE];
	memset(szPsTmp, 0, MAX_PS_SIZE);
	int s32Ret = Media2PSPacket(pBufferFrame->GetData(), pBufferFrame->GetLength(), 
		&stPSInof, (unsigned char*)szPsTmp, &nOutLen);
	
	if(s32Ret != 0)
	{
		//释放动态申请的空间
		//PrintErrInfo(__FILE__, __LINE__, "封PS失败!");
		return -1;
	}
	pBufferPsPkg->Append((unsigned char*)szPsTmp, nOutLen);

	return nOutLen;
}
int CPsPackage::PackageH2642Ps(unsigned char *pFrame, int nLen, int nFrameType, CBuffer *pBufferPsPkg)
{

	StMediaInfo mediaInfo;
	if (nFrameType == eFT_I)
		mediaInfo.s32MediaType = 0x01;
	else
	{
		mediaInfo.s32MediaType = 0x02;
	}
	mediaInfo.s32VideoHeight = m_nH;
	mediaInfo.s32VideoWidth = m_nW;
	mediaInfo.s32VideoFPS = m_nFPS;

	mediaInfo.u32TimeStamp = m_nTimeStamp;
	m_nTimeStamp += 90000/m_nFPS;/*3000*/;//???

	StMediaInfo *pstMediaInfo = &mediaInfo;
	StPSInfo stPSInof;
	memset(&stPSInof, 0, sizeof(StPSInfo));
	stPSInof.u64SCR = pstMediaInfo->u32TimeStamp * 300;
	stPSInof.u32MuxRate = m_nBitRate/50.0;//188*8.0/3.0;	//应该与 码率/50 相对应。这里暂时写死，以后要跟码率联系起来。
	stPSInof.s32MediaType = pstMediaInfo->s32MediaType;
	//stPSInof.s32Flag |= (pstMediaInfo->s32LaunchFlag & 0x01) ? 0x01 :0x00;
	stPSInof.s32Flag = (mediaInfo.s32MediaType==0x01)? 0x01:0x00;

	int nOutLen = 0;
	char szPsTmp[MAX_PS_SIZE];
	memset(szPsTmp, 0, MAX_PS_SIZE);
	int s32Ret = Media2PSPacket(pFrame, nLen, 
		&stPSInof, (unsigned char*)szPsTmp, &nOutLen);

	if(s32Ret != 0)
	{
		//释放动态申请的空间
		//PrintErrInfo(__FILE__, __LINE__, "封PS失败!");
		return -1;
	}
	pBufferPsPkg->Append((unsigned char*)szPsTmp, nOutLen);

	return nOutLen;
}
#define  PrintErrInfo 
int CPsPackage::Media2PSPacket(IN unsigned char *pMediaBuf, IN int s32MediaBufSize, IN StPSInfo *pstPsInfo, \
				   OUT unsigned char *pPSBuf, OUT int *pPSBufSize)
{
	//参数检查
	if(NULL == pMediaBuf || s32MediaBufSize < 0 || NULL == pstPsInfo || NULL == pPSBuf)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}

	unsigned char *pBuf = pPSBuf;
	int s32BufSize = 0;

	//写pack_start_code 4字节
	Write32bit(&pBuf[s32BufSize], 0x000001ba);
	s32BufSize += 4;

	//写system_clock_reference_base 6字节
	WriteSCR(&pBuf[s32BufSize], pstPsInfo->u64SCR);
	s32BufSize += 6;

	//写program_mux_rate 3字节
	pBuf[s32BufSize ++] = (pstPsInfo->u32MuxRate >> 14) & 0xff;
	pBuf[s32BufSize ++] = (pstPsInfo->u32MuxRate >> 6) & 0xff;//zsyddl
	pBuf[s32BufSize ++] = ((pstPsInfo->u32MuxRate << 2) & 0xfc) | 0x03;
	
	//------ 以上4+6 + 3 = 13 bytes;
	//写pack_stuffing_length 1字节
	unsigned short u16StuffingLen = BYTE_ALIGNMENT - (s32BufSize + 1/*还应该加上系统头的长度*/) % BYTE_ALIGNMENT;
	pBuf[s32BufSize ++] = (u16StuffingLen & 0x07) | 0xf8;

	//写stuffing_byte 与pack_stuffing_length有关，一次一个字节
	int i = 0;
	for(i = 0; i < u16StuffingLen; i ++)
	{
		pBuf[s32BufSize ++] = 0xff;
	}
	// end ps header  16 bytes

	//根据情况还要添加系统头(system_header)，本例不需要，暂时不填加
	//todo：

	//I帧前需要添加PSM头和SYS头
	unsigned int u32PSMSize = 0;
	unsigned int u32SYSSize = 0;
	//if(pstPsInfo->s32MediaType & 0x01)
	if(pstPsInfo->s32Flag & 0x01)
	{

		//sys头需要完善
		//PS包信息中的标志位
#define FIXED_FLAG_SYS_HEADER					(1 << 1)	//系统头中fixed_flag标志
#define CSPS_FLAG_SYS_HEADER					(1 << 2)	//系统头中CSPS标志
#define SYSTEM_AUDIO_LOCK_FLAG_SYS_HEADER		(1 << 3)	//系统头中system_audio_lock_flag标志
#define SYSTEM_VIDEO_LOCK_FLAG_SYS_HEADER		(1 << 4)	//系统头中system_video_lock_flag标志
#define PACKET_RATE_RESTRICTION_FLAG_SYS_HEADER	(1 << 5)	//系统头中packet_rate_restriction_flag标志
#define PSTD_BUFFER_BOUND_SCALE_SYS_HEADER		(1 << 6)	//系统头中P-STD_buffer_size_bound标志
		//写sys包
		unsigned int u32SysHeadFlag = 0;
		u32SysHeadFlag = (u32SysHeadFlag | SYSTEM_VIDEO_LOCK_FLAG_SYS_HEADER | PSTD_BUFFER_BOUND_SCALE_SYS_HEADER);
		//18 bytes
		if(0 != WriteSystemHeader(&pBuf[s32BufSize], (unsigned short*)&u32SYSSize, u32SysHeadFlag, 9 + 3, 272250, 0, 1, 0XE0, 7169))
		{
			PrintErrInfo(__FILE__, __LINE__, "添加SYS头失败!");
			return -1;
		}
		else
		{
			s32BufSize += u32SYSSize;
		}


		//写psm 24bytes
		if(0 != CreatePSM(STREAM_TYPE_VIDEO_H264, STREAM_TYPE_AUDIO_G711, &pBuf[s32BufSize], &u32PSMSize))
		{
			PrintErrInfo(__FILE__, __LINE__, "添加PSM头失败!");
			return -1;
		}
		else
		{
			s32BufSize += u32PSMSize;
		}
	}

	//给PES流的结构体赋值
	StPESInfo stPesInfo;
	memset(&stPesInfo, 0, sizeof(StPESInfo));
	if(0x10 == pstPsInfo->s32MediaType)
	{
		stPesInfo.u8StreamID = 0xC0;    //音频的streamId为0xC0
	}
	else if(0x0B && pstPsInfo->s32MediaType)
	{
		stPesInfo.u8StreamID = 0xE0;    //视频的streamId为0xE0
	}
	//stPesInfo.u8StreamID = 0xE0;  
	stPesInfo.u32Flag |=  OPTIONAL_FIELDS_ONLY_PTS;//zsy OPTIONAL_FIELDS_ONLY_PTS;    //只有显示时间戳(PTS)字段
	stPesInfo.u64Pts = pstPsInfo->u64SCR / 300;       //显示时间戳就等于SCR
	//stPesInfo.u64Dts = stPesInfo.u64Pts;
	//后面跟PES流
	int s32PESSize = 0;
	int s32Ret = Media2PESStream(pMediaBuf, s32MediaBufSize, &stPesInfo, &pBuf[s32BufSize], &s32PESSize);
	if(s32Ret != 0)
	{
		PrintErrInfo(__FILE__, __LINE__, "封装PES失败!");
		return -1;
	}
	else
	{
		s32BufSize += s32PESSize;
		*pPSBufSize = s32BufSize;
	}

	return 0;
}

unsigned short CPsPackage::Write16bit(OUT unsigned char *pBuf, IN unsigned short u16Value)
{
	//参数检查
	if(NULL == pBuf)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}

	*pBuf ++ = (u16Value >> 8) & 0xff;
	*pBuf = u16Value & 0xff;

	return 0;
}
unsigned short CPsPackage::Write32bit(OUT unsigned char *pBuf, IN unsigned int u32Value)
{
	//参数检查
	if(NULL == pBuf)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}

	*pBuf ++ = (u32Value >> 24) & 0xff;
	*pBuf ++ = (u32Value >> 16) & 0xff;
	*pBuf ++ = (u32Value >> 8) & 0xff;
	*pBuf ++ = u32Value & 0xff;

	return 0;
}

int CPsPackage::WriteSCR(OUT unsigned char *pBuf, IN unsigned long long u64SCR)
{
	//参数检查
	if(NULL == pBuf || u64SCR <= 0)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}

	unsigned long long u64SCRBase = u64SCR / 300;
	unsigned short u16SCRExtern = u64SCR % 300;
	*pBuf ++ = ((u64SCRBase >> 27) & 0x38) | ((u64SCRBase >> 28) & 0x03) | 0x44;
	*pBuf ++ = (u64SCRBase >> 20) & 0xff;
	*pBuf ++ = ((u64SCRBase >> 12) & 0xf8) | ((u64SCRBase >> 13) & 0x03) | 0x04;
	*pBuf ++ = (u64SCRBase >> 5) & 0xff;
	*pBuf ++ = ((u64SCRBase << 3) & 0xf8) | ((u16SCRExtern >> 7) & 0x03) | 0x04;
	*pBuf ++ = ((u16SCRExtern << 1) & 0xfe) | 0x01;

	return 0;
}
//18 bytes
int CPsPackage::WriteSystemHeader(unsigned char *pBuf, unsigned short *pu16bufLen, unsigned int u32SysHeadFlag, \
					  unsigned short u16HeaderLength, unsigned int u32RateBound, unsigned char u8AudioBound, \
					  unsigned char u8Videobound, unsigned char u8StreamID, unsigned short u16PSTDBufSize)
{	
	//参数检查	
	if((u32RateBound > 0x3fffff) || (u8AudioBound > 0x3f) || (u8Videobound > 0x1f) || (u16PSTDBufSize > 0x1fff) || (NULL == pBuf))	
	{		
		printf("WriteSystemHeader:参数检查失败\n");		
		return -1;	
	}	

	//申请空间	
	unsigned char *p = pBuf;	

	//写system_header_start_code 4字节	
	Write32bit(p, 0x000001bb);	
	p += 4;	

	//写header_length 2字节	
	Write16bit(p, u16HeaderLength);	
	p += 2;

	//写rate_bound 3字节	
	*p ++ = ((u32RateBound >> 15) & 0x7f) | 0x80;	
	*p ++ = (u32RateBound >> 7) & 0xff;	
	*p ++ = ((u32RateBound << 1) & 0xfe) | 0x01;	
   
	//写audio_bound及标志位	
	*p ++ = ((u8AudioBound << 2) & 0xfc) | (u32SysHeadFlag & FIXED_FLAG_SYS_HEADER) | ((u32SysHeadFlag & CSPS_FLAG_SYS_HEADER) >> 2);	

	//写video_bound及标志位	
	*p ++ = (u8Videobound & 0x1f) | ((u32SysHeadFlag & SYSTEM_AUDIO_LOCK_FLAG_SYS_HEADER) << 4) | \
		((u32SysHeadFlag & SYSTEM_VIDEO_LOCK_FLAG_SYS_HEADER) << 2) | 0x20;	

	//写packet_rate_restriction_flag标志位	
	*p ++ = ((u32SysHeadFlag & PACKET_RATE_RESTRICTION_FLAG_SYS_HEADER) << 3) | 0x7f;	

	/*	 
	* 下面为节目流中包含的节目，可能包括音频和视频。目前只包含一个视频，包含更多的内容以后扩展	 
	*/	
	//写video stream_id	
	*p ++ = u8StreamID;	
	//写P-STD_buffer_bound_scale和P-STD_buffer_size_bound	
	*p ++ = 0xc0 | ((u32SysHeadFlag & PSTD_BUFFER_BOUND_SCALE_SYS_HEADER) >> 1) | ((u16PSTDBufSize >> 8) & 0x1f);	
	*p ++ = (u16PSTDBufSize & 0xff);	


	//写audio stream_id	
	*p ++ = 0xc0;	
	//写P-STD_buffer_bound_scale和P-STD_buffer_size_bound	
	*p ++ = 0xc0 | ((u32SysHeadFlag & PSTD_BUFFER_BOUND_SCALE_SYS_HEADER) >> 1) | ((u16PSTDBufSize >> 8) & 0x1f);	
	*p ++ = (u16PSTDBufSize & 0xff);	

	*pu16bufLen = p - pBuf;
	///--4 + 2 + 3 + + 9= 18  
	return 0;
}

//24 bytes
int CPsPackage::CreatePSM(IN unsigned int u32VideoType, IN unsigned int u32AudioType, OUT unsigned char *pBuf, OUT unsigned int *pBufSize)
{
	//参数检查
	if(NULL == pBuf || NULL == pBufSize)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}
	else
	{
		*pBufSize = 0;
	}

	unsigned char *p = pBuf;
	unsigned int u32Count = 0;
	//写packet_start_code_prefix 3字节
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x01;

	//写map_stream_id 1字节
	p[u32Count ++] = 0xbc;

	//写program_stream_map_length 2字节 在没有descriptor()且只有一路节目情况下，program_stream_map_length = 14字节。
	//两路节目(音视频)时需多加4个字节
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x0e + 4;

	//写current_next_indicator和program_stream_map_version
	p[u32Count ++] = 0xe0;

	//后面一个字节为填充字段
	p[u32Count ++] = 0xff;

	//写program_stream_info_length 2字节
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x00;

	//写elementary_stream_map_length,在没有descriptor()，且只有一路节目情况下，elementary_stream_map_length = 4字节
	//两路节目(音视频)时需多加4个字节
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x04 + 4;

	//写视频节目
	//写stream_type
	p[u32Count ++] = u32VideoType;

	//写elementary_stream_id
	p[u32Count ++] = 0xe0;

	//写elementary_stream_info_length
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x00;

	//写音频节目
	//写stream_type
	p[u32Count ++] = u32AudioType;

	//写elementary_stream_id
	p[u32Count ++] = 0xc0;

	//写elementary_stream_info_length
	p[u32Count ++] = 0x00;
	p[u32Count ++] = 0x00;

	//CRC校验
	unsigned int u32CRC = 0; //CalcCrc32(p, u32Count); zsy ???
	Write32bit(&p[u32Count], u32CRC);
	u32Count += 4;

	*pBufSize = u32Count;

	return 0;
}
//pes header 目前为 16 bytes
int CPsPackage::Media2PESStream(IN unsigned char *pMediaBuf, IN int s32MediaBufSize, IN StPESInfo *pstPesInfo, \
					OUT unsigned char *pPESBuf, OUT int *pPESBufSize)
{
	//参数检查
	if(NULL == pMediaBuf || s32MediaBufSize <= 0 || NULL == pstPesInfo || NULL == pPESBuf || \
		NULL == pPESBufSize)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}
	else
	{
		*pPESBufSize = 0;
	}

	//判断媒体数据有没有大于规定的长度(最大媒体数据长度由SPLIT_PACKET_LENGTH宏定义)，超过进行分帧处理
	int s32RemainDataLen = s32MediaBufSize;
	int u32OutDataLen = 0;
	unsigned char *pSplitPacketInBuf = pMediaBuf;
	unsigned char *pSplitPacketOutBuf = pPESBuf;
	unsigned u32PacketNum = 0;
	StPESInfo stSplitePacketPesInfo;
	//给stSplitePacketPesInfo赋值，非第一包的PES头可以简化，去掉其他字段
	memset(&stSplitePacketPesInfo, 0, sizeof(StPESInfo));
	stSplitePacketPesInfo.u8StreamID = pstPesInfo->u8StreamID;

	while(s32RemainDataLen > 0)
	{
		if(s32RemainDataLen > SPLIT_PACKET_LENGTH)
		{
			if(0 == u32PacketNum)
			{
				Media2PESPacket(pSplitPacketInBuf, SPLIT_PACKET_LENGTH, pstPesInfo, pSplitPacketOutBuf, &u32OutDataLen);
			}
			else
			{
				Media2PESPacket(pSplitPacketInBuf, SPLIT_PACKET_LENGTH, &stSplitePacketPesInfo, pSplitPacketOutBuf, &u32OutDataLen);
			}

			s32RemainDataLen -= SPLIT_PACKET_LENGTH;
			pSplitPacketInBuf += SPLIT_PACKET_LENGTH;
			pSplitPacketOutBuf += u32OutDataLen;
			*pPESBufSize += u32OutDataLen;
		}
		else
		{
			if(0 == u32PacketNum)
			{
				Media2PESPacket(pSplitPacketInBuf, s32RemainDataLen, pstPesInfo, pSplitPacketOutBuf, &u32OutDataLen);
			}
			else
			{
				Media2PESPacket(pSplitPacketInBuf, s32RemainDataLen, &stSplitePacketPesInfo, pSplitPacketOutBuf, &u32OutDataLen);
			}

			s32RemainDataLen -= s32RemainDataLen;
			pSplitPacketInBuf += s32RemainDataLen;
			pSplitPacketOutBuf += u32OutDataLen;
			*pPESBufSize += u32OutDataLen;
		}
	}

	return 0;
}
//长度根据 标记位 计算 ,可变
int CPsPackage::Media2PESPacket(IN unsigned char *pMediaBuf, IN int s32MediaBufSize, IN StPESInfo *pstPesInfo, \
					OUT unsigned char *pPESBuf, OUT int *pPESBufSize)
{
	//参数检查
	if(NULL == pMediaBuf || s32MediaBufSize < 0 || NULL == pstPesInfo || NULL == pPESBuf)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}
	else
	{
		*pPESBufSize = 0;
	}

	//计算PES_header_data_length字段的长度
	unsigned char u8PESHeaderDataLength = 0;
	int s32Ret = CalculatePESHeaderDataLength(pstPesInfo);
	//PES_header_data_length字段的长度用一个字节表示，因此必须小于0xff。同时需要考虑以后字节对齐时添加的填充位数量
	if( s32Ret< 0 || s32Ret > 0xff - BYTE_ALIGNMENT)
	{
		PrintErrInfo(__FILE__, __LINE__, "计算PES_header_data_length字段长度失败!");
		return -1;
	}
	else
	{
		u8PESHeaderDataLength = s32Ret;
	}
	/*
	//计算PES_packet_length字段的长度
	unsigned short u16PESPacketLength = 3 + u8PESHeaderDataLength + s32MediaBufSize;//根据协议可知

	//保证封装完的PES包是字节对齐的。如果未对齐，则增加u8PESHeaderDataLength的值，即在PES头后面增加填充位
	u8PESHeaderDataLength += BYTE_ALIGNMENT - (u16PESPacketLength + 6) % BYTE_ALIGNMENT;//+6是因为PES_packet_length字段前还有6字节的内容
	u16PESPacketLength += BYTE_ALIGNMENT - (u16PESPacketLength + 6) % BYTE_ALIGNMENT;
	*/

	//unsigned char u8PESHeaderStuffLen = BYTE_ALIGNMENT - (u8PESHeaderDataLength + 9) % BYTE_ALIGNMENT;//+9是因为PES_packet_length字段前还有9字节的内容
	unsigned char u8PESHeaderStuffLen = BYTE_ALIGNMENT - (u8PESHeaderDataLength + 9) % BYTE_ALIGNMENT;//+9是因为PES_packet_length字段前还有9字节的内容
	u8PESHeaderDataLength += u8PESHeaderStuffLen;
	//计算PES_packet_length字段的长度
	//unsigned short u16PESPacketLength = 3 + u8PESHeaderDataLength + s32MediaBufSize;//根据协议可知
	//zsyddl // 表示 在  u16PESPacketLength 字段后 有多少字节, 目前是 6 个字节 
	unsigned short u16PESPacketLength = 3 + u8PESHeaderDataLength + s32MediaBufSize;//根据协议可知

	////////////////////////////////////////////////////
	/*
	if(pMediaBuf[4] == 0x67){
	printf("--->u8PESHeaderDataLength %x\n", u8PESHeaderDataLength);
	printf("s32MediaBufSize %x\n", s32MediaBufSize);
	printf("u16PESPacketLength %x\n", u16PESPacketLength);
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	}
	*/

	/*
	unsigned char u16PESPacketStuffLen = BYTE_ALIGNMENT - (u16PESPacketLength + 6) % BYTE_ALIGNMENT;
	u16PESPacketLength += u16PESPacketStuffLen;
	*/

	unsigned char *pPesStream = pPESBuf;    //封PES包的buf，封完之后就是输出的内容
	unsigned int s32PesStreamLen = 0;       //PES流的长度,即pPesStream的长度

	//写packet_start_code_prefix  24bits 值为固定的0x000001
	pPesStream[s32PesStreamLen ++] = 0x00;
	pPesStream[s32PesStreamLen ++] = 0x00;
	pPesStream[s32PesStreamLen ++] = 0x01;
	//写stream_id    8bits
	pPesStream[s32PesStreamLen ++] = pstPesInfo->u8StreamID;
	//写PES_packet_length    16bits
	if(0 != Write16bit(&pPesStream[s32PesStreamLen], u16PESPacketLength))
	{
		PrintErrInfo(__FILE__, __LINE__, "写PES媒体数据长度失败!");
		return -1;
	}
	else
	{
		s32PesStreamLen += 2;
	}
	//写optional PES HEADER的第一个字节。目前是写公定值，以后应该扩展成可配置的。
	pPesStream[s32PesStreamLen ++] = 0x80;
	//写optional PES HEADER第二个字节(7个标志位)
	unsigned char u8TempFlag = (pstPesInfo->u32Flag & 0x3f);
	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ONLY_PTS)
	{
		u8TempFlag |= 0x80;
	}
	else if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PTS_DTS)
	{
		u8TempFlag |= 0xC0;
	}
	pPesStream[s32PesStreamLen ++] = u8TempFlag;
	//写PES_header_data_length字段的长度
	pPesStream[s32PesStreamLen ++] = u8PESHeaderDataLength;
	//写optional fields
	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PTS_DTS)
	{
		//写PTS DTS
		if(WriteTimeStamp(&pPesStream[s32PesStreamLen], 0x03, pstPesInfo->u64Pts))
		{
			PrintErrInfo(__FILE__, __LINE__, "写PTS失败!");
			return -1;
		}
		s32PesStreamLen += 5;
		//
		if(WriteTimeStamp(&pPesStream[s32PesStreamLen], 0x01, pstPesInfo->u64Dts))
		{
			PrintErrInfo(__FILE__, __LINE__, "写DTS失败!");
			return -1;
		}
		s32PesStreamLen += 5;
	}
	else if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ONLY_PTS)
	{
		//写PTS
		if(WriteTimeStamp(&pPesStream[s32PesStreamLen], 0x02, pstPesInfo->u64Pts))
		{
			PrintErrInfo(__FILE__, __LINE__, "写PTS失败!");
			return -1;
		}
		s32PesStreamLen += 5;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ESCR)
	{
		//todo 写ESCR字段
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ES_RATE)
	{
		//todo 写ES_RATE字段
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_DSM_TRICK_MODE)
	{
		//todo 写DSM_TRICK_MODE字段
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ADDITIONAL_COPY_INFO)
	{
		//todo 写ADDITIONAL_COPY_INFO字段
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PREVIOUS_PES_CRC)
	{
		//todo 写PREVIOUS_PES_CRC字段
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PES_EXTENSION)
	{
		//todo 写PES_EXTENSION字段
	}

	/*
	//PES头最后，填充字节，保证字节对齐
	while((int)(u16PESPacketLength + 6 - s32PesStreamLen - s32MediaBufSize) > 0) //+6时因为PESPacketLength字段前还有6个字节的内容
	{
	pPesStream[s32PesStreamLen ++] = 0xFF;
	}
	*/
	//PES头最后，填充字节，保证字节对齐
	while(u8PESHeaderStuffLen > 0)
	{
		pPesStream[s32PesStreamLen ++] = 0xFF;
		u8PESHeaderStuffLen --;
	}    

	memcpy(&pPesStream[s32PesStreamLen], pMediaBuf, s32MediaBufSize);
	s32PesStreamLen += s32MediaBufSize;

	/*
	//H264填充字节，保证字节对齐
	while(u16PESPacketStuffLen > 0)
	{
	pPesStream[s32PesStreamLen ++] = 0xFF;
	u16PESPacketStuffLen --;
	}    
	*/
	*pPESBufSize = s32PesStreamLen;

	return 0;
}

int CPsPackage::CalculatePESHeaderDataLength(IN StPESInfo *pstPesInfo)
{
	//参数检查
	if(NULL == pstPesInfo)
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}

	/*
	//可配置PES包头(optional PES HEADER)中的7个标志位
	#define OPTIONAL_FIELDS_ONLY_PTS              (1 << 7)  //  |-与OPTIONAL_FIELDS_PTS_DTS为一个标志位--|
	#define OPTIONAL_FIELDS_PTS_DTS               (1 << 6)  //  |-与OPTIONAL_FIELDS_ONLY_PTS为一个标志位-|
	#define OPTIONAL_FIELDS_ESCR                  (1 << 5)
	#define OPTIONAL_FIELDS_ES_RATE               (1 << 4)
	#define OPTIONAL_FIELDS_DSM_TRICK_MODE        (1 << 3)
	#define OPTIONAL_FIELDS_ADDITIONAL_COPY_INFO  (1 << 2)
	#define OPTIONAL_FIELDS_PREVIOUS_PES_CRC      (1 << 1)
	#define OPTIONAL_FIELDS_PES_EXTENSION         (1 << 0)
	*/

	int s32Length = 0;
	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ONLY_PTS)
	{
		s32Length += 5;
	}
	else if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PTS_DTS)
	{
		s32Length += 10;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ESCR)
	{
		s32Length += 6;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ES_RATE)
	{
		s32Length += 3;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_DSM_TRICK_MODE)
	{
		s32Length += 1;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_ADDITIONAL_COPY_INFO)
	{
		s32Length += 1;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PREVIOUS_PES_CRC)
	{
		s32Length += 4;
	}

	if(pstPesInfo->u32Flag & OPTIONAL_FIELDS_PES_EXTENSION)
	{
		//OPTIONAL_FIELDS_PES_EXTENSION比较复杂，暂时也没用到，因此这里没有处理该字段的情况，以后需要了在扩展
	}

	return s32Length;
}

unsigned short CPsPackage::WriteTimeStamp(OUT unsigned char *pBuf, IN unsigned char u8ID, IN unsigned long long u64TimeStamp)
{
	//参数检查
	if((NULL == pBuf) || (u8ID <= 0) || (u64TimeStamp < 0))
	{
		PrintErrInfo(__FILE__, __LINE__, "参数检查失败!");
		return -1;
	}

	/* 1: 4 bit id value | TS [32..30] | marker_bit */
	*pBuf ++ = ((u8ID << 4) | ((u64TimeStamp >> 29) & 0x0E) | 0x01) & 0xff;
	/* 2, 3: TS[29..15] | marker_bit */
	if(0 != Write16bit(pBuf, ((u64TimeStamp >> 14) & 0xfffe) | 0x01))
	{
		PrintErrInfo(__FILE__, __LINE__, "写时间戳2，3字节失败!");
		return -1;
	}
	pBuf += 2;
	/* 4, 5: TS[14..0] | marker_bit */
	if(0 != Write16bit(pBuf, ((u64TimeStamp << 1) & 0xfffe) | 0x01))
	{
		PrintErrInfo(__FILE__, __LINE__, "写时间戳4，5字节失败!");
		return -1;
	}

	return 0;
}
