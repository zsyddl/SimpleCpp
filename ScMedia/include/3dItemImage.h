#pragma once
#include "3dgraphicitem.h"
#include <string>
#include <vector>

using namespace std;
typedef enum _e3dPixFormat{
	e3dPF_RGBA = 1,
	e3dPF_YV12,
	e3dPF_YV420,
	e3dPF_YV2,
}e3dPixFormat;
class IDirect3DSurface9;
class MEDIASDK_API C3dItemImage :
	public C3dGraphicItem
{
public:
	C3dItemImage(C3dGraphics *p3dGraphic, int nTexturePixW=600, int nTexturePixH=480, e3dPixFormat pixFormat=e3dPF_RGBA );
	virtual ~C3dItemImage(void);
	
	virtual int Create(float fX, float fY, float fZ);

	virtual int SetImageData(char *pData, int nW, int nH, int nPixFormat = e3dPF_RGBA);
	//从内存数据文件(jpeg, png, bmp ...)加载
	virtual int SetImageFromFile(char *pData, int nDataLen);

	virtual int GetImageData(char *pData, int &nW, int &nH);
	virtual int GetImageData(char *pFile);
	
/*	int SetOsd(int nIndex, char *pText);*/

	virtual  int Render(C3dGraphics *pGraphic);

	virtual int Release();
	virtual int	ReCreate(C3dGraphics *p3dGraphics);
protected:
	

	e3dPixFormat m_3dPixFormat;
	//为显示 YV 12 格式的 picutre
	IDirect3DSurface9  *m_pD3dSurface;
	IDirect3DSurface9  *m_pSysSurface;

	unsigned int m_nSrcMaskColor;
//	ID3DXFont*   m_pD3dFont; 

/*	vector<string* > m_lstOsd;*/
// 	int m_nPixsX;
// 	int m_nPixsY;
};
