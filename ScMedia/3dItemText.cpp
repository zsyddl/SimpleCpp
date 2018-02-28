#include "StdAfx.h"
#include "3dItemText.h"
#include "3dGraphics.h"
#include "D3dPresenter.h"

C3dItemText::C3dItemText(C3dGraphics *p3dGraph, int nTexturePixW, int nTexturePixH )
:C3dGraphicItem(p3dGraph, nTexturePixW, nTexturePixH)
, m_nFontSize(40)
, m_strText("")
, m_clrText(0xffff0000)
{
	//m_pStrText = new string("");
	memset(m_szText, 0, sizeof(m_szText));
	memset(m_szFontName, 0, sizeof(m_szFontName));
	strcpy(m_szFontName, "arial");//arial , FixedSys , "Times New Roman"

	wcsncpy(m_strText, L"hello d3d text", wcslen(L"hello d3d text"));
}

C3dItemText::~C3dItemText(void)
{	
	m_strText.clear();
	if (m_pD3dFont)
	{
		m_pD3dFont->Release();
		m_pD3dFont = NULL;
	}


	//m_strText.erase();
}
int C3dItemText::Create(float fX, float fY, float fZ)
{
	C3dGraphicItem::Create(fX, fY, fZ);

	if (!m_p3dGraphic)
		return -1;

	m_fX = fX;
	m_fY = fY;
	m_fZ = fZ;
	EnterCriticalSection(&m_hLock) ;
	IDirect3DDevice9 *pD3dDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();

	D3DXFONT_DESC   d3dFont;

	memset(&d3dFont,0,sizeof(d3dFont));
	d3dFont.Height=m_nFontSize;   //   in   logical   units
	//d3dFont.Width=m_nFontSize;     //   in   logical   units
	//d3dFont.Weight=500;//   boldness,   range   0(light)   -   1000(bold)
	//d3dFont.Italic=FALSE;
	//d3dFont.CharSet=DEFAULT_CHARSET;
	strcpy((char*)d3dFont.FaceName, m_szFontName);
	/*d3dFont.FaceName[0] = 'T';
	d3dFont.FaceName[1] = 'a';
	d3dFont.FaceName[2] = 'h';
	d3dFont.FaceName[3] = 'o';
	d3dFont.FaceName[4] = 'm';
	d3dFont.FaceName[5] = 'a';
	*/

	CreateTextPixmap();

	LeaveCriticalSection(&m_hLock) ;
	return 0;
}
int C3dItemText::Render(C3dGraphics *pGraphic)
{
	if (!m_bVisible)
		return 0;

	RECT rect;

	int nWndW, nWndH;
	m_p3dGraphic->GetSize(nWndW, nWndH);

	rect.left= nWndW*m_fX/2;
	rect.top = nWndH*m_fY/2;
	rect.right = rect.left+300;
	rect.bottom = rect.top+100;



	return 0;
}
int C3dItemText::Release()
{

	C3dGraphicItem::Release();


	return 0;
}
int C3dItemText::ReCreate(C3dGraphics *p3dGraphics)
{
	EnterCriticalSection(&m_hLock) ;
	 
	LeaveCriticalSection(&m_hLock) ;

	return  Create(m_fX, m_fY, m_fZ);
}
int C3dItemText::SetText(char *pText)
{
	EnterCriticalSection(&m_hLock) ;
	//m_strText.clear();
	m_strText = pText;
	//strncpy(m_szText, pText, 255);
	
	LeaveCriticalSection(&m_hLock) ;

	ReCreate(m_p3dGraphic);

	return 0;
}
int C3dItemText::CreateTextPixmap()
{
	HRESULT hr = S_OK;

	BitmapData bd;
	Status status;
//	m_ = (m_pMedia->GetRegion()->m_vct3Size.x+0.1);
//	 = (m_pMedia->GetRegion()->m_vct3Size.y+0.1);
	TRACE("Text Width: %d, Height %d\n", m_nWidth, m_nHeight);
	Rect rect(0,0, m_nWidth, m_nHeight);

	m_pBitmap = new Bitmap(m_nWidth, m_nHeight, PixelFormat32bppARGB);
	if (!m_pBitmap)
	{//这个lock错误？不行啊
		return S_FALSE;
	}
	
	status = m_pBitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bd);
	//memset(bd.Scan0, 0, bd.Stride * bd.Height);

	//创建含有ALpha通道的位图；,alpha 为 255 完全不透明， 颜色为 0，0，0 以颜色混合则 不显示，剩下的只有文字才显示
	int count = bd.Stride/sizeof(ARGB); //一行的数量
	ARGB *pArgb = new ARGB[count];	
	for(int i=0; i<count; i++)
		pArgb[i] = 0；

	for(i=0; i<bd.Height; i++)
		memcpy(LPBYTE(bd.Scan0)+i*bd.Stride, pArgb, bd.Stride);

	m_pBitmap->UnlockBits(&bd);

	m_pGraphics = new Graphics(m_pBitmap);

	m_pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);

	//边缘颜色
	
	Color color = ARGB(255, 255, 0, 0);//ARGB
	Pen pen(color, 1.0);

	
