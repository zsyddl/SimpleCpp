// SimpleSocket.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ScNetworkDefine.h"
#include "SocketPumper.h"
#include "SelectPumper.h"

//when initialize this variable, call construct function SimpleSocket::CSimpleSocket::CSimpleSocket()
SimpleCpp::CScNetwork SimpleCpp::CScNetwork::m_instance;