#pragma once
#include "render.h"

class CAudioOut;
class CAudioRender :
	public CRender
{
	friend static void __stdcall  OnAudioDataWrite(void *pParam) ;
public:
	CAudioRender(void);
	virtual ~CAudioRender(void);
	
	virtual int Open(int nChannels=2, int nBitPerSample=16, int nSampleRate=44100);
	virtual int DoFilter();
protected:
	//void OnFillAudio();
	static void __stdcall OnAudioDataWrite(void *pParam) ;
protected:
	CAudioOut *m_pAudioOut;
	bool m_bAudioReady;
};
