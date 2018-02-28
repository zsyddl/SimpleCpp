
#ifndef DHSYSABLE_H
#define DHSYSABLE_H


//////////////////////////////////////////////////////////////////////////
//	查询类型
typedef enum
{
	ABILITY_WATERMARK_CFG = 17,			//水印配置能力
	ABILITY_WIRELESS_CFG = 18,			//wireless配置能力
	ABILITY_DEVALL_INFO = 26,			//设备的能力列表
	ABILITY_CARD_QUERY = 0x0100,		//卡号查询能力
	ABILITY_MULTIPLAY = 0x0101,			//多画面预览能力
	ABILITY_INFRARED = 0X0121,			//无线报警能力
	ABILITY_TRIGGER_MODE = 0x0131,
	//2009-12-16 刘阳 修改 start
	ABILITY_SAVIA_COUNT = 33,		/* SAVIA数目*/
	//2009-12-16 刘阳 修改 end
	ABILITY_STR_LEN_EN = 35,		/* 字符串长度(非98设备) */
	//2009-12-23 刘阳 修改 start
	ABBI_TLV = 40,				//登陆返回包是否包含TLV数据
	ABBI_DECODER,				//解码器能力
	ABBI_AUTOREGISTER,			//主动注册能力
	ABBI_SAVIA,					//SAVIA能力
	ABBI_AUDIOIN,				//音频输入
	ABBI_PTZTOUR,				//云台软巡航能力
	ABBI_TALK,					//对讲能力
	ABBI_MVIEW,					//多画面预览画面分割组合能力
	ABBI_SNAP,					//抓拍能力
	//
	ABBI_DEVID,					//设备ID
	ABBI_VERSION,				//软件版本
	ABBI_AUTH_LIST,				//当前登录用户权限
	ABBI_SYS_ATTR_EX,			//设备属性
	//
	ABBI_ISTLVPROTOCAL,			//是否支持TLV协议
	ABBI_IVIDEO,				//平台接入能力
	ABBI_ENCODE,				//编码能力
	ABBI_TIME,					//时间能力
	ABBI_FTP,					//ftp能力
	ABBI_WHITEBLACK,			//黑白名单能力			2010-5-13 luhj add
	ABBI_DEVTYPE,				//设备类型
	ABBI_RTSP,					//RTSP能力
	ABBI_AUDIOVALUE,			//音频音量调节			2010-6-29 luhj add
	ABBI_RECORD,				//录像能力
} RV_SYS_ABILITY;

//////////////////////////////////////////////////////////////////////////
//	设备支持功能列表
enum 
{
	EN_FTP = 0,							//FTP 按位，1：传送录像文件 2：传送抓图文件
	EN_SMTP,							//SMTP 按位，1：报警传送文本邮件 2：报警传送图片
	EN_NTP,								//NTP	按位：1：调整系统时间
	EN_AUTO_MAINTAIN,					//自动维护 按位：1：重启 2：关闭 3:删除文件
	EN_VIDEO_COVER,						//区域遮挡 按位：1：多区域遮挡
	EN_AUTO_REGISTER,					//主动注册	按位：1：注册后sdk主动登陆
	EN_DHCP,							//DHCP	按位：1：DHCP
	EN_UPNP,							//UPNP	按位：1：UPNP
	EN_COMM_SNIFFER,					//串口抓包 按位：1:CommATM
	EN_NET_SNIFFER,						//网络抓包 按位： 1：NetSniffer
	EN_BURN,							//刻录功能 按位：1：查询刻录状态
	EN_VIDEO_MATRIX,					//视频矩阵 按位：1：是否支持视频矩阵
	EN_AUDIO_DETECT,					//音频检测 按位：1：是否支持音频检测
	EN_STORAGE_STATION,					//存储位置 按位：1：Ftp服务器(Ips) 2：U盘 3：NFS 4：DISK 5：SBM
	EN_IPSSEARCH,						//IPS存储查询 按位：1：IPS存储查询	
	EN_SNAP,							//抓图  按位：1：分辨率2：帧率3：抓图方式4：抓图文件格式5：图画质量
	EN_DEFAULTNIC,						//支持默认网卡查询 按位 1：支持
	EN_SHOWQUALITY,						//CBR模式下显示画质配置项 按位 1:支持
};

