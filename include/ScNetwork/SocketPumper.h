#pragma once

#include "ScNetworkDefine.h"
#include "Socket.h"
#include <map>
#include <set>
#include <vector>

using namespace std;

namespace SimpleCpp{
	typedef enum _eSocketPumperType{
		eSocketPumperType_Select = 0,
		eSocketPumperType_IOCP,
		eSocketPumperType_EPOOL,
		eSocketPumperType_Unknown,
	}eSocketPumperType;
	class CAsyncSocket;
	class CSocket;
	
//各种 socket pumper
//: select, iocp, epoll 等，这里会选用 libevent
class SCNETWORK_API CSocketPumper
{
	//declare this friendly class in order to CSimpleSocket class can call variable m_pDefaultPumper
	friend class CScNetwork;
public:
	CSocketPumper(void);
	virtual ~CSocketPumper(void);
	//
	virtual int		Open();
	virtual int		Open(char *pcServerIp, int nServerPort, char *pcId = NULL);
	virtual int		Close();
	//
	virtual int		Append(CSocket *pSocket);
	virtual int		Remove(CSocket *pSocket);

	virtual void SetWorkingState(bool bWorking);
	virtual int SetSocketEventHandler(CSocketEventHandler *pEventHandler);

	virtual int		SocketCount();
	virtual int		GetMaxSocket(){return m_nMaxSocket;};
	virtual	void	SetMaxSocket(int nMax){m_nMaxSocket = nMax;}
	static CSocketPumper *GetDefault();
protected:
	set<CSocket*>		m_setSocket;
	vector<CSocket*>	m_vecAddSocket;
	set<CSocket*>		m_setDelSocket;
	int					m_nMaxSocket;
	static CSocketPumper*	m_pDefaultPumper;
};

}
