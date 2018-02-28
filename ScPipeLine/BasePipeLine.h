#pragma once
#include <vector>
#include "PipeLineDef.h"
#include <map>
#include <string>
/*
*数据流框架
*/
namespace SimpleCpp{
	class CThread;
	class CMutex;
}
using namespace SimpleCpp;
namespace SimpleCpp{
	class CBaseFilter;

	typedef enum _ePipeLineState{
		ePipeLineState_None=0,
		ePipeLineState_Run,
		ePipeLineState_Pause,
		ePipeLineState_ToStop,
		ePipeLineState_Stop,
		ePipeLineState_Close,
	}ePipeLineState; 

	using namespace std;
class PIPELINE_API CBasePipeLine
{
public:
	CBasePipeLine(void);
	virtual ~CBasePipeLine(void);

	virtual int Open(string &strParam=string(""));
	virtual int Close();

	virtual int Start();
	virtual int Stop();
	virtual int Pause();
	virtual int Resume();

	virtual int AppendFilter(CBaseFilter* pFilter);
	virtual CBaseFilter*	QueryFilterByName(string &strName);
	virtual CBaseFilter*	QueryFilterByType(string &strType);
	CBaseFilter*	GetFirstFilter();
	CBaseFilter*	GetLastFilter();
	//有一个线程。用于所有 filter 获得 执行 runFilter的机会
	virtual int		Run();

	int GetSleepTime() const { return m_nSleepTime; }
	void SetSleepTime(int val) { m_nSleepTime = val; }
protected:
	static int __stdcall ThreadProc(void *pData);
	CThread         *m_pThrStream;
	int				m_nSleepTime;

	ePipeLineState			m_state;
	vector<CBaseFilter*>	m_lstFilter;
	CMutex			*m_pMutexFilters;

	map<string, string> m_mapParam;
};

//typedef CBaseStream CBasePipeLine;

}
