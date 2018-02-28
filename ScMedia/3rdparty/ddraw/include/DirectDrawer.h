//////////////////////////////////////////////////////////////////////////
// DirectDrawer.h
//////////////////////////////////////////////////////////////////////////
#ifndef __DDRAW_DEC_H__
#define __DDRAW_DEC_H__
#include "GeneralTypes.h"
#include "colorspace.h"
// please remember!!! to debug this application you have to install directxsdk // http://www.google.com/search?hl=en&q=directx+sdk
#include <wingdi.h>
#include <ddraw.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ddraw.lib")
typedef void (CALLBACK* CallbackDrawFun)(HDC hDc, long nUser);

//==============================================================================
class CDirectDrawer
{
public:
    LPDIRECTDRAW7        g_pDD;
    HMONITOR m_hMonitor;

	enum { DD_BORDER = 2 };
	CDirectDrawer();
	~CDirectDrawer();
	BOOL InitDirectDraw( HWND hWnd, int font_size, bool bTryOverlay , CallbackDrawFun, long iface, unsigned int bUseGpu=0);
	void CloseDirectDraw();
	BOOL BlitBitmap( const PRESENTATION_ITEM* pcPresentItem );
	BOOL DDrawOnPaint();
	void DDrawOnSize(UINT state, int cx, int cy);
	void DDrawOnMove(int x, int y);
	void	 GetDDrawSizes(unsigned int & h, unsigned int & v);
	COLORREF GetBkGroundColor();
	void	 GetDisplayMode( int& iDisplayCx, int& iDisplayCy, int& iFrameRate );
	
	void SetYUVTrans(BOOL bGPU);
	void Snapshot( char *pFile );

private:
	BOOL CreateSurfaces(bool);
	void ShowMessage(int n, char * message, char * title);
	DWORD ConvertGDIColor(COLORREF dwGDIColor, LPDIRECTDRAWSURFACE7 pdds);
	bool GetDestRect(RECT * prcDst);
	bool CreateOverlaySurface(unsigned int width, unsigned int height, int type);
	bool CreateOffscreenSurface(unsigned int width, unsigned int height);
	void AdjustSizeForHardwareLimits(RECT * prcSrc, RECT * prcDst);

	static const COLORREF crDDrawBackgroundColor;
	LPDIRECTDRAWSURFACE7 g_pDDSPrimary;	// 
	LPDIRECTDRAWSURFACE7 g_pDDSOffscr;	// 
	LPDIRECTDRAWSURFACE7 g_pDDSBack;	// 
    LPDIRECTDRAWSURFACE7 g_pDDSOsd;
	LPDIRECTDRAWSURFACE7 m_pSurfaceSnapshot;
	DDOVERLAYFX          g_OverlayFX;
	DWORD                g_dwOverlayFlags;

	bool fUseOverlay;
	bool fEXCLUSIVE;
	DDCAPS drivercaps;
	DDCAPS helcaps;

	/* Added by weilizhi, 2009/4/24 */
	bool is_subhandle;
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;

	/* Added by weilizhi, 2009/5/14 */
	int osdleft;
	int osdright;
	int osdtop;
	int osdbottom;

    ABSOLUTETIME osdtime;
    HANDLE   osdfont;

	unsigned int h_size_wnd_ddraw;
	unsigned int v_size_wnd_ddraw;
	int overlay_surface_type;
	HWND hDDrawWindow;

    int m_SRectWidth;
    int m_SRectHeight;
	//show yuv data using gpu; false , trans yuv to rgb by using cpu , and show rgb
	// to display yuv by using gpu, need to create surface in local video memory;
	BOOL m_bGPUYUV;

	ColorSpaceDll& m_csdll;

    void (CALLBACK* DrawFun)(HDC hDc, long nUser);
    long iface;
	CRITICAL_SECTION m_hLock;
};
#endif // __DDRAW_DEC_H__
