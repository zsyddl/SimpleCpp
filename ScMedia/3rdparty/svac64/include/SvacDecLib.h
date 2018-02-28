#ifndef	_SVACDECLIB_H_
#define _SVACDECLIB_H_

#define MAX_CUSTOM_EXT_COUNT          8
#define MAX_CUSTOM_EXT_SIZE          1024 // 256
#define MAX_DIVA_INIT_SIZE           1024
#define MAX_DIVA_INFOR_SIZE          32768 //8192 // 512

typedef enum /*seek_status*/
{
    SVAC_SEEK_ERROR = -1,
    SVAC_SEEK_SUCCESS,
	SVAC_SEEK_NOTFOUND,
    SVAC_SEEK_NEED_MORE_BITS
}SVAC_SEEK_STATUS;

typedef enum /*auth_status*/
{
    SVAC_AUTH_DEC_ERROR =	-1,
	SVAC_AUTH_END,
    SVAC_AUTH_NEED_MORE_BITS,
	SVAC_AUTH_NEED_PASSWORD,
	SVAC_AUTH_SUCCESS,
	SVAC_AUTH_CONTINUE,
	SVAC_AUTH_ERROR,
	SVAC_AUTH_BL_ERROR,
	SVAC_AUTH_EL_ERROR,
	SVAC_AUTH_NODATA,
	SVAC_AUTH_BL_NODATA,
	SVAC_AUTH_EL_NODATA
}SVAC_AUTH_STATUS;

typedef enum /*dec_status*/
{
    
    SVAC_DEC_ERROR =	-1,
    SVAC_DEC_SUCCESS	,
    SVAC_DEC_NEED_MORE_BITS,
    SVAC_DEC_NO_PICTURE,
    SVAC_DEC_BUF_OVERFLOW,
	SVAC_DEC_PASSWORD_ERROR
/*	SVAC_DEC_AUTH_ERROR,
	SVAC_DEC_BL_AUTH_ERROR,
	SVAC_DEC_EL_AUTH_ERROR,
	SVAC_DEC_NO_AUTH_DATA,
	SVAC_DEC_BL_NO_AUTH_DATA,
	SVAC_DEC_EL_NO_AUTH_DATA,
	SVAC_DEC_AUTH_SUCCESS*/
}SVAC_DEC_STATUS;

typedef struct
{
    unsigned char type;
    unsigned short length;
    unsigned char *pbuf;
	unsigned char reserved[2];
}SVAC_EXT_INFO_CUSTOM;

typedef struct 
{
	unsigned int    roi_infor_en;						//ROI 区域信息，最低位为0，代表该roi不存在
	unsigned short	top;								//ROI 顶边坐标 [y_min]	
	unsigned short	left;								//ROI 左边坐标 [x_min]
	unsigned short	bot;								//ROI 底边坐标 [y_max]
	unsigned short	right;								//ROI 右边坐标 [x_max]
}SVAC_ROI;

typedef struct
{
	unsigned short  len;		// val的字节数
	unsigned char	*DIVA_init_extension_val;
} DIVA_init_extension;

typedef struct
{
	unsigned short  len;		// val的字节数
	unsigned char	*DIVA_info_extension_val;
} DIVA_info_extension;

