#pragma once
#include <vector>
#include "SocketPumper.h"
#include "ScNetworkDefine.h"

/*
*自动多线程平衡的 select pumper
*/
using namespace std;

namespace SimpleCpp{

#define SOCKET_PER_SELECT 625

class CSelectPumper;

class SCNETWORK_API CMultiSelectPumper :
	public CSocketPumper
{
public:
	CMultiSelectPumper(void);
	virtual ~CMultiSelectPumper(void);
	//
	virtual int Open();
	virtual int CreatePumper(int nCount = 5);
	virtual int DestroyPumper();

	virtual int Append(CAsyncSocket *pSocket);
	virtual int Remove(CAsyncSocket *pSocket);

protected:
	CSelectPumper*	GetFreePumper();
protected:
	//大约 一个线程 500 个 socket
	CSelectPumper *m_pSelectedPumper;
	vector<CSelectPumper *> m_vecSelectPumper;
};

}