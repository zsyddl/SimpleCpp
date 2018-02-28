#pragma once

class C3dGraphics;
//#include "D3dPresenter.h"
#include <string>
using namespace std;

class IDirect3DDevice9;
class IDirect3DTexture9 ;
class IDirect3DVertexBuffer9;
class MEDIASDK_API C3dGraphicItem
{
public:
	C3dGraphicItem(C3dGraphics *p3dGraph, int nTexturePixW=600, int nTexturePixH=480 );
	virtual ~C3dGraphicItem(void);
	
	virtual int Create(float fX, float fY, float fZ);
	virtual int SetGeometry(float x, float y, float z, 
		float xangle, float yangle, float zangle,
		float cx, float cy, float cz);
	virtual int SetSize(float cx, float cy, float cz);
	virtual int SetAngle(float xAngle, float yAngle, float zAngle);
	virtual int SetPos(float x, float y, float z);
	virtual int Render(C3dGraphics *pGraphic);
	virtual int GetPos(float &fX, float &fY, float &fZ);
	virtual int GetSize(float &fCX, float &fCY, float &fCZ);
	virtual bool HitTest(float fX, float fY);

	int SetVisible(bool bVisible=true);
	bool IsVisible();
	
	virtual int SetAlpha(float fAlpha);

	virtual int Release();
	virtual int	ReCreate(C3dGraphics *p3dGraphics);

	bool Lock();
	void Unlock();

	//虚拟尺寸
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fAngleX;
	float m_fAngleY;
	float m_fAngleZ;
	float m_fSizeX;
	float m_fSizeY;
	float m_fSizeZ;
	float m_fAlpha;
	//纹理像素尺寸
	int	 m_nTextureXPix;
	int  m_nTextureYPix;
	int  m_nZPix;
protected:
	virtual HRESULT		InitGeometry();
	void		SetPos(IDirect3DDevice9 *pDev, float x, float y, float z, 
		float xangle, float yangle, float zangle,
		float cx, float cy, float cz);
	virtual HRESULT     CreateTexture();//当丢失之后 需要重新创建
	// call on device lost 
	

	CRITICAL_SECTION  m_hLock;
	
	C3dGraphics *m_p3dGraphic;
protected:
	//ID3DXMesh *m_pTeapot; // 茶壶网格
	//CComPtr<IDirect3DSurface9>		m_pSysSurface; //后台缓存
	IDirect3DTexture9		*m_pSysTexture; //后台缓存
	IDirect3DTexture9		*m_pRendTexture;  //实时渲染的texture

	IDirect3DVertexBuffer9	*m_pVertexBuffer; //定点模型缓存
	int m_nVertexPoints;
	int						m_nPrimitiveCount;
	//
	/*int m_nId;*/
	bool   m_bVisible;
	//string m_strId;
};
