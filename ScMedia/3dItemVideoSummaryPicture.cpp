#include "StdAfx.h"
#include "3dItemVideoSummaryPicture.h"
#include "3dGraphics.h"
#include "3dItemText2d.h"
#include "D3dPresenter.h"
 
C3dItemVideoSummaryPicture::C3dItemVideoSummaryPicture(C3dGraphics *p3dGraphic, int nTexturePixW/*=600*/, int nTexturePixH/*=480*/, e3dPixFormat pixFormat/*=ImgPF_YV420*/)
:C3dItemImage(p3dGraphic, nTexturePixW, nTexturePixH, pixFormat)
, m_p3dItem2dText(NULL)
, m_s64TimeStamp(0)
, m_nTimeBaseNum(0)
, m_nTimeBaseDen(1)
, m_s64StartTimestamp(0)
, m_s64ObjectId(-1)
, m_nCornerLineLen(8)
, m_pMask(NULL)
, m_nBlockRows(0)
, m_nBlockCols(0)
, m_bEnableMask(false)
{
}

C3dItemVideoSummaryPicture::~C3dItemVideoSummaryPicture(void)
{
	if (m_p3dItem2dText)
	{
		m_p3dGraphic->RemoveItem(m_p3dItem2dText);

		delete m_p3dItem2dText;
		m_p3dItem2dText = NULL;
	}
	if (m_pMask)
	{
		delete m_pMask;
		m_pMask = NULL;
	}
}

