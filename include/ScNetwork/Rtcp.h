#pragma once
#ifndef RTCP_H
#define RTCP_H

#include <string>
#include <list>
#include <Windows.h>
#include "SimpleSocket.h"

using namespace std;

namespace SimpleCpp{

#define BUF_LEN_32 32
#define BUF_LEN_64 64
#define BUF_LEN_128 128
#define BUF_LEN_256 256
#define BUF_LEN_512 512
#define BUF_LEN_1024 1024

typedef enum _ePacketType
{
	ePacketType_Invalid = -1,
	ePacketType_SR = 200,
	ePacketType_RR = 201,
	ePacketType_SDES = 202,
	ePacketType_Bye = 203,
	ePacketType_App
}ePacketType;

class CSenderInfo
{
public:
	CSenderInfo();
	~CSenderInfo();

	void SetSsrc(UINT32 nSsrc);
	UINT32 GetSsrc();
	void SetNtpTimeStamp(UINT64 nTimeStamp);
	UINT64 GetNtpTimeStamp();
	void SetRtpTimeStamp(UINT32 nTimeStamp);
	UINT32 GetRtpTimeStamp();
	void SetSentPktCnt(UINT32 nSentPktCnt);
	UINT32 GetSentPktCnt();
	void SetSentOctCnt(UINT32 nSentOctCnt);
	UINT32 GetSentOctCnt();

private:
	UINT32 m_nSsrc;
	UINT64 m_nNtpTimeStamp;
	UINT64 m_nRtpTimeStamp;
	unsigned int m_nSentPktCnt;
	unsigned int m_nSentOctCnt;
};

class CReportBlock
{
public:
	CReportBlock();
	~CReportBlock();

	void SetSsrc(UINT32 nSsrc);
	UINT32 GetSsrc();
	void SetFractionLost(UCHAR nFractionLost);
	UCHAR GetFractionLost();
	void SetPacketsLost(UCHAR *pcPktsLost, int nLen = 3);
	UCHAR *GetPacketsLost();
	void SetExtSeqNum(UINT32 nExtSeqNum);
	UINT32 GetExtSeqNum();
	void SetInterArrivalJitter(UINT32 nJitter);
	UINT32 GetInterArrivalJitter();
	void SetLSR(UINT32 nLsr);
	UINT32 GetLSR();
	void SetDLSR(UINT32 nDLsr);
	UINT32 GetDLSR();

private:
	UINT32 m_nSsrc;
	UCHAR m_nFractionLost;
	UCHAR *m_pcPktsLost;
	int m_nLen;
	UINT32 m_nExtSeqNum;
	UINT32 m_nJitter;
	UINT32 m_nLsr;
	UINT32 m_nDLsr;
};

class CRtcpHeaderMessage
{
public:
	CRtcpHeaderMessage();
	~CRtcpHeaderMessage();

	void SetRtcpVersion(UCHAR nVersion);
	void SetRtcpPadding(UCHAR nPadding);
	void SetReportCnt(UCHAR nReportCnt);
	void SetRtcpPacketType(UCHAR nPacketType);
	void SetPacketLength(UCHAR nPacketLen);

	UCHAR m_nVersion;
	UCHAR m_nPadding;
	UCHAR m_nReportCnt;
	UCHAR m_nPacketType;
	UCHAR m_nPacketLen;
};

class CChunk:
	public CRtcpHeaderMessage
{
public:
	CChunk();
	~CChunk();
	
	void SetSsrc(UINT32 nSubItemSsrc);
	UINT32 GetSsrc();
	void SetSubItemType(UCHAR nType);
	UCHAR GetSubItemType();
	void SetSubItemTextLen(UCHAR nLen);
	UCHAR  GetSubItemTextLen();
	void SetSubItemText(string strText);
	string GetSubItemText();

private:
	UINT32 m_nSubItemSsrc;
	UCHAR m_nSubItemType;
	UCHAR m_nSubItemTextLen;
	string m_strSubItemText;
};

}
#endif