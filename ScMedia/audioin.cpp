#include "StdAfx.h"
#include "audioin.h"

#define NUM_BUFFER 16
#define DATA_LENGTH 2048

CAudioIn::CAudioIn(int nChannels, int nBitPerSample, int nSampleRate)
:m_pWHdr(NULL)
, m_hThread(NULL)
{
	//默认的 声音格式
	m_WF.wFormatTag		= WAVE_FORMAT_PCM;
	m_WF.cbSize			= 0;
	m_WF.wBitsPerSample	= nBitPerSample;
	// 8.0 kHz, 16k, 11.025 kHz, 22.05 kHz, and 44.1 kHz.
	m_WF.nSamplesPerSec	= nSampleRate;
	m_WF.nChannels		= nChannels;

	m_WF.nBlockAlign	=  m_WF.nChannels     *(m_WF.wBitsPerSample/8);
	m_WF.nAvgBytesPerSec = m_WF.nSamplesPerSec*(m_WF.wBitsPerSample/8)*m_WF.nChannels;
	//
	m_nBufferSize = (DATA_LENGTH * m_WF.wBitsPerSample/8 * m_WF.nChannels  );
	m_hDevice = NULL;
}

CAudioIn::~CAudioIn(void)
{
	Close();
}
bool CAudioIn::IsOpen()
{
	return m_hDevice!=NULL ? true:false;
}
void CAudioIn::SetOnDataCB(Cb_OnAudioIn onData, void * pParam)
{
	m_cbOnData = onData;
	m_pCBParam = pParam;
}
DWORD WINAPI CAudioIn::AudioInThreadProc(LPVOID lpParam)
{
	CAudioIn* pAI = (CAudioIn*)lpParam;
	if (pAI == NULL)
		return -1;
	MSG msg;

	while(1)//(GetMessage(&msg,0,0,0))
	{
		if (!PeekMessage(&msg, 0,  0, 0, PM_REMOVE) )
		{
			Sleep(10);
			continue;
		}
		
		switch(msg.message)
		{
		case WIM_DATA:
			{
				WAVEHDR* pWH=(WAVEHDR*)msg.lParam;
				waveInUnprepareHeader((HWAVEIN)msg.wParam, pWH, sizeof(WAVEHDR));

				if(pWH->dwBytesRecorded != pAI->m_nBufferSize)
					break;//it's not full recorded,i think the wave recorder has ben

				//
				if (pAI->m_cbOnData)
				{//有声音数据
					pAI->m_cbOnData(pWH->lpData, pAI->m_nBufferSize, pAI->m_pCBParam, 0);
				}

				waveInPrepareHeader((HWAVEIN)msg.wParam, pWH, sizeof(WAVEHDR));
				waveInAddBuffer((HWAVEIN)msg.wParam, pWH, sizeof(WAVEHDR));
			}
			break;
		case WM_QUIT:
			goto ret;
			break;
		}
	}	

ret:
	return msg.wParam;
}
int CAudioIn::Open()
{

	//
	MMRESULT mmr;
	mmr = waveInOpen(0, WAVE_MAPPER, &m_WF, 0, 0, WAVE_FORMAT_QUERY);
	if(mmr)
	{
/*		SetLastMMError(mmr);*/
		return mmr;
	}	
	//首先创建 线程
	m_hThread = CreateThread(0, 0, AudioInThreadProc,this,0,&m_dwThreadId);
	if(!m_hThread)
		return -1;
	mmr = waveInOpen(((LPHWAVEIN)&m_hDevice),WAVE_MAPPER, &m_WF, m_dwThreadId, (DWORD)this, CALLBACK_THREAD);
	if(mmr)
	{
		return mmr;
	}


	return 0;
}
int CAudioIn::Close()
{
	Stop();

	if(!m_hDevice)
		return -1;
	MMRESULT mmr;
/*
	mmr=waveInReset((HWAVEIN)m_hDevice);
	if(mmr)
		return -1;*/

	//Sleep(1500);//非常重要
	
/*
	if(mmr)
		return -1;*/

	
	m_hDevice = NULL;
	return 0;
}
int	CAudioIn::Start()
{
	if(!m_hDevice)
		return -1;
 	if(AllocBuffer() != 0)
 		return -1;
	MMRESULT mmr;
	mmr = waveInStart((HWAVEIN)m_hDevice);
	if(mmr)
		return -1;
	//successful;

	return 0;
}
int CAudioIn::Stop()
{
	if(!m_hDevice)
		return -1;
	MMRESULT mmr;
	mmr=waveInReset((HWAVEIN)m_hDevice);
	
	mmr=waveInClose((HWAVEIN)m_hDevice);
// 	if(mmr)
// 		return -1;
	m_hDevice = NULL;
	if (m_hThread)
	{
		DWORD dwExitCode;
		int  nTries = 30;

		do {
			PostThreadMessage(m_dwThreadId, WM_QUIT, 0 , 0);
			Sleep(100);
			GetExitCodeThread(m_hThread, &dwExitCode);
			nTries--;
		}while (dwExitCode== STILL_ACTIVE && nTries >= 0);

		if (dwExitCode == STILL_ACTIVE)
		{
			//vivs_warning("vivs_voice_dispose, WM_QUIT false,  to terminate thread in");
			TerminateThread(m_hThread, 0);
		}

		CloseHandle(m_hThread);

		m_hThread = NULL;
	}


	Sleep(100);
 	if(FreeBuffer() != 0)
 		return -1;

	return 0;
}
int CAudioIn::AllocBuffer()
{
	BOOL bRet=FALSE;
	UINT i;
	MMRESULT mmr;
	m_pWHdr = new WAVEHDR[NUM_BUFFER];
	//为了使录音连续，采用多个缓冲区
	for(i=0; i<NUM_BUFFER; i++)
	{
		ZeroMemory(&m_pWHdr[i], sizeof(WAVEHDR));
		m_pWHdr[i].lpData = new char[m_nBufferSize];
		m_pWHdr[i].dwBufferLength = m_nBufferSize;
		mmr = waveInPrepareHeader((HWAVEIN)m_hDevice, &m_pWHdr[i], sizeof(WAVEHDR));
		if(mmr)		
			return -1;

		mmr = waveInAddBuffer((HWAVEIN)m_hDevice, &m_pWHdr[i], sizeof(WAVEHDR));
		if(mmr)
			return -1;

	}

	return 0;
}
int CAudioIn::FreeBuffer()
{
	
	UINT i;
	if(!m_pWHdr)
		return -1;

	for(i=0;i<NUM_BUFFER;i++)
	{
		waveInUnprepareHeader((HWAVEIN)m_hDevice, &m_pWHdr[i], sizeof(WAVEHDR));
		if(m_pWHdr[i].lpData)
		{
			delete [](m_pWHdr[i].lpData);
			m_pWHdr[i].lpData = NULL;
		}


	}
	delete []m_pWHdr;
	m_pWHdr = NULL;

	return 0;
}