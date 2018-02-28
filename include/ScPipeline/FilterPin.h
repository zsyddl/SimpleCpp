
#ifndef FILTER_PIN_H
#define FILTER_PIN_H

#include <string>
#include "PipeLineDef.h"
#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>
#include <string>
#include <vector>

namespace SimpleCpp{
	class CFilterPinBuffer;
}
namespace SimpleCpp
{
	class CThread;
	class CMutex;
}
using namespace SimpleCpp;
using namespace std;
using namespace SimpleCpp;

typedef enum _eFilterPinIOType{
	eFPT_In = 0,
	eFPT_Out,
}eFilterPinIOType;
//运行状态
/*
typedef enum _eFilterState{
	eFilterState_Run = 0,
	eFilterState_ToStop,
	eFilterState_Stop,
	eFilterState_Pause,
}eFilterState;*/
//播放模式
typedef enum _eFilterPlayMode{
	eFPM_None=0,
	eFPM_Play,
	eFPM_PlayBackward,
	eFPM_PlayNextFrame,
	eFPM_PlayPrevFrame ,
	
}eFilterPlayMode;

 

typedef enum _eFilterPinName{
	eFPN_Video = 0,
	eFPN_Audio,
	
}eFilterPinName;

namespace SimpleCpp{
//分为 out pin 和in pin 
class PIPELINE_API CFilterPin{
 
	friend class CBaseFilter;
public:
	CFilterPin(CBaseFilter *pFilter);
	~CFilterPin();

	int Init(eFilterPinIOType ioType, int nBufferCount);
	int SetAutoFreeMemory(int nReversedMemorySize=0);
	//
	//
	vector<CFilterPin*> &GetNextPins(){return m_lstNextPinIn;};
	int GetConnectedPinCount();
	//外部调用的 filter 之间调用 
	int GetBuffer(CFilterPinBuffer **pBuffer);
	int PutBuffer(CFilterPinBuffer *pBuffer);
	int GetPreparedBufferConut(){return m_lstBusyBuffer.size();};
	//
	int ResizeBuffers(int nBufferCount);

	//fliter 内部调用。
	int GetFreeBuffer(CFilterPinBuffer **pBuffer);
	int GetFreeBufferCount(){return m_lstFreeBuffer.size();};
	//如果 操作分配的大小，是否 删除，重新分配一个更小的内存
	int PutFreeBuffer(CFilterPinBuffer *pBuffer, int nReverseMemory=0);
	//从头部获取
	int PutPreparedBuffer(CFilterPinBuffer *pBuffer,  bool bToTail = true  );
	int GetPreparedBuffer(CFilterPinBuffer **pBuffer, bool bFromHead = true);
	// 获取 prepare 数据的 另一个版本，从尾部获取
// 	int PutPreparedBufferToHead();
	//释放所有 busy 到 free
	void FreeAllBuffer();
	//
	int Clear();
	//如果在 子类里面调用了该函数，应该在程序退出的时候，在该行数模块 手动释放
	template <class T> int CreateBuffer(int nCount=1){
		//m_pMutexFreeBuffer->Lock();
		for (int i=0; i<nCount; i++)
		{
			//这个 new 如何处理？ 在  程序退出的时候可能有问题 
			CFilterPinBuffer *pBuffer = new T;
			//m_lstFreeBuffer.push_back(pBuffer);
			//不要直接调用 m_lstFreeBuffer 因为是模板，在 dll 和应用 程序 之间会交叉 new delete 操作内存，会异常
			PutFreeBuffer(pBuffer);
		}
		//m_pMutexFreeBuffer->Unlock();
		return 0;
	}
	template <class T> int ResizeBuffer(int nBufferCount=1){
		if (nBufferCount <= m_nBufferCount)
			return m_nBufferCount;
		int nNeedMore = nBufferCount - m_nBufferCount ;

		//m_pMutexFreeBuffer->Lock();
		for (int i=0; i<nNeedMore; i++)
		{
			//这个 new 如何处理？ 在  程序退出的时候可能有问题 
			CFilterPinBuffer *pBuffer = new T;
			//m_lstFreeBuffer.push_back(pBuffer);
			//不要直接调用 m_lstFreeBuffer 因为是模板，在 dll 和应用 程序 之间会交叉 new delete 操作内存，会异常
			PutFreeBuffer(pBuffer);
		}
		//m_pMutexFreeBuffer->Unlock();
		return m_nBufferCount;
	}
	virtual int Flush();
	const char *GetType();
	eFilterPinIOType GetIoType();
	//信号有数据 返回值，  输出pin
	//sigc::signal<int, CFilterPin*, CFilterPinBuffer*>		m_sigPinOut;
	sigc::signal<int, CFilterPin*, CFilterPinBuffer*>*		m_pSigPinOut;
	//数据输入通知。
	virtual int OnPinIn(CFilterPin* pPinPrevOut, CFilterPinBuffer* pPinBuffer);
	//void AppendBuffer(CFilterPinBuffer *pBuffer);
	virtual int Connect(CFilterPin*	pNextPinIn);
	//
	virtual int Emit(CFilterPinBuffer *pPinBuffer);
	//
protected:
	//
	bool FindNextPinIn(CFilterPin *pPin/*, int &nIndex*/);
	
	list<CFilterPinBuffer *> m_lstFreeBuffer;
	CMutex *m_pMutexFreeBuffer;
	list<CFilterPinBuffer *> m_lstBusyBuffer;
	CMutex *m_pMutexBusyBuffer;

	//CMutex *m_pMutextFlush;
	int m_nBufferCount;
	eFilterPinIOType m_nIOType;
	//filter宿主 
	CBaseFilter *m_pFilter;
	//自动 释放 过多的 内存。
	int m_nReversePinBufferMemorySize;
	//连接的 in pin，一个 out pin 可以连接到多个 in pin 
	vector<CFilterPin *>	m_lstNextPinIn;
	//如果是 in pin 这个指向 上一个 和它连接的 out pin
	vector<CFilterPin *>	m_lstPrevPinOut;
	//CFilterPin  *			m_pPrevPinOut;
	//
	int    m_nPinName;
	string m_strPinName;
	string m_strType;
};
}
#endif