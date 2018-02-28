#pragma once

#ifndef SEMAPHOREDATA_H
#define SEMAPHOREDATA_H
#include <semaphore.h>

typedef sem_t HANDLE;
#define NULL 0

namespace SThread
{
	class CSemaphoreData
	{
		friend class CSemaphore;
		public:
			CSemaphoreData();
			virtual ~CSemaphoreData();
	
		protected:
			HANDLE m_hSemaphore;
	};
}

#endif