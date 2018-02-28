#include "StdAfx.h"
#include ".\d3dpresenter.h"

#define FAILED_RETURN_hr
CD3dPresenter::CD3dPresenter(void)
:m_hWnd(0)
, m_pD3D9(0)
, m_pD3DDevice(0)
, m_nD3dHeight(720)
, m_nD3dWidth(1280)
{
}

CD3dPresenter::~CD3dPresenter(void)
{
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	if (m_pD3D9)
	{
		m_pD3D9->Release();
		m_pD3D9 = NULL;
	}
}
HRESULT	CD3dPresenter::Init(	HWND			hWnd, 
					  D3DSWAPEFFECT	SwapEffect,
					  UINT			BackBufferCount,	/*=0*/
					  D3DFORMAT		BackBufferFormat,	/*=D3DFMT_UNKNOWN*/
					  UINT			BackBufferWidth,	/*= 0*/
					  UINT			BackBufferHeight,	/*= 0*/
					  BOOL			bFullScreen,		/*=FALSE*/
					  UINT			FullScreen_RefreshRateInHz,	/*=D3DPRESENT_RATE_DEFAULT*/
					  UINT			PresentationInterval		/*=D3DPRESENT_INTERVAL_DEFAULT*/)
{
	if (!m_pD3D9)
	{
		//if(NULL == (m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
#ifdef USE_D3D9_EX
		Direct3DCreate9Ex(D3D_SDK_VERSION, &m_pD3D9);
		if( !m_pD3D9)
		{
			debug("%s, %d: Direct3DCreate9Ex false\n", __FUNCTION__, __LINE__);
			return E_FAIL;
		}
#else
		if(NULL == (m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
		{
			debug("%s, %d: Direct3DCreate9 false\n", __FUNCTION__, __LINE__);
			return E_FAIL;
		}
#endif
	}
	m_nD3dWidth = BackBufferWidth;
	m_nD3dHeight = BackBufferHeight;
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	// 重新创建会卡死在这里 ReCreate
// 	DWORD dwStyle;
// 	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
// 	dwStyle |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
// 	SetWindowLong(hWnd, GWL_STYLE, dwStyle);

	RECT rcWnd;
	GetWindowRect(hWnd, &rcWnd);
	//ShowWindow(hWnd, SW_HIDE);
	m_hWnd = hWnd;
	//Get the current display mode
	//debug("%s, %d\n", __FUNCTION__, __LINE__);
	m_pD3D9->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &m_dm);
	//debug("%s, %d\n", __FUNCTION__, __LINE__);
	ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
	m_d3dpp.SwapEffect		= SwapEffect;

	/*	SwapEffect = D3DSWAPEFFECT_FLIP;*/

	if(D3DSWAPEFFECT_COPY == SwapEffect)
		m_d3dpp.BackBufferCount	= 1;
	else
		m_d3dpp.BackBufferCount	= BackBufferCount;

	m_d3dpp.BackBufferFormat	= BackBufferFormat;
	if(m_d3dpp.BackBufferFormat == D3DFMT_UNKNOWN)
		m_d3dpp.BackBufferFormat = m_dm.Format;

	if(bFullScreen)
	{
		m_d3dpp.Windowed						= FALSE;
		m_d3dpp.hDeviceWindow					= hWnd;
		m_d3dpp.FullScreen_RefreshRateInHz		= FullScreen_RefreshRateInHz;
		m_d3dpp.PresentationInterval			= PresentationInterval;

		if(BackBufferWidth == 0)
			m_d3dpp.BackBufferWidth	= m_dm.Width;
		else
			m_d3dpp.BackBufferWidth	= BackBufferWidth;

		if(BackBufferHeight == 0)
			m_d3dpp.BackBufferHeight	= m_dm.Height;
		else
			m_d3dpp.BackBufferHeight	= BackBufferHeight;

	}
	else
	{
		m_d3dpp.Windowed   = TRUE;
		if(BackBufferWidth != 0)
			m_d3dpp.BackBufferWidth = BackBufferWidth;
		if(BackBufferHeight!= 0)
			m_d3dpp.BackBufferHeight= BackBufferHeight;

		m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_ONE;
		

	}
	//debug("%s, %d\n", __FUNCTION__, __LINE__);
	HRESULT hr = NOERROR;
	/*    ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));*/
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.hDeviceWindow = hWnd;
	//	m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	//	m_d3dpp.BackBufferCount = 3;
	m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;// m_dm.Format;
	/*	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;*/
	//	m_d3dpp.BackBufferHeight = 768;
	//	m_d3dpp.BackBufferWidth = 1024;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD/*D3DSWAPEFFECT_COPY*/;//D3DSWAPEFFECT_DISCARD;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES/*D3DMULTISAMPLE_NONMASKABLE*//*D3DMULTISAMPLE_NONE*/ /*D3DMULTISAMPLE_4_SAMPLES*/;
	m_d3dpp.MultiSampleQuality = 0;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //生成16 bit的Z缓存
	m_d3dpp.Flags = 0;//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//函数检查当前设备是否支持图形多重采样(除锯齿);P171
	D3DMULTISAMPLE_TYPE multiSampleType = m_d3dpp.MultiSampleType;
	D3DDEVTYPE MultisampleDevtype=(D3DDEVTYPE )-1;
	DWORD numq=0;
	//debug("%s, %d\n", __FUNCTION__, __LINE__);
	if( SUCCEEDED(m_pD3D9->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_REF , D3DFMT_A8R8G8B8, true, 
		multiSampleType, &numq ) ) )
	{

		MultisampleDevtype=D3DDEVTYPE_REF;//REF软件模式
	}
	//debug("%s, %d\n", __FUNCTION__, __LINE__);
	if( SUCCEEDED(m_pD3D9->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL , D3DFMT_A8R8G8B8, true, 
		multiSampleType, &numq  ) ) )
	{

		MultisampleDevtype=D3DDEVTYPE_HAL;//HAL硬件层
	}
	//禁用 多纹理 有这个 会无法抓图.
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	numq = 0;

	if (numq >= 1)
		m_d3dpp.MultiSampleQuality = numq-1;

