// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SIMPLESOCKET_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SCNETWORK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef SIMPLE_SOCKET_H
#define SIMPLE_SOCKET_H
#ifdef SIMPLESOCKET_EXPORTS
#define SCNETWORK_API __declspec(dllexport)
#else
#define SCNETWORK_API __declspec(dllimport)
#endif

#ifdef OS_LINUX
#define SCNETWORK_API
#endif

// 此类是从 SimpleSocket.dll 导出的
// class SCNETWORK_API CSimpleSocket {
// public:
// 	CSimpleSocket(void);
// 	// TODO: 在此添加您的方法。
// };

extern SCNETWORK_API int nSimpleSocket;

SCNETWORK_API int fnSimpleSocket(void);

typedef enum SCNETWORK_API _eCommunicationStyle
{
	eCommunicationStyle_Select = 1,
	eCommunicationStyle_MultiSelect,
	eCommunicationStyle_Iocp
}eCommunicationStyle;

#define BUF_LEN_32 32
#define BUF_LEN_64 64
#define BUF_LEN_128 128
#define BUF_LEN_256 256
#define BUF_LEN_512 512
#define BUF_LEN_1024 1024

namespace SimpleCpp{

	class CScNetwork
	{
		CScNetwork();
		~CScNetwork();

		static int Init();
		static int Free();
		static CScNetwork m_instance;
	};


}

#endif
