
#ifndef SessionSocket_H
#define SessionSocket_H

#include "ScpDefine.h"
#include "AsyncSocket.h"
class CBuffer;

using namespace SimpleSocket;
namespace Scp{
	class CSession;
	class CSimpleMessage;

class SCP_API CSessionSocket: public CAsyncSocket
{
public:
	CSessionSocket(CSession* pSession);
	~CSessionSocket();

	int			OnRecv(int nErr);
protected:
	CSession*	m_pSession;
	//
	CSimpleMessage*	m_pMessageTmp;
	//接收数据的缓存。
	CBuffer*	m_pRecvBuffer;
};
}
#endif