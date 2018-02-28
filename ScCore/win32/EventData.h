/*
* for windows
*/

#pragma once


namespace SimpleCpp
{
	class CEventData
	{
		friend class CEvent;
		public:
			CEventData(void);
			virtual ~CEventData(void);
		protected:
			HANDLE m_hEvent;
	};
}