typedef struct 
{
	DWORD IsFucEnable[512];				//功能列表能力集,下标对应上述的枚举值,按位表示子功能
} RV_DEV_ENABLE_INFO;

//////////////////////////////////////////////////////////////////////////
//	卡号查询能力结构体
typedef struct 
{
	char		IsCardQueryEnable;
	char		iRev[3];
} RV_CARD_QUERY_EN;

//////////////////////////////////////////////////////////////////////////
//	wireless能力结构体
typedef struct 
{
	char		IsWirelessEnable;
	char		iRev[3];
} RV_WIRELESS_EN;

//////////////////////////////////////////////////////////////////////////
//	图象水印能力结构体
typedef struct 
{
	char		isSupportWM;	//1 支持；0 不支持
	char		supportWhat;	//0：文字水印；1：图片水印；2：同时支持文字水印和图片水印
	char		reserved[2];
} RV_WATERMAKE_EN;

//////////////////////////////////////////////////////////////////////////
//	多画面预览能力结构体
typedef struct  
{
	int			nEnable;			//1 支持；0 不支持
	DWORD		dwMultiPlayMask;	//保留
	char		reserved[4];
} RV_MULTIPLAY_EN;

//2009-12-16 刘阳 修改 start
//////////////////////////////////////////////////////////////////////////
//	SAVIA数目能力结构体
typedef struct 
{
	DWORD		dwCount;
} RV_SAVIA_COUNT;
//2009-12-16 刘阳 修改 end

typedef struct  
{
	BOOL bSupport;//是否支持
	int	 nAlarmInCount;//输入个数
	int  nAlarmOutCount;//输出个数
	int  nRemoteAddrCount;//遥控器个数
	BYTE reserved[32];
}RV_WIRELESS_ALARM_INFO;

