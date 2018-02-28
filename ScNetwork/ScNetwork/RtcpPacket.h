#pragma once
#ifndef RTCPPACKET_H
#define RTCPPACKET_H

#include "Rtcp.h"

namespace SimpleCpp{

class CRtcpSrPacket;
class CRtcpRrPacket;
class CRtcpSdesPacket;
class CRtcpByePacket;
class CRtcpSdesPacket;

class CRtcpPacket
{
public:
	CRtcpPacket();
	~CRtcpPacket();

	int Init();
	int UnInit();
	virtual int GetPackedData(char *pPackedData, int *pPackedLen);
	virtual int Parse(char *pMsgBuf, int nMsgLen);

protected:
	char *m_pBuf;
	int m_nBufLen;

private:
	CRtcpSrPacket *m_pRtcpSrPacket;
	CRtcpRrPacket *m_pRtcpRrPacket;
	CRtcpSdesPacket *m_pRtcpSdesPacket;
	CRtcpByePacket *m_pRtcpByePacket;
};

class CRtcpSrPacket:
	public CRtcpPacket
{
public:
	CRtcpSrPacket();
	~CRtcpSrPacket();

	void SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader);
	int SetSenderInfo(CSenderInfo *pSenderInfo);
	int AddReportBlock(CReportBlock *pReportBlock);
	int DeleteReportBlock(CReportBlock *pReportBlock);
	CReportBlock *GetFirstReportBlock();
	CReportBlock *GetNextReportBlock(CReportBlock *pReportBlock);
	virtual int GetPackedData(char *pPackedData, int *pPackedLen);
	virtual int Parse(char *pMsgBuf, int nMsgLen);

private:
	CRtcpHeaderMessage *m_pRtcpHeader;
	CSenderInfo *m_pSenderInfo;
	list<CReportBlock *> m_listReportBlock;
};

class CRtcpRrPacket:
	public CRtcpPacket
{
public:
	CRtcpRrPacket();
	~CRtcpRrPacket();

	void SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader);
	void SetSsrc(UINT32 nSsrc);
	int AddReportBlock(CReportBlock *pReportBlock);
	int DeleteReportBlock(CReportBlock *pReportBlock);
	CReportBlock *GetFirstReportBlock();
	CReportBlock *GetNextReportBlock(CReportBlock *pReportBlock);
	virtual int GetPackedData(char *pPackedData, int *pPackedLen);
	virtual int Parse(char *pMsgBuf, int nMsgLen);

private:
	UINT32 m_nSsrc;
	CRtcpHeaderMessage *m_pRtcpHeader;
	list<CReportBlock *> m_listReportBlock;
};

class CRtcpSdesPacket:
	public CRtcpPacket
{
public:
	CRtcpSdesPacket();
	~CRtcpSdesPacket();

	void SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader);
	void AddChunk(CChunk *pChunk);
	void DeleteChunk(CChunk *pChunk);

	virtual int GetPackedData(char *pPackedData, int *pPackedLen);
	virtual int Parse(char *pMsgBuf, int nMsgLen);

private:
	CRtcpHeaderMessage *m_pRtcpHeader;
	list<CChunk *> m_listChunk;
};

class CRtcpByePacket:
	public CRtcpPacket
{
public:
	CRtcpByePacket();
	~CRtcpByePacket();

	void SetRtcpHeader(CRtcpHeaderMessage *pRtcpHeader);
	void AddSsrc(UINT32 nSsrc);
	void DeleteSsrc(UINT32 nSsrc);
	void SetByeReason(char *pByeReason, char nByeReasonLen);
	virtual int GetPackedData(char *pPackedData, int *pPackedLen);
	virtual int Parse(char *pMsgBuf, int nMsgLen);

private:
	CRtcpHeaderMessage *m_pRtcpHeader;
	char m_nByeReasonLen;
	char *m_pByeReason;
	list<UINT32> m_listByePktSsrc;
};

}

#endif