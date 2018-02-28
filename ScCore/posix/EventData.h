/*
* for windows
*/

#pragma once

#include <pthread.h>
typedef struct  
{
	bool state;
	bool manual_reset;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}event_t;

#define event_handle event_t*


namespace SThread
{
	class CEventData
	{
		friend class CEvent;
		public:
			CEventData(void);
			virtual ~CEventData(void);
		protected:
			event_handle m_hEvent;
	};
}