//2010-2-23 刘阳 修改 start
//////////////////////////////////////////////////////////////////////////
//	云台软巡航能力结构体
typedef  struct
{
	unsigned short usPresetNums ;  //每个通道预置点最大个数
	unsigned short usTourNums;     //每个通道最大巡航轨迹个数
    int iRes [4]; //保留
}RV_PTZ_TOUR_EN;
//////////////////////////////////////////////////////////////////////////
//	主动注册能力结构体
typedef  struct
{
	unsigned short usRegServerNums ;	//主动注册服务器个数 现有实现为1个
	unsigned short usNetMode;			//主动注册模式
	// 支持多种网络模式 每一位标识是否支持 0标识不支持，1标识支持
	// 第0位 视频和信令单连接
	// 第1位 多连接（媒体多连接，支持设备主动连接服务器发送视频）
    int iRes [4];						//保留
}RV_AUTO_REGISTER_EN;
//////////////////////////////////////////////////////////////////////////
//	解码器能力结构体
typedef  struct
{
	unsigned short usDecoderNums ;  //解码器最大路数
	unsigned short usNetMode;		//支持多种网络模式 每一位标识是否支持 0标识不支持，1标识支持
									// 第0位 通用网络vs100，vs200；默认此为支持 置1
									// 第1位 支持rtsp Client
									//  第2位 支持vs300；
	int iAbilityMask;				//每路解码器解码能力 ，现有最大16通道，32位每2位标识一个通道
									// 00 最大支持CIF，01 最大支持D1，10，最大支持720p, 11 更高
	unsigned char ucDecoderType; 	// 解码器形式， 如果纯解码器，vs100根据需求裁剪页面
									// 0混合dvr（既有模拟也可以数字），1纯解码器（只有数字）
	unsigned char ucModeAblity;//// 第0位 是否支持多连接轮巡的方式
	unsigned short  usMaxChNum;//支持多连接方式时,每路支持的轮巡列表的通道的最大数目
	int iRes [3];					//保留
}RV_DECODE_EN;
//////////////////////////////////////////////////////////////////////////
//	抓拍能力结构体
typedef struct
{	
    //!按位表示 0-触发抓拍, 1-定时抓拍, 2-告警抓拍
    unsigned long type_mask;
	unsigned long uiSnapMaxCount;/*最大支持抓拍张数*/
	unsigned long uiCountsPerSecond;/*!<1s几张图片，按位表示
									第0位 1s1张
									第1位 1s2张
									第2位 1s3张
									第3位 1s4张
									第4位 1s5张
									*/					
	unsigned long uiMaxSeconds;/*最大支持几秒1张*/
	unsigned long uiRes[28];
}RV_ABILITY_SNAP_T;
//////////////////////////////////////////////////////////////////////////
//	智能模块能力结构体
typedef struct
{
	unsigned char ucSaviaNums ;		//savia数目
	unsigned char ucSurportAl;		//支持的算法
	unsigned char ucRes[22];		//保留字
}RV_SAVIA_EN;
//////////////////////////////////////////////////////////////////////////
//	对讲（对讲音频输入+音频输出）能力结构体
typedef struct
{
	unsigned char ucSurportTalkIn ;  	//对讲音频输入（设备存在单独的音频采集） 1:支持：0：不支持
	unsigned char ucTalkInMask;     	//设备音频输入编码格式类型  按位标识是否支持编码格式 
	//第0位 是否支持g711a， 第1位 pcm8k16b 第2位 g711u等
	unsigned char ucSurportTalkOut;		//对讲音频输出   1:支持：0：不支持
	unsigned char ucTalkOutMask;     	//设备音频解码格式类型  按位标识是否支持解码格式
	//第0位 是否支持g711a， 第1位 pcm8k16b  第2位 g711u等
}RV_TALK_EN;
//////////////////////////////////////////////////////////////////////////
//	音频输入能力结构体
typedef struct
{
	unsigned long uiAudio ;				//0~31为依次表示通道1~通道32有音频输入 1标识有，0标识
	unsigned long uiAudioMask ;  		//设备伴音编码格式类型  按位标识是否支持编码格式 
	//第0位 是否支持g711a， 第1位 pcm8k16b  第2位 g711u等
}RV_AUDIO_IN_EN;
//////////////////////////////////////////////////////////////////////////
//	画面分割组合能力结构体
#define MAX_SPLIT_COMINE_NUMS 32
#define MAX_SPLITCOMBINE_NUMBER 32
typedef struct
{
	unsigned int iChMask;//通道掩码，按位标识，总32位4字节,最大32种形式
						 /* 例如 4画面 ，支持 模式（1，2，3，4通道）（1，3，4，5）
						 对应的掩码为  0000 0000 0000 0000 0000 0000 0000 1111=0x0F
						 0000 0000 0000 0000 0000 0000 0001 1101=0x1D
						 如果9画面 支持（1，2，4，5，6，7，8，9，10通道）
						 对应的掩码为0000 0000 0000 0000 0000 0011 1111 1011=0x3FB
						 此处掩码内容，是需要根据 SPLIT_MODE中uiSplitType来定，现有制定是包含关系还是排除关系
							*/
	unsigned char ucSpecialCh;/*特殊通道（约束为自动排在最前或者大窗口显示）*/
	unsigned char ucRes[3];
}RV_SPLIT_COMBINE_CH;
typedef struct
{
	unsigned char ucSplitMode;/*多画面预览模式，参考枚举split_combine_t*/
	unsigned char ucSplitType;    
								/*支持分屏组合的类型 ，每两位对应画面预览模式iSplitMode一位
								0 全部任意组合， 
								如果为任意
								1 包含关系，只有少部分组合组合满足，现有约束最大64个；
								2 排除关系，最大64个排除组合；
								如果是包含或者排除方式，SPLIT_COMBINE数组标示具体信息
								其他 暂无定义，后续扩展使用
								*/
	unsigned char ucCombinNums;	/*组合中总个数，现有最大N_SPLIT_COMINE_NUMS 32个*/
	unsigned char ucRes;
	RV_SPLIT_COMBINE_CH stCombineCh[MAX_SPLIT_COMINE_NUMS];
}RV_SPLIT_COMBINE;
typedef struct
{
	unsigned int iSplitMode;  
						//支持多种画面预览模式，按位标识，参考枚举split_combine_t
						// 第0位 SPLIT1 此默认都支持，此为单画面，全屏
						// 第1位 SPLIT2 
						//  第2位 SPLIT4
						// 第3位 SPLIT8 
						//  第4位 SPLIT9
						// 第5位 SPLIT16 
						//  第6位 SPLITIP
						// 第7位 SPLIT6 
						//  第8位 SPLIT12
	long lCount;		//SPLIT_COMBINE的数目
	RV_SPLIT_COMBINE m_stSplitCombi[MAX_SPLITCOMBINE_NUMBER];//SPLIT_COMBINE数组
}RV_SPLIT_EN;
//////////////////////////////////////////////////////////////////////////
//	登陆返回的用户权限
typedef struct
{
	unsigned long uiMonitorMask;
	unsigned long uiPlayMask;
	unsigned long uiNetPreviewMask;
	unsigned long uiControlConfigMask;
	unsigned long uiReserved[4];
}RV_USER_AUTH_LIST;
//////////////////////////////////////////////////////////////////////////
//	登陆返回的设备属性
typedef struct
{
	unsigned char iVideoInCaps;     // 视频输入接口数量
	unsigned char iVideoOutCaps;     //视频输出接口数量
	unsigned char iAudioInCaps;     // 音频输入接口数量
	unsigned char iAudioOutCaps;     // 音频输出接口数量	
	unsigned char iAlarmInCaps;     // 报警输入接口数 
	unsigned char iAlarmOutCaps;    // 报警输出接口数
	unsigned char iDiskNum;          // 实际使用硬盘数 sdk的byDiskNum
	unsigned char iAetherNetPortNum;// 网络接口数sdk的iIsMutiEthIf 	
	unsigned char iUsbPortNum;          // USB接口数
	unsigned char iDecodeChanNum;     // 本地解码(回放)路数 
	unsigned char iComPortNum;      // 串口数
	unsigned char iParallelPortNum;   // 并口口数	
	unsigned char iSpeechInCaps;     // 对讲输入接口数量
	unsigned char iSpeechOutCaps;     // 对讲输出接口数量
	unsigned char bRes[2];   // 保留字	
}RV_SYSATTR_EX_T;
//平台接入能力结构体
typedef struct
{
	int iIVideoMask;//支持平台的掩码 第0位 BELL 第1位 HXHT 第2位 VS300 .可能设备支持多个平台接入
	int iRes[3];
}RV_IVIDEO_EN;
//编码能力结构体
typedef struct
{
	unsigned int	uiMaxEncodePower;				//产品支持的最高编码能力
	unsigned short	usSupportChannel;				//- 每块 DSP 支持最多输入视频通道数 
	unsigned short	usChannelSetSync;				//- DSP 每通道的最大编码设置是否同步 0-不同步, 1-同步
													//每一通道支持码流情况
	unsigned char	ucVideoStandardMask;			//视频制式掩码，按位表示设备能够支持的视频制式 0 PAL 1 NTSC
	unsigned char	ucEncodeModeMask;				//编码模式掩码，按位表示设备能够支持的编码模式设置 ，0 VBR，1 CBR
	unsigned short	usStreamCap;					//按位表示设备支持的多媒体功能，
													//第一位表示支持主码流
													//第二位表示支持辅码流1
													//第三位表示支持jpg抓图，捉图功能另外定义，在此不加入
	unsigned int	uiImageSizeMask;				//主码流编码掩码 枚举按照capture_size_t 
	unsigned int	uiImageSizeMask_Assi[32];		//主码流取不同的编码时候，辅码流支持的编码格式，例如, 
													//uiImageSizeMask_Assi【0】表示主码流是CAPTURE_SIZE_D1时，辅码流支持的编码格式，
													//uiImageSizeMask_Assi【1】表示主码流是CAPTURE_SIZE_HD1时，辅码流支持的编码格式。。。
	unsigned char	ucSupportPolicy;				//是否支持特殊编码策略 0 不支持 1 支持
	unsigned char	ucRes[11];						//保留
}RV_ENCODE_CAPS_NET_EN;

