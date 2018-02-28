
#ifndef AUDIO_IN_H
#define AUDIO_IN_H

#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

/*#include "devicein.h"*/
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

typedef  void (__stdcall *Cb_OnAudioIn)(char *pData, int nLength, void *pParam, void *pReversed);

class CAudioIn/*:public CDeviceIn*/
{
public:
	CAudioIn(int nChannels=2, int nBitPerSample=16, int nSampleRate=44100);
	virtual ~CAudioIn(void);
	virtual int		Open();
	virtual int		Close();
	virtual int     Start();
	virtual int		Stop();

	bool	IsOpen();
	
	void	SetOnDataCB(Cb_OnAudioIn onData, void * pParam);
	//
protected:
	static DWORD WINAPI AudioInThreadProc(LPVOID lpParam);
	//
	int		AllocBuffer();
	int		FreeBuffer();
protected:
	//录音线程的 句柄
	::HANDLE			m_hThread;
	//线程 id
	DWORD			m_dwThreadId;
	// 声音格式
	WAVEFORMATEX	m_WF;
	//数据缓存
	WAVEHDR			*m_pWHdr;	
	int				m_nBufferSize;
	//
	Cb_OnAudioIn	m_cbOnData;
	::HANDLE		m_hDevice;
	//
	void		*m_pCBParam;
};
#endif