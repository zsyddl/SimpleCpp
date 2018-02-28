========================================================================
    控制台应用程序：test 项目概述
========================================================================
-o
flex -+ -Pxx frame.l
%option c++

$(ProjectDir)bison -d frame.y
$(ProjectDir)flex -+ -Pxx frame.l
//
D:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include
复制 iosream文件为 iostream.h
创建  unistd.h头文件
#ifndef _UNISTD_H
#define _UNISTD_H
#include <io.h>
#include <process.h>
#endif /* _UNISTD_H */
//根据不同的 flex bison 版本。生成的头文件名称有区别
// y有的是 .hpp 有的是 .h 有的是 .c.h
/////////////////////////////////////////////////////////////////////////////