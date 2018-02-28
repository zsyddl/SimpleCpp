#pragma once
#include "3dgraphicitem.h"
#include <string>
#define ULONG_PTR unsigned long
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")


using namespace std;
//绘制到图片里面，使用alpha ，合成
class  MEDIASDK_API C3dItemText :
	public C3dGraphicItem
{
public:
	C3dItemText(C3dGraphics *p3dGraph, int nTexturePixW=600, int nTexturePixH=480 );
	virtual ~C3dItemText(void);

	int SetFontSize(int nW, int nH);
	int SetFontColor(unsigned char nR, unsigned char nG,  unsigned char nB);
	int SetFontColor(long lColorRgb);
	int SetFont(char *pSzName, int nSize, long lRGB);
	void GetFont(char *pSzName, int &nSize, long &lRGB);
	int SetText(char *pText);

	virtual int Create(float fX, float fY, float fZ);
	virtual int Render(C3dGraphics *pGraphic);
	virtual int Release();
	virtual int ReCreate(C3dGraphics *p3dGraphics);

protected:
	int CreateTextPixmap();

	//string m_strText;
	char   m_szFontName[256];
	
	char	m_szText[255];
	COLORREF m_clrText;
	int    m_nFontSize;
	
	LOGFONT  m_lgFont;
	WCHAR		*m_strText;
	long		m_lLength;
	Bitmap		*m_pBitmap;
	Graphics	*m_pGraphics;
	//内存位图大小
	int			m_nWidth;
	int			m_nHeight;
	//	LOGFONT  m_lgFont;
	Font		*m_pFont;
	StringFormat* m_pStringFormat;
};
