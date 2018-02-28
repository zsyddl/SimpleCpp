#pragma once
#include <Windows.h>
#include <string>
#include "Rtcp.h"
#include "RtcpPacket.h"

namespace SThread
{
	class CThread;
}

using namespace std;
using namespace SThread;
using namespace SimpleCpp;

#define CriticalAccessSet(locker, var, val) do{\
		EnterCriticalSection(&locker);\
		m_##var = val;\
		LeaveCriticalSection(&locker);\
	}while(0)

#define CriticalAccessGet(locker, var, val) do{\
	EnterCriticalSection(&locker);\
	val = m_##var;\
	LeaveCriticalSection(&locker);\
}while(0)

#define CriticalAccessCmpWithRet(locker, var, cmpVal, ret, retVal) do{\
	EnterCriticalSection(&locker);\
	ret = 0;\
	if (cmpVal != m_##var)\
	{\
		LeaveCriticalSection(&locker);\
		ret = retVal;\
		return ret;\
	}\
	LeaveCriticalSection(&locker);\
}while(0)

#define RTCP_BUF_LEN 50*1024

typedef enum _eThreadRunState
{
	eThreadRunState_Run = 1,
	eThreadRunState_Stop,
	eThreadRunState_ToStop,
}eThreadRunState;

typedef enum _eRtpRemotePort
{
	eRtpRemotePort_UnSet = 1,
	eRtpRemotePort_Set,
	eRtpRemotePort_Exception,
}eRtpRemotePort;

class CBaseMediaDataTransmission;
class CRtcpMediaData;

class CRtcpSession
{
	public:
		CRtcpSession();
		~CRtcpSession();

		int Open(string &strLocalIp, int nLocalPort, string &strRemoteIp, int nRemotePort);
		int Close();
		int SetPacketSeqNum(unsigned short sExtHighSeqNum);
		int IncreasePacketCnt();
		int SetRtcpRemotePort(int nRemotePort);
		int SendPacket();
		int SendByePacket();

	protected:
		CBaseMediaDataTransmission *m_pRtcpMediaData;

	private:
		static int __stdcall ProcRtcpSession(void *pParam);
		int ClearPacketCnt();
		int RecvPacket();
		int GetRtpPort();

		CThread *m_pRtcpThread;
		eThreadRunState m_eRtcpThreadState;
		TIMERPROC m_TimerOutProcFunc;

		unsigned short m_nPacketCounter;
		unsigned short m_sPreSeqNum;
		unsigned short m_sCurSeqNum;

		/*互斥访问线程状态变量*/
		CRITICAL_SECTION m_hThreadStateMutex;

		/*互斥访问序列号和端口号*/
		CRITICAL_SECTION m_hSyncVarMutex;
		char *m_pcBuffer;
		int m_nBufLen;

		string m_strRemoteIp;
		int m_nRemotePort;
		string m_strLocalIp;
		int m_nLocalPort;
		char *m_pRecvBuffer;
};