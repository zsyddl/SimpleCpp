#pragma once
#include "3dgraphicitem.h"
#include <string>


using namespace std;
class ID3DXFont;
//dx 接口直接绘制
class  MEDIASDK_API C3dItemText2d :
	public C3dGraphicItem
{
public:
	C3dItemText2d(C3dGraphics *p3dGraph, int nTexturePixW=600, int nTexturePixH=480 );
	virtual ~C3dItemText2d(void);

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
	//为什么使用 string 内存泄露？
	string m_strText;
	char   m_szFontName[256];
	
	char	m_szText[255];
	COLORREF m_clrText;
	int    m_nFontSize;
	ID3DXFont*   m_pD3dFont;

};
