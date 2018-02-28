
#ifndef AUDIO_OUT_H
#define AUDIO_OUT_H


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma once
/*#include "deviceout.h"*/
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
typedef void (__stdcall * OnDataWrite)(void *pParam) ;

#include <queue>

using namespace std;
class CAudioOut/*:public CDeviceOut*/
{
	friend static DWORD WINAPI ThreadAudioOutProc(LPVOID lpParam);
public:
	CAudioOut(int nChannels=2, int nBitPerSample=16, int nSampleRate=44100);
	virtual ~CAudioOut(void);
	//
	virtual int		Open(int nChannels=2, int nBitPerSample=16, int nSampleRate=44100);
	virtual int		Close();
	virtual int		Start();
	virtual int     Stop();
	int		FillData(char *pData, int nLength);
	void	SetOnWriteCB(OnDataWrite cbWrite, void *pParam){
		m_cbOnDataWrite = cbWrite;
		m_pCBParam = pParam;
	};
protected:
	//线程函数
	static DWORD WINAPI ThreadAudioOutProc(LPVOID lpParam);
protected:
	//录音线程的 句柄
	::HANDLE			m_hThread;
	//线程 id
	DWORD			m_dwThreadId;
	// 声音格式
	WAVEFORMATEX	m_WF;
	//数据
	std::queue<WAVEHDR *> m_lstBuffer;
	int				m_nMaxBuffers;
	//
	OnDataWrite		m_cbOnDataWrite;
	void			*m_pCBParam;
	//
	::HANDLE			m_hDevice;
	//
	CRITICAL_SECTION m_hMutex;
	//
};
#endif