//2010-2-23 刘阳 修改 end
//时间能力结构体
typedef struct
{
	unsigned char ucSupTimeAdjust;  //是否支持校时功能
									//按位操作
									//第0位 Dst 支持夏令时
									//第一位 SNTP（udp ：123）， 现在默认是此ntp
									//第二位 NTP（udp：37，tcp：37）
	unsigned char ucRes[11];   //保留字
}RV_TIME_EN;

//////////////////////////////////////////////////////////////////////////
//	ftp能力结构体
typedef struct
{
	//按位操作
    //第0位 支持ftp录像上传
    //第1位 支持ftp图片上传
	unsigned char ucSupFtp;
	//保留字 
	unsigned char ucRes[15];
}RV_FTP_EN;

//2010-5-13 luhj add
//黑白名单能力结构体
typedef struct  
{
	unsigned short	usMaxBlackNums;	//最大黑名单数目
	unsigned short	usMaxWhiteNums;	//最大白名单数目
	unsigned long	res[12];		//保留
}RV_WHITEBLACK_EN;
// 
// //   音量能力结构体
// typedef struct
// {	
// 	unsigned long uiAudioVolumn;    //0~31为依次表示通道~通道音频输入支持音量控制,音频输入"有无"能力集参考RV_AUDIO_IN_EN	
// 	unsigned char ucTalk;           //第位表示对讲输入音频支持音量控制,对讲音频输入"有无"能力集参考RV_TALK_EN	
// 	unsigned char ucRes[3];	
// 	unsigned long uiRes[14];	
// }RV_AUDIOVALUE_EN;

