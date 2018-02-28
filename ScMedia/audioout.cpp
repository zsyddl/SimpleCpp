#include "StdAfx.h"
#include "audioout.h"

CAudioOut::CAudioOut(int nChannels, int nBitPerSample, int nSampleRate)
:m_nMaxBuffers(16)
, m_hDevice(0)
, m_hThread(0)
{
	m_WF.wFormatTag		= WAVE_FORMAT_PCM;
	m_WF.cbSize			= 0;
	m_WF.wBitsPerSample	= nBitPerSample;
	m_WF.nSamplesPerSec	= nSampleRate;
	m_WF.nChannels		= nChannels;
	m_WF.nAvgBytesPerSec= m_WF.nSamplesPerSec*(m_WF.wBitsPerSample/8);
	m_WF.nBlockAlign	= m_WF.nChannels     *(m_WF.wBitsPerSample/8);

	m_cbOnDataWrite = NULL;

	InitializeCriticalSection(&m_hMutex);
}

CAudioOut::~CAudioOut(void)
{
	Close();
	for (int i=0; m_lstBuffer.size() > 0; )
	{
		WAVEHDR *pWaveHdr = m_lstBuffer.front();
		if (pWaveHdr)
		{
			if (pWaveHdr->lpData)
			{
				delete pWaveHdr->lpData;
				pWaveHdr->lpData = NULL;
			}
			delete pWaveHdr;
			pWaveHdr = NULL;
		}
		m_lstBuffer.pop();
	}

	DeleteCriticalSection(&m_hMutex);
}
DWORD WINAPI CAudioOut::ThreadAudioOutProc(LPVOID lpParam)
{
	CAudioOut *pAO = (CAudioOut *) lpParam;
	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		switch(msg.message)
		{
//		case IOM_AUDIO:
//			pMgr->OnDecodeAudioData((PACK_AUDIO*)msg.lParam,(int)msg.wParam);
//			break;
		case WOM_DONE:
			{
				WAVEHDR* pwh=(WAVEHDR*)msg.lParam;
				EnterCriticalSection(&pAO->m_hMutex);
				waveOutUnprepareHeader((HWAVEOUT)msg.wParam, pwh, sizeof(WAVEHDR));

				//delete pwh->lpData;//删除Play调用时分配的内存
				//pwh->dwBufferLength = 0;
				//pwh->lpData = NULL;
				pAO->m_lstBuffer.push(pwh);
				if (pAO->m_cbOnDataWrite)
				{
					pAO->m_cbOnDataWrite(pAO->m_pCBParam);
				}
				LeaveCriticalSection(&pAO->m_hMutex);
			}
			break;
		case WM_QUIT:
			return 0;
		}
	}
	return msg.wParam;
}
int CAudioOut::Open(int nChannels, int nBitPerSample, int nSampleRate)
{
	if (m_hDevice
		|| m_hThread)
	{
		return -1;
	}
	//
	m_WF.wFormatTag		= WAVE_FORMAT_PCM;
	m_WF.cbSize			= 0;
	m_WF.wBitsPerSample	= nBitPerSample;
	m_WF.nSamplesPerSec	= nSampleRate;
	m_WF.nChannels		= nChannels;
	m_WF.nAvgBytesPerSec= m_WF.nSamplesPerSec*(m_WF.wBitsPerSample/8);
	m_WF.nBlockAlign	= m_WF.nChannels     *(m_WF.wBitsPerSample/8);
	//
	for (int i=0; i<m_nMaxBuffers; i++)
	{
		WAVEHDR *pWaveHdr = new WAVEHDR;
		memset(pWaveHdr, 0, sizeof(WAVEHDR));
		m_lstBuffer.push(pWaveHdr);
	}
	//首先创建 线程
	m_hThread = CreateThread(0, 0, ThreadAudioOutProc, this, 0, &m_dwThreadId);
	if(!m_hThread)
		return -1;
	//
	MMRESULT mmr;
	mmr=waveOutOpen(0,WAVE_MAPPER, &m_WF,0,0,WAVE_FORMAT_QUERY);
	if(mmr)
	{
		return -1;
	}
	mmr=waveOutOpen((LPHWAVEOUT)&m_hDevice,WAVE_MAPPER, &m_WF, m_dwThreadId, (DWORD)this, CALLBACK_THREAD);
	if(mmr)
	{
		return -1;
	}
	mmr=waveOutRestart((HWAVEOUT)m_hDevice);
	if(mmr)
		return -1;

	return 0;
}
int CAudioOut::Close()
{

	if(!m_hDevice)
		return -1;
	MMRESULT mmr;
	mmr=waveOutReset((HWAVEOUT)m_hDevice);
	if(mmr)
		return -1;
	mmr=waveOutClose((HWAVEOUT)m_hDevice);
	if(mmr)
		return -1;
	m_hDevice = NULL;
	//需要线程数据同步。
	if (m_dwThreadId > 0)
	{
		int nTries = 5;
		DWORD dwExitCode = STILL_ACTIVE;
		while (nTries-- >= 0 && dwExitCode == STILL_ACTIVE)
		{
			PostThreadMessage(m_dwThreadId, WM_QUIT, 0 ,0);
			Sleep(20);
			GetExitCodeThread(m_hThread, &dwExitCode);
		}
		if (dwExitCode == STILL_ACTIVE)
		{
			//if run to here very dangerous //
			TerminateThread(m_hThread, 0);
		}
	}
	CloseHandle(m_hThread);
	m_dwThreadId = 0;
	m_hThread = 0;

	return 0;
}
int	CAudioOut::Start()
{
	MMRESULT mmr;
	mmr=waveOutRestart((HWAVEOUT)m_hDevice);
	if(mmr)
		return -1;

	return 0;
}
int CAudioOut::Stop()
{

	Close();
	return 0;
}
int	CAudioOut::FillData(char *pData, int nLength)
{
	if (m_lstBuffer.size() <= 0)
		return -1;
	if (!m_hDevice)
		return -2;
	if (nLength <= 0)
		return -3;
	BOOL bRet=FALSE;
	
	EnterCriticalSection(&m_hMutex);

	WAVEHDR *pWaveHdr = m_lstBuffer.front();
	if (pWaveHdr == NULL)
	{
		LeaveCriticalSection(&m_hMutex);
		return 0;
	}
	m_lstBuffer.pop();
	LeaveCriticalSection(&m_hMutex);

	if (pWaveHdr->lpData )
	{
		if (pWaveHdr->dwBufferLength != nLength)
		{
			delete pWaveHdr->lpData;
			pWaveHdr->lpData = NULL;
			pWaveHdr->lpData = new  char[nLength];
			pWaveHdr->dwBufferLength = nLength;
		}
		else
		{//不需要分配内存

		}
	}
	else
	{
		pWaveHdr->lpData = new  char[nLength];
		pWaveHdr->dwBufferLength = nLength;
	}
	memcpy(pWaveHdr->lpData, pData, nLength);
	//
	MMRESULT mmr;
	mmr=waveOutPrepareHeader((HWAVEOUT)m_hDevice, pWaveHdr, sizeof(WAVEHDR));
	if(mmr)
	{
		m_lstBuffer.push(pWaveHdr);
		return -1;
	}
	mmr=waveOutWrite((HWAVEOUT)m_hDevice, pWaveHdr, sizeof(WAVEHDR));
	if(mmr)
	{
		m_lstBuffer.push(pWaveHdr);
		return -1;
	}


	return 0;
}
