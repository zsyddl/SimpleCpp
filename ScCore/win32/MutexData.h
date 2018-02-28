/*
* for windows
*/

#pragma once


namespace SimpleCpp{
class CMutexData
{
	friend class CMutex;
public:
	CMutexData(void);
	virtual ~CMutexData(void);
protected:
	CRITICAL_SECTION  m_hHandle;
};

}
