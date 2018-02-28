#include "StdAfx.h"
#include "AudioRender.h"
#include "audioout.h"
#include "FilterPinBuffer.h"

using namespace SimpleCpp;
CAudioRender::CAudioRender(void)
: m_pAudioOut(0)
, m_bAudioReady(true)
{

}

CAudioRender::~CAudioRender(void)
{
	if (m_pAudioOut)
	{
		m_pAudioOut->Close();
		delete m_pAudioOut;
		m_pAudioOut = NULL;
	}
}
int CAudioRender::Open(int nChannels, int nBitPerSample, int nSampleRate)
{
	CMediaFilter::Open();


	if (m_pAudioOut)
	{
		return -1;
	}
	m_pAudioOut = new CAudioOut();
	if (m_pAudioOut == NULL)
		return -1;
	if (m_pAudioOut->Open(nChannels, nBitPerSample, nSampleRate) < 0)
		return -2;

	m_pAudioOut->SetOnWriteCB(OnAudioDataWrite, this);

	return 0;
}
int CAudioRender::DoFilter()
{
	if (!m_bAudioReady)
		return -1;
	//取一个 In pin 的prepared buffer
	if (m_lstPinIn.size() <= 0)
		return -1;
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return -2;
	CFilterPinBuffer *pBuffer = 0;
	pPinIn->GetPreparedBuffer(&pBuffer);
	if (!pBuffer)
		return -2;
	//
	if (m_pAudioOut)
	{
		if (m_pAudioOut->FillData(pBuffer->GetData(), pBuffer->GetLength()) < 0)
		{
			//繁忙。。。
			
		}
	}
	//将使用完毕的 buffer 放回到 In pin 的 free 队列
	pBuffer->Clear();
	pPinIn->PutFreeBuffer(pBuffer);

	return 0;
}

void CAudioRender::OnAudioDataWrite( void *pParam )
{
	CAudioRender *pAudioRender = (CAudioRender*)pParam;
	if (!pAudioRender)
		return ;
	//pAudioRender->OnFillAudio();
	pAudioRender->m_bAudioReady = true;
}
/*
void CAudioRender::OnFillAudio()
{
	//取一个 In pin 的prepared buffer
	if (m_lstPinIn.size() <= 0)
		return ;
	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return ;
	CFilterPinBuffer *pBuffer = 0;
	pPinIn->GetPreparedBuffer(&pBuffer);
	if (!pBuffer)
		return ;
	//
	if (m_pAudioOut)
	{
		if (m_pAudioOut->FillData(pBuffer->GetData(), pBuffer->GetLength()) < 0)
		{
			//繁忙。。。

		}
	}
	//将使用完毕的 buffer 放回到 In pin 的 free 队列
	pBuffer->Clear();
	pPinIn->PutFreeBuffer(pBuffer);

}
*/