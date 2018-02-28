/*////////////////////////////////////////////////////////////////////////////////
// JVSDKP.h
// JVSDKP 动态连接库的导出头文件，应用程序应包含此头文件
// JVSDKP用于实现对采集卡所压缩数据的解码和显示，为上层应用程序提供二次开发
// 该SDK采用C编写，在C++下使用时需要extern "C"{}
// 济南中维世纪科技有限公司
// 最后更新：2009-05-16
//////////////////////////////////////////////////////////////////////////////////*/


#ifndef		_JVSDK_H
#define		_JVSDK_H


#define JVSDKP_API extern "C" __declspec(dllexport)


//SDK通知事件的lParam
#define	WM_LPARAM_BITMAP	0x01		//抓图
#define	WM_LPARAM_STOP		0x02		//回放停止

//帧类型
#define JVS_TYPE_P			0x00
#define JVS_TYPE_IDR		0x01
#define JVS_TYPE_AUD		0x02
#define JVS_TYPE_SKIP		0x03
#define JVS_TYPE_D			0x04

typedef struct _JVS_HEADER
{
	unsigned int	i_flags;
	unsigned int	i_frame_width;
	unsigned int	i_frame_height;
	unsigned int	i_total_frames;
} JVS_HEADER, *PJVS_HEADER;


//初始化/释放SDK
JVSDKP_API	BOOL	__stdcall	JVSP_InitSDK(BOOL bDecodeOnly);
JVSDKP_API	void	__stdcall	JVSP_ReleaseSDK();
JVSDKP_API	void	__stdcall	JVSP_RegisterNotify(HWND hWnd, DWORD dwNotifyMsg);

//打开/关闭播放器
JVSDKP_API	int		__stdcall	JVSP_CreatePlayer( BOOL bPreviewRGB );	//失败返回-1，其他为播放器句柄
JVSDKP_API	BOOL	__stdcall	JVSP_ReleasePlayer (int nPlayer);

JVSDKP_API	BOOL	__stdcall	JVSP_OpenFile ( int nPlayer, char * szFilename );

//播放
JVSDKP_API	BOOL	__stdcall	JVSP_Play ( int nPlayer );
JVSDKP_API	void	__stdcall	JVSP_Pause ( int nPlayer );
JVSDKP_API	BOOL	__stdcall	JVSP_Stop ( int nPlayer );
JVSDKP_API	BOOL	__stdcall	JVSP_ZoomIn(int nPlayer, BOOL bZoomIn, int x, int y);
JVSDKP_API	BOOL	__stdcall	JVSP_RefreshWindow(int nPlayer);
JVSDKP_API	void	__stdcall	JVSP_StepForward(int nPlayer);	//单步向前


//////////////////////////////////////////抓图////////////////////////////////
JVSDKP_API	BOOL	__stdcall	JVSP_GetBitmapSize(int nPlayer, DWORD * dwWidth, DWORD * dwHeight);
JVSDKP_API	BOOL	__stdcall	JVSP_GetBitmap(int nPlayer, PUCHAR pBuffer);


//参数设定
JVSDKP_API	void	__stdcall	JVSP_SetFPS(int nPlayer, BOOL bFastPlay, ULONG ulFrameRate);
JVSDKP_API	BOOL	__stdcall	JVSP_SetVideoPreview(int nPlayer, HWND hWnd, RECT * pRect, BOOL bVideoPreview);
JVSDKP_API	void	__stdcall	JVSP_SetVideoCrop(int nPlayer, DWORD dwPixelCroped);

//获取参数
JVSDKP_API	ULONG	__stdcall	JVSP_GetTotalFrames(int nPlayer);
JVSDKP_API	ULONG	__stdcall	JVSP_GetCurFrameNum(int nPlayer);
JVSDKP_API	BOOL	__stdcall	JVSP_GetKeyFramePos(int nPlayer, ULONG * ulKeyFramePos, ULONG * ulFrameNum);
JVSDKP_API	BOOL	__stdcall	JVSP_SetAudioPreview(int nPlayer, HWND hWnd, BOOL bPreview);

JVSDKP_API	BOOL	__stdcall	JVSP_GotoKeyFrame(int nPlayer, ULONG ulFrame, BOOL bPause );

//流模式
JVSDKP_API	BOOL	__stdcall	JVSP_OpenStream(int nPlayer, int nFrameWidth, int nFrameHeight);
JVSDKP_API	BOOL	__stdcall	JVSP_WriteToStreamOneFrame(int nPlayer, PUCHAR pBuffer, int nSize);
JVSDKP_API	BOOL	__stdcall	JVSP_CloseStream(int nPlayer);

//更多
JVSDKP_API	BOOL	__stdcall	JVSP_PlayKeyFrame(int nPlayer, DWORD dwInterval);
//获取压缩文件头数据
JVSDKP_API	BOOL	__stdcall	JVSP_GetHeaderData(int nPlayer, PJVS_HEADER pheaderData);

//DirectShow Filter
JVSDKP_API	BOOL	__stdcall	JVSP_GetDecodedFrame(int nPlayer, PUCHAR pFrameData);

//解码卡相关
JVSDKP_API	int		__stdcall	JVSP_InitC501();
JVSDKP_API	BOOL	__stdcall	JVSP_BindC501(int nPlayer, int nDevice);
JVSDKP_API	void	__stdcall	JVSP_EnableC501Output(int nPlayer, BOOL bEnableOutput);


#endif