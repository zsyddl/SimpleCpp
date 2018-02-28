
/*
* for windows 32
*
*/
#pragma once

namespace SimpleCpp{
 
class CThreadData
{
	friend class CThread;
public:
	CThreadData(void);
	virtual ~CThreadData(void);

protected:

	HANDLE			m_hThread;
	DWORD			m_dwThreadID;

};
}