// 	if(MultisampleDevtype==(D3DDEVTYPE )-1)
// 		return E_FAIL;

	D3DDEVTYPE d3dDevType = D3DDEVTYPE_HAL;
	hr = m_pD3D9->CheckDeviceType(	D3DADAPTER_DEFAULT, 
		d3dDevType, 
		m_dm.Format,
		m_d3dpp.BackBufferFormat,
		m_d3dpp.Windowed);

	FAILED_RETURN_hr;
	m_d3dDevType = d3dDevType;
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	int nCount = m_pD3D9->GetAdapterCount();
	int nAdapter = D3DADAPTER_DEFAULT;
	HMONITOR hMonitorCur = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	for (int i=0; i<nCount; i++)
	{
		HMONITOR hMonitor = m_pD3D9->GetAdapterMonitor(i);
		if (hMonitor == hMonitorCur)
		{
			nAdapter = i;
		}
	}

	HWND hParent;
	hParent = GetParent(hWnd);
	//SetParent(hWnd, 0);
	//ShowWindow(hWnd, SW_HIDE);
	
	//D3DCREATE_FPU_PRESERVE 参数 和 qWebView 不一致,
	//程序要 使用 D3DCREATE_FPU_PRESERVE ，否则会修改fpu精度到24bit。并且不会恢复默认设置，导致程序运行的其他模块出错
	//
	debug("%s, %d: Adapter count %d, use %d.\n", __FUNCTION__, __LINE__, nCount, nAdapter);
#ifdef USE_D3D9_EX
	D3DDISPLAYMODEEX diplayModeEx;
	diplayModeEx.Size = sizeof(diplayModeEx);
	diplayModeEx.Format = m_d3dpp.BackBufferFormat;
	diplayModeEx.Height = m_d3dpp.BackBufferHeight;
	diplayModeEx.Width  = m_d3dpp.BackBufferWidth;
	diplayModeEx.RefreshRate = 25;
	diplayModeEx.ScanLineOrdering = D3DSCANLINEORDERING_INTERLACED;
	hr = m_pD3D9->CreateDeviceEx(	nAdapter, 
		m_d3dDevType, 
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED|D3DCREATE_FPU_PRESERVE/*|D3DCREATE_NOWINDOWCHANGES*/,
		&m_d3dpp,
		NULL,
		&m_pD3DDevice);
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	if(FAILED(hr))
	{
		debug("%s, %d false: %d\n", __FUNCTION__, __LINE__, hr);
		hr = m_pD3D9->CreateDeviceEx(	nAdapter, 
			m_d3dDevType, 
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED|D3DCREATE_FPU_PRESERVE ,
			&m_d3dpp,
			NULL,
			&m_pD3DDevice);
		
		FAILED_RETURN_hr;
	}