int C3dItemVideoSummaryPicture::Create(float fX, float fY, float fZ)
{
	
	C3dItemImage::Create(fX, fY, fZ);
	//这个很消耗时间
	if (!m_p3dItem2dText)
	{
		m_p3dItem2dText = new C3dItemText2d(m_p3dGraphic);
		m_p3dItem2dText->Create(fX, fY, fZ);

		m_p3dGraphic->AppendItem(m_p3dItem2dText);

		//m_p3dItem2dText->SetText("00:00:00");
	}

	m_nMaskBlockSize = 8;
	m_nBlockCols = m_nTextureXPix/m_nMaskBlockSize;
	m_nBlockRows = m_nTextureYPix/m_nMaskBlockSize;
	m_pMask = new uint8[m_nBlockCols*m_nBlockRows];

	memset(m_pMask, 255, m_nBlockRows*m_nBlockCols);
	/*int nLinePos = 0;
	for (int y=0; y<m_nMaskH/2; y++)
	{
		for (int x=0; x<m_nMaskW; x++)
		{
			m_pMask[nLinePos+x] = 1;
		}
		nLinePos += m_nMaskW;
	}*/


	return 0;
}
void C3dItemVideoSummaryPicture::ReCreateGeometry()
{
	if (!m_p3dGraphic)
		return ;
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
	//int nBlockRows = 3;; //m_nTextureXPix/m_nMaskBlockSize;
	//int nBlockCols = 3;; // m_nTextureYPix/m_nMaskBlockSize;
	//memset(m_pMask, 255, nBlockRows*nBlockCols);
	if (m_nBlockCols >= 4
		&& m_nBlockRows >= 4)
	{
		//m_nBlockCols = 4;// nBlockCols;
		//m_nBlockRows = 4; //nBlockRows;
	}

// 	if (m_nBlockRows >= 2
// 		&& m_nBlockCols >= 2)
// 	{
// 		m_nBlockCols /= 2;
// 		m_nBlockRows /= 2;
// 	} 


	int nBlockCols = m_nBlockCols;
	int nBlockRows = m_nBlockRows;
	memset(m_pMask, 255, nBlockRows*nBlockCols);
	//m_fAlpha = 1.0;

	m_nVertexPoints = (m_nBlockCols)*2*3*(m_nBlockRows);

	m_nPrimitiveCount = m_nBlockCols*m_nBlockRows*2;
	
 
	IDirect3DDevice9* p3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();//m_pDMD3D9->GetD3DDevice9();
	if (p3dDev == NULL)
	{
		return  ;
	}
	//创建顶点缓存区，并获取接口IDirect3DVertexBuffer9的指针
	HRESULT hr = p3dDev->CreateVertexBuffer(
		sizeof(VERTEX_CUSTOM1) * m_nVertexPoints , //缓存区尺寸
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );
	//把顶点数据填入顶点缓存区
	void* pVertices = 0;
	hr = m_pVertexBuffer->Lock( 0, sizeof(VERTEX_CUSTOM1) * m_nVertexPoints, (void**)&pVertices, 0 );
	//memcpy( pVertices, verticesRect, sizeof(verticesRect) );
	VERTEX_CUSTOM1 *pVerticePoints = (VERTEX_CUSTOM1 *)pVertices;
	VERTEX_CUSTOM1 *pVerticePoint = (VERTEX_CUSTOM1 *)pVertices;
	//更别做每一行的 顶点，每一行作为三角形条带
	unsigned char *pMask = m_pMask;
	float fXOff = -0.5;
	float fYOff = -0.5;
	float fAlpha;
	for (int y=0; y<nBlockRows; y++)
	{
		float fY = -1.0/nBlockRows*y + 0.5;
		float fX = 1.0/nBlockCols;
		float fTy = 1.0/nBlockRows*y/* + 0.01*/;

		pMask = m_pMask+y*nBlockCols;

		//bl
		pVerticePoint->x = -0.5;
		pVerticePoint->z = 0;
		pVerticePoint->y =  -1.0/nBlockRows*(y+1)+0.5;
		pVerticePoint->tx = 0;
		pVerticePoint->ty = 1.0/nBlockRows*(y+1);;
		fAlpha = m_fAlpha * 0;
		pVerticePoint->color = D3DCOLOR_ARGB((uint8)fAlpha, 0, 0, 0);

		pVerticePoint++;
		//tl
		pVerticePoint->x = -0.5;
		pVerticePoint->z = 0;
		pVerticePoint->y = fY;
		pVerticePoint->tx = 0;
		pVerticePoint->ty = fTy;// 1.0/nBlockRows*y;
		fAlpha = m_fAlpha * 0;
		pVerticePoint->color = D3DCOLOR_ARGB((uint8)fAlpha, 0, 0, 0);

		pVerticePoint++;

		//br
		pVerticePoint->x = 1.0/nBlockCols-0.5;
		pVerticePoint->z = 0;
		pVerticePoint->y = -1.0/nBlockRows*(y+1)+0.5;
		pVerticePoint->tx = 1.0/nBlockCols;
		pVerticePoint->ty = 1.0/nBlockRows*(y+1);
		fAlpha = m_fAlpha * (*pMask);
		pVerticePoint->color = D3DCOLOR_ARGB((uint8)fAlpha, 0, 0, 0);

		pVerticePoint++;

		*pVerticePoint = pVerticePoint[-2];
		pVerticePoint++;
		//tr
		//pMask++;
		pVerticePoint->x = 1.0/nBlockCols-0.5;
		pVerticePoint->z = 0;
		pVerticePoint->y = fY;
		pVerticePoint->tx = 1.0/nBlockCols;
		pVerticePoint->ty = fTy; //1.0/nBlockRows*y;
		fAlpha = m_fAlpha * (*pMask);
		pVerticePoint->color = D3DCOLOR_ARGB((uint8)fAlpha, 0, 0, 0);

		pVerticePoint++;


		*pVerticePoint = pVerticePoint[-3];
		pVerticePoint++;

	
		for (int x=2; x<=nBlockCols; x++)
		{
			if (x == nBlockCols)
			{
				fAlpha = 0;
			}
			else
			{
				fAlpha = (*pMask);
			}
			*pVerticePoint = pVerticePoint[-1];
			pVerticePoint++;

			*pVerticePoint = pVerticePoint[-3];
			pVerticePoint++;

			pVerticePoint->x = (float)x/(float)nBlockCols - 0.5;
			pVerticePoint->y = -(float)(y+1)/(float)nBlockRows + 0.5;
			pVerticePoint->z = 0;
			fAlpha = m_fAlpha * fAlpha;
			pVerticePoint->color = D3DCOLOR_ARGB((uint8)fAlpha, 0, 0, 0);
			pVerticePoint->tx = (float)x/(float)nBlockCols;
			pVerticePoint->ty = (float)(y+1)/(float)nBlockRows;

			pVerticePoint++;
		
			 
			*pVerticePoint = pVerticePoint[-2];
			pVerticePoint++;

			pVerticePoint->x = (float)x/(float)nBlockCols - 0.5;
			pVerticePoint->y = fY;;//-(float)y/(float)nBlockRows + 0.5;
			pVerticePoint->z = 0;
			fAlpha = m_fAlpha * fAlpha;
			pVerticePoint->color = D3DCOLOR_ARGB((uint8)fAlpha, 0, 0, 0);
			pVerticePoint->tx = (float)x/(float)nBlockCols;
			pVerticePoint->ty = fTy; //(float)y/(float)nBlockRows;
			pVerticePoint++;

			*pVerticePoint = pVerticePoint[-3];
			pVerticePoint++;


			
			pMask++;
		}
	}
	hr = m_pVertexBuffer->Unlock();
}
int C3dItemVideoSummaryPicture::Render(C3dGraphics *pGraphic)
{
	if (m_3dPixFormat != e3dPF_YV420)
	{

		IDirect3DDevice9* pDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
		HRESULT hr  = pDev->UpdateTexture(m_pSysTexture, m_pRendTexture);
		if (FAILED(hr))
		{
			//TRACE("更新 渲染纹理 失败\n");
			return S_FALSE;
		}
		else
		{
			//TRACE("更新 渲染纹理 成功\n");
		}
	}

	C3dItemImage::Render(pGraphic);

	/*
		HRESULT hr = S_OK;
	EnterCriticalSection(&m_hLock) ;
	
	IDirect3DDevice9* pDev = pGraphic->m_pGraphicEngine->GetD3DDevice9();
	if (!pDev)
	{
		LeaveCriticalSection(&m_hLock);

		return -1;
	}

	SetPos(pDev, m_fX, m_fY, m_fZ, 
		m_fAngleX, m_fAngleY, m_fAngleZ,
		m_fSizeX, m_fSizeY, m_fSizeZ);

	//关闭...不然需要 设置灯光 否则看不见。。。
	pDev->SetRenderState( D3DRS_LIGHTING, false );


	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND,   D3DBLEND_INVSRCALPHA);

	pDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

	pDev->SetTextureStageState(0, D3DTSS_COLORARG1,  D3DTA_TEXTURE);
	//pDev->SetTextureStageState(0, D3DTSS_COLORARG2,  D3DTA_TEXTURE);   //Ignored
	pDev->SetTextureStageState(0, D3DTSS_COLOROP,    D3DTOP_SELECTARG1);
	//来自于环境光, alpha 也可以是混合出来的
	pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//使用的 appha 来自于 纹理 alpha通道
	//pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	// 使用 环境光 与 纹理的乘积 作为最终 alpha 数值
//	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE  );
	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);

	// set blending factors so that alpha component determines transparency
	//pDev->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCCOLOR);
	//pDev->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_DESTCOLOR);
	pDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);

	//pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//pDev->SetSamplerState(0, D3DSAMP_MAXFILTER, D3DTEXF_ANISOTROPIC);
	//pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	//pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	
	if (   m_pRendTexture)
	{
		hr = pDev->SetTexture( 0, m_pRendTexture);
		if (FAILED(hr))
		{
			//TRACE("SetTexture() false\n");
		}
		else
		{
			//TRACE("SetTexture() success\n");
		}
	}
	//设置自定义的FVF
	hr = pDev->SetFVF( D3DFVF_CUSTOMVERTEX);
	//绑定顶点缓存区至设备数据源
	hr = pDev->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(VERTEX_CUSTOM1) );


	
	//hr = pDev->SetVertexShader( D3DFVF_CUSTOMVERTEX);
	if (FAILED(hr))
	{
		//TRACE("SetFVF() false\n");
	}

	//绘制图元，其中参数1为图元格式，参数3为三角形数目
	if (m_nPrimitiveCount <= 2)
	{
		hr = pDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_nPrimitiveCount );
	}
	else
	{//分条带绘制
		int nPrimitiveLineCount = 0;
		nPrimitiveLineCount = m_nBlockCols*2;
		int nLinePoints = 0;
		nLinePoints = 2*m_nBlockCols + 2;
		
		hr = pDev->DrawPrimitive( D3DPT_TRIANGLELIST,
				0, 
				m_nPrimitiveCount);
		

	}

	hr = pDev->SetTexture( 0, NULL );

	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	LeaveCriticalSection(&m_hLock);

	*/

	if (m_p3dItem2dText)
	{
		char szTmp[64];
		int64 s64TimeStamp = m_s64TimeStamp - m_s64StartTimestamp;
		if (m_nTimeBaseDen == 0)
		{
			m_nTimeBaseDen = 1;
		}
		int64 s64Time = s64TimeStamp* 1000*m_nTimeBaseNum/m_nTimeBaseDen;
		s64Time /= 1000;
		int nHours(0), nMinutes(0), nSeconds(0);
		nHours = s64Time/3600;
		nMinutes = s64Time%3600/60;
		nSeconds = s64Time%60;
		sprintf(szTmp, "%02d:%02d:%02d", nHours, nMinutes, nSeconds);
		m_p3dItem2dText->SetText(szTmp);
	}

	if(m_p3dItem2dText /*&& m_pD3dSurface*/)
	{
		float fY = m_fY;
		if (fY > 0.07)
		{
			fY = m_fY - 0.07;
		}
		m_p3dItem2dText->SetPos(m_fX, fY, m_fZ);
		m_p3dItem2dText->Render(pGraphic);
	}
	return 0;
}
int C3dItemVideoSummaryPicture::Release()
{
	if (m_p3dItem2dText)
	{
		m_p3dItem2dText->Release();
	}
	return C3dItemImage::Release();
}
int	C3dItemVideoSummaryPicture::ReCreate(C3dGraphics *p3dGraphics)
{

	C3dItemImage::ReCreate(p3dGraphics);
	if (m_p3dItem2dText)
		m_p3dItem2dText->ReCreate(p3dGraphics);

	return 0;
}
int C3dItemVideoSummaryPicture::SetTimeStamp(int64 s64TimeStamp)
{
	m_s64TimeStamp = s64TimeStamp;
	

	return 0;
}
int C3dItemVideoSummaryPicture::SetImageFromFile(char *pData, int nLen)
{
	if (!m_pSysTexture)
		return -1;
	C3dItemImage::SetImageFromFile(pData, nLen);

	//if (m_bEnableMask)
		//MaskPicture(m_pMask);

	IDirect3DDevice9* pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
	IDirect3DSurface9 *pSurfaceRender=0;
	HRESULT hr=S_OK;
	
	IDirect3DSurface9 *pSurface = 0;
	/*m_pSysTexture->GetSurfaceLevel(0, &pSurface);
	if (!pSurface)
		return -1;
	HDC dc=0;
	pSurface->GetDC(&dc);
	if (!dc)
		return -2;
	*/

	int w = m_nTextureXPix;
	int h = m_nTextureYPix;
	// 	::MoveToEx(dc, 0, 0, NULL);
	// 	::LineTo(dc, m_nCornerLineLen, 0);
	// 	::MoveToEx(dc, 0, 0, NULL);
	// 	::LineTo(dc, 0, m_nCornerLineLen);
	DrawLine(0, 0, m_nCornerLineLen, 0);
	DrawLine(0, 0, 0, m_nCornerLineLen);

	// 	::MoveToEx(dc, w, 0, NULL);
	// 	::LineTo(dc, w, m_nCornerLineLen);
	// 	::MoveToEx(dc, w, 0, NULL);
	// 	::LineTo(dc, w-m_nCornerLineLen, 0);

	DrawLine(w-1, 0, w-1-m_nCornerLineLen, 0);
	DrawLine(w-1, 0, w-1, m_nCornerLineLen);

	// 	::MoveToEx(dc, w, h, NULL);
	// 	::LineTo(dc, w, h-m_nCornerLineLen);
	// 	::MoveToEx(dc, w, h, NULL);
	// 	::LineTo(dc, w-m_nCornerLineLen, h);
	DrawLine(w-1, h-1, w-1-m_nCornerLineLen, h-1);
	DrawLine(w-1, h-1, w-1, h-m_nCornerLineLen);

	// 	::MoveToEx(dc, 0, h, NULL);
	// 	::LineTo(dc, m_nCornerLineLen, h);
	// 	::MoveToEx(dc, 0, h, NULL);
	// 	::LineTo(dc, 0, h-m_nCornerLineLen);
	DrawLine(0, h-1, 0, h-1-m_nCornerLineLen);
	DrawLine(0, h-1, m_nCornerLineLen, h-1);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = w;
	rect.bottom = h;
	//::FillRect(dc, &rect, (HBRUSH) (COLOR_WINDOW+1));

	//pSurface->ReleaseDC(dc);

	//pSurface->Release();

	return hr;
	
 
	return 0;
}
int C3dItemVideoSummaryPicture::SetImageData(char *pData, int nW, int nH, int nPixFormat/* = ImgPF_RGB*/)
{
	C3dItemImage::SetImageData(pData, nW, nH, nPixFormat);
	
	IDirect3DDevice9* pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
	IDirect3DSurface9 *pSurfaceRender=0;
	HRESULT hr = S_OK;

	/*
	IDirect3DSurface9 *pSurface = 0;
	m_pSysTexture->GetSurfaceLevel(0, &pSurface);
	if (!pSurface)
		return -1;
	HDC dc=0;
	pSurface->GetDC(&dc);
	if (!dc)
		return -2;

	int w = m_nTextureXPix;
	int h = m_nTextureYPix;
// 	::MoveToEx(dc, 0, 0, NULL);
// 	::LineTo(dc, m_nCornerLineLen, 0);
// 	::MoveToEx(dc, 0, 0, NULL);
// 	::LineTo(dc, 0, m_nCornerLineLen);
	DrawLine(0, 0, m_nCornerLineLen, 0);
	DrawLine(0, 0, 0, m_nCornerLineLen);
	
// 	::MoveToEx(dc, w, 0, NULL);
// 	::LineTo(dc, w, m_nCornerLineLen);
// 	::MoveToEx(dc, w, 0, NULL);
// 	::LineTo(dc, w-m_nCornerLineLen, 0);
	
	DrawLine(w-1, 0, w-1-m_nCornerLineLen, 0);
	DrawLine(w-1, 0, w-1, m_nCornerLineLen);

// 	::MoveToEx(dc, w, h, NULL);
// 	::LineTo(dc, w, h-m_nCornerLineLen);
// 	::MoveToEx(dc, w, h, NULL);
// 	::LineTo(dc, w-m_nCornerLineLen, h);
	DrawLine(w-1, h-1, w-1-m_nCornerLineLen, h-1);
	DrawLine(w-1, h-1, w-1, h-m_nCornerLineLen);

// 	::MoveToEx(dc, 0, h, NULL);
// 	::LineTo(dc, m_nCornerLineLen, h);
// 	::MoveToEx(dc, 0, h, NULL);
// 	::LineTo(dc, 0, h-m_nCornerLineLen);
	DrawLine(0, h-1, 0, h-1-m_nCornerLineLen);
	DrawLine(0, h-1, m_nCornerLineLen, h-1);
	
	pSurface->ReleaseDC(dc);

	pSurface->Release();
	return hr;
	*/
	hr = m_pRendTexture->GetSurfaceLevel(0, &pSurfaceRender);
	if (hr !=S_OK){
		//TRACE("get surface false: %0x\n", hr);
	}
	if (pD3dDev && pSurfaceRender)
	{
		int w = m_nTextureXPix;
		int h = m_nTextureYPix;
		RECT rcFill;
		D3DCOLOR clr;
		clr = 0xFFFFffFF;

		// top left
		rcFill.top = 0; 
		rcFill.bottom = rcFill.top+1;
		rcFill.left = 0;
		rcFill.right = m_nCornerLineLen;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);
		rcFill.bottom = rcFill.top+m_nCornerLineLen;
		rcFill.left = 0;
		rcFill.right = rcFill.left+1;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);
		//top right
		rcFill.top = 0; 
		rcFill.bottom = rcFill.top+1;
		rcFill.left = w-m_nCornerLineLen;
		rcFill.right = w;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);
		rcFill.top = 0; 
		rcFill.bottom = rcFill.top+m_nCornerLineLen;
		rcFill.left = w-1;
		rcFill.right = w;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);

		// bottom left
		rcFill.top = h-m_nCornerLineLen; 
		rcFill.bottom = h;
		rcFill.left = 0;
		rcFill.right = rcFill.left+1;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);
		rcFill.top = h-1; 
		rcFill.bottom = h;
		rcFill.left = 0;
		rcFill.right = rcFill.left+m_nCornerLineLen;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);

		//bottom right
		rcFill.top = h-1; 
		rcFill.bottom = h;
		rcFill.left = w-m_nCornerLineLen;
		rcFill.right = w;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);
		rcFill.top = h-m_nCornerLineLen; 
		rcFill.bottom = h;
		rcFill.left = w-1;
		rcFill.right = w;
		pD3dDev->ColorFill(pSurfaceRender, &rcFill, clr);
		// must release
		pSurfaceRender->Release();
	}

	return hr;
}

