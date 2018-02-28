#include "RtcpSession.h"
#include "RtcpMediaData.h"
#include "RtcpPacket.h"
#include "Thread.h"

CRtcpSession::CRtcpSession()
{
	m_pRtcpThread = NULL;
	m_nPacketCounter = 0;
	m_sPreSeqNum = 0;
	m_sCurSeqNum = 0;
	m_strRemoteIp.clear();
	m_nRemotePort = 0;
	m_strLocalIp.clear();
	m_nLocalPort = 0;
	m_eRtcpThreadState = eThreadRunState_Stop;
	m_pRecvBuffer = NULL;
	m_pcBuffer = NULL;
	m_nBufLen = 0;
	m_pRtcpMediaData = NULL;
	InitializeCriticalSection(&m_hThreadStateMutex);
	InitializeCriticalSection(&m_hSyncVarMutex);
}

CRtcpSession::~CRtcpSession()
{

}

int CRtcpSession::SetPacketSeqNum(unsigned short sExtHighSeqNum)
{
	CriticalAccessSet(m_hSyncVarMutex, sCurSeqNum, sExtHighSeqNum);
	return 0;
}

int CRtcpSession::IncreasePacketCnt()
{
	EnterCriticalSection(&m_hSyncVarMutex);
	m_nPacketCounter++;
	LeaveCriticalSection(&m_hSyncVarMutex);

	return 0;
}

int CRtcpSession::ClearPacketCnt()
{
	CriticalAccessSet(m_hSyncVarMutex, nPacketCounter, 0);
	return 0;
}

int CRtcpSession::RecvPacket()
{
	int nRecvedBytes = -1;
	int nRecvBufferSize = RTCP_BUF_LEN;
	int nLocalPort = 0;

	nRecvedBytes = m_pRtcpMediaData->Recv(m_pRecvBuffer, nRecvBufferSize);
	if (0 < nRecvedBytes)
	{
		CRtcpPacket *pRtcpPacket = new CRtcpPacket;
		pRtcpPacket->Init();
		pRtcpPacket->Parse(m_pRecvBuffer, nRecvedBytes);
		//process recv packets
	}

	return 0;
}

int CRtcpSession::GetRtpPort()
{
	int nRet = 0;

	while (1)
	{
		CriticalAccessCmpWithRet(m_hThreadStateMutex, eRtcpThreadState, eThreadRunState_Run, nRet, eRtpRemotePort_Exception);
		if (eRtpRemotePort_Exception == nRet)
			return eRtpRemotePort_Exception;

		int nPort = m_nRemotePort;
		CriticalAccessCmpWithRet(m_hSyncVarMutex, nRemotePort, 0, nRet, eRtpRemotePort_Set);
		if (eRtpRemotePort_Set == nRet)
			return eRtpRemotePort_Set;

		Sleep(5);
	}
}