//   音量能力结构体

typedef struct
{	
    //0~31为依次表示通道~通道音频输入支持音量控制,音频输入"有无"能力集参考AUDIO_IN_ABILITY
    unsigned long uiAudioVolumn ;    
    //第0位表示对讲输入音频支持音量控制,对讲音频输入"有无"能力集参考TALK_ABILITY
    //第1位表示对讲输出音频支持音量控制和静音,对讲音频输出"有无"能力集参考TALK_ABILITY
    //第2位表示对讲输入音频支持MIC输入
    //第3位表示对讲输入音频支持Liner输入
    unsigned char ucTalk;          
    //音频输入的声道，1：单声道；2：双声道
    unsigned char ucAudioChannel;    
    unsigned char ucRes;
    //音频输出(对讲输出)的声道，1：单声道；2：双声道
    unsigned char ucAudioOutChannel; 
    unsigned long uiRes[14];	
}RV_AUDIOVALUE_EN;


//add end

/////////////////////////////////////////////////////////////////////////
//	rtsp能力结构体
typedef struct  
{
	unsigned char ucAbilty;//第1位表示是否支持rtsp实时监视，第2位表示是否支持rtsp录像回放
	unsigned char Res[31] ;//保留
}RV_RTSP_EN;

/////////////////////////////////////////////////////////////////////////
//	非98设备字符串长度能力结构体
typedef struct  
{
	unsigned short usPPPoeNameLen;//pppoe用户名长度（包括null字符）
	unsigned short usPPPoePwdLen;//pppoe密码长度（包括null字符）
	unsigned short usRes[30] ;//保留
}RV_STR_LEN_EN;

//////////////////////////////////////////////////////////////////////////
//      录像相关能力结构体
typedef struct
{
	unsigned long dwFlag;//第0位表示支持新的"录像控制"协议
	unsigned char ucRes[16];
}RV_RECORD_EN;

#endif // DHSYSABLE_H

