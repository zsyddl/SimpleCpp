#include "StdAfx.h"
#include "DahuaRender.h"

#include "dhplay.h"
#include "ActiveYUV.h"
#include "FilterPinBuffer.h"

#pragma comment(lib, "ActiveYUV.lib")

CDahuaRender::CDahuaRender(void)
:m_nDhStreamHandle(0)
{

}

CDahuaRender::~CDahuaRender(void)
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

int CDahuaRender::Open( HWND hWnd, int nPlayBuffer )
{
	CRender::Open(hWnd);
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

// 	if (!PLAY_RigisterDrawFun(self->priv->port, vivs_dh_listener_draw, (long)self))
// 	{
// 		self->priv->could_work = FALSE;
// 		vivs_warning("RigisterDrawFun() return false. so play error. form handle is %d", self->priv->handle);
// 		return;
// 	}

	if (0)
	{
		if (!PLAY_SetDecCallBackEx(m_nDhStreamHandle, /*DecDataCallBack*/0, (long)this) )
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
	}
	else
	{
		if (!PLAY_Play(m_nDhStreamHandle, (HWND)hWnd))
		{
// 			vivs_warning("Play() Fail, handle_port = %d, error code(%d)", \
// 				self->priv->port, PLAY_GetLastError(self->priv->port));
			return -4;
		}
	}

	return 0;
}

int CDahuaRender::DoFilter()
{

	CFilterPin *pPinIn = m_lstPinIn.at(0);
	if (!pPinIn)
		return -1;
	CFilterPinBuffer *pPinBuffer = NULL;
	do  
	{
		pPinBuffer = NULL;
		pPinIn->GetPreparedBuffer(&pPinBuffer);
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
		pPinIn->PutFreeBuffer(pPinBuffer);
	}while(pPinBuffer);

	
	return 0;
}
