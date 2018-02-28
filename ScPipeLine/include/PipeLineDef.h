
#ifndef PIPELINE_H
#define PIPELINE_H
/*
* 管道流水线库，可以流一样处理数据，安装不同的filter
*/
#ifdef PIPELINE_EXPORTS
#define PIPELINE_API __declspec(dllexport)
#else
#define PIPELINE_API __declspec(dllimport)
#endif

#define debug(format, ...)	do{ \
	char szMsg[4096];\
	memset(szMsg, 0, sizeof(szMsg));\
	sprintf(szMsg, format, __VA_ARGS__);\
	OutputDebugStringA(szMsg);  \
}while (0);

namespace PipeLine{
// 	typedef long long int64;
// 	typedef long long int64_t;
}
#endif