/*
#ifdef UNICODE
	wcscpy( m_strText, tchar );
#else
	MultiByteToWideChar(CP_ACP, 0, tchar,-1, m_strText, 10);
#endif
*/
	LOGFONT &lgFont = m_lgFont;
	WCHAR wfontFamily[LF_FACESIZE];
	//MultiByteToWideChar(CP_ACP, 0, lgFont.lfFaceName,-1, wfontFamily, LF_FACESIZE-1);
	MultiByteToWideChar(CP_ACP, 0, _T("tahoma"),-1, wfontFamily, LF_FACESIZE-1);
	
//	wcsncpy(fontFamily, , MAX_PATH-1);
//	long lFsize = abs(lgFont.lfHeight)/(m_pMedia->GetRegion()->m_vct3Size.x+0.1);
//	lFsize = m_pMedia->m_pRegion->m_vct3Size.x
	m_pFont = new Font(wfontFamily, 50, FontStyleRegular, UnitPixel, NULL);
//	float m_fontHeight = m_pFont->GetHeight(m_pGraphics);

	m_pStringFormat = new StringFormat();
	UINT fmtFlags = m_pStringFormat->GetFormatFlags();
	fmtFlags |= StringFormatFlagsLineLimit;
	m_pStringFormat->SetFormatFlags(fmtFlags);

	GraphicsPath *fontPath = new GraphicsPath(FillModeAlternate);
	FontFamily fontFamily;
	m_pFont->GetFamily(&fontFamily);

	Status sts;

	sts = fontPath->AddString(m_strText, 
		wcslen(m_strText),
		&fontFamily, 
		m_pFont->GetStyle(), 
		m_pFont->GetSize(), 
		Rect(0,0,m_nWidth,m_nHeight),
		m_pStringFormat);
	fontPath->Widen(&pen);

	sts = m_pGraphics->DrawPath(&pen, fontPath); //描边？
	//内部填充
//	((CD3DMediaText*)m_pMedia)->m_colorFont = D3DCOLOR_ARGB(255, 250,0,250);;
	SolidBrush brush( D3DCOLOR_ARGB(255, 250,0, 250) );
	m_pGraphics->FillPath(&brush, fontPath);


	delete []pArgb;
	delete fontPath;
//	delete []tchar;

	
	D3DLOCKED_RECT	d3dLock;
	hr = m_pSysTexture->LockRect(0, &d3dLock, NULL, D3DLOCK_DISCARD);

	BitmapData bd;
	Status status;
	Rect rect(0, 0, m_nWidth, m_nHeight);
	status = m_pBitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bd);

	memcpy(d3dLock.pBits, bd.Scan0, bd.Stride * rect.Height);

	m_pSysTexture->UnlockRect(0);
	m_pBitmap->UnlockBits(&bd);
/*
 	CComPtr<IDirect3DSurface9> pSysSurface, pSurface;
 	m_pSysTexture->GetSurfaceLevel(0, &pSysSurface);
 	m_pRTTexture->GetSurfaceLevel(0, &pSurface);
	m_pDMD3D9->GetD3DDevice9()->StretchRect( pSysSurface, 
											NULL,
											pSurface, NULL,
											D3DTEXF_NONE ) ;
											*/
	//m_pDMD3D9->GetD3DDevice9()->UpdateTexture(m_pSysTexture, m_pRendTexture);
	IDirect3DDevice9* pDev = m_p3dGraphic->m_pGraphicEngine->GetD3DDevice9();
	pDev->UpdateTexture(m_pSysTexture, m_pRendTexture);


	return 0;
}

void C3dItemText::GetFont( char *pSzName, int &nSize, long &lRGB )
{
	strcpy(pSzName, m_szFontName);
	nSize = m_nFontSize;
	lRGB = m_clrText;
}

int C3dItemText::SetFont( char *pSzName, int nSize, long lRGB )
{
	bool bReCreate=false;
	EnterCriticalSection(&m_hLock) ;
	if (strcpy(pSzName, m_szFontName) != 0)
	{
		bReCreate = true;
	}
	if (nSize != m_nFontSize)
	{
		bReCreate = true;
	}
	m_clrText = lRGB ;

	LeaveCriticalSection(&m_hLock) ;
	if (bReCreate){
		ReCreate(NULL);
	}
	return 0;
}
