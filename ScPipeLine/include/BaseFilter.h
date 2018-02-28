#pragma once

#include "PipeLineDef.h"
/*
#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>*/
#include <string>
#include <vector>
#include <map>

class CBuffer;

namespace SimpleCpp{
	class CThread;
}

namespace PipeLine{
	using namespace std;
	using namespace SimpleCpp;
	
	class CBasePipeLine;
	class CFilterPinBuffer;
	class CBaseFilter;
	class CFilterPin;
	//信号 序号
//typedef sigc::signal<int, CBaseFilter*, int, CFilterPinBuffer* >	SigPinOut;
	typedef enum _eFilterState{
		eFilterState_Invalid=-1,
		eFilterState_Open =0,
		eFilterState_Run ,
		eFilterState_ToStop,
		eFilterState_Stop,
		eFilterState_Pause,
		eFilterState_Unknown,
	}eFilterState;
// 	typedef long long int64;
// 	typedef long long int64_t;
#ifndef int64_t
#define  int64_t long long 
#endif

	typedef void (__stdcall *FilterCB_Progress)(float fSpeed, int64_t s64CurPos,  int64_t s64Total, void *pUserData, void *pReversed);
	typedef void (__stdcall *FilterCB_Finished)(void *pUserData, void *pReversed);
	typedef void (__stdcall *FilterCB_Event)(void *pEvent, int nEventLen, void *pUserData, void *pReversed);


	class PIPELINE_API CBaseFilter
{
public:
	CBaseFilter(CBasePipeLine* pStream =NULL);
	virtual ~CBaseFilter(void);

	virtual int Attach(CBasePipeLine* pStream);
	//
	virtual int		Open(char* pParam, bool bHasThread=false);

	virtual int		Close();
	void			CreateFilterThread( bool bHasThread );
	bool			HasFilterThread() const;

	//
	virtual int		Start();
	virtual int		Pause();
	virtual int		Resume();
	virtual int		Stop();
	//
	eFilterState GetState(){return m_state;}
	virtual bool IsKind(char *pSzKind);
	virtual char *GetType();
	virtual char *GetName();
	virtual char *GetKind();
	//
	CFilterPin*		GetPinOut(int nIndex);
	CFilterPin*		GetPinIn(int nIndex);
	//					前一个pinOut, 当前的pinIn, buffer;
	//每一份数据都会被调用一次
	virtual int  OnPinIn(CFilterPin* pPrevPinOut, CFilterPin* pPinIn, CFilterPinBuffer *pPinBuffer);
	//
	//工作函数。每个filter都会有这个函数执行的机会。
	// 不管是 有独立线程还是没有独立线程。都是最终调用这个函数获得执行机会
	virtual int DoFilter();
	//
	//filter 独立线程调用的函数。
	virtual int Run();
	//
	const char*	GetParam(char *pName);

	virtual CBaseFilter* CreateObject();
	virtual void DeleteObject(CBaseFilter* pFilter);
	//
	static void ParseParam(char *pParam, int nLen, map<string, string> &mapParamValue);
	static void ParseParamCmd(char *pParam, char *pName, char *pVal);

	//
	int		SetEventCb(FilterCB_Event cbEvent, void *pUserData, void *pReversed);
	int		SetProgressCb(FilterCB_Progress cbProgress, void *pUserData, void *pReversed);
	int		SetFinishedCb(FilterCB_Finished cbFinished, void *pUserData, void *pReversed);

protected:
	//
	static int __stdcall ThreadProc(void *pData);
	//
	string  m_strName;
	string	m_strKind;
	string	m_strType;
	//型号名字， 就是类的名字
	char	m_szType[32];
	//分类 source  decoder encoder render等
	char	m_szKind[32];
	//具体的对象名字 用户可以设定 ，
	char	m_szName[32];

	CBasePipeLine*	m_pStream;
	//工作线程。一般在source 里面使用。其他filter 可以不理会。
	CThread*	m_pThread;
	bool		m_bHasFilterThread;
	
	//void		HasFilterThread(bool val) { m_bHasFilterThread = val; }
	//状态
	eFilterState	m_state;
	int				m_nSleepTime;
	//
	vector<CFilterPin *> m_lstPinIn;
	vector<CFilterPin *> m_lstPinOut;
	//
	string  m_strParam;
	// cmd line 解析的 名字和函数 对应
	map<string, string> m_mapParamValue;
	//输出的平时缓存

	//回调函数
	FilterCB_Progress	m_cbProgress;
	void *m_pCbProgressUserData;
	void *m_pCbProgressReversed;
	FilterCB_Finished	m_cbFinished;

	void *m_pCbFinishedUserData;
	void *m_pCbFinishedReversed;
	FilterCB_Event		m_cbEvent;
	void *m_pCbEventUserData;
	void *m_pCbEventReversed;
};
#ifdef int64_t
#undef  int64_t
#endif

}