typedef struct
{
	unsigned char  TsEnable;							//是否包含时间信息
    unsigned char  Ts_time_stamp_flag;					//
    unsigned char  Ts_ref_date_flag;
    unsigned short Ts_time_year;						//年
    unsigned char  Ts_time_month;						//月
    unsigned char  Ts_time_day;							//日
    unsigned char  Ts_time_hour;						//时	
    unsigned char  Ts_time_minute;						//分	
    unsigned char  Ts_time_second;						//秒
    unsigned short Ts_time_sec_fractional;				
	unsigned char  SeEnable;							//是否包含监控信息
    unsigned char  Se_roi_extension_flag;				//是否包含ROI扩展信息
    unsigned char  Se_event_extension_flag;				//是否包含事件扩展信息
    unsigned char  Se_event_num[4+1];					//事件信息
    unsigned char  Se_alert_extension_flag;				//是否包含报警信息
    unsigned char  Se_vm_alert_extension_flag;			//是否包含vimico报警信息
    unsigned char  Se_alert_num;						//报警事件数目
    unsigned char  Se_alert_appendix_flag[16];			//报警事件信息
    unsigned char  Se_alert_appendix_length[16];		//报警事件长度		
	unsigned char  Svac_inf_position_idc;				//位置编号信息
	unsigned short  Svac_inf_camera_idc;				//摄像头编号信息
	unsigned char  FrameNum;							//帧数，i帧置0，p帧逐帧加1
	unsigned char  RoiEn;								//图像是否包含ROI信息	
	unsigned char  RoiNum;								//图像包含ROI数目
	unsigned char  RoiSkipMode;							//ROI背景跳过模式
	unsigned char  RoiSvcSkipMode;						//ROI_SVC背景跳过模式		
	SVAC_ROI	   RoiRect[15];							//ROI信息，如SVC开启，代表基本层ROI矩形信息
	SVAC_ROI	   RoiSvcElRect[15];					//SVC增强层ROI矩形信息，SVC下有效

	unsigned char	AInitEnable;
	unsigned char	AInit[256];
	unsigned char	AResEnable;
	unsigned char	ARes[256];

	unsigned char  vimc_enabel;
	unsigned char  custom_enabel;
	unsigned int   ExtEnableFlag[8];
	unsigned char  svac_ext_info_custom_num;
	SVAC_EXT_INFO_CUSTOM svac_ext_info_custom[MAX_CUSTOM_EXT_COUNT];

	DIVA_init_extension svac_diva_init;
	DIVA_info_extension svac_diva_info;

	int Reserved[252];
//	int Reserved[256];
}SVAC_EXT_INFOR_INTERNAL;


typedef struct
{
	unsigned char *pBuf;
	unsigned int  uBufLen;
	unsigned int  uSeekTypeFlag[8];
	unsigned int  reserved[4];
}SVAC_SEEK_INPUT_PARAM;

#define SEEK_SPS_MAX 4
typedef struct 
{
 	int						find_sps_flag;				//0-没发现新的sps  >0-找到新的sps的个数.
#ifdef LINUX
	unsigned long long		sps_offset[SEEK_SPS_MAX];
#else
	unsigned __int64		sps_offset[SEEK_SPS_MAX];	//当find_sps_flag >0时，表示sps在整个码流中的偏移量。
														//当find_sps_flag==0时，无意义。
#endif
	SVAC_EXT_INFOR_INTERNAL inf;						//监控扩展信息
} SVAC_SEEK_OUTPUT_PARAM;


typedef struct
{
	int  rsa_e;
	int  rsa_n;
	char aes_key[16];
}SVAC_AUTH_KEY;

typedef struct
{
	char *pBuf;
	int  uBufLen;
	int	 iLastFlag;
	int	 iOneframeFlag;
	int  reserved[4];
}SVAC_AUTH_INPUT_PARAM;


typedef struct 
{
	void*	pBitstream;		// 码流首地址
	int		nLength;		// 码流长度
	int		iLastFlag;		// pBuf中的数据是否是视频流的最后数据, 0-否，1-是
	int		iOneframeFlag;	// 码流类型，标记当前输入的数据是否正好到一帧的结尾。0-否，1-是
	int		nColorspace;	// 解码后输出的YUV格式。0:400 1:420	2:422
} SVAC_DEC_INPUT_PARAM;

typedef struct 
{
	int		iPicFlag;		//  0: Frame, 1: Field
	int		iSvcOpenFlag;		// 是否开启svc模式
	int		iSvcDataFlag;		// 是否是否已解码svc
	int		iExtFlag;		// 是否有SVAC扩展信息
	int		nWidth;			// 帧宽(基本层)
	int		nHeight;		// 帧高(基本层)
	int		nWidth_El;		// 帧宽(增强层)
	int		nHeight_El;		// 帧高(增强层)
	int		iAuthResFlag;	// 低4bit:基本层，高4bit:增强层。 0:success  1:error  2:no data  3:no res
//	int		nStrideY;		//Luma plane stride in pixel
//	int 	nStrideUV;		//Chroma plane stride in pixel
	SVAC_EXT_INFOR_INTERNAL *pSvacExtInfo;
	void* 	pY;				// 解码后y分量输出地址
	void*	pU;				// 解码后u分量输出地址
	void* 	pV;				// 解码后v分量输出地址
	void* 	pY_SVC_EL;		// SVC增强层解码后y分量输出地址
	void*	pU_SVC_EL;		// SVC增强层解码后u分量输出地址
	void* 	pV_SVC_EL;		// SVC增强层解码后v分量输出地址
} SVAC_DEC_OUTPUT_PARAM;

