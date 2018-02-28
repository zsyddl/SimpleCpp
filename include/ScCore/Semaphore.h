#pragma once

#ifndef SEMAPHOREENCAP_H
#define SEMAPHOREENCAP_H

#ifdef WIN32
#include "Windows.h"
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "Kernel32.lib")

#ifdef STHREAD_EXPORTS
#define SCCORE_API __declspec(dllexport)
#else
#define SCCORE_API __declspec(dllimport)
#endif

#else
#include <semaphore.h>
#include <time.h>
#include <errno.h>

#define SCCORE_API

#endif

namespace SimpleCpp
{
	class CSemaphoreData;
	class SCCORE_API CSemaphore
	{
		public:
			CSemaphore();
			virtual ~CSemaphore();

			//lValue:windows下此值表示信号量初始化最大值，linux下此值表示是否在进程间（0）或者一个进程的线程间共享（非0）
			long Init(long lInitCount, long lValue = 10000);
			bool Wait();
			bool TryWait(long lTimeOut);
			long Post();

			long Destory();

		protected:
			CSemaphoreData *m_pSemaphoreData;
	};
}

#endif