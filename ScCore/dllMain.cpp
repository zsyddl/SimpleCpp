// SThread.cpp : 定义 DLL 应用程序的入口点。
//
   
#include "stdafx.h"
#include "dllMain.h"
bool APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// 这是导出变量的一个示例
SCCORE_API int nSThread=0;

// 这是导出函数的一个示例。
SCCORE_API int fnSThread(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 SThread.h
CSThreadLib::CSThreadLib()
{ 
	return; 
}
