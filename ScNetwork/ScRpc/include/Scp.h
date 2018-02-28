// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SCP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SCP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。


#ifndef SCP_H
#define SCP_H

#include <vector>
#include <string>
#include <set>
#include <queue>
#include <map>
#include <list>
#include "ScpDefine.h"
#include "Types.h"

using namespace std;
namespace SimpleSocket{
	class CAsyncSocket;
	class CSocket;
	class CSocketPumper;
}
namespace SThread{
	class CThread;
	class CMutex;
	class CThreadPool;
}
using namespace SimpleSocket;
using namespace SThread;
namespace Scp{
	class CMessage;
	class CRequest;
	class CResponse;
	class CRegister;
	class CRegisterAck;
	class CSession;
	class CListenSocketHandler;
	class CSessionSocketHandler;
	class CScp;
	class CMessageHandler;
	class CProtocol;
	class CMessageProcessor;
	class CTransport;
	
class CRpcDispatcher;
class CRpcService;
class CFactory;
class CServiceContainer;
// 此类是从 Scp.dll 导出的; 
class SCP_API CScp {
	friend class CListenSocketHandler;
	friend class CSession;

public:
	CScp(void);
	~CScp();
	//
	static int Init();
	static int Free();
	//
	int		Open(string strLocalIp, int nLocalPort/*, string strRemoteIp, int nRemotePort*/);
	void	Close();
	//
	//
	void	SetListenSocket(CSocket* pSocket);;
	CSocket *GetListenSocket();;
	//void	SetMessageHandler(CMessageHandler* pHandler);
	void	SetMessageProcessor(CMessageProcessor *pProcessor){m_pProcessor = pProcessor;};
	CMessageProcessor *GetMessageProcessor(){return m_pProcessor;};
	void	SetProtocol(CProtocol *pProcotol){m_pProcotol = pProcotol;}
	CProtocol *GetProtocol(){return m_pProcotol;}
	void	SetTransport(CTransport *pTransport);
	CTransport*	GetTransport(){return m_pTransport;};
	//同步等待返回的消息
	int		Send(CMessage *pRequest, CMessage **pResponse, int nMilliSeconds );
	int		Send(CResponse *pResponse);
	int		Send(CRequest  *pRequest);
	int		Send(CMessage*	pMessage);

	CSession*	FindSession( int nId );
	//通常用于客户端
	virtual CSession*	CreateSession();;
	virtual void		DestroySession(CSession* pSession);
	//登记会话 
	int		RegisterSession(int nId, CSession* pSession);
	//
	//


	//内部调用的
	int		AppendSession(CSession* pSession);
	void	RemoveSession(CSession* pSession);
	void	RemoveSession(int nId);
	map<CSession*, CSession*>& GetSessions(){return m_mapId2Session;}
	//
	//CMessage*	FindRequestMessage(int nMessageId);
	//
	void		SetFactory(CFactory *pFactory){m_pFactory = pFactory;};
	CFactory*	GetFactory(){return  m_pFactory;}
	

	CSocketPumper *GetSocketPumper(){return m_pSocketPumper;};

	//virtual int OnServiceConnect(CSession*pSession, string& strServieName, int64 i64IdFrom, int64 i64IdTo);

//内部调用接口
	//virtual int		AppendService(CRpcService* pService);
	//virtual int		RemoveService(CRpcService* pService);
	//CRpcService*	GetService(string& strName, int64 i64IdTo);
	//virtual	int		OnSessionServiceLost(CSession* pSession, string& strName, int64 i64IdTo);
	//session 通信 超时。
	virtual int		OnNoMessageTimeout(CSession* pSession, int nTimes);
protected:
	//接收到的网络消息
	/*int			PushRecvMessage(CMessage* pMessage);
	CMessage*	PopRecvMessage();
	//发送的消息队列，用于等待同步消息返回
	int			AppendSendMessage(CMessage* pMessage);
	int			RemoveSendMessage(CMessage*pMessage);
	//
	static int __stdcall	ThreadMessageHandler(void *pParam);
	int			RunMessageHandler();*/
protected:

	CMessageHandler*	m_pMessageHandler;
	CMutex*				m_pMutexMessageHandler;

	map<CSession*, CSession*>	m_mapId2Session;
	list<CSession*>				m_lstSession;
	CMutex*				m_pMutexSessions;
	
	//map<int, CSession*>	m_mapId2Session;
	CMutex*				m_pMutexMapId;


	CSocket*			m_pSocketListen;
	CListenSocketHandler*	m_pListenHandler;
	//接收的消息队列。
	//queue<CMessage*>	m_queueRecvMessage;
	//CMutex*				m_pMutexRecvMessage;
	//
	//CThread*			m_pThreadMessageHandler;
	//发送的消息映射队列， 用于等待发送消息之后的回应，同步消息
	map<int, CMessage*>	m_mapId2SendMessage;
	CMutex*				m_pMutexMapSendMessage;
	//
	CFactory*			m_pFactory;
	//
	CMessageProcessor*	m_pProcessor;
	//
	CProtocol*			m_pProcotol;
	//CTransport
	CTransport*			m_pTransport;
	//CThreadPool

	CSocketPumper *		m_pSocketPumper;
	//所有的服务 和 dispatcher, 仅仅是查询用。不用于 dispatch
	//vector<CRpcService*>	m_lstService;
	//CRpcDispatcher*			m_pDispatcher;
	//CServiceContainer*		m_pServiceContainer;
	//根service， 全局服务，比如CreateObject, DestroyObject, Connect OnConnect
	//用于驱动 Dispatcher的 线程池。
	CThreadPool*		m_pThreadPoolProcessor;;
};
}
extern SCP_API int nScp;

SCP_API int fnScp(void);

#endif
