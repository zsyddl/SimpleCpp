#pragma once

#ifndef EVENT_H
#define EVENT_H
#ifdef OS_LINUX
#define SCCORE_API
#endif
#include "ScCoreDefine.h"


namespace SimpleCpp
{
	class CEventData;
	class SCCORE_API CEvent
	{
		public:
			CEvent();
			virtual ~CEvent();

			/*创建一个匿名事件对象bAutoReset  true   人工重置false  自动重置*/
			int Create();

			/*将当前事件对象设置为有信号状态若自动重置，则等待该事件对象的所有线程只有一个可被调度
				若人工重置，则等待该事件对象的所有线程变为可被调度*/
			int Set();

			/*以当前事件对象，阻塞线程，将其永远挂起直到事件对象被设置为有信号状态*/
			bool Wait();

			/*以当前事件对象，阻塞线程，将其挂起指定时间间隔之后线程自动恢复可调度*/
			bool Wait(long lMilliseconds);

			/*将当前事件对象设置为无信号状态*/
			void Reset();
            void Destroy();
    //        void Close();

		protected:
			CEventData *m_pEventData;
	};
}

#endif
