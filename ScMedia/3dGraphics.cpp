#include "StdAfx.h"
#include "3dgraphics.h"
#include "3dGraphicItem.h"
#include "D3dPresenter.h"
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
 
C3dGraphics::C3dGraphics(void)
:m_fLogicXSize(1)
, m_fLogicYSize(1)
, m_fLogicZSize(1)
, m_hMonitor(0)
, m_hWnd(0)
{
	InitializeCriticalSection(&m_hLock);
	m_pGraphicEngine = new CD3dPresenter();
}


C3dGraphics::~C3dGraphics(void)
{
	if (m_pGraphicEngine)
	{
		delete m_pGraphicEngine;
		m_pGraphicEngine = 0;
	}
	DeleteCriticalSection(&m_hLock);
	
}
int C3dGraphics::Create(HWND hWnd)
{
	int nRt = m_pGraphicEngine->Create(hWnd);

	m_hWnd = hWnd;
	m_hMonitor = ::MonitorFromWindow((HWND)hWnd, MONITOR_DEFAULTTONEAREST);
	
	return nRt;
}
HWND C3dGraphics::GetWnd()
{

	return m_hWnd;
}
int C3dGraphics::GetWndSize(int &nW, int &nH)
{
	nW = 0;
	nH = 0;
	if (m_hWnd<=0)
	{
		return -1;
	}

	RECT rcWnd;
	if (GetWindowRect(m_hWnd, &rcWnd))
	{
		nW = rcWnd.right - rcWnd.left;
		nH = rcWnd.bottom - rcWnd.top;
	}
	else
	{	
		return -1;
	}
	return 0;
}
void C3dGraphics::GetSize(int &nW, int &nH){
	if (m_pGraphicEngine)
		return m_pGraphicEngine->GetSize(nW, nH);
}
int C3dGraphics::AppendItem(C3dGraphicItem* pItem)
{
	EnterCriticalSection(&m_hLock) ;

	list<C3dGraphicItem*>::iterator item;
	bool bAlreadyAdd = 0;
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		if (*item == pItem)
		{
			bAlreadyAdd = true;
			break;
		}
	}
	if (bAlreadyAdd)
	{
		LeaveCriticalSection(&m_hLock) ;
		return m_lstItems.size();
	}

	 m_lstItems.push_back(pItem);
	LeaveCriticalSection(&m_hLock) ;

	 return m_lstItems.size();
}
int C3dGraphics::AddItem2Head(C3dGraphicItem* pItem)
{
	EnterCriticalSection(&m_hLock) ;

	list<C3dGraphicItem*>::iterator item;
	bool bAlreadyAdd = 0;
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		if (*item == pItem)
		{
			bAlreadyAdd = true;
			break;
		}
	}
	if (bAlreadyAdd)
	{
		LeaveCriticalSection(&m_hLock) ;
		return m_lstItems.size();
	}

	m_lstItems.push_front(pItem);
	LeaveCriticalSection(&m_hLock) ;

	return m_lstItems.size();
}
int C3dGraphics::RemoveItem(C3dGraphicItem* pItem)
{
	EnterCriticalSection(&m_hLock) ;
	list<C3dGraphicItem*>::iterator item;
	bool bFind = false;
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		if (*item == pItem)
		{
			bFind = true;
			break;
		}
	}
	if (bFind)
	{
		m_lstItems.erase(item);
	}
	LeaveCriticalSection(&m_hLock) ;

	return m_lstItems.size();
}
//强制刷新 输出图形
int C3dGraphics::Refresh()
{
	//debug("%s\n", __FUNCTION__);
	EnterCriticalSection(&m_hLock) ;
	//debug("%s ++ 1\n", __FUNCTION__);
	if (CheckDeviceLost() == D3DERR_DEVICENOTRESET
		|| CheckMonitorChanged() >= 0)
	{
		int nRt = Reset();
		if (nRt >= 0)
		{
			debug("recreate 3d device success\n");
		}
		else
		{
			debug("recreate 3d device false\n");
		}
	}
	// 检测 显示器发生变化

	

	//debug("%s to render \n", __FUNCTION__);
	if (CheckDeviceLost() == D3D_OK)
	{
		Render();
	}
	//debug("%s render end \n", __FUNCTION__);
	LeaveCriticalSection(&m_hLock) ;
	
	//debug("%s -- \n", __FUNCTION__);
	return 0;
}
int C3dGraphics::HitTest(float fX, float fY)
{
	EnterCriticalSection(&m_hLock) ;
	list<C3dGraphicItem*>::iterator item;
	C3dGraphicItem* p3dItem(0);
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		p3dItem = *item;
		if (p3dItem)
		{
			p3dItem->HitTest(fX, fY);
		}
	}

	LeaveCriticalSection(&m_hLock) ;

	return 0;
}
//
int C3dGraphics::Render()
{
	//EnterCriticalSection(&m_hLock) ;
	m_pGraphicEngine->RenderBegin();
	list<C3dGraphicItem*>::iterator item;
	C3dGraphicItem* p3dItem(0);
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		p3dItem = *item;
		if (p3dItem && p3dItem->IsVisible())
			p3dItem->Render(this);
	}
	m_pGraphicEngine->RenderEnd();

	//LeaveCriticalSection(&m_hLock) ;
	return 0;
}
int C3dGraphics::CheckDeviceLost()
{
	int nState = D3D_OK;

	if (!m_pGraphicEngine)
		return false;
	IDirect3DDevice9 *pD3dDev = m_pGraphicEngine->GetD3DDevice9();
	if (!pD3dDev)
		return false;

	
	switch(pD3dDev->TestCooperativeLevel())
	{
	case D3D_OK:
		break;
	case D3DERR_DEVICELOST:
		nState = D3DERR_DEVICELOST;
		break;
	case D3DERR_DEVICENOTRESET:
		// OnLost Device;
		//IDirect3DDevice9::Reset();
		//On Reset device;
		nState = D3DERR_DEVICENOTRESET;
		break;
	default:
		break;
	}
	return nState;
}
int C3dGraphics::CheckMonitorChanged()
{
	HMONITOR hMonitor = ::MonitorFromWindow((HWND)m_hWnd, MONITOR_DEFAULTTONEAREST);
	if (hMonitor != m_hMonitor)
	{
		return 1;
	}
	else
	{
		return -1;
	}
	return 0;
}
int C3dGraphics::Reset()
{
	list<C3dGraphicItem*>::iterator item;
	C3dGraphicItem* p3dItem(0);
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		p3dItem = *item;
		if (p3dItem)
			p3dItem->Release();
	}
	/*m_pGraphicEngine->GetD3DDevice9()->Reset();*/
	debug("%s: m_pGraphicEngine->ReCreate()\n", __FUNCTION__);
	if (m_pGraphicEngine->ReCreate()<0)
		return -1;
	debug("%s: m_pGraphicEngine->ReCreate() end\n", __FUNCTION__);

	m_hMonitor = ::MonitorFromWindow((HWND)m_hWnd, MONITOR_DEFAULTTONEAREST);
	/*C3dGraphicItem* p3dItem(0);*/
	for (item = m_lstItems.begin();
		item != m_lstItems.end(); 
		item++)
	{
		p3dItem = *item;
		if (p3dItem)
			p3dItem->ReCreate(this);
	}
	

	return 0;
}
int C3dGraphics::SnapShot(char *pSaveFile)
{
	int nW(0),  nH(0);
	m_pGraphicEngine->GetSize(nW, nH);
	IDirect3DSurface9* pSystemSurfaceBuffer=0;	
#ifdef USE_D3D9_EX
	IDirect3DDevice9Ex *pD3dDev(0);	
	pD3dDev = (IDirect3DDevice9Ex*) m_pGraphicEngine->GetD3DDevice9();
#else
	//
	IDirect3DDevice9 *pD3dDev(0);
	pD3dDev = m_pGraphicEngine->GetD3DDevice9();
	;;
#endif

	if (!pD3dDev)
	{
		//LeaveCriticalSection(&m_hLock) ;
		return -1;
	}
	D3DFORMAT d3dFormat;/* = MAKEFOURCC('Y', 'V', '1', '2')*/;
	d3dFormat = D3DFMT_A8R8G8B8;//D3DFMT_R5G6B5;//D3DFMT_A8R8G8B8;
	//更是变小. 数据果然快了一倍
	//yv 12 不能做 render targe
	//d3dFormat = (D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2');
	DWORD dwTime = timeGetTime();

	HRESULT hr = pD3dDev->CreateOffscreenPlainSurface(nW, nH, d3dFormat
		, /*D3DPOOL_DEFAULT*/D3DPOOL_SYSTEMMEM,
		&pSystemSurfaceBuffer,
		NULL);
	if (hr != S_OK)
	{
		return -1;
	}
	IDirect3DSurface9* pSurfaceImage=0;	
	hr = pD3dDev->CreateOffscreenPlainSurface(nW, nH, d3dFormat
		, D3DPOOL_DEFAULT,  //default memcpy copy 速度 特别慢.500 ms/frame
		&pSurfaceImage,
		NULL);
	if (hr != S_OK)
	{
		return -1;
	}
//	else
//	{
//		
//		D3DLOCKED_RECT d3d_rect;
//		if( FAILED(pSurfaceImage->LockRect(&d3d_rect,NULL,D3DLOCK_DONOTWAIT)))
//		{
//			//debug("lock surface false\n");
//			pSurfaceImage->UnlockRect();
//			return -1;
//		}
//		char *pMem = new char[d3d_rect.Pitch*nH*2];
//		dwTime = timeGetTime();
//		memcpy(pMem, d3d_rect.pBits, d3d_rect.Pitch*nH);
//		debug("%s: %d: time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
//
//		pSurfaceImage->UnlockRect();
//		
//		ID3DXBuffer *pD3dBuffer;
// 
//		hr = D3DXCreateBuffer(d3d_rect.Pitch*nH*2, 	&pD3dBuffer);
//		if (hr == S_OK)
//		{// 时间很长 和上面copy更多.
//			dwTime = timeGetTime();
//			hr = D3DXSaveSurfaceToFileInMemory(&pD3dBuffer, D3DXIFF_BMP, pSurfaceImage, NULL, NULL );
//			if (hr == S_OK)
//			{
//				debug("%s: %d: ok time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
//			}else{
//				debug("%s: %d: false time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
//			}
//			pD3dBuffer->Release();
//		}
//
//	
//		pSurfaceImage->Release();
//		delete pMem;
//
//// 		hr = pD3dDev->GetRenderTargetData(pSurfaceImage, pSurfaceBuffer);
//// 		if (hr == S_OK)
//// 		{
//// 			debug("get image ok\n");
//// 		}
//// 		else
//// 		{
//// 			debug("get image false\n");
//// 		}
//// 		debug("%s: %d: time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
//	}


	EnterCriticalSection(&m_hLock) ;

	IDirect3DSurface9 *pRenderSurface = 0;
#ifdef USE_D3D9_EX
	
// 	hr = pD3dDev->CreateRenderTargetEx(nW, nH, d3dFormat
// 		, D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE, 0, false, 
// 		&pRenderSurface,
// 		NULL, 
// 		0);
// 	hr = pD3dDev->CreateOffscreenPlainSurfaceEx(nW, nH, d3dFormat
// 		, D3DPOOL_DEFAULT,  //default memcpy copy 速度 特别慢.500 ms/frame
// 		&pRenderSurface,
// 		NULL,
// 		0);

	hr = pD3dDev->CreateRenderTarget(nW, nH, d3dFormat
		, D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE, 0, false, 
		&pRenderSurface,
		NULL);
#else

	//hr = pD3dDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurface);
	hr = pD3dDev->CreateRenderTarget(nW, nH, d3dFormat
		, D3DMULTISAMPLE_TYPE::D3DMULTISAMPLE_NONE, 0, false, 
		&pRenderSurface,
		NULL);
#endif
	if (hr != S_OK)
	{
		debug("create render target false\n");
		return -2;
	}
	else
	{
		debug("create render target success\n");
	}
	//
	if (hr == S_OK)
	{
		char *pMem = new char[nW*nH*4];
		dwTime = timeGetTime();
		int nTimes = 1;
		for(int i=0; i<nTimes; i++)
		{
			//pSurfaceImage 是 解码出来的 数据 surface
			//首先将 解码的数据 surface 渲染到 render face
			hr = S_OK;
			//不占据cpu,  
			//比较奇怪.单独测试 消耗时间 0  一起和 GetRenderTargetData 测试 消耗20% 左右时间
			hr = pD3dDev->StretchRect(pSurfaceImage, NULL, pRenderSurface, NULL, D3DTEXF_LINEAR);
			if (hr != S_OK)
			{
				debug("update image to render surface false\n");
			}
			else
			{
				//再从 render surface 转移到 system memory 这样就可以拷贝了 不同的显卡 这个函数效率差别很大.
				//intel 的 消耗是 ati的3倍时间 280-316 ms ati的 100-125 ms. 注释掉这个,为30 ms(而且和显卡差别很小.)
				//这个方法会被阻塞并释放一些cpu,即使全部循环,cpu 也只有1-3% 平均2% , 所以不能使用时间计算cpu消耗.实际比这个cpu消耗低很多.

				// front 25ms
				//hr = pD3dDev->GetFrontBufferData(0, pSystemSurfaceBuffer);
				//15路每秒 6% cpu. 集显卡6% 10路27秒. 只能到4路 视频拷贝
				//4.7ms
				hr = pD3dDev->GetRenderTargetData(pRenderSurface, pSystemSurfaceBuffer);
				//特别慢的
				//hr = D3DXLoadSurfaceFromSurface(pSystemSurfaceBuffer, NULL, NULL, pSurfaceImage, NULL, NULL, D3DTEXF_LINEAR, 0);
				if (hr != S_OK)
				{
					switch (hr )
					{
					case D3DERR_DRIVERINTERNALERROR:
						{
							debug("get render target false: D3DERR_DRIVERINTERNALERROR\n");
						}
						break;
					case D3DERR_DEVICELOST:
						{
							debug("get render target false: D3DERR_DEVICELOST\n");
						}
						break;
					case D3DERR_INVALIDCALL:
						{
							debug("get render target false: D3DERR_INVALIDCALL\n");
						}
						break;
					}
					debug("get render target false\n");
				}
				else
				{
					//debug("get render target/D3DXLoadSurfaceFromSurface ok\n");
				}
				//system mem 到内存了
				D3DLOCKED_RECT d3d_rect;
				if( FAILED(pSystemSurfaceBuffer->LockRect(&d3d_rect,NULL,D3DLOCK_DONOTWAIT)))
				{
					debug("lock surface false\n");
					pSystemSurfaceBuffer->UnlockRect();
					continue;
				}
				//是这里占据的剩余cpu
				memcpy(pMem, d3d_rect.pBits, d3d_rect.Pitch*nH);
				//debug("%s: %d: time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
				
				pSystemSurfaceBuffer->UnlockRect();
				//pRenderSurface->
				//
			}
		}
		delete pMem;
		pSurfaceImage->Release();
		debug("%s: %d: time elapsed: %.1f\n", __FUNCTION__, __LINE__, (timeGetTime()-dwTime)/(float)nTimes);
	}
	else
	{
		LeaveCriticalSection(&m_hLock) ;
		return -1;
	}

	pRenderSurface->Release();

	debug("%s: %d: time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
	LeaveCriticalSection(&m_hLock) ;

	//debug("%s: %d: time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);
	hr = D3DXSaveSurfaceToFileA(pSaveFile, D3DXIFF_JPG, pSystemSurfaceBuffer, NULL, NULL);
	if (hr == S_OK)
	{
		debug("save 2 file ok\n");
	}
	else
	{
		debug("save 2 file false\n");
	}
	//debug("%s: %d: time elapsed: %d\n", __FUNCTION__, __LINE__, timeGetTime()-dwTime);

	pSystemSurfaceBuffer->Release();

	return 0;
}

int C3dGraphics::SnapShot( unsigned char *pBuffer, int &nW, int &nH )
{
	EnterCriticalSection(&m_hLock) ;

	IDirect3DDevice9 *pD3dDev = m_pGraphicEngine->GetD3DDevice9();
	if (!pD3dDev)
	{
		LeaveCriticalSection(&m_hLock) ;
		return -1;
	}
	IDirect3DSurface9 *pSurface = 0;
	pD3dDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
	if (pSurface)
	{
		//HRESULT hr = D3DXSaveSurfaceToFileA(pSaveFile, D3DXIFF_JPG, pSurface, NULL, NULL);
		D3DLOCKED_RECT d3d_rect;
		if( FAILED(pSurface->LockRect(&d3d_rect,NULL, D3DLOCK_READONLY)))
		{
			debug("%s: lock surface false\n", __FUNCTION__);
			pSurface->UnlockRect();
			LeaveCriticalSection(&m_hLock) ;
			return -1;
		}
 
		D3DSURFACE_DESC surfaceDesc;
		pSurface->GetDesc(&surfaceDesc);
		nW = surfaceDesc.Width;
		nH = surfaceDesc.Height;
		int nPixBytes = 1;

		int nLinePos = 0;
		for (int i=0; i<surfaceDesc.Height; i++)
		{
			memcpy(pBuffer+nLinePos, (char*)d3d_rect.pBits+nLinePos, d3d_rect.Pitch);
			nLinePos += d3d_rect.Pitch;
		}
		//memcpy(pBuffer, d3d_rect.pBits, );
	 
		pSurface->Release();
	}
	else
	{
		LeaveCriticalSection(&m_hLock) ;
		return -2;
	}
	LeaveCriticalSection(&m_hLock) ;
	return 0;
}

void C3dGraphics::SetGraphicsSize( int nW, int nH )
{
	int nOldW(0), nOldH(0);
	if (!m_pGraphicEngine)
	{
		return ;
	}
	EnterCriticalSection(&m_hLock) ;
	
	m_pGraphicEngine->GetSize(nOldW, nOldH);
	if (nW != nOldW
		|| nH != nOldH)
	{

		m_pGraphicEngine->SetD3dSize(nW, nH);

		Reset();
	}

	LeaveCriticalSection(&m_hLock) ;
}
