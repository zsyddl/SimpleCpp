
#ifndef ScNetworkDefine_h
#define ScNetworkDefine_h


#ifdef SIMPLE_NETWORK_EXPORTS
#define SCNETWORK_API __declspec(dllexport)
#else
#define SCNETWORK_API __declspec(dllimport)
#endif

#ifdef OS_LINUX
#define SCNETWORK_API
#endif

#ifdef WIN32

#ifndef  FD_SETSIZE
#undef FD_SETSIZE
#define FD_SETSIZE 10000
#endif

#endif


typedef int HSOCKET;
typedef enum _eSocketPumperType
{
	eSocketPumperType_Select = 1,
	eSocketPumperType_MultiSelect,
	eSocketPumperType_Iocp,
}eSocketPumperType;

#define BUF_LEN_32 32
#define BUF_LEN_64 64
#define BUF_LEN_128 128
#define BUF_LEN_256 256
#define BUF_LEN_512 512
#define BUF_LEN_1024 1024



#endif