int CRtcpSession::SendPacket()
{
	CRtcpHeaderMessage *pHeaderMsg = new CRtcpHeaderMessage;
	CRtcpHeaderMessage *pSdesHeader = new CRtcpHeaderMessage;
	CReportBlock *pReportBlock = new CReportBlock;
	CChunk *pChunk = new CChunk;
	CRtcpRrPacket cRrItem;
	CRtcpSdesPacket cSdesItem;
	unsigned int nExpectedPacket = 0;
	double nLost = 0;
	double nFrac = 0;
	unsigned char nFractionLost = 0;
	string strIp;
	int nPort;

	strIp = m_strRemoteIp;
	nPort = m_nRemotePort + 1;

	pHeaderMsg->SetRtcpVersion(2);
	pHeaderMsg->SetRtcpPadding(0);
	pHeaderMsg->SetReportCnt(1);
	pHeaderMsg->SetRtcpPacketType(201);
	
	pSdesHeader->SetRtcpVersion(2);
	pSdesHeader->SetRtcpPadding(0);
	pSdesHeader->SetReportCnt(1);
	pSdesHeader->SetRtcpPacketType(202);

	pReportBlock->SetSsrc(0);
	UCHAR acArray[3] = "34";
	pReportBlock->SetPacketsLost(acArray);
	pReportBlock->SetInterArrivalJitter(0);
	pReportBlock->SetLSR(0);
	pReportBlock->SetDLSR(0);

	unsigned short usSeqNum = 0;
	CriticalAccessGet(m_hSyncVarMutex, sCurSeqNum, usSeqNum);

	unsigned short usPacketsCounter = 0;
	CriticalAccessGet(m_hSyncVarMutex, nPacketCounter, usPacketsCounter);
	CriticalAccessSet(m_hSyncVarMutex, nPacketCounter, 0);

	pReportBlock->SetExtSeqNum(usSeqNum);
	
	if (usSeqNum < m_sPreSeqNum)
	{
		nExpectedPacket = ((unsigned long)usSeqNum + 65536) - m_sPreSeqNum;
	}
	else
	{
		nExpectedPacket = usSeqNum - m_sPreSeqNum;
	}

	if (nExpectedPacket != 0 && m_sPreSeqNum != 0 && usSeqNum != 0)
	{
		nLost = (double)(nExpectedPacket - usPacketsCounter);
		nFrac = nLost/(double)nExpectedPacket;
		nFractionLost = (unsigned char)(nFrac *256.0);
		
		pReportBlock->SetFractionLost(nFractionLost);

		//sdes
		pChunk->SetSsrc(0);
		pChunk->SetSubItemType(1);
		pChunk->SetSubItemTextLen(6);
		pChunk->SetSubItemText("hello");

		cRrItem.SetSsrc(1234);
		cRrItem.SetRtcpHeader(pHeaderMsg);
		cRrItem.AddReportBlock(pReportBlock);
		cSdesItem.SetRtcpHeader(pSdesHeader);
		cSdesItem.AddChunk(pChunk);

		int nRrPacketLen = 0;
		cRrItem.GetPackedData(m_pcBuffer, &nRrPacketLen);
		int nSdesPacketLen = 0;
		cSdesItem.GetPackedData(m_pcBuffer + nRrPacketLen, &nSdesPacketLen);

		m_pRtcpMediaData->SendTo(m_pcBuffer, nRrPacketLen + nSdesPacketLen, (char *)strIp.c_str(), nPort);
		m_nBufLen = RTCP_BUF_LEN;

		/*unsigned char aucTemp[20] = "CNAME";
		cRtcpPacket.SetSdesItemSubType(1);
		cRtcpPacket.SetSdesItemLength(11);

		unsigned char auText[20] = "zhangtaotao";
		cRtcpPacket.SetSdesItemText(auText, 11);

		cRtcpPacket.PackSdesPacket();

		pRtcpSession->m_nPacketCounter = 0;
		pRtcpSession->m_pRtcpMediaData->SendTo((char *)cRtcpPacket.GetPackData(), cRtcpPacket.GetSdesItemPacketLen(), (char *)strIp.c_str(), nPort);*/
	}
	
	m_sPreSeqNum = usSeqNum;

	return 0;
}

int CRtcpSession::SendByePacket()
{
	CRtcpHeaderMessage *pRrHeaderMsg = new CRtcpHeaderMessage;
	CReportBlock *pReportBlock = new CReportBlock;
	CRtcpRrPacket *pRrPkt = new CRtcpRrPacket;
	CRtcpHeaderMessage *pByeHeaderMsg = new CRtcpHeaderMessage;
	CRtcpByePacket *pByePkt = new CRtcpByePacket;

	pRrHeaderMsg->SetRtcpVersion(2);
	pRrHeaderMsg->SetRtcpPadding(0);
	pRrHeaderMsg->SetReportCnt(1);
	pRrHeaderMsg->SetRtcpPacketType(201);

	pRrPkt->SetSsrc(1234);
	pRrPkt->SetRtcpHeader(pRrHeaderMsg);

	pReportBlock->SetSsrc(0);
	pReportBlock->SetFractionLost(0);
	UCHAR acArray[3] = "34";
	pReportBlock->SetPacketsLost(acArray);
	pReportBlock->SetInterArrivalJitter(0);
	pReportBlock->SetLSR(5);
	pReportBlock->SetDLSR(6);

	pRrPkt->AddReportBlock(pReportBlock);

	pByeHeaderMsg->SetRtcpVersion(2);
	pByeHeaderMsg->SetRtcpPadding(0);
	pByeHeaderMsg->SetReportCnt(1);
	pByeHeaderMsg->SetRtcpPacketType(203);

	pByePkt->SetRtcpHeader(pByeHeaderMsg);

	pByePkt->AddSsrc(1);
	pByePkt->AddSsrc(2);
	pByePkt->AddSsrc(3);
	pByePkt->AddSsrc(4);

	int nReasonLen = 20;
	char acReason[20];

	memset(acReason, 0, nReasonLen);
	memcpy(acReason, "zhangtaotao", strlen("zhangtaotao"));
	pByePkt->SetByeReason(acReason, sizeof(acReason));

	string strIp;
	int nPort;

	strIp = m_strRemoteIp;
	nPort = m_nRemotePort + 1;
	int nPackedRrLen = 0;
	int nPackedByeLen = 0;
	int nPackedDataBufLen = BUF_LEN_1024;
	char *pcPackedData = new char[nPackedDataBufLen];

	memset(pcPackedData, 0, nPackedDataBufLen);
	pRrPkt->GetPackedData(pcPackedData, &nPackedRrLen);
	pByePkt->GetPackedData(pcPackedData + nPackedRrLen, &nPackedByeLen);

	m_pRtcpMediaData->SendTo(pcPackedData, nPackedRrLen + nPackedByeLen, (char *)strIp.c_str(), nPort);

	return 0;
}

