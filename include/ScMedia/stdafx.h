// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#ifdef WIN32

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>


#define debug(format, ...)	do{ \
	char szMsg[255];\
	memset(szMsg, 0, sizeof(szMsg));\
	sprintf(szMsg, format, __VA_ARGS__);\
	OutputDebugStringA(szMsg);  \
}while (0);


// TODO: 在此处引用程序需要的其他头文件
#ifdef MEDIASDK_EXPORTS
#define MEDIASDK_API __declspec(dllexport)
#else
#define MEDIASDK_API __declspec(dllimport)
#endif

#endif //WIN32

#endif
