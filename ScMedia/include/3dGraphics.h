#pragma once

#include <list>
/*
负责3d绘图的类
*/
using namespace std;
class C3dGraphicItem;
class CD3dPresenter;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct VERTEX_CUSTOM1 {	
	float x, y, z;
	DWORD color;
	float tx, ty;
};
class MEDIASDK_API C3dGraphics
{
public:
	C3dGraphics(void);
	~C3dGraphics(void);

	int Create(HWND hWnd);
	HWND GetWnd();
	int GetWndSize(int &nW, int &nH);
	void GetSize(int &nW, int &nH);
	//
	int AddItem2Head(C3dGraphicItem* pItem);
	int AppendItem(C3dGraphicItem* pItem);
	int RemoveItem(C3dGraphicItem* pItem);
	//强制刷新 输出图形
	int Refresh();
	//
	int SnapShot(char *pSaveFile);
	int SnapShot(unsigned char *pBuffer, int &nW, int &nH);
	int HitTest(float fX, float fY);

	void SetGraphicsSize(int nW, int nH);
	//
	//engine, 这个是 directx的，或许也可以是 opengl 的
	CD3dPresenter *m_pGraphicEngine;
protected:
	int Render();

	int CheckDeviceLost();
	int CheckMonitorChanged();
	int Reset();
protected:
	
	HMONITOR m_hMonitor;
	HWND	 m_hWnd;
	
	list<C3dGraphicItem*> m_lstItems;
	//同步锁
	CRITICAL_SECTION  m_hLock;
	//3d虚拟尺寸
	float m_fLogicXSize;
	float m_fLogicYSize;
	float m_fLogicZSize;
};
