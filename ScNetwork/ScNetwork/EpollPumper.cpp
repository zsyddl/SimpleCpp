#ifdef OS_LINUX

#include "Thread.h"
#include "mutex.h"
#include "EpollPumper.h"
#include "AsyncSocket.h"
#include "ThreadTimer.h"

using namespace SimpleCpp;

namespace SimpleCpp
{

CEpollPumper::CEpollPumper()
:CSocketPumper()
,m_pRateCfg(NULL)
,m_pEventBase(NULL)
,m_pSocketEventHandler(NULL)
,m_pThreadServer(NULL)
,m_pThreadClient(NULL)
,m_pServerMutex(NULL)
,m_pClientMutex(NULL)
,m_pMutexAddSockets(NULL)
,m_pMutexDelSockets(NULL)
,m_pcIp(NULL)
,m_nPort(0)
{
    m_pMutexAddSockets = new SimpleCpp::CMutex;
    m_pMutexDelSockets = new SimpleCpp::CMutex;
    m_pServerMutex = new SimpleCpp::CMutex;
    m_pClientMutex = new SimpleCpp::CMutex;

    m_pThreadTimer = new CThreadTimer;
    m_pThreadTimer->Open();
}

CEpollPumper::~CEpollPumper()
{

}

int CEpollPumper::Init()
{
    int nRet = 0;

    m_pCfg = event_config_new();
    if (NULL == m_pCfg)
    {
        return -1;
    }

    nRet = event_config_avoid_method(m_pCfg, "select");
    nRet |= event_config_set_flag(m_pCfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    if (0 != nRet)
    {
        return -1;
    }

    nRet = event_config_set_num_cpus_hint(m_pCfg, 32);
    if (0 != nRet)
    {
        return -1;
    }

    size_t rate = 2 * 1024;
    size_t burst = 2 * 1024;
    struct timeval sTick;
    sTick.tv_sec = 1;
    sTick.tv_usec = 0;

    m_pRateCfg = ev_token_bucket_cfg_new(rate, burst, rate, burst, &sTick);
    if (NULL == m_pRateCfg)
    {
        return -1;
    }

    m_pEventBase = event_base_new_with_config(m_pCfg);
    if (NULL == m_pEventBase)
    {
        return -1;
    }

    return 0;
}

void CEpollPumper::OnRecv(struct bufferevent *pBufEvent, void *pObj)
{
    int nRet = 0;
    pTimeroutInfo pInfo = (pTimeroutInfo)pObj;
    CEpollPumper *pEpollPumper = (CEpollPumper *)(pInfo->pObj);

    short nFlag = EV_READ | EV_WRITE;
    bufferevent_disable(pBufEvent, nFlag);

    if (NULL != pEpollPumper->m_pSocketEventHandler)
    {
        pEpollPumper->m_pSocketEventHandler->OnRecv((CSocket *)(pBufEvent), 0);
    }

    bufferevent_enable(pBufEvent, nFlag);
}

void CEpollPumper::OnSend(struct bufferevent *pBufEvent, void *pObj)
{
    int nRet = 0;
    pTimeroutInfo pInfo = (pTimeroutInfo)pObj;
    CEpollPumper *pEpollPumper = (CEpollPumper *)(pInfo->pObj);

    short nFlag = EV_READ | EV_WRITE;
    bufferevent_disable(pBufEvent, nFlag);

    if (NULL != pEpollPumper->m_pSocketEventHandler)
    {
        pEpollPumper->m_pSocketEventHandler->OnSend((CSocket *)pBufEvent, 0);
    }

    bufferevent_enable(pBufEvent, nFlag);
}

void CEpollPumper::OnEvent(struct bufferevent *pBufEvent, short nFlag, void *pObj)
{
    int nEvent = EV_WRITE;
    pTimeroutInfo pInfo = (pTimeroutInfo)pObj;
    CEpollPumper *pEpollPumper = (CEpollPumper *)(pInfo->pObj);

    if (nFlag & BEV_EVENT_CONNECTED)
    {
        pInfo->enEventState = Bufferevent_Connected;
        return;
    }
    else if (nFlag & BEV_EVENT_ERROR)
    {
        int nError = 0;

        printf("CEpollPumper::OnEvent socket error.\r\n");
        nError = bufferevent_socket_get_dns_error(pBufEvent);
        if (nError)
        {
            bufferevent_disable(pBufEvent, nFlag);
            pInfo->enEventState = Bufferevent_Error;
            bufferevent_enable(pBufEvent, nFlag);
        }
    }
    else if (nFlag & BEV_EVENT_TIMEOUT)
    {
        printf("CIocpPumperEx::OnEvent no send data during specified time.\r\n");

        bufferevent_disable(pBufEvent, nEvent);
        pInfo->enEventState = Bufferevent_Error;
        bufferevent_free(pBufEvent);
    }
    else if (nFlag & BEV_EVENT_EOF)
    {
        printf("CIocpPumperEx::OnEvent BEV_EVENT_EOF.\r\n");

        bufferevent_disable(pBufEvent, nEvent);
        pInfo->enEventState = Bufferevent_Error;
        bufferevent_free(pBufEvent);
    }

    return;
}

void CEpollPumper::OnAccept(struct evconnlistener *base, evutil_socket_t clientSocket, struct sockaddr *addr, int socklen, void *pObj)
{
    int nFlag = 0;
    event_base *evBase = NULL;
    struct bufferevent *pBufEvent = NULL;
    CEpollPumper *pEpollPumper = NULL;

    pTimeroutInfo pInfo = (pTimeroutInfo)pObj;
    pEpollPumper = (CEpollPumper *)(pInfo->pObj);
    evBase = pEpollPumper->m_pEventBase;
    pBufEvent = bufferevent_socket_new(evBase, clientSocket, BEV_OPT_CLOSE_ON_FREE);

    nFlag = EV_READ | EV_WRITE;
    bufferevent_setcb(pBufEvent, OnRecv, OnSend, NULL, pObj);
    bufferevent_enable(pBufEvent, nFlag);
}

void CEpollPumper::ServerLoop()
{
    int nRet = 0;
    struct sockaddr_in addr;

    nRet = Init();
    if (0 != nRet)
    {
        printf("CEpollPumper::ServerLoop init failed.\r\n");
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(m_nPort);

    pTimeroutInfo pInfo = new sTimeroutInfo;
    pInfo->pObj = this;
    pInfo->pBufEvent = NULL;
    pInfo->enEventState = Bufferevent_Created;
    int nFlag = LEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE | LEV_OPT_REUSEABLE;
    m_pListener = evconnlistener_new_bind(m_pEventBase, OnAccept, pInfo, nFlag, 200, (struct sockaddr *)(&addr), sizeof(addr));
    while (1)
    {
        m_pServerMutex->Lock();
        if (Thread_SetExit == m_nThrServerRunState)
        {
            m_pServerMutex->Unlock();
            break;
        }
        m_pServerMutex->Unlock();

        nRet = event_base_dispatch(m_pEventBase);
        if (0 > nRet)
        {
            printf("CEpollPumper::ServerLoop jump event base dispatch.\r\n");
        }

        usleep(1000);
    }

    printf("CEpollPumper::ServerLoop exit loop.\r\n");

    m_pServerMutex->Lock();
    m_nThrServerRunState = Thread_IsExit;
    m_pServerMutex->Unlock();

    if (NULL != pInfo)
    {
        delete pInfo;
    }
}

int CEpollPumper::OnTimerOutProc(int nId, void *pObj)
{
    int nRet = 0;
    struct bufferevent *pBufEvent = NULL;
    CEpollPumper *pEpollPumper = NULL;
    CThreadTimer *pThreadTimer = NULL;
    pTimeroutInfo pInfo = NULL;

    pInfo = (pTimeroutInfo)pObj;
    pEpollPumper = (CEpollPumper *)pInfo->pObj;
    pBufEvent = pInfo->pBufEvent;

    if (NULL != pEpollPumper->m_pSocketEventHandler)
    {
        pEpollPumper->m_pSocketEventHandler->OnTimer((CSocket *)pBufEvent, 0);
    }

    return 0;
}

void CEpollPumper::ClientLoop()
{
    int nRet = 0;
    int nFlag = 0;
    struct sockaddr_in addr;
    CSocket *pAsyncSocket = NULL;
    struct bufferevent *pBufEvent = NULL;

    nRet = Init();
    if (0 != nRet)
    {
        printf("CEpollPumper::ClientLoop init failed.\r\n");
        return;
    }

    evthread_make_base_notifiable(m_pEventBase);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, m_pcIp, &(addr.sin_addr.s_addr));
    addr.sin_port = htons(m_nPort);

    while (1)
    {
        m_pMutexAddSockets->Lock();
        for (vector<CSocket *>::iterator iter = m_vecAddSocket.begin(); iter != m_vecAddSocket.end(); iter++)
        {
            pAsyncSocket = *iter;
            m_setSocket.insert(pAsyncSocket);
            int fd = pAsyncSocket->GetSocketFd();
            nFlag = BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS;
            pBufEvent = bufferevent_socket_new(m_pEventBase, fd, nFlag);
            if (NULL == pBufEvent)
            {
                printf("CEpollPumper::ClientLoop bufferevent_socket_new failed.\r\\n");
                continue;
            }

            nRet = bufferevent_set_rate_limit(pBufEvent, m_pRateCfg);
            if (0 != nRet)
            {
                printf("CEpollPumper::ClientLoop set rate failed.\r\n");
            }

            nFlag = EV_WRITE;
            bufferevent_enable(pBufEvent, nFlag);

            pTimeroutInfo pInfo = new sTimeroutInfo;
            pInfo->pObj = this;
            pInfo->pBufEvent = pBufEvent;
            pInfo->enEventState = Bufferevent_Created;
            m_pThreadTimer->AddTimer(fd, true, 1, OnTimerOutProc, pInfo);
            m_setTimerInfo.insert(pInfo);

            bufferevent_setcb(pBufEvent, OnRecv, OnSend, OnEvent, pInfo);
            nRet = bufferevent_socket_connect(pBufEvent, (struct sockaddr *)&addr, sizeof(addr));
            if (0 != nRet)
            {
                printf("CEpollPumper::ClientLoop connect failed.\r\n");
                continue;
            }

            usleep(10);
        }

        int nCount = m_vecAddSocket.size();
        printf("This epoll pumper has %d sockets.\r\n", nCount);

        m_pThreadTimer->StartAllTimer();
        m_vecAddSocket.clear();
        m_pMutexAddSockets->Unlock();

        while (1)
        {
            nRet = event_base_dispatch(m_pEventBase);
            if (0 > nRet)
            {
                break;
            }

            usleep(1000);
        }
    }
}

void *CEpollPumper::ThreadServerProc(void *pObj)
{
    CEpollPumper *pEpollPumper = NULL;

    pEpollPumper = (CEpollPumper *)pObj;
    pEpollPumper->ServerLoop();

    return NULL;
}

void *CEpollPumper::ThreadClientProc(void *pObj)
{
    CEpollPumper *pEpollPumper = NULL;

    pEpollPumper = (CEpollPumper *)pObj;
    pEpollPumper->ClientLoop();

    return NULL;
}

int CEpollPumper::ProcessServer(char *pcIp, int nPort)
{
    int nRet = -1;

    if (NULL != m_pThreadServer)
    {
        return -1;
    }

    m_pThreadServer = new CThread;
    if (NULL == m_pThreadServer)
    {
        printf("CEpollPumper::Open new thread failed.\r\n");
        return -1;
    }

    nRet = m_pThreadServer->Create(ThreadServerProc, this);
    if (0 != nRet)
    {
        printf("CEpollPumper::Open open thread failed.\r\n");
        return -1;
    }

    return 0;
}

int CEpollPumper::ProcessClient(char *pcIp, int nPort)
{
    int nRet = -1;

    if (NULL != m_pThreadClient)
    {
        return -1;
    }

    m_pThreadClient = new CThread;
    if (NULL == m_pThreadClient)
    {
        printf("CEpollPumper::Open new thread failed.\r\n");
        return -1;
    }

    nRet = m_pThreadClient->Create(ThreadClientProc, this);
    if (0 != nRet)
    {
        printf("CEpollPumper::Open open thread failed.\r\n");
        return -1;
    }

    return 0;
}

int CEpollPumper::Open(char *pcIp, int nPort, char *pcId)
{
    const char *pcServer = "server";
    const char *pcClient = "client";
    int nRet = -1;

    m_pcIp = pcIp;
    m_nPort = nPort;

    if (strcmp(pcId, pcServer) == 0)
    {
        nRet = ProcessServer(pcIp, nPort);
        if (0 != nRet)
        {
            printf("CEpollPumper::Open server failed.\r\n");
        }

        return nRet;
    }
    else if (strcmp(pcId, pcClient) == 0)
    {
        nRet = ProcessClient(pcIp, nPort);
        if (0 != nRet)
        {
            printf("CEpollPumper::Open server failed.\r\n");
        }

        return nRet;
    }
    else
    {
        printf("CEpollPumper::Open input parameter error.\r\n");
        return -1;
    }
}

int CEpollPumper::Append(CSocket *pSocket)
{
    m_pMutexAddSockets->Lock();
    m_vecAddSocket.push_back(pSocket);
    m_pMutexAddSockets->Unlock();
}

int CEpollPumper::Remove(CSocket *pSocket)
{
    m_pMutexAddSockets->Lock();
    m_vecDelSocket.push_back(pSocket);
    m_pMutexAddSockets->Unlock();
}

int CEpollPumper::Close()
{
    m_pServerMutex->Lock();
    m_nThrServerRunState = Thread_SetExit;
    m_pServerMutex->Unlock();

    while (1)
    {
        m_pServerMutex->Lock();
        if (Thread_IsExit == m_nThrServerRunState)
        {
            m_pServerMutex->Unlock();
            break;
        }
        m_pServerMutex->Unlock();
        usleep(2000);
    }

    if (NULL != m_pThreadServer)
    {
        m_pThreadServer->Close();
        delete m_pThreadServer;
        m_pThreadServer = NULL;
    }

    m_pClientMutex->Lock();
    m_nThrClientRunState = Thread_SetExit;
    m_pClientMutex->Unlock();

    while (1)
    {
        m_pClientMutex->Lock();
        if (Thread_IsExit == m_nThrClientRunState)
        {
            m_pClientMutex->Unlock();
            break;
        }
        m_pClientMutex->Unlock();
        usleep(2000);
    }

    if (NULL != m_pThreadClient)
    {
        m_pThreadClient->Close();
        delete m_pThreadClient;
        m_pThreadClient = NULL;
    }

    if (NULL != m_pServerMutex)
    {
        delete m_pServerMutex;
        m_pServerMutex = NULL;
    }

    if (NULL != m_pClientMutex)
    {
        delete m_pClientMutex;
        m_pClientMutex = NULL;
    }

    if (NULL != m_pListener)
    {
        evconnlistener_free(m_pListener);
        m_pListener = NULL;
    }

    if (NULL != m_pCfg)
    {
        event_config_free(m_pCfg);
        m_pCfg = NULL;
    }

    if (NULL != m_pRateCfg)
    {
        ev_token_bucket_cfg_free(m_pRateCfg);
        m_pRateCfg = NULL;
    }

    if (NULL != m_pEventBase)
    {
        event_base_free(m_pEventBase);
        m_pEventBase = NULL;
    }

    if (NULL != m_pThreadTimer)
    {
        m_pThreadTimer->Close();
        delete m_pThreadTimer;
    }
}

int CEpollPumper::SetSocketEventHandler(CSocketEventHandler *pSocketEventHandler)
{
    m_pSocketEventHandler = pSocketEventHandler;
}

}

#endif
