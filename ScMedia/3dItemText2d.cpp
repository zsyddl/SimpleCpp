#include "StdAfx.h"
#include "3dItemText2d.h"
#include "3dGraphics.h"
#include "D3dPresenter.h"
 
C3dItemText2d::C3dItemText2d(C3dGraphics *p3dGraph, int nTexturePixW, int nTexturePixH )
:C3dGraphicItem(p3dGraph, nTexturePixW, nTexturePixH)
, m_pD3dFont(0)
, m_nFontSize(40)
, m_strText("")
, m_clrText(0xffffffff)
{
	//m_pStrText = new string("");
	memset(m_szText, 0, sizeof(m_szText));
	memset(m_szFontName, 0, sizeof(m_szFontName));
	strcpy(m_szFontName, "arial");//arial , FixedSys , "Times New Roman"
}

C3dItemText2d::~C3dItemText2d(void)
{	
	m_strText.clear();
	if (m_pD3dFont)
	{
		m_pD3dFont->Release();
		m_pD3dFont = NULL;
	}


	//m_strText.erase();
}
int C3dItemText2d::Create(float fX, float fY, float fZ)
{
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
	D3DXCreateFontIndirect(pD3dDev, &d3dFont, &m_pD3dFont); 

	LeaveCriticalSection(&m_hLock) ;
	return 0;
}
int C3dItemText2d::Render(C3dGraphics *pGraphic)
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
	EnterCriticalSection(&m_hLock) ;
	if (m_pD3dFont)
	{
		m_pD3dFont->DrawTextA( NULL,     //前面加个NULL就能通过了
			  m_strText.data(),  //_T("2012-5-2 11:20:00")
			-1,   //   size   of   string   or   -1   indicates   null   terminating   string
			&rect,                         //   rectangle   text   is   to   be   formatted   to   in   windows   coords
			DT_TOP   |   DT_LEFT,   //   draw   in   the   top   left   corner   of   the   viewport
			m_clrText);             //   black   text 
		
	}
	LeaveCriticalSection(&m_hLock) ;

	return 0;
}
int C3dItemText2d::Release()
{

	C3dGraphicItem::Release();
	if (m_pD3dFont)
	{
		m_pD3dFont->Release();
		m_pD3dFont = NULL;
	}
	return 0;
}
int C3dItemText2d::ReCreate(C3dGraphics *p3dGraphics)
{
	EnterCriticalSection(&m_hLock) ;
	if (p3dGraphics != NULL)
		m_p3dGraphic = p3dGraphics;
	if (m_pD3dFont)
	{
		m_pD3dFont->Release();
		m_pD3dFont = NULL;
	}
	LeaveCriticalSection(&m_hLock) ;
	return  Create(m_fX, m_fY, m_fZ);
}
int C3dItemText2d::SetText(char *pText)
{
	EnterCriticalSection(&m_hLock) ;
	//m_strText.clear();
	m_strText = pText;
	//strncpy(m_szText, pText, 255);

	LeaveCriticalSection(&m_hLock) ;
	return 0;
}

void C3dItemText2d::GetFont( char *pSzName, int &nSize, long &lRGB )
{
	strcpy(pSzName, m_szFontName);
	nSize = m_nFontSize;
	lRGB = m_clrText;
}

int C3dItemText2d::SetFont( char *pSzName, int nSize, long lRGB )
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
