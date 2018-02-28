
#ifndef SocketEventHandler_H
#define SocketEventHandler_H

#include "ScNetworkDefine.h"

namespace SimpleCpp{

	class CSocket;
	class SCNETWORK_API CSocketEventHandler{
	public :
		virtual int OnAccept(CSocket *pSocket, int nErr){return 0;};
		virtual int OnConnect(CSocket *pSocket, int nErr){return 0;};
		virtual int OnRecv(CSocket *pSocket, int nErr){return 0;};
		virtual int OnSend(CSocket *pSocket, int nErr){return 0;};
		virtual int OnClose(CSocket *pSocket, int nErr){return 0;};
		virtual int OnTimer(CSocket *pSocket, int nErr){return 0;};
		virtual int OnException(CSocket *pSocket, int nErr) {return 0;};

		//virtual int OnTimeOut(CSocket *pSocket, int nErr, int nTimes){return 0;};
		virtual int OnNoSendTimeOut(CSocket *pSocket, int nErr, int nTimes){return 0;};
		virtual int OnNoRecvTimeOut(CSocket *pSocket, int nErr, int nTimes){return 0;};
		
		virtual int OnNoDataTimeOut(CSocket *pSocket, int nErr, int nTimes){return 0;};
	};


}
#endif