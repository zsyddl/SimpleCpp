#pragma once

//#include "Socket.h"
//#include "AsyncSocket.h"
#include "Buffer.h"
#include <string>
#include <map>
#include "ScpDefine.h"
#include "Transport.h"

namespace SimpleSocket{
	class CSocket;
	class CAsyncSocket;
	class CSocketEventHandler;
}
namespace SThread{
	class CThread;
	class CMutex;
}
using namespace SThread;

using namespace SimpleSocket;
using namespace std;

namespace Scp{
#define MAX_SOCKET_RECV_SIZE_PER_TIME (65*1024)
class CMessage;
class CScp;
class CSession;
class CProtocol;
class CMessageProcessor;
class CTransport;
class CRpcService;
//class CSessionSocket;
// 
// class CSessionSocketHandler
// 	: public CSocketEventHandler
// {
// public:
// 	CSessionSocketHandler(CScp *pScp);
// 	~CSessionSocketHandler();
// 	
// 	virtual int OnRecv(CSocket *pSocket, int nErr);
// 
// protected:
// 	CScp*	m_pScp;
// };
// class CSessionSocket: public CAsyncSocket
// {
// public:
// 	CSessionSocket(CSession* pSession);
// 	~CSessionSocket();
// 
// 	int OnRecv(int nErr);
// protected:
// 	CSession*	m_pSession;
// 	//
// 	CMessage*	m_pMessageTmp;
// 	//接收数据的缓存。
// 	CBuffer*	m_pRecvBuffer;
// };
class SCP_API CSession:
	public CTransportHandler
{
	friend class CScp;
	friend class CMessage;
public:
	CSession(CScp *pScp=NULL);
	virtual ~CSession(void);

	virtual int		Open(string strRemoteIp, int nRemotePort, 
						string strLocalIp, int nLocalPort);
	//
	//
	virtual void	Close();

	virtual	void	DestroyResponse(CMessage* pMessage);
	//virtual int		Attach(CAsyncSocket* pSocket);

	int		GetSessionId(){return m_nSessionId;}
	void	SetSessionId(int nId){m_nSessionId = nId;}

	int		Send(CMessage *pMessage, int nTimeout=2*1000);
	// nTimeout， milliseconds;
	int		Send(CMessage *pRequest, CMessage **pResponse, int nTimeout=0);

	//通知有完整的消息收到；[将 消息 添加到队列]。
	virtual int		OnMessage(CMessage *pMessage);
	//
	//
	void				SetProtocol(CProtocol *pProtocol){m_pProtocol = pProtocol;};
	CProtocol*			GetProtocol(){return m_pProtocol;};
	void				SetProcessor(CMessageProcessor* pProcessor);;
	CMessageProcessor*	GetProcessor(){return m_pProcessor;}
	void				SetTransport(CTransport* pTransport);;
	CTransport*			GetTransport(){return m_pTransport;};
	//
	CScp*	GetScp(){return m_pScp;}
	void	SetScp(CScp* pScp){ m_pScp = pScp;}
	//
	//CRpcService*	GetRootService();
	//void	SetRootService(CRpcService* pRootService);
	virtual	int			SetNoDataTimeout(int nTimeout);
protected:
	//transport 接收到消息
	virtual int			OnRecv(CTransport* pTransport);
	virtual	int			OnNoDataTimeout(CTransport* pTransport, int nTimes);
	//
	//消息队列。主要是收到的消息.
	//发送的消息都是直接发送出去，等待返回(同步情况)
	int			AppendSendMessage(CMessage* pMessage);
	int			RemoveSendMessage( CMessage*pMessage );
	CMessage*	FindRequestMessage(int nMessageId);
	//
	//CSessionSocketHandler	*m_pSocketHandler;
	//传输
	CTransport*			m_pTransport;
//	CAsyncSocket*		m_pSocket;
	CScp*				m_pScp;
	//协议处理的类，对 transport的 数据，做协议解析。
	CProtocol*			m_pProtocol;
	//
	//消息处理器，主要是负责 消息分发。给对应的服务接口
	//对协议解析出来的消息，做进一步处理。
	CMessageProcessor*	m_pProcessor;
	//
	int					m_nSessionId;
	//
	int					m_nMessageSeq;
	//
	//发送的消息映射队列， 用于等待发送消息之后的回应，同步消息
	map<int, CMessage*>	m_mapId2SendMessage;
	CMutex*				m_pMutexMapSendMessage;
	CMutex*				m_pMutex;
	//
	//CRpcService*		m_pRootService;
};
}