#else

	hr = m_pD3D9->CreateDevice(	nAdapter, 
		m_d3dDevType, 
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED|D3DCREATE_FPU_PRESERVE/*|D3DCREATE_NOWINDOWCHANGES*/,
		&m_d3dpp,
		&m_pD3DDevice);
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	if(FAILED(hr))
	{
		hr = m_pD3D9->CreateDevice(	nAdapter, 
			m_d3dDevType, 
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED|D3DCREATE_FPU_PRESERVE ,
			&m_d3dpp,
			&m_pD3DDevice);

		FAILED_RETURN_hr;
	}
#endif
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	if (m_pD3DDevice)
	{
		hr = m_pD3DDevice->GetDeviceCaps(&m_d3dCaps9);
		FAILED_RETURN_hr;
	}
	if (m_pD3DDevice && m_d3dpp.MultiSampleType != D3DMULTISAMPLE_NONE)
	{
		m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS ,true);
	}

	return hr;
}
int CD3dPresenter::Create(HWND hWnd)
{


	HRESULT hr = S_OK;
	m_hWnd = hWnd;
	hr = Init(hWnd, D3DSWAPEFFECT_DISCARD, 1);

	//如果是POW2的显卡，就让它刷新频率小一些
	//if(IsTexturePow2())
	//	SetFrameTime(30);

    ClearScene(D3DCOLOR_XRGB(0, 0, 150));
	return 0;
}
int CD3dPresenter::ReCreate()
{
	if ( m_hWnd <= 0)
		return -1;
	
	HRESULT hr = S_OK;
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	hr = Init(m_hWnd, D3DSWAPEFFECT_DISCARD, 1, D3DFMT_UNKNOWN, m_nD3dWidth, m_nD3dHeight);
	debug("%s, %d\n", __FUNCTION__, __LINE__);
	//如果是POW2的显卡，就让它刷新频率小一些
	//if(IsTexturePow2())
	//	SetFrameTime(30);

	ClearScene(D3DCOLOR_XRGB(0, 0, 150));
	return 0;
}
void CD3dPresenter::ClearScene(D3DCOLOR clrBkg)
{
	
}
void CD3dPresenter::SetViewPort()
{
	if (!m_pD3DDevice)
		return ;
	D3DXVECTOR3 eye( 0.0f, 0.0f, -2.414213f/*-2.43f*/ ); //观察点
	D3DXVECTOR3 lookat( 0.0f, 0.0f, 0.0f ); //视线目标点
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f ); //上方向
	D3DXMATRIX  matView;
	//计算视角变换矩阵
	::D3DXMatrixLookAtLH( &matView, &eye, &lookat, &up );
	//把视角变换矩阵设置到渲染环境
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
	D3DXMATRIXA16 matProj;
	//计算透视投影变换矩阵//45度，距离 2.414213刚好完整看见-1,-1, -1,1, 1,1, 1, -1, 的部分
	::D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f*LOG_UNIT_SIZE );
	//把投影变换矩阵设置到渲染环境
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );	
}
void CD3dPresenter::RenderBegin()
{
	HRESULT hr = S_OK;
	if (!m_pD3DDevice)
		return ;
	//CSingleLock  lock(&m_lockSec);
	//lock.Lock();
	//用指定颜色清除/后备缓存区
	hr = m_pD3DDevice->Clear(
		0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		//D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f), //指定使用red色 rgba 0-1.0
				D3DCOLOR_RGBA(0, 0, 0, 255), //指定使用蓝色
		1.0f, 0);

//	m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, /*D3DTEXF_GAUSSIANQUAD*//*D3DTEXF_PYRAMIDALQUAD*//*D3DTEXF_POINT*/D3DTEXF_LINEAR/*D3DTEXF_ANISOTROPIC*/);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, /*D3DTEXF_GAUSSIANQUAD*//*D3DTEXF_PYRAMIDALQUAD*//*D3DTEXF_POINT*/D3DTEXF_LINEAR/*D3DTEXF_ANISOTROPIC*/);

//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER,  D3DTEXF_LINEAR);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL,  4);
//	
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 32);
	hr = m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pD3DDevice->BeginScene();
	SetViewPort();
	//纹理的过虑采样方式，
//	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,   TRUE);
	//D3DTEXF_GAUSSIANQUAD 

	
}
void CD3dPresenter::RenderEnd()
{
	if (!m_pD3DDevice)
		return ;
	//CSingleLock lock(&m_lockSec);
	//lock.Lock();
	m_pD3DDevice->EndScene();

	//交换当前/后备缓存区，刷新窗口
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
void CD3dPresenter::SetD3dSize( int nW, int nH )
{
	m_nD3dHeight = nH;
	m_nD3dWidth = nW;
}