bool C3dItemVideoSummaryPicture::HitTest( float fX, float fY )
{
	bool bRet = C3dGraphicItem::HitTest(fX, fY);

	if (bRet)
	{
		debug("%s, pts: %d\n", __FUNCTION__, (int) m_s64TimeStamp);
	}
	return bRet;
}

void C3dItemVideoSummaryPicture::SetTimeBase( int nTimeBaseNum, int nTimeBaseDen )
{
	//int64 s64Time = s64Ts* 1000*m_nTimeBaseNum/m_nTimeBaseDen;
	m_nTimeBaseDen = nTimeBaseDen;
	m_nTimeBaseNum = nTimeBaseNum;
}
void C3dItemVideoSummaryPicture::SetStartTimestamp(int64 s64Timestamp)
{
	m_s64StartTimestamp = s64Timestamp; 
}
void C3dItemVideoSummaryPicture::SetObjectId(int64 s64ObjectId)
{
	m_s64ObjectId = s64ObjectId;
}
int64 C3dItemVideoSummaryPicture::GetObjectId()
{
	return m_s64ObjectId;
}
void C3dItemVideoSummaryPicture::DrawLine(int x0, int y0, int x1, int y1)
{
	if (!m_pSysTexture)
		return ;

	D3DLOCKED_RECT lockRect;
	HRESULT hr = S_OK;
	hr = m_pSysTexture->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD);
	if (FAILED(hr))
	{
		return ;
	}
	unsigned char *pImage = (unsigned char*)lockRect.pBits;
	if (x0 == x1)
	{
		if (y1 < y0)
		{
			int y = y0;
			y0 = y1;
			y1 = y;
		}
		int nLinePos = y0*m_nTextureXPix*4 + x0*4;
		int nLineStride = m_nTextureXPix*4;
		for (int y= y0; y<=y1; y++)
		{
			pImage[nLinePos] = 255;
			pImage[nLinePos+1] = 255;
			pImage[nLinePos+2] = 255;
			pImage[nLinePos+3] = 255;
	
			nLinePos += nLineStride;
		}
	}
	else if (y0 == y1)
	{
		if (x1 < x0)
		{
			int x = x0;
			x0 = x1;
			x1 = x;
		}
		int nLinePos = y0*m_nTextureXPix*4 + x0*4;
		unsigned char *pPixel = pImage + nLinePos;
		for (int x=x0; x<=x1; x++)
		{
			pPixel[ 0] = 255;
			pPixel[ 1] = 255;
			pPixel[ 2] = 255;
			pPixel[ 3] = 255;
			/*pImage[nLinePos + x] = 255;*/
			pPixel += 4;
		}
	}

	m_pSysTexture->UnlockRect(0);

}
int C3dItemVideoSummaryPicture::SetImageMask(uint8 *pMask, int nBlockSize)
{
	int nW = m_nTextureXPix/nBlockSize;
	int nH = m_nTextureYPix/nBlockSize;
	if (m_nBlockCols != nW
		|| m_nBlockRows != nH)
	{
		if (m_pMask)
			delete m_pMask;
		m_pMask = new uint8[nW*nH];
		m_nBlockRows = nH;
		m_nBlockCols = nW;
	}
	m_nMaskBlockSize = nBlockSize;
	memcpy(m_pMask, pMask, nW*nH);
	//设置mask之后要重建模型，以便设置alpha
	if (m_bEnableMask)
	{
		ReCreateGeometry();
	}
	

	return 0;
}
//根据mask，将 不需要显示的部份透明，alpha 通道 设置为 0, 其余为 255
void C3dItemVideoSummaryPicture::MaskPicture(uint8 *pMask)
{
	if (!m_pSysTexture)
		return ;

	D3DLOCKED_RECT lockRect;
	HRESULT hr = S_OK;
	hr = m_pSysTexture->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD);
	if (FAILED(hr))
	{
		return ;
	}

	unsigned char *pImage = (unsigned char*)lockRect.pBits;
	int nLineMask = 0;
	uint8 nMaskAlpha(0);
	for (int yMask=0; yMask<m_nBlockRows; yMask++)
	{
		for (int xMask=0; xMask<m_nBlockCols; xMask++)
		{
			//
			nMaskAlpha =  m_pMask[nLineMask + xMask];
			if (nMaskAlpha == 255)
			{//正常显示
				//continue;
			}
			
			//将该块透明
			int nYPic = yMask*m_nMaskBlockSize;// + xMask*m_nMaskBlockSize;
			int nXPic = xMask*m_nMaskBlockSize;
			
			int nLinePixelStride = m_nTextureXPix*4;
			int nLinePixelPos = nYPic * nLinePixelStride + nXPic*4;
			for (int yBlock =0; yBlock<m_nMaskBlockSize; yBlock++)
			{
				for (int xBlock=0; xBlock<m_nMaskBlockSize; xBlock++)
				{
					int nPixelPos = nLinePixelPos + (xBlock<<2);
					pImage[nPixelPos+3] = nMaskAlpha;
				}
				nLinePixelPos += nLinePixelStride;
			}
			//
		}
		nLineMask += m_nBlockCols;
	}

	m_pSysTexture->UnlockRect(0);

}