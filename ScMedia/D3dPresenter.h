#pragma once

#include "tchar.h"
#include "dshow.h"
#include "atlbase.h"
#include "d3d9.h"
#include "d3dx9.h"
/*#include "afxmt.h"*/

#define LOG_UNIT_SIZE (1.0f)
#define LOG_VIEW_W (1.0f)
#define LOG_VIEW_H (1.0f)
#define LOG_VIEW_Z (1.0f)
class CD3DTexture{
public:
	CD3DTexture();
	~CD3DTexture();

	
};
#define USE_D3D9_EX

/*
送入一个图像，给显示出来。 可以设置fps
*/
class CD3dPresenter
{
public:
	CD3dPresenter(void);
	~CD3dPresenter(void);

	int Create(HWND hWnd);
	void ClearScene(D3DCOLOR clrBkg);
	HRESULT				Init(	HWND			hWnd,\
		D3DSWAPEFFECT	SwapEffect,\
		UINT			BackBufferCount		= 0,\
		D3DFORMAT		BackBufferFormat	= D3DFMT_UNKNOWN,\
		UINT			BackBufferWidth		= 1024,\
		UINT			BackBufferHeight	= 768,\
		BOOL			bFullScreen			= FALSE,\
		UINT			FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT,\
		UINT			PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT);

	void				SetViewPort();
	void				RenderBegin();
	void				RenderEnd();	
	IDirect3DDevice9*	GetD3DDevice9() { return m_pD3DDevice; }
	IDirect3D9*	        GetD3D9() { return m_pD3D9; }
	//显示 一个图片
	int					RenderPicture(char *pBuff, int nLen, int nPixFormat);

	int ReCreate();
	void GetSize(int &nW, int &nH){nW = m_d3dpp.BackBufferWidth; nH = m_d3dpp.BackBufferHeight;};

	void SetD3dSize(int nW, int nH);

protected:
#ifdef USE_D3D9_EX
	IDirect3D9Ex					*m_pD3D9;
	IDirect3DDevice9Ex				*m_pD3DDevice;
#else
	IDirect3D9						*m_pD3D9;
	IDirect3DDevice9				*m_pD3DDevice;
#endif
	D3DDEVTYPE						m_d3dDevType;


	D3DCAPS9						m_d3dCaps9;
	D3DDISPLAYMODE					m_dm;
	D3DPRESENT_PARAMETERS			m_d3dpp; 

	HWND							m_hWnd;
	int m_nD3dWidth;
	int m_nD3dHeight;
	//用于显示的
// 	CComPtr<IDirect3DSurface9>		m_pSysSurface;
// 	CComPtr<IDirect3DTexture9>		m_pRTTexture;
	//同步加锁
	//CCritSec						m_lockSec;
	//CCriticalSection				m_lockSec;
};
