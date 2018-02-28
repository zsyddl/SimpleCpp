#pragma once
#ifndef NEWQUEUE_H
#define NEWQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "ScNetworkDefine.h"

using namespace std;

namespace SimpleCpp
{
	class CMutex;
};

namespace SimpleCpp{

class SCNETWORK_API CQueue
{
	public:
		CQueue();
		virtual ~CQueue();

		int PushToQueue(void *);
		void PopFromQueue();
		void *GetFromQueue();
		int QueueSize();
		bool QueueIsEmpty();

	private:
		SimpleCpp::CMutex*	m_pQueueMutex;
		queue<void *> m_Queue;
};

}

#endif