typedef struct 
{
	int		width;
	int		height;
	int		roi_flag;
    int		svc_flag;
	int		color_space;
	int		bit_depth_luma;
	int		bit_depth_chroma;
} SVAC_PREFETCH_PARAM;

#ifdef LINUX
extern int SVACDecInit();
extern char* SVACDecLibVer();
extern void* SVACDecOpen();
extern void* SVACDecOpen2(int thread_num);
extern void SVACDecClose(void *handle);
extern int SVACDecSetKey(void* handle, unsigned char* key);
extern int SVACPrefetchParam(unsigned char *pBuf, int iBufLen, SVAC_PREFETCH_PARAM *pPreParam);
extern int SVACDecFrame(void* hHandle, SVAC_DEC_INPUT_PARAM *deci, SVAC_DEC_OUTPUT_PARAM *deco);

extern void *SVACSeekOpen();
extern void SVACSeekClose(void *hseek);
extern int SVACSeekExtension(void *hseek, SVAC_SEEK_INPUT_PARAM *seeki, SVAC_SEEK_OUTPUT_PARAM *seeko);

extern void *SVACAuthOpen();
extern void SVACAuthClose(void *handle);
extern void SVACAuthSetKey(void *handle, SVAC_AUTH_KEY *param);
extern int SVACAuthDec(void *handle, SVAC_AUTH_INPUT_PARAM *authi);

#else

extern __declspec(dllexport) int SVACDecInit();
extern __declspec(dllexport) char* SVACDecLibVer();
extern __declspec(dllexport) void* SVACDecOpen();
extern __declspec(dllexport) void* SVACDecOpen2(int thread_num);
extern __declspec(dllexport) void SVACDecClose(void *handle);
extern __declspec(dllexport) int SVACDecSetKey(void* handle, unsigned char* key);
extern __declspec(dllexport) int SVACPrefetchParam(unsigned char *pBuf, int iBufLen, SVAC_PREFETCH_PARAM *pPreParam);

/* $Function		   :	SVAC_Dec_Decode
************************************************************************
** Argument			:	handle		- 解码器句柄
**					:	deci			- 解码码流输入信息
**					:	deco		- 解码帧输出信息
**					:
** Performance 		:
**					:
** Return			:	-1-DEC_ERROR
**					:    0-DEC_SUCCESS
**					:    1-DEC_NEED_MORE_BITS
**					:    2-DEC_NO_PICTURE
**					:    3-DEC_BUF_OVERFLOW
**					:
** Description		:	解码一帧
**					:   1当解码返回DEC_SUCCESS时，说明解码完成一帧数据，
**					:   保持UINT8 *pBuf的值不变，UINT32 uBufLen改为0，
**					:   继续调用本函数。直到返回值是非DEC_SUCCESS。
**					:   2当解码返回DEC_NEED_MORE_BITS时，继续调用本函数, 
**					:   并传入新的数据和长度。
**					:   3当解码返回DEC_NO_PICTURE时，解码结束。
**					:   4当解码返回其他值时，出错。
**					:
** Modification		:
***********************************************************************/
extern __declspec(dllexport) int SVACDecFrame(void* hHandle, SVAC_DEC_INPUT_PARAM *deci, SVAC_DEC_OUTPUT_PARAM *deco);


extern __declspec(dllexport) void *SVACSeekOpen();
extern __declspec(dllexport) void SVACSeekClose(void *hseek);
extern __declspec(dllexport) int SVACSeekExtension(void *hseek, SVAC_SEEK_INPUT_PARAM *seeki, SVAC_SEEK_OUTPUT_PARAM *seeko);

extern __declspec(dllexport) void *SVACAuthOpen();
extern __declspec(dllexport) void SVACAuthClose(void *handle);
extern __declspec(dllexport) void SVACAuthSetKey(void *handle, SVAC_AUTH_KEY *param);
extern __declspec(dllexport) int SVACAuthDec(void *handle, SVAC_AUTH_INPUT_PARAM *authi);
#endif

#endif
