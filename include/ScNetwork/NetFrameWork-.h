

#ifndef NetFrameWork_H
#define NetFrameWork_H
  
#include "NFWConfig.h"

#ifdef WIN32
#ifndef  FD_SETSIZE
#undef FD_SETSIZE
#define FD_SETSIZE 10000
#endif
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define debug(format, ...)	do{ \
	char szMsg[255];\
	memset(szMsg, 0, sizeof(szMsg));\
	sprintf(szMsg, format, __VA_ARGS__);\
	OutputDebugStringA(szMsg);  \
}while (0);

namespace NFW{
	class CInitNFW
	{
	public:
		CInitNFW(){

			WSADATA wsaData;
			int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
			if (iResult != NO_ERROR)
			{
				//printf("Error at WSAStartup()\n");
			}

		};
		~CInitNFW()
		{
			WSACleanup();
		};
	};
	static CInitNFW m_initNFW;
}

#else

#define NFW_API

#endif

typedef int HSOCKET;






#endif