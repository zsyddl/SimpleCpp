// stdafx.cpp : 只包括标准包含文件的源文件
// VivsMediaBox.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "StdAfx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#ifndef WIN32
#include <time.h>
long timeGetTime()
{
    unsigned int uptime = 0;
    struct timespec on;
    if (clock_gettime(CLOCK_MONOTONIC, &on)==0)
    {
        uptime = on.tv_sec*1000 + on.tv_nsec/1000000;
    }

    return  uptime;
};
#endif
