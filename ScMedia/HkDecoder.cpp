#include "StdAfx.h"
#include "HkDecoder.h"
#include "VideoFramePinBuffer.h"
#include "WindowsPlayM4.h"
#include "FilterManager.h"

// hk 需要根据 avdataend 分帧,如果不分帧, hk解码器不解码
//
void  CHkDecoder::HkDecDataCallBack(long nPort,char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1,long nReserved2)
{
	if (pFrameInfo->nType == T_AUDIO8
		|| pFrameInfo->nType == T_AUDIO16)
	{
		return ;
	}

	//debug("%s\n", __FUNCTION__);
	CHkDecoder *pHkDecoder = (CHkDecoder*) nReserved1;
	if (pHkDecoder->GetState() != eFilterState_Run)
		return ;
	if (pHkDecoder->m_lstPinOut.size() <= 0)
		return ;
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
	pVideoFrame->SetUsedSize(pFrameInfo->nWidth*pFrameInfo->nHeight*3);
	
	memcpy(pVideoFrame->m_pY, (char *)pBuf, pFrameInfo->nWidth * pFrameInfo->nHeight);
	memcpy(pVideoFrame->m_pV, (char *)(pBuf + pFrameInfo->nWidth * pFrameInfo->nHeight), pFrameInfo->nWidth * pFrameInfo->nHeight/4);
	memcpy(pVideoFrame->m_pU, (char *)(pBuf + pFrameInfo->nWidth * pFrameInfo->nHeight * 5/4), pFrameInfo->nWidth * pFrameInfo->nHeight/4);

	

	pPinOut->PutPreparedBuffer(pVideoFrame);

}
CHkDecoder g_regDecoderHk;
CHkDecoder::CHkDecoder(void)
:CDecoder()
, m_nHkStreamHandle(-1)
, m_bHkInit(false)
, m_nHkHeaderLen(0)
, m_hWnd(0)
{
	strcpy(m_szType, FILTER_DECODER_HK);
	strcpy(m_szName, FILTER_DECODER_HK);

	CFilterManager::RegistFilter(&g_regDecoderHk);
}

CHkDecoder::~CHkDecoder(void)
{
	int nRet = 0;
	int nError = 0;
	if (m_nHkStreamHandle)
	{
		nRet = PlayM4_Stop(m_nHkStreamHandle);
		if(!nRet)
		{
			nError = PlayM4_GetLastError(m_nHkStreamHandle);
			//vivs_warning("Stop failed for reason %d, error code(%d)", hr, m);
		}

		nRet = PlayM4_CloseStream(m_nHkStreamHandle);
		if(!nRet)
		{
			nError = PlayM4_GetLastError(m_nHkStreamHandle);
			//vivs_warning("CloseStream failed for reason %d, error code(%d)", hr, m);
		}

		PlayM4_FreePort(m_nHkStreamHandle);

		m_nHkStreamHandle = -1;
	}
}

int CHkDecoder::Open(char *pParam/*=0*/)
{
	CDecoder::Open();
	//创建 buffer
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;

	pPinOut->Clear();

	pPinOut->CreateBuffer<CVideoFramePinBufer>(2);
	//

	if (!PlayM4_GetPort((LONG *)&m_nHkStreamHandle))
	{
// 		self->priv->could_work = FALSE;
// 		vivs_warning("PlayM4_GetPort return false. so play error. form handle is %d, port(%d)", 
// 			self->priv->port, self->priv->port);
	}
	else
	{
//		vivs_debug("hk Listener get port (%d), form handle(%d)", self->priv->port, self->priv->port);
	}
	m_nHkHeaderLen = PlayM4_GetFileHeadLength();

	if (!PlayM4_SetStreamOpenMode(m_nHkStreamHandle, STREAME_FILE))
	{   
		//self->priv->could_work = FALSE;
		PlayM4_FreePort(m_nHkStreamHandle);
		m_nHkStreamHandle = -1;
		//vivs_warning("SetStreamOpenMode() failed, error(%d)", PlayM4_GetLastError(self->priv->port));
	}

	return 0;
}

int CHkDecoder::DoFilter()
{
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut || pPinOut->GetFreeBufferCount() <= 0)
	{
		return CDecoder::DoFilter();
	}

	CFilterPin *pPin = m_lstPinIn.at(0);
	if (!pPin)
		return -1;
	CFilterPinBuffer *pPinBuffer = 0;
	pPin->GetPreparedBuffer(&pPinBuffer);
	if (!pPinBuffer)
		return -2;

	if (!m_bHkInit)
	{

		if (!PlayM4_OpenStream(m_nHkStreamHandle, (PBYTE)pPinBuffer->GetData(), m_nHkHeaderLen, HK_SOURCE_BUFFER_LEN))
		{
// 			vivs_warning("OpenStream() Fail, handle_port = %d, header_len = %d, error code(%d).", \
// 				self->priv->port, self->priv->header_len, PlayM4_GetLastError(self->priv->port));

			goto ret;
		}

		if (!PlayM4_SetDecCallBackMend(m_nHkStreamHandle, HkDecDataCallBack, (long)this) )
		{
			//vivs_warning("PlayM4_SetDecCallBackMend() return false.   error. form handle is %d", self->priv->port);
			goto ret;
		}
// 		if (!PlayM4_SetDisplayBuf(m_nHkStreamHandle, 20))
// 		{
// 			goto ret;
// 		}
		if (!PlayM4_Play(m_nHkStreamHandle, (HWND)0))
		{
// 			vivs_warning("Play() Fail, handle_port = %d, form_handle = %d, error code(%d)", \
// 				self->priv->port, self->priv->port, PlayM4_GetLastError(self->priv->port));
			goto ret;
		}
		if (!PlayM4_PlaySoundShare(m_nHkStreamHandle))
		{
	//		debug("PlayM4_PlaySoundShare false: error[%d]\n", PlayM4_GetLastError(self->priv->port));
	//		vivs_warning("PlayM4_PlaySoundShare() return false.   error[%d]. form handle is %d", PlayM4_GetLastError(self->priv->port), self->priv->port);
			//return -2;
		}

		m_bHkInit = true;
		pPinBuffer->Clear(0, m_nHkHeaderLen);
	}

#ifdef debug_hk_pkg
	static FILE *file = NULL;
	if (!file)
	{
		file =	fopen("f:\\tmp\\hk-2.hkpkg", "w+b");
	}
	if (file)
	{
		fwrite(pPinBuffer->GetData(), pPinBuffer->GetLength(), 1, file);
		fflush(file);
	}
#endif
	int nTries =0 ;
	while (!PlayM4_InputData(m_nHkStreamHandle, (PBYTE)(pPinBuffer->GetData()), pPinBuffer->GetLength())
		&& nTries++<100)
		//if (!PlayM4_InputData(self->priv->port, (PBYTE)(data), (data_len)))
	{
		// 			vivs_warning("InputData() Fail, handle_port(%d), form_handle(%d), header_len(%d), error code(%d) \
		// 						 Current data_len(%d)", \
		// 						 self->priv->port, self->priv->port, page->len, PlayM4_GetLastError(self->priv->port), \
		// 						 PlayM4_GetSourceBufferRemain(self->priv->port));

		//PlayM4_ResetSourceBuffer(m_nHkStreamHandle);
		Sleep(10);
	}
ret:
	pPin->PutFreeBuffer(pPinBuffer);

	return CDecoder::DoFilter();
}

void CHkDecoder::SetWnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

CMediaFilter * CHkDecoder::CreateObject()
{
	return new CHkDecoder();

}
