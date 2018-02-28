#include "StdAfx.h"
#include "DhDecoder.h"
#include "dhplay.h"
#include "ActiveYUV.h"
#include "VideoFramePinBuffer.h"

#pragma comment(lib, "ActiveYUV.lib")
// hk 需要根据 avdataend 分帧,如果不分帧, hk解码器不解码
//
void  CDhDecoder::DhDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	if (pFrameInfo->nType == T_AUDIO8
		|| pFrameInfo->nType == T_AUDIO16)
	{
		return ;
	}
	CDhDecoder *pHkDecoder = (CDhDecoder*) nReserved1;
	CFilterPin *pPinOut = pHkDecoder->m_lstPinOut.at(0);
	if (!pPinOut)
		return;
	CFilterPinBuffer  *pPinBuffer = NULL;
	pPinOut->GetFreeBuffer(&pPinBuffer);
	if (!pPinBuffer)
		return ;

	CVideoFramePinBufer *pVideoFrame = 0;
	pVideoFrame = dynamic_cast<CVideoFramePinBufer*>(pPinBuffer);
	if (!pVideoFrame)
	{
		pPinOut->PutFreeBuffer(pPinBuffer);
		return ;
	}
	// copy yuv to video frame;
	pVideoFrame->SetFrameSize(pFrameInfo->nWidth, pFrameInfo->nHeight);


	memcpy(pVideoFrame->m_pY, (char *)pBuf, pFrameInfo->nWidth * pFrameInfo->nHeight);
	memcpy(pVideoFrame->m_pU, (char *)(pBuf + pFrameInfo->nWidth * pFrameInfo->nHeight), pFrameInfo->nWidth * pFrameInfo->nHeight/4);
	memcpy(pVideoFrame->m_pV, (char *)(pBuf + pFrameInfo->nWidth * pFrameInfo->nHeight * 5/4), pFrameInfo->nWidth * pFrameInfo->nHeight/4);

	pPinOut->PutPreparedBuffer(pVideoFrame);

}

CDhDecoder::CDhDecoder(void)
:m_nDhStreamHandle(0)
{
}

CDhDecoder::~CDhDecoder(void)
{
	int nRet = 0;
	int nError = 0;
	if ( m_nDhStreamHandle > 0 )
	{
		nRet = PLAY_Stop(m_nDhStreamHandle);
		if(!nRet)
		{
			nError = PLAY_GetLastError(m_nDhStreamHandle);
			///vivs_warning("Stop failed for reason %d, error code(%d)", hr, m);
		}

		nRet = PLAY_CloseStream(m_nDhStreamHandle);
		if(!nRet)
		{
			nError = PLAY_GetLastError(m_nDhStreamHandle);
			//vivs_warning("CloseStream failed for reason %d, error code(%d)", hr, m);
		}

		nRet = PLAY_ReleasePort(m_nDhStreamHandle);
		if(!nRet)
		{
			nError = PLAY_GetLastError(m_nDhStreamHandle);
			//vivs_warning("FreePort failed for reason %d, error code(%d)", hr, m);
		}
		m_nDhStreamHandle = 0;
	}
}
int CDhDecoder::Open()
{
	CDecoder::Open();
	if (!PLAY_GetFreePort((LONG*)&m_nDhStreamHandle))
	{

		//not return, for not decode.;
		return -1;
	}
	PLAY_SetStreamOpenMode(m_nDhStreamHandle, STREAME_REALTIME/*STREAME_FILE*/);
	if (!PLAY_OpenStream(m_nDhStreamHandle, NULL, 0, 80*1024))
	{
		// 		vivs_warning("OpenStream() Fail, handle_port = %d, error code(%d).", \
		// 			self->priv->port, PLAY_GetLastError(self->priv->port));
		return -2;
	}
	//PLAY_ResetBuffer();
	//PLAY_ResetSourceBuffer()
	if (!PLAY_SetDisplayBuf(m_nDhStreamHandle, 16))
	{
		// 		vivs_warning("PLAY_SetDisplayBuf() Fail, handle_port = %d, error code(%d).", \
		// 			self->priv->port, PLAY_GetLastError(self->priv->port));
	}


	if (!PLAY_SetDecCallBackEx(m_nDhStreamHandle,  DhDecDataCallBack, (long)this) )
	{
		// 			vivs_warning("PLAY_SetDecCallBackEx() Fail, handle_port = %d, error code(%d)", \
		// 				self->priv->port, PLAY_GetLastError(self->priv->port));
	}

	if (!PLAY_Play(m_nDhStreamHandle, (HWND)0))
	{
		// 			vivs_warning("Play() Fail, handle_port = %d, error code(%d)", \
		// 				self->priv->port, PLAY_GetLastError(self->priv->port));
		return -3;
	}


	return 0;
}
int CDhDecoder::DoFilter()
{

	CFilterPin *pPin = m_lstPinIn.at(0);
	if (!pPin)
		return -1;
	CFilterPinBuffer *pPinBuffer = 0;
	pPin->GetPreparedBuffer(&pPinBuffer);
	if (!pPinBuffer)
		return -2;


	if (m_nDhStreamHandle > 0 && pPinBuffer->GetLength() > 0)
	{
		bool bRet = PLAY_InputData(m_nDhStreamHandle, (PBYTE)pPinBuffer->GetData(), pPinBuffer->GetLength());
		if (!bRet)
		{
			debug("InputData() Fail, handle_port = %d,   data_len = %d, error code(%d)\n", \
				m_nDhStreamHandle,  pPinBuffer->GetLength(), PLAY_GetLastError(m_nDhStreamHandle));		
		}
	}
	pPinBuffer->Clear();

	pPin->PutFreeBuffer(pPinBuffer);

	return 0;
}