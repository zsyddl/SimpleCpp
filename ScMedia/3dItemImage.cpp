#include "StdAfx.h"
#include "3ditemimage.h"
#include "3dGraphics.h"
#include "D3dPresenter.h"
/*#include "Yuv2Rgb.h"*/

#define  TRACE

typedef unsigned char uint8_t ;
C3dItemImage::C3dItemImage(C3dGraphics *p3dGraphic, int nTexturePixW, int nTexturePixH, e3dPixFormat pixFormat )
:C3dGraphicItem(p3dGraphic, nTexturePixW, nTexturePixH)
, m_3dPixFormat(pixFormat)
//, m_pD3dFont(0)
, m_pD3dSurface(0)
, m_pSysSurface(0)
{
	//255, 0, 135, 0
	m_nSrcMaskColor = D3DCOLOR_ARGB(0, 0, 0, 0);
}

C3dItemImage::~C3dItemImage(void)
{
	/*
	if (m_pD3dFont)
	{
		m_pD3dFont->Release();
		m_pD3dFont = NULL;
	}*/
	if (m_pD3dSurface)
	{
		m_pD3dSurface->Release();
		m_pD3dSurface = NULL;
	}
	if (m_pSysSurface)
	{
		m_pSysSurface->Release();
		m_pSysSurface = NULL;
	}

}
int C3dItemImage::Create(float fX, float fY, float fZ)
{
	C3dGraphicItem::Create(fX, fY, fZ);

	IDirect3DDevice9* pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
	if (pD3dDev)
	{
		HRESULT hr=S_FALSE ;
		//sys->d3d(yuv)->d3d rgb
		if (m_3dPixFormat == e3dPF_YV420)
		{
			//d3d de 
			hr = pD3dDev->CreateOffscreenPlainSurface(m_nTextureXPix, m_nTextureYPix, (D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2')
				, D3DPOOL_DEFAULT,
				&m_pD3dSurface,
				NULL);
			//系统的
			hr = pD3dDev->CreateOffscreenPlainSurface(m_nTextureXPix, m_nTextureYPix, (D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2')
				, D3DPOOL_SYSTEMMEM,
				&m_pSysSurface,
				NULL);
			//
			if (hr == S_OK)
			{
				debug("create off screen sys surface ok \n");
			}
			else
			{
				debug("create off screen sys surface false \n");
			}
			//yv12 需要混合 的 texture 必须是 rgb 不能是rgba
			if (m_pRendTexture)
			{
				m_pRendTexture->Release();
				m_pRendTexture = NULL;
			}
			hr = D3DXCreateTexture(	
				pD3dDev,
				m_nTextureXPix,
				m_nTextureYPix,
				0,
				D3DUSAGE_RENDERTARGET ,
				D3DFMT_R8G8B8,
				D3DPOOL_DEFAULT,
				&m_pRendTexture);
			if (FAILED(hr))
			{
				TRACE("创建 渲染纹理 失败\n");
				return S_FALSE;
			}
			else
			{
				TRACE("创建 渲染纹理 成功\n");
			}
			if (m_pSysTexture)
			{
				m_pSysTexture->Release();
				m_pSysTexture = NULL;
			}
			hr = D3DXCreateTexture(	
				pD3dDev,
				m_nTextureXPix,
				m_nTextureYPix,
				1,
				/*D3DUSAGE_DYNAMIC*/0,
				D3DFMT_R8G8B8,
				D3DPOOL_SYSTEMMEM,
				&m_pSysTexture);
			if (FAILED(hr))
			{
				TRACE("创建sys 纹理失败\n");
				return S_FALSE;
			}
			else
			{
				TRACE("创建sys 纹理成功\n");
			}
		}
		if (hr == S_OK)
		{
			TRACE("create yv12 off surface success\n");
		}
		else
		{
			TRACE("create yv12 off surface false\n");
		}
	}


	/*
	D3DXFONT_DESC   d3dFont;

	memset(&d3dFont,0,sizeof(d3dFont));
	d3dFont.Height=25;   //   in   logical   units
	d3dFont.Width=12;     //   in   logical   units
	d3dFont.Weight=500;//   boldness,   range   0(light)   -   1000(bold)
	d3dFont.Italic=FALSE;
	d3dFont.CharSet=DEFAULT_CHARSET;
	//strcpy(d3dFont.FaceName, "Tahoma");
	d3dFont.FaceName[0] = 'T';
	d3dFont.FaceName[1] = 'a';
	d3dFont.FaceName[2] = 'h';
	d3dFont.FaceName[3] = 'o';
	d3dFont.FaceName[4] = 'm';
	d3dFont.FaceName[5] = 'a';

	//m_pD3dFont->
	D3DXCreateFontIndirect(pD3dDev, &d3dFont, &m_pD3dFont); 
	*/
	return 0;
}
int C3dItemImage::Render(C3dGraphics *pGraphic)
{
	if (m_3dPixFormat != e3dPF_YV420)
	{

		IDirect3DDevice9* pDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
		HRESULT hr  = pDev->UpdateTexture(m_pSysTexture, m_pRendTexture);
		if (FAILED(hr))
		{
			TRACE("更新 渲染纹理 失败\n");
			return S_FALSE;
		}
		else
		{
			TRACE("更新 渲染纹理 成功\n");
		}
	}
	C3dGraphicItem::Render(pGraphic);

	return 1;
}
int C3dItemImage::SetImageData(char *pData, int nW, int nH, int nPixFormat)
{
	if (!pData)
		return -1;
	if (!m_pRendTexture || !m_pD3dSurface || !m_pSysTexture)
		return -1;
	if (nPixFormat != m_3dPixFormat)
	{
		return -2;
	}
	/*return 0;*/
	HRESULT hr = S_OK;
	D3DLOCKED_RECT	d3dLock;

	//memset(d3dLock.pBits, 150, nWidth*nHeight);
	long lLen = (m_nTextureXPix)*(m_nTextureYPix);
	//只是 亮度 
	/*memcpy(d3dLock.pBits, pData, lLen );*/
	//char *pPixRGBData = new char[m_nTextureYPix*m_nTextureXPix*3];
	if ( nPixFormat ==  e3dPF_YV420)
	{
		//pPixRGBData = new char[m_nTextureYPix*m_nTextureXPix*3];
		//YUV_TO_RGB24((unsigned char*)pData, m_nTextureXPix, (unsigned char*)pData+m_nTextureXPix*m_nTextureYPix, (unsigned char*)pData+m_nTextureXPix*m_nTextureYPix*5/4, 
		//	         m_nTextureXPix/2, (unsigned char*)pPixRGBData, m_nTextureXPix, m_nTextureYPix, m_nTextureXPix*3 );
		//使用 surface 转换
		//hr = m_pSysTexture->UnlockRect(0);
		IDirect3DDevice9* pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();

		DWORD dwTime = timeGetTime();
		D3DLOCKED_RECT d3d_rect;
		//消耗cpu极少. 20倍 还没有增加 解码的50%的cpu
		//不知道为什么 使用 sys 到 d3d的 surface 更新会有问题. 内存访问错误
		//for (int i0=0 ;i0<20; i0++){
		if( FAILED(m_pD3dSurface->LockRect(&d3d_rect,NULL,D3DLOCK_DONOTWAIT)))
		{
			//debug("lock surface false\n");
			m_pD3dSurface->UnlockRect();
			return -1;
		}
		const int w = m_nTextureXPix, h = m_nTextureYPix;
		BYTE * const p = (BYTE *)d3d_rect.pBits;
		const int stride = d3d_rect.Pitch;
		//memset(p, 200, w*h);
		int i = 0;
 		for(i = 0;i < h;i ++)
 		{
 			memcpy(p + i * stride, pData + i * w, w);
 		}
 		for(i = 0;i < h/2;i ++)
 		{
 			memcpy(p + stride * h + i * stride / 2,pData + w * h + w * h /4 + i * w / 2, w / 2);
 		}
 		for(i = 0;i < h/2;i ++)
 		{
 			memcpy(p + stride * h + stride * h / 4 + i * stride / 2,pData + w * h + i * w / 2, w / 2);
 		}
		if( FAILED(m_pD3dSurface->UnlockRect()))
		{
			debug("unlock surface false\n");
			return -1;
		}
		//}
		//debug("%s: memcpy to surface time elapsed : %d\n", __FUNCTION__, timeGetTime() -dwTime);
		
		if (hr !=S_OK){
			TRACE("get surface false: %0x\n", hr);
		}
		
		//sys->d3d 
		//返回值正常 但是 失败.有错误信息打印提示 
		//hr = pD3dDev->UpdateSurface(m_pSysSurface, NULL, pSurfaceRender, NULL);
		if (hr !=S_OK){
			TRACE("UpdateSurface surface false: %0x\n", hr);
		}
		//
		
		IDirect3DSurface9 *pSurfaceRender=0;
		hr = m_pRendTexture->GetSurfaceLevel(0, &pSurfaceRender);
		if (pD3dDev && pSurfaceRender)
		{//d3d yuv->d3d rgb render
			//hr = pD3dDev->UpdateSurface(m_pD3dSurface, NULL, pSurfaceRender, NULL);
			hr = pD3dDev->StretchRect(m_pD3dSurface, NULL, pSurfaceRender, NULL, D3DTEXF_LINEAR);
			if (hr != S_OK)
			{
				/*D3DERR_INVALIDCALL*/
				debug("updata surface false: %0x\n", hr);
			}
			// must release
			pSurfaceRender->Release();


		}

		//hr = pD3dDev->UpdateTexture(m_pSysTexture, m_pRendTexture);

	}
	else if(m_3dPixFormat == e3dPF_RGBA){
		hr = m_pSysTexture->LockRect(0, &d3dLock, NULL, D3DLOCK_DISCARD);
		if (hr != S_OK && hr != D3D_OK)
		{
			TRACE("m_pRTYTexture->LockRect false\n");
			return -1;
		}
		memcpy(d3dLock.pBits, pData, m_nTextureXPix*m_nTextureYPix*4);


		hr = m_pSysTexture->UnlockRect(0);

		hr = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9()->UpdateTexture(m_pSysTexture, m_pRendTexture);
		
	}


	return 0;
}

//从 内存中的 文件加载数据： jpeg, bmp, png 等
int C3dItemImage::SetImageFromFile(char *pData, int nDataLen)
{
	HRESULT hr = S_OK;
	IDirect3DSurface9 *pSurfaceRender=0;
	m_pSysTexture->GetSurfaceLevel(0, &pSurfaceRender);
	if (!pSurfaceRender)
		return -1;

	D3DXLoadSurfaceFromFileInMemory(pSurfaceRender, NULL, NULL, 
				pData, nDataLen, 
				NULL, D3DX_DEFAULT ,
				m_nSrcMaskColor,
				NULL);

	pSurfaceRender->Release();

	return 0;
}

int C3dItemImage::GetImageData(char *pData, int &nW, int &nH)
{
	HRESULT hr = S_FALSE;
	
	IDirect3DSurface9 *pSurfaceSrc(0), *pSurfaceDes(0);
	m_pSysTexture->GetSurfaceLevel(0, &pSurfaceDes);
	m_pRendTexture->GetSurfaceLevel(0, &pSurfaceSrc);
//  	hr = D3DXLoadSurfaceFromSurface(pSurfaceDes, NULL, NULL, pSurfaceSrc, NULL, NULL ,
//  		D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER, 0);

	IDirect3DDevice9* pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
	IDirect3DTexture9 *text(0);
	IDirect3DSurface9* surf(0);
// 	hr = pD3dDev->CreateTexture(m_nTextureXPix, m_nTextureYPix, 1, /*D3DUSAGE_AUTOGENMIPMAP*/D3DUSAGE_DYNAMIC,
// 		D3DFMT_R8G8B8, D3DPOOL_SYSTEMMEM,
// 		&text, NULL);

//	text->GetSurfaceLevel(0,&surf);
	hr = pD3dDev->GetRenderTargetData(pSurfaceSrc, pSurfaceDes);
	pSurfaceDes->Release();
	pSurfaceSrc->Release();

	D3DLOCKED_RECT d3d_rect;
	IDirect3DSurface9 *pSurface=0;
	hr = m_pSysTexture->GetSurfaceLevel(0, &pSurface);
	if( FAILED(pSurface->LockRect(&d3d_rect,NULL, D3DLOCK_READONLY)))
	{
		//debug("lock surface false\n");
		pSurface->UnlockRect();
		return -1;
	}
 
	D3DSURFACE_DESC surfaceDesc;
	pSurface->GetDesc(&surfaceDesc);
	nW = surfaceDesc.Width;
	nH = surfaceDesc.Height;
 
	int nLinePos = 0;
	for (int i=0; i<surfaceDesc.Height; i++)
	{
		memcpy(pData+nLinePos, (char*)d3d_rect.pBits+nLinePos, d3d_rect.Pitch);
		nLinePos += d3d_rect.Pitch;
	}
	pSurface->UnlockRect();
	//memcpy(pBuffer, d3d_rect.pBits, );

	pSurface->Release();

	return 0;
}
int C3dItemImage::GetImageData(char *pFile)
{
	IDirect3DDevice9* pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
	
	IDirect3DSurface9 *pSurface(0);
	m_pRendTexture->GetSurfaceLevel(0, &pSurface);
	HRESULT hr = D3DXSaveSurfaceToFileA(pFile, D3DXIFF_BMP , pSurface, NULL, NULL);
	if (hr == S_OK)
	{
		debug("save 2 file ok\n");
	}
	else
	{
		debug("save 2 file false\n");
	}
	pSurface->Release();

	return 0;
}
int		C3dItemImage::ReCreate(C3dGraphics *p3dGraphics)
{

	m_p3dGraphic = p3dGraphics;

	Release();
	//
	HRESULT hRt = Create(m_fX, m_fY, m_fZ);

	return hRt;
}
int C3dItemImage::Release()
{
	C3dGraphicItem::Release();
/*	if (m_pD3dFont)
	{
		m_pD3dFont->Release();
		m_pD3dFont = NULL;
	}*/
	if (m_pD3dSurface)
	{
		m_pD3dSurface->Release();
		m_pD3dSurface = NULL;
	}
	if (m_pSysSurface)
	{
		m_pSysSurface->Release();
		m_pSysSurface = NULL;
	}

	return 0;
}