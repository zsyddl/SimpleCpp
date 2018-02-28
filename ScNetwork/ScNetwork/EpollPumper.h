#ifdef OS_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

#include "Socket.h"
#include "ScNetworkDefine.h"
#include "SocketPumper.h"
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/thread.h"
#include "event2/util.h"

using namespace std;
namespace SimpleCpp
{
    class CThread;
    class CMutex;
};

class CThreadTimer;

#define MAX_EPOLLMONITOR_SIZE 10000

typedef enum _sThreadRunningState
{
    Thread_InitState = 0,
    Thread_SetExit,
    Thread_IsExit
}sThreadRunningState;

typedef enum _eBuffereventState
{
    Bufferevent_Created = 0,
    Bufferevent_Connected,
    Bufferevent_Error
}eBuffereventState;

typedef struct _sTimeroutInfo
{
    void *pObj;
    bufferevent *pBufEvent;
    time_t nBeginTime;
    time_t nCurTime;
    eBuffereventState enEventState;
}sTimeroutInfo, *pTimeroutInfo;

namespace SimpleCpp
{

class CEpollPumper:public CSocketPumper
{
    public:
        CEpollPumper();
        virtual ~CEpollPumper();

        virtual int Open(char *pcIp, int nPort, char *pcId);
        virtual int Close();

        virtual int Append(CSocket *pSocket);
        virtual int Remove(CSocket *pSocket);

        int SetSocketEventHandler(CSocketEventHandler *pSocketEventHandler);

        static void OnAccept(struct evconnlistener *base, evutil_socket_t clientSocket, struct sockaddr *addr, int socklen, void *pObj);
        static void OnRecv(struct bufferevent *pBufEvent, void *pObj);
        static void OnSend(struct bufferevent *pBufEvent, void *pObj);
        static void OnEvent(struct bufferevent *pBufEvent, short nFlag, void *pObj);

    private:
        int Init();
        int ProcessServer(char *pcIp, int nPort);
        int ProcessClient(char *pcIp, int nPort);
        static void *ThreadServerProc(void *pObj);
        static void *ThreadClientProc(void *pObj);
        void ServerLoop();
        void ClientLoop();

        static int OnTimerOutProc(int nId, void *pObj);

        char *m_pcIp;
        int m_nPort;

        struct event_base *m_pEventBase;
        struct event_config *m_pCfg;
        ev_token_bucket_cfg *m_pRateCfg;
        evconnlistener *m_pListener;

        CSocketEventHandler *m_pSocketEventHandler;

        SimpleCpp::CThread *m_pThreadServer;
        sThreadRunningState m_nThrServerRunState;
        SimpleCpp::CMutex *m_pServerMutex;
        SimpleCpp::CThread *m_pThreadClient;
        sThreadRunningState m_nThrClientRunState;
        SimpleCpp::CMutex *m_pClientMutex;

        SimpleCpp::CMutex *m_pMutexAddSockets;
        SimpleCpp::CMutex *m_pMutexDelSockets;

        CThreadTimer *m_pThreadTimer;
        set<pTimeroutInfo> m_setTimerInfo;
};

}

#endif