int CRtcpSession::ProcRtcpSession(void *pParam)
{
	int nEvent = -1;
	int nSocketRet = 0;
	CRtcpSession *pRtcpSession = NULL;
	string strIp;
	int nPort;
	int nState = 0x01 | 0x08;
	int &nStateRef = nState;
	pRtcpSession = (CRtcpSession *)pParam;
	int nCount = 0;
	int nLocalPort = 0;

	nLocalPort = pRtcpSession->m_nLocalPort;
	strIp = pRtcpSession->m_strRemoteIp;
	nPort = pRtcpSession->m_nRemotePort + 1;
	pRtcpSession->m_pRtcpMediaData = new CRtcpMediaData;
	if (NULL != pRtcpSession->m_pRtcpMediaData)
	{
		nSocketRet = pRtcpSession->m_pRtcpMediaData->Open(pRtcpSession->m_strLocalIp, nLocalPort, strIp, nPort);
		if (nSocketRet != 0)
		{
			goto ret;
		}
	}

	nCount = 0;
	while(1)
	{
		EnterCriticalSection(&pRtcpSession->m_hThreadStateMutex);
		if (eThreadRunState_Run != pRtcpSession->m_eRtcpThreadState)
		{
			LeaveCriticalSection(&pRtcpSession->m_hThreadStateMutex);
			break;
		}
		LeaveCriticalSection(&pRtcpSession->m_hThreadStateMutex);

		if(nCount >= 300)
		{
			nEvent = pRtcpSession->m_pRtcpMediaData->Select(nStateRef, 2);
			if (nEvent & 0x01)
			{
				pRtcpSession->RecvPacket();
			}		

			pRtcpSession->SendPacket();
			nCount = 0;
		}else{
			nCount ++;
		}
		
		//Sleep(3000);
		Sleep(10);
	}
	
	pRtcpSession->SendByePacket();

ret:

	return 0;
}

int CRtcpSession::Open(string &strLocalIp, int nLocalPort, string &strRemoteIp, int nRemotePort)
{
	int nRet = 0;

	m_strLocalIp = strLocalIp;
	m_nLocalPort = nLocalPort;
	m_strRemoteIp = strRemoteIp;
	m_nRemotePort = nRemotePort;

	NewMem(m_pRecvBuffer, RTCP_BUF_LEN, char);
	NewMem(m_pcBuffer, RTCP_BUF_LEN, char);
	if (NULL == m_pRecvBuffer || NULL == m_pcBuffer)
	{
		DelMem(m_pRecvBuffer);
		DelMem(m_pcBuffer);
		return -1;
	}

	m_nBufLen = RTCP_BUF_LEN;

	m_pRtcpThread = new CThread;

	if (NULL != m_pRtcpThread)
	{
		m_eRtcpThreadState = eThreadRunState_Run;
		nRet = m_pRtcpThread->Create(ProcRtcpSession, this);
	}

	if (0 != nRet || NULL == m_pRtcpThread)
	{
		if (NULL != m_pRtcpThread)
		{
			m_pRtcpThread->Close();
			delete m_pRtcpThread;
			m_pRtcpThread = NULL;
		}

		DelMem(m_pRecvBuffer);
		DelMem(m_pcBuffer);

		m_eRtcpThreadState = eThreadRunState_Stop;

		return -1;
	}

	return 0;
}

int CRtcpSession::Close()
{
	CriticalAccessSet(m_hThreadStateMutex, eRtcpThreadState, eThreadRunState_Stop);

	if(NULL != m_pRtcpThread)
	{
		while (m_pRtcpThread->IsRunning())
		{
			Sleep(10);
		}

		void (CThread::*fp1)(void);
		fp1 = &CThread::Close;
		CLoseSocketOrThread(m_pRtcpThread, m_pRtcpThread, fp1);
	}

	void (CBaseMediaDataTransmission::*fp2)(void);
	fp2 = &CBaseMediaDataTransmission::Close;
	
	if (NULL != m_pRtcpMediaData)
		CLoseSocketOrThread(m_pRtcpMediaData, m_pRtcpMediaData, fp2);

	DelMem(m_pRecvBuffer);
	DelMem(m_pcBuffer);

	DeleteCriticalSection(&m_hThreadStateMutex);
	DeleteCriticalSection(&m_hSyncVarMutex);

	return 0;
}

int CRtcpSession::SetRtcpRemotePort(int nRemotePort)
{
	CriticalAccessSet(m_hSyncVarMutex, nRemotePort, nRemotePort);
	return 0;
}