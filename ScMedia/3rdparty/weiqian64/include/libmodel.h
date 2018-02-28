
#ifndef DHNETSDK_H
#define DHNETSDK_H

//#include "HikClient.h"
#include "libclient.h"

#ifdef WIN32

#ifdef DHNETSDK_EXPORTS
#define CLIENT_API  __declspec(dllexport) 
#else	//#ifdef DHNETSDK_EXPORTS
#define CLIENT_API  __declspec(dllimport)   //VC 用
#endif

#define CALLBACK __stdcall
#define CALL_METHOD  __stdcall  //__cdecl

#else	//linux

#define CLIENT_API	extern "C"
#define CALL_METHOD
#define CALLBACK

//#define RELEASE_HEADER	//发布头文件
#ifdef RELEASE_HEADER

#define WORD	unsigned short
#define DWORD	unsigned long
#define LPDWORD	DWORD*
#define BOOL	unsigned long
#define TRUE	0x01
#define FALSE	0x00
#define BYTE	unsigned char
#define LONG	long
#define UINT	unsigned int
#define HDC		void*
#define HWND	void*
#define LPVOID	void*
#define NULL	0
typedef struct  tagRECT
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
}RECT;
#else	//内部编译
#include "../netsdk/osIndependent.h"
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

	/************************************************************************
	** 常量定义
	***********************************************************************/
#define RV_SERIALNO_LEN 		48		//设备序列号字符长度
#define RV_MAX_DISKNUM 			32		//最大硬盘个数
#define RV_MAX_SDCARDNUM		32		//最大SD卡个数
#define RV_MAX_BURNING_DEV_NUM	32		//最大刻录设备个数
#define RV_BURNING_DEV_NAMELEN	32		//刻录设备名字最大长度
#define RV_MAX_LINK 			6		
#define RV_MAX_CHANNUM 			16		//最大通道个数
#define RV_MAX_ALARMIN 			128		//最大报警输入个数
#define RV_MAX_ALARMOUT 		64		//最大报警输出个数

#define RV_MAX_RIGHT_NUM		100		//用户权限个数上限
#define RV_MAX_GROUP_NUM		20		//用户组个数上限
#define RV_MAX_USER_NUM			200		//用户个数上限
#define RV_RIGHT_NAME_LENGTH	32		//权限名长度
#define RV_USER_NAME_LENGTH		8		//用户名长度
#define RV_USER_PSW_LENGTH		8		//用户密码长度
#define RV_MEMO_LENGTH			32		//备注长度

#define RV_MAX_STRING_LEN		128
	//chenfeng20090520 -s
#define RV_DEF_REALPLAY_BUF_NUM (3) 
#define RV_MIN_REALPLAY_BUF_NUM (1) 
#define RV_MAX_REALPLAY_BUF_NUM (50) 
	//chenfeng20090520 -E

	//侦听设备回调类型
#define RV_DVR_SERIAL_RETURN 1   //设备发送序列号回调
#define RV_DVR_DISCONNECT -1      //验证期间设备断线回调

	//通道画面字符叠加相关常量
#define MAX_STRING_LINE_LEN	6		//最多六行
#define MAX_PER_STRING_LEN  20		//每行最大长度

	//2010-1-12刘阳修改start
	//DDNS数目定义
#define MAX_DDNS_COUNT 16
	//2010-1-12刘阳修改end

	//回调报警信息类型，对应CLIENT_StartListen
#define RV_COMM_ALARM			0x1100	//常规报警信息(包括外部报警，视频丢失，动态检测)
#define RV_SHELTER_ALARM		0x1101	//视频遮挡报警
#define RV_DISK_FULL_ALARM		0x1102	//硬盘满报警
#define RV_DISK_ERROR_ALARM		0x1103	//硬盘故障报警
#define RV_SOUND_DETECT_ALARM	0x1104	//音频检测报警
#define RV_ALARM_DECODER_ALARM	0x1105	//报警解码器报警

	//扩展回调报警信息类型，对应CLIENT_StartListenEx
	//各报警类型详细分开
#define RV_ALARM_ALARM_EX			0x2101	//外部报警
#define RV_MOTION_ALARM_EX			0x2102	//动态检测报警
#define RV_VIDEOLOST_ALARM_EX		0x2103	//视频丢失报警
#define RV_SHELTER_ALARM_EX			0x2104	//遮挡报警
#define RV_SOUND_DETECT_ALARM_EX	0x2105	//音频检测报警
#define RV_DISKFULL_ALARM_EX		0x2106	//硬盘满报警
#define RV_DISKERROR_ALARM_EX		0x2107	//坏硬盘报警
#define RV_ENCODER_ALARM_EX			0x210A	//编码器报警
#define RV_URGENCY_ALARM_EX			0x210B	//紧急报警
#define RV_WIRELESS_ALARM_EX		0x210C	//无线报警
#define RV_NEW_SOUND_DETECT_ALARM_EX 0x210D //新音频检测报警报警信息的结构体见RV_NEW_SOUND_ALARM_STATE;
#define RV_DECODER_ALARM_EX			0x210E	//解码器报警

#define RV_SAVIA_ALARM_TRACK		0x210F	//智能：跟踪算法报警
#define RV_SAVIA_ALARM_CROSSLINE	0x2110	//智能：越线算法报警
#define RV_SAVIA_ALARM_CROSSNET		0x2111	//智能：入侵算法报警
#define RV_SAVIA_ALARM_WANDER		0x2112	//智能：徘徊算法报警
#define RV_SAVIA_ALARM_CONVERSE		0x2113	//智能：逆行算法报警
#define RV_SAVIA_ALARM_STATIC		0x2114	//智能：遗留算法报警
#define RV_SAVIA_ALARM_LOST			0x2115	//智能：遗失算法报警
#define RV_SAVIA_ALARM_DAMAGE		0x2116	//智能：喷涂算法报警
#define RV_INTE_ALARM_SMOKEFIRE		0x2117	//智能：烟火算法报警

#define RV_INTE_ALARM_RED_LEVEL_1	0x2118	//智能：智能红色检测告警1档
#define RV_INTE_ALARM_RED_LEVEL_2	0x2119	//智能：智能红色检测告警2档
#define RV_INTE_ALARM_RED_LEVEL_3	0x211a	//智能：智能红色检测告警3档
#define RV_INTE_ALARM_RED_LEVEL_4	0x211b	//智能：智能红色检测告警4档
#define RV_INTE_ALARM_RED_LEVEL_5	0x211c	//智能：智能红色检测告警5档
#define RV_INTE_ALARM_RED_LEVEL_6	0x211d	//智能：智能红色检测告警6档


#define RV_CONFIG_RESULT_EVENT_EX	0X3000  //修改配置的返回码返回结构见DEV_SET_RESULT
#define RV_REBOOT_EVENT_EX			0x3001	//设备重启事件
#define RV_AUTO_TALK_START_EX		0x3002  //设备主动开始语音对讲
#define RV_AUTO_TALK_STOP_EX		0x3003  //设备主动停止语音对讲

	//查询设备状态接口参数：查询类型 用于CLIENT_QueryDevState参数
#define RV_DEVSTATE_COMM_ALARM		0x0001	//查询普通报警(包括外部报警，视频丢失，动态检测)
#define RV_DEVSTATE_SHELTER_ALARM	0x0002	//查询遮挡报警
#define RV_DEVSTATE_RECORDING		0x0003	//查询录象状态
#define RV_DEVSTATE_DISK			0x0004	//查询硬盘状态
#define RV_DEVSTATE_RESOURCE		0x0005	//系统资源状态
#define RV_DEVSTATE_BITRATE			0x0006	//获取通道码流
#define RV_DEVSTATE_CONN			0x0007	//获取设备连接状态
#define RV_DEVSTATE_PROTOCAL_VER	0x0008	//获取网络协议版本号,	pBuf = int*
#define RV_DEVSTATE_TALK_ECTYPE		0x0009	//获取设备支持的语音对讲格式列表，见结构体DHDEV_TALKFORMAT_LIST
#define RV_DEVSTATE_SD_CARD			0x000A	//获取SD卡信息（IPC类产品）
#define RV_DEVSTATE_BURNING_DEV			0x000B	//查询刻录机信息
#define RV_DEVSTATE_BURNING_PROGRESS	0x000C	//查询刻录进度
#define RV_DEVSTATE_PLATFORM		0x000D	//获取设备支持的接入平台
#define RV_DEVSTATE_CAMERA			0x000E	//获取摄像头属性信息，pBuf = DHDEV_CAMERA_INFO *，可以有多个结构体
#define RV_DEVSTATE_SOFTWARE		0x000F	//设备软件版本信息
#define RV_DEVSTATE_LANGUAGE        0x0010  //设备支持的语音种类
#define RV_DEVSTATE_DSP				0x0011	//DSP能力描述
#define	RV_DEVSTATE_OEM				0x0012	//OEM
#define	RV_DEVSTATE_NET				0x0013  //网络运行状态信息
#define RV_DEVSTATE_TYPE			0x0014  //设备类型
#define RV_DEVSTATE_SNAP			0x0015	 //抓图功能能力查询
	//抓图视频编码类型
#define CODETYPE_MPEG4   0
#define CODETYPE_H264    1
#define CODETYPE_JPG     2

	//错误类型代号，CLIENT_GetLastError函数的返回
#define _EC(x)						(0x80000000|x)
#define NET_NOERROR 				0				//没有错误
#define NET_ERROR					-1				//未知错误
#define NET_SYSTEM_ERROR			_EC(1)			//Windows系统出错
#define NET_NETWORK_ERROR			_EC(2)			//网络错误，可能是因为网络超时
#define NET_DEV_VER_NOMATCH			_EC(3)			//设备协议不匹配
#define NET_INVALID_HANDLE			_EC(4)			//句柄无效
#define NET_OPEN_CHANNEL_ERROR		_EC(5)			//打开通道失败
#define NET_CLOSE_CHANNEL_ERROR		_EC(6)			//关闭通道失败
#define NET_ILLEGAL_PARAM			_EC(7)			//用户参数不合法
#define NET_SDK_INIT_ERROR			_EC(8)			//SDK初始化出错
#define NET_SDK_UNINIT_ERROR		_EC(9)			//SDK清理出错
#define NET_RENDER_OPEN_ERROR		_EC(10)			//申请render资源出错
#define NET_DEC_OPEN_ERROR			_EC(11)			//打开解码库出错
#define NET_DEC_CLOSE_ERROR			_EC(12)			//关闭解码库出错
#define NET_MULTIPLAY_NOCHANNEL		_EC(13)			//多画面预览中检测到通道数为0
#define NET_TALK_INIT_ERROR			_EC(14)			//录音库初始化失败
#define NET_TALK_NOT_INIT			_EC(15)			//录音库未经初始化
#define	NET_TALK_SENDDATA_ERROR		_EC(16)			//发送音频数据出错
#define NET_REAL_ALREADY_SAVING		_EC(17)			//实时数据已经处于保存状态
#define NET_NOT_SAVING				_EC(18)			//未保存实时数据
#define NET_OPEN_FILE_ERROR			_EC(19)			//打开文件出错
#define NET_PTZ_SET_TIMER_ERROR		_EC(20)			//启动云台控制定时器失败
#define NET_RETURN_DATA_ERROR		_EC(21)			//对返回数据的校验出错
#define NET_INSUFFICIENT_BUFFER		_EC(22)			//没有足够的缓存
#define NET_NOT_SUPPORTED			_EC(23)			//当前SDK未支持该功能
#define NET_NO_RECORD_FOUND			_EC(24)			//查询不到录象
#define NET_NOT_AUTHORIZED			_EC(25)			//无操作权限
#define NET_NOT_NOW					_EC(26)			//暂时无法执行
#define NET_NO_TALK_CHANNEL			_EC(27)			//未发现对讲通道
#define NET_NO_AUDIO				_EC(28)			//未发现音频
#define NET_NO_INIT					_EC(29)			//CLientSDK未经初始化
#define NET_DOWNLOAD_END			_EC(30)			//下载已结束
#define NET_EMPTY_LIST				_EC(31)			//查询结果为空

#define NET_ERROR_GETCFG_SYSATTR	_EC(32)			//获取配置失败位置：系统属性
#define NET_ERROR_GETCFG_SERIAL		_EC(33)			//获取配置失败位置：序列号
#define NET_ERROR_GETCFG_GENERAL	_EC(34)			//获取配置失败位置：常规属性
#define NET_ERROR_GETCFG_DSPCAP		_EC(35)			//获取配置失败位置：DSP能力描述
#define NET_ERROR_GETCFG_NETCFG		_EC(36)			//获取配置失败位置：网络属性
#define NET_ERROR_GETCFG_CHANNAME	_EC(37)			//获取配置失败位置：通道名称
#define NET_ERROR_GETCFG_VIDEO		_EC(38)			//获取配置失败位置：视频属性
#define NET_ERROR_GETCFG_RECORD		_EC(39)			//获取配置失败位置：录象定时配置
#define NET_ERROR_GETCFG_PRONAME	_EC(40)			//获取配置失败位置：解码器协议名称
#define NET_ERROR_GETCFG_FUNCNAME	_EC(41)			//获取配置失败位置：232串口功能名称
#define NET_ERROR_GETCFG_485DECODER	_EC(42)			//获取配置失败位置：解码器属性
#define NET_ERROR_GETCFG_232COM		_EC(43)			//获取配置失败位置：232串口属性
#define NET_ERROR_GETCFG_ALARMIN	_EC(44)			//获取配置失败位置：外部报警输入属性
#define NET_ERROR_GETCFG_ALARMDET	_EC(45)			//获取配置失败位置：图像检测报警属性
#define NET_ERROR_GETCFG_SYSTIME	_EC(46)			//获取配置失败位置：设备时间
#define NET_ERROR_GETCFG_PREVIEW	_EC(47)			//获取配置失败位置：预览参数
#define NET_ERROR_GETCFG_AUTOMT		_EC(48)			//获取配置失败位置：自动维护配置
#define NET_ERROR_GETCFG_VIDEOMTRX	_EC(49)			//获取配置失败位置：视频矩阵配置
#define NET_ERROR_GETCFG_COVER		_EC(50)			//获取配置失败位置：视频区域遮挡
#define NET_ERROR_GETCFG_WATERMAKE	_EC(51)			//获取配置失败位置：图象水印配置
#define NET_ERROR_GETCFG_ETHERNET	_EC(300)		//获取配置失败位置：网卡配置
#define NET_ERROR_GETCFG_WLAN		_EC(301)		//获取配置失败位置：无线网络信息
#define NET_ERROR_GETCFG_WLANDEV	_EC(302)		//获取配置失败位置：搜索无线网络设备
#define NET_ERROR_GETCFG_REGISTER	_EC(303)		//获取配置失败位置：主动注册参数配置
#define NET_ERROR_GETCFG_CAMERA		_EC(304)		//获取配置失败位置：摄像头属性配置
#define NET_ERROR_GETCFG_INFRARED	_EC(305)		//获取配置失败位置：红外报警配置
#define NET_ERROR_GETCFG_SOUNDALARM	_EC(306)		//获取配置失败位置：音频报警配置
#define NET_ERROR_GETCFG_STORAGE    _EC(307)		//获取配置失败位置：存储位置配置
#define NET_ERROR_GETCFG_MAIL		_EC(308)		//获取邮件配置失败


#define NET_ERROR_SETCFG_GENERAL	_EC(55)			//设置配置失败位置：常规属性
#define NET_ERROR_SETCFG_NETCFG		_EC(56)			//设置配置失败位置：网络属性
#define NET_ERROR_SETCFG_CHANNAME	_EC(57)			//设置配置失败位置：通道名称
#define NET_ERROR_SETCFG_VIDEO		_EC(58)			//设置配置失败位置：视频属性
#define NET_ERROR_SETCFG_RECORD		_EC(59)			//设置配置失败位置：录象定时配置
#define NET_ERROR_SETCFG_485DECODER	_EC(60)			//设置配置失败位置：解码器属性
#define NET_ERROR_SETCFG_232COM		_EC(61)			//设置配置失败位置：232串口属性
#define NET_ERROR_SETCFG_ALARMIN	_EC(62)			//设置配置失败位置：外部报警输入属性
#define NET_ERROR_SETCFG_ALARMDET	_EC(63)			//设置配置失败位置：图像检测报警属性
#define NET_ERROR_SETCFG_SYSTIME	_EC(64)			//设置配置失败位置：设备时间
#define NET_ERROR_SETCFG_PREVIEW	_EC(65)			//设置配置失败位置：预览参数
#define NET_ERROR_SETCFG_AUTOMT		_EC(66)			//设置配置失败位置：自动维护配置
#define NET_ERROR_SETCFG_VIDEOMTRX	_EC(67)			//设置配置失败位置：视频矩阵配置
#define NET_ERROR_SETCFG_COVER		_EC(69)			//设置配置失败位置：视频区域遮挡
#define NET_ERROR_SETCFG_WATERMAKE	_EC(60)			//设置配置失败位置：图象水印配置
#define NET_ERROR_SETCFG_WLAN		_EC(61)			//设置配置失败位置：无线网络信息
#define NET_ERROR_SETCFG_WLANDEV	_EC(62)			//设置配置失败位置：选择无线网络设备
#define NET_ERROR_SETCFG_REGISTER	_EC(63)			//设置配置失败位置：主动注册参数配置
#define NET_ERROR_SETCFG_CAMERA		_EC(64)			//设置配置失败位置：摄像头属性配置
#define NET_ERROR_SETCFG_INFRARED	_EC(65)			//设置配置失败位置：红外报警配置
#define NET_ERROR_SETCFG_SOUNDALARM	_EC(66)			//设置配置失败位置：音频报警配置
#define NET_ERROR_SETCFG_STORAGE    _EC(67)			//设置配置失败位置：存储位置配置

#define NET_AUDIOENCODE_NOTINIT		_EC(70)			//音频编码接口没有成功初始化
#define NET_DATA_TOOLONGH			_EC(71)			//数据过长

#define NET_UNSUPPORTED				_EC(72)			//设备不支持该操作
#define NET_DEVICE_BUSY				_EC(73)			//设备资源不足
#define NET_SERVER_STARTED			_EC(74)			//服务器已经启动
#define NET_SERVER_STOPPED			_EC(75)			//服务器尚未成功启动

#define NET_LISTER_INCORRECT_SERIAL	_EC(80)			//输入序列号有误
#define NET_QUERY_DISKINFO_FAILED	_EC(81)			//获取硬盘信息失败

#define NET_LOGIN_ERROR_PASSWORD	_EC(100)		//密码不正确
#define NET_LOGIN_ERROR_USER		_EC(101)		//帐户不存在
#define NET_LOGIN_ERROR_TIMEOUT		_EC(102)		//等待登录返回超时
#define NET_LOGIN_ERROR_RELOGGIN	_EC(103)		//帐号已登录
#define NET_LOGIN_ERROR_LOCKED		_EC(104)		//帐号已被锁定
#define NET_LOGIN_ERROR_BLACKLIST	_EC(105)		//帐号已被列为黑名单
#define NET_LOGIN_ERROR_BUSY		_EC(106)		//资源不足，系统忙
#define NET_LOGIN_ERROR_CONNECT		_EC(107)		//连接主机失败"
#define NET_LOGIN_ERROR_NETWORK		_EC(108)		//网络连接失败"

#define NET_RENDER_SOUND_ON_ERROR	_EC(120)		//Render库打开音频出错
#define NET_RENDER_SOUND_OFF_ERROR	_EC(121)		//Render库关闭音频出错
#define NET_RENDER_SET_VOLUME_ERROR	_EC(122)		//Render库控制音量出错
#define NET_RENDER_ADJUST_ERROR		_EC(123)		//Render库设置画面参数出错
#define NET_RENDER_PAUSE_ERROR		_EC(124)		//Render库暂停播放出错
#define NET_RENDER_SNAP_ERROR		_EC(125)		//Render库抓图出错
#define NET_RENDER_STEP_ERROR		_EC(126)		//Render库步进出错
#define NET_RENDER_FRAMERATE_ERROR	_EC(127)		//Render库设置帧率出错

#define NET_GROUP_EXIST				_EC(140)		//组名已存在
#define	NET_GROUP_NOEXIST			_EC(141)		//组名不存在
#define NET_GROUP_RIGHTOVER			_EC(142)		//组的权限超出权限列表范围
#define NET_GROUP_HAVEUSER			_EC(143)		//组下有用户，不能删除
#define NET_GROUP_RIGHTUSE			_EC(144)		//组的某个权限被用户使用，不能出除
#define NET_GROUP_SAMENAME			_EC(145)		//新组名同已有组名重复
#define	NET_USER_EXIST				_EC(146)		//用户已存在
#define NET_USER_NOEXIST			_EC(147)		//用户不存在
#define NET_USER_RIGHTOVER			_EC(148)		//用户权限超出组权限
#define NET_USER_PWD				_EC(149)		//保留帐号，不容许修改密码
#define NET_USER_FLASEPWD			_EC(150)		//密码不正确
#define NET_USER_NOMATCHING			_EC(151)		//密码不匹配


#define NET_CONFIG_DEVBUSY			_EC(999)		//暂时无法设置
#define NET_CONFIG_DATAILLEGAL		_EC(1000)		//配置数据不合法

	//远程配置接口CLIENT_GetDEVConfig,CLIENT_GetDEVConfig参数：命令定义
#define RV_DEV_DEVICECFG			1		//获取设备参数
#define RV_DEV_NETCFG				2		//获取网络参数
#define RV_DEV_CHANNELCFG			3		//获取通道配置－图像及压缩参数等
#define RV_DEV_PREVIEWCFG 			4		//获取预览参数
#define RV_DEV_RECORDCFG			5		//获取录像时间参数
#define RV_DEV_COMMCFG				6		//获取串口参数
#define RV_DEV_ALARMCFG 			7		//获取报警参数
#define RV_DEV_TIMECFG 				8		//获取DVR时间
#define RV_DEV_TALKCFG				9		//对讲参数
#define RV_DEV_AUTOMTCFG			10		//自动维护配置		
#define	RV_DEV_VEDIO_MARTIX			11		//本机矩阵控制策略配置	
#define RV_DEV_MULTI_DDNS			12		//多ddns服务器配置
#define RV_DEV_SNAP_CFG				13		//抓图相关配置
#define RV_DEV_WEB_URL_CFG			14		//HTTP路径配置
#define RV_DEV_FTP_PROTO_CFG		15		//FTP上传配置
#define RV_DEV_INTERVIDEO_CFG		16		//平台接入配置，此时channel参数代表平台类型，
	//channel=4  代表贝尔阿尔卡特
	//channel=10	 代表中兴力维
	//channel=11 代表U网通
#define RV_DEV_VIDEO_COVER			17		//区域遮挡配置
#define RV_DEV_TRANS_STRATEGY		18		//传输策略配置，画质优先\流畅性优先
#define RV_DEV_DOWNLOAD_STRATEGY	19		//录象下载策略配置，高速下载\普通下载
#define RV_DEV_WATERMAKE_CFG		20		//图象水印配置
#define RV_DEV_WLAN_CFG				21		//无线网络配置
#define RV_DEV_WLAN_DEVICE_CFG		22		//搜索无线设备配置
#define RV_DEV_REGISTER_CFG			23		//主动注册参数配置
#define RV_DEV_CAMERA_CFG			24		//摄像头属性配置
#define RV_DEV_INFRARED_CFG 		25		//获取红外报警参数
#define RV_DEV_SNIFFER_CFG			26		//Sniffer抓包配置
#define RV_DEV_MAIL_CFG				27		//邮件配置,建议以后采用此类型配置
#define RV_DEV_DNS_CFG				28		//DNS服务器配置
#define RV_DEV_NTP_CFG				29		//NTP配置
#define RV_DEV_AUDIO_DETECT_CFG		30		//音频检测配置
#define RV_DEV_STORAGE_STATION_CFG  31      //存储位置配置
#define RV_DEV_PTZ_OPT_CFG			32		//云台操作属性
	//add by HY 2009-11-2 begin
#define RV_DEV_DDNS_CFG				33		//DDNS配置
	//add by HY 2009-11-2 end
	//add by HY 2009-11-16 begin
#define RV_DEV_UPNP_CFG				34		//UPNP配置
	//add by HY 2009-11-16 end
	//add by 刘阳 2009-12-14 begin
#define RV_DEV_SAVIA_ALARM_CFG		35		//SAVIA报警参数配置
#define RV_DEV_SAVIA_COUNT_CFG		36		//SAVIA数目
	//add by 刘阳 2009-12-14 end
	//add by 刘阳 2009-12-28 start
#define RV_DEV_BLACKWHITE_CFG		37		//黑白名单参数
	//add by 刘阳 2009-12-28 end
	//add by 刘阳 2010-1-12 begin
#define RV_DEV_DDNS_CFG_EX			38		//DDNS配置
	//add by 刘阳 2010-1-12 end
	//add by 刘阳 2010-1-21 begin
#define RV_DEV_DECODER_CFG			39		//解码器配置
	//add by 刘阳 2010-1-21 end
	//add by 刘阳 2010-2-10 begin
#define RV_DEV_REG_SERVER_CFG		40		//主动注册配置
	//add by 刘阳 2010-2-10 end
	//add by 刘阳 2010-3-1 begin
#define RV_DEV_DISPLAY_SPLIT		41		//画面分割通道组合配置
#define RV_DEV_DISPLAY_TOUR			42		//轮巡配置
#define RV_DEV_DISPLAY_SPLITTOUR	43		//画面分割通道组合轮巡配置
#define RV_DEV_SNAP_CFG_NEW			44		//抓图相关配置
	//add by 刘阳 2010-3-1 end
#define RV_DEV_DECODER_ALARM_CFG	45		//解码器告警配置
#define RV_DEV_MARGIN_CFG			46		//TV调节配置
//2010-3-18 luhj-0007
#define RV_DEV_IPCONFLICT_CFG		47		//IP冲突配置
#define RV_DEV_MAIL_CFG_EX			48		//新邮箱配置		luhj add
#define RV_DEV_DEV_CFG				49		//设备信息(使用TLV协议)
#define RV_DEV_CHANNEL_ENC_CFG		50		//设备通道编码信息(使用TLV协议)
#define RV_DEV_NET_BASE_CFG			51		//基本网络配置(使用TLV协议)
#define RV_DEV_NET_APP_CFG			52		//应用网络配置(使用TLV协议)
#define RV_DEV_ALARM_CFG			53		//获取所有报警配置(使用TLV协议)
#define RV_DEV_ALARM_IN_CFG			54		//设置报警输入报警配置(使用TLV协议)
#define RV_DEV_ALARM_MOTION_CFG		55		//设置视频动检报警配置(使用TLV协议)
#define RV_DEV_ALARM_LOSS_CFG		56		//设置视频视频丢失报警配置(使用TLV协议)
#define RV_DEV_ALARM_BIND_CFG		57		//设置视频视频遮挡报警配置(使用TLV协议)
#define RV_DEV_ALARM_NODISK_CFG		58		//设置无硬盘报警配置(使用TLV协议)
#define RV_DEV_ALARM_ERRDISK_CFG	59		//设置硬盘错误报警配置(使用TLV协议)
#define RV_DEV_ALARM_FULLDISK_CFG	60		//设置硬盘满报警配置(使用TLV协议)
#define RV_DEV_ALARM_NETBROKEN_CFG	61		//设置断网报警配置(使用TLV协议)
#define RV_DEV_ALARM_IPCONFLICT_CFG	62		//设置IP冲突报警配置(使用TLV协议)

#define RV_DEV_INTERVIDEO_BELL_CFG	64		//bell平台接入配置
#define RV_DEV_INTERVIDEO_MEGA_CFG	65		//互信互通平台接入配置
#define RV_DEV_INTERVIDEO_VS300_CFG	66		//VS300平台接入配置
#define RV_DEV_INTERVIDEO_ZXSHENYAN_CFG	67		//神眼平台接入配置


#define RV_DEV_DISK_CFG				80		//磁盘配置(使用TLV协议)
#define RV_DEV_NET_COMM_CFG			81		//网络通用配置(使用TLV协议)
#define RV_DEV_EMAIL_CFG			82		//EMAIL配置(使用TLV协议)
#define RV_DEV_DNS_CFG_TLV			83		//dns配置(使用TLV协议)
#define RV_DEV_UPNP_CFG_TLV			84		//upnp配置(使用TLV协议)
#define RV_DEV_PPPOE_CFG			85		//pppoe配置(使用TLV协议)
#define RV_DEV_MDDNS_CFG			86		//DDNS配置(使用TLV协议)
#define RV_DEV_CODE_CFG				87		//编码配置(使用TLV协议)
#define RV_DEV_NTP_CFG_TLV			88		//NTP配置(使用TLV协议)
#define RV_DEV_OUT_MODE_CFG			89		//输出模式配置(使用TLV协议)

//2010-3-31 luhj-0016
#define	RV_DEV_PRESET_CFG			90//81		//云台预置点配置
#define RV_DEV_TOUR_CFG				91//82		//云台点间巡航配置
#define RV_DEV_ADDPRESET_CFG		92//83		//增加预置点操作
#define RV_DEV_DELETEPRESET_CFG		93//84		//删除预置点操作
#define	RV_DEV_ADDTOURPRESET_CFG	94//85		//增加巡航预置点操作
#define RV_DEV_DELTOURPRESET_CFG	95//86		//删除巡航预置点操作
#define	RV_DEV_CLEARTOUR_CFG		96//87		//删除巡航线路
//luhj-0016

#define	RV_DEV_FTP_CFG				97			//FTP设置
#define	RV_DEV_RTSP_CFG				98			//RTSP设置

#define	RV_DEV_INTERED_CFG			99			//智能红色检测报警设置

#define RV_DEV_NETCFG_RHEX			100			//获取网络参数(非98设备remotehost长度64字节)
#define	RV_DEV_RECCTL_CFG			101			//录像控制参数(使用TLV协议)
#define RV_DEV_COMMCFG_EX			102			//获取串口参数(超过16通道)
#define RV_DEV_CHANNEL_NAME			103			//获取通道名(使用TLV协议)
#define RV_DEV_AUDIO_T				104			//音频配置(使用TLV协议)
#define RV_DEV_DIGICHAN_TOUR		105			//数字通道轮训配置(使用TLV协议)

#define RV_DEV_USER_END_CFG			1000

	/////////////////////远程配置结构体相关常量///////////////////////
#define RV_MAX_MAIL_ADDR_LEN		128	//邮件发(收)人地址最大长度
#define RV_MAX_MAIL_SUBJECT_LEN		64	//邮件主题最大长度
#define RV_MAX_IPADDR_LEN			16	//IP地址字符串长度
#define RV_MACADDR_LEN				40	//MAC地址字符串长度
#define RV_MAX_URL_LEN				128	//URL字符串长度
#define RV_MAX_DEV_ID_LEN			48	//机器编号最大长度
#define	RV_MAX_HOST_NAMELEN			64  //32	//主机名字符串长度，
#define RV_MAX_HOST_PSWLEN			32	//长度
#define RV_MAX_HOST_PSWLEN_EX		64	//长度
#define RV_MAX_NAME_LEN				16	//通用名字字符串长度
#define RV_MAX_NAME_LEN_EX			32	//邮箱用户名字符串长度	luhj-0015	
#define RV_MAX_ETHERNET_NUM			2	//以太网口最大个数
#define	RV_DEV_SERIALNO_LEN			48	//序列号字符串长度
#define RV_DEV_TYPE_LEN				32	//设备类型字符串长度
#define RV_N_WEEKS					7	//一周的天数	
#define RV_N_TSECT					6	//通用时间段个数
#define RV_N_REC_TSECT				6	//录像时间段个数
#define RV_N_COL_TSECT				2	//颜色时间段个数	
#define RV_CHAN_NAME_LEN			32	//通道名长度,dvr DSP能力限制，最多32字节		
#define RV_N_ENCODE_AUX				3	//扩展码流个数	
#define RV_N_TALK					1	//最多对讲通道个数
#define RV_N_COVERS					1	//遮挡区域个数	
#define RV_N_CHANNEL				16	//最大通道个数	
#define RV_N_ALARM_TSECT			2	//报警提示时间段个数
#define RV_MAX_ALARMOUT_NUM			16	//报警输出口个数上限
#define RV_MAX_VIDEO_IN_NUM			16	//视频输入口个数上限
#define RV_MAX_ALARM_IN_NUM			16	//报警输入口个数上限
#define RV_MAX_DISK_NUM				16	//硬盘个数上限，暂定为16
#define RV_MAX_DECODER_NUM			16	//解码器（485）个数上限	
#define RV_MAX_DECODER_NUM_EX		32	//解码器个数上限	
#define RV_MAX_232FUNCS				10	//232串口功能个数上限
#define RV_MAX_232_NUM				2	//232串口个数上限
#define RV_MAX_DECPRO_LIST_SIZE		100	//解码器协议列表个数上限
#define RV_FTP_MAXDIRLEN			240	//FTP文件目录最大长度
#define RV_MATRIX_MAXOUT			16	//矩阵输出口最大个数
#define RV_TOUR_GROUP_NUM			6	//矩阵输出组最大个数
#define RV_MAX_DDNS_NUM				10	//设备支持的ddns服务器最大个数
#define RV_MAX_SERVER_TYPE_LEN		32	//ddns服务器类型 最大字符串长度
#define RV_MAX_DOMAIN_NAME_LEN		256	//ddns域名 最大字符串长度
#define RV_MAX_DDNS_ALIAS_LEN		32	//ddns服务器别名 最大字符串长度
#define RV_MOTION_ROW				32	//动态检测区域的行数
#define RV_MOTION_COL				32	//动态检测区域的列数
#define	RV_FTP_USERNAME_LEN			64  //FTP配置，用户名最大长度
#define	RV_FTP_PASSWORD_LEN			64  //FTP配置，密码最大长度
#define	RV_TIME_SECTION				2	//FTP配置，每天时间段个数
#define RV_FTP_MAX_PATH				240	//FTP配置，文件路径名最大长度
#define RV_INTERVIDEO_UCOM_CHANID	32	//平台接入配置，U网通通道ID
#define RV_INTERVIDEO_UCOM_DEVID	32	//平台接入配置，U网通设备ID
#define RV_INTERVIDEO_UCOM_REGPSW	16	//平台接入配置，U网通注册密码
#define RV_INTERVIDEO_UCOM_USERNAME	32	//平台接入配置，U网通用户名
#define RV_INTERVIDEO_UCOM_USERPSW	32	//平台接入配置，U网通密码
#define RV_INTERVIDEO_NSS_IP		32	//平台接入配置，中兴力维IP
#define RV_INTERVIDEO_NSS_SERIAL	32	//平台接入配置，中兴力维serial
#define RV_INTERVIDEO_NSS_USER		32	//平台接入配置，中兴力维user
#define RV_INTERVIDEO_NSS_PWD		50	//平台接入配置，中兴力维password

#define RV_MAX_VIDEO_COVER_NUM		16	//遮挡区域最大个数
#define RV_MAX_WATERMAKE_DATA		4096	//水印图片数据最大长度
#define RV_MAX_WATERMAKE_LETTER		128	//水印文字最大长度
#define RV_MAX_WLANDEVICE_NUM		10	//最多搜索出的无线设备个数
#define RV_MAX_ALARM_NAME			64	//地址长度
#define RV_MAX_REGISTER_SERVER_NUM	10	//主动注册服务器个数
#define RV_SNIFFER_FRAMEID_NUM		6   //6个FRAME ID 选项
#define RV_SNIFFER_CONTENT_NUM		4   //每个FRAME对应的4个抓包内容
#define RV_MAX_PROTOCOL_NAME_LENGTH 20
#define RV_SNIFFER_GROUP_NUM		4   //4组抓包设置
#define MAX_PATH_STOR              240  //远程目录的长度
#define MAX_HHCHANNELNAME_LEN		32	//数字通道名称长度
	//2009-12-15 刘阳 修改 start
#define RV_MAX_SAVIA_NUM			16	//savia数目最大值
	//2009-12-15 刘阳 修改 end
	//2009-12-28 刘阳 修改 start
#define RV_MAX_BLACK_IP_NUM			128	//黑名单数目最大值
#define RV_MAX_WHITE_IP_NUM			128	//白名单数目最大值
	//2009-12-28 刘阳 修改 end
	//2010-1-21 刘阳 修改 start
#define RV_MAX_DECODER_CHAN_NUM			32	//解码器通道数目最大值
	//2010-1-21 刘阳 修改 end


	//分辨率列表，用于与分辨率掩码进行与、或操作
#define	RV_CAPTURE_SIZE_D1			0x00000001
#define RV_CAPTURE_SIZE_HD1			0x00000002
#define RV_CAPTURE_SIZE_BCIF		0x00000004
#define RV_CAPTURE_SIZE_CIF			0x00000008
#define RV_CAPTURE_SIZE_QCIF		0x00000010	
#define RV_CAPTURE_SIZE_VGA			0x00000020	
#define RV_CAPTURE_SIZE_QVGA		0x00000040
#define RV_CAPTURE_SIZE_SVCD		0x00000080
#define RV_CAPTURE_SIZE_QQVGA		0x00000100
#define RV_CAPTURE_SIZE_SVGA		0x00000200
#define RV_CAPTURE_SIZE_XVGA		0x00000400
#define RV_CAPTURE_SIZE_WXGA		0x00000800
#define RV_CAPTURE_SIZE_SXGA		0x00001000
#define RV_CAPTURE_SIZE_WSXGA		0x00002000   
#define RV_CAPTURE_SIZE_UXGA		0x00004000
#define RV_CAPTURE_SIZE_WUXGA       0x00008000

	//云台属性列表，用于与分辨率掩码进行与、或操作
	//lowMask
#define PTZ_DIRECTION				0x00000001				/*!< 方向 */
#define PTZ_ZOOM					0x00000002	   			/*!< 变倍 */
#define PTZ_FOCUS					0x00000004				/*!< 聚焦 */
#define PTZ_IRIS					0x00000008				/*!< 光圈 */
#define PTZ_ALARM					0x00000010				/*!< 报警功能 */
#define PTZ_LIGHT					0x00000020				/*!< 灯光 */
#define PTZ_SETPRESET				0x00000040				/*!< 设置预置点 */
#define PTZ_CLEARPRESET				0x00000080				/*!< 清除预置点 */
#define PTZ_GOTOPRESET				0x00000100				/*!< 转至预置点 */
#define PTZ_AUTOPANON				0x00000200				/*!< 水平开始 */
#define PTZ_AUTOPANOFF				0x00000400				/*!< 水平结束 */
#define PTZ_SETLIMIT				0x00000800				/*!< 设置边界 */
#define PTZ_AUTOSCANON				0x00001000				/*!< 自动扫描开始 */
#define PTZ_AUTOSCANOFF				0x00002000				/*!< 自动扫描开停止 */
#define PTZ_ADDTOUR					0x00004000				/*!< 增加巡航点 */
#define PTZ_DELETETOUR				0x00008000				/*!< 删除巡航点 */
#define PTZ_STARTTOUR				0x00010000				/*!< 开始巡航 */
#define PTZ_STOPTOUR				0x00020000				/*!< 结束巡航 */
#define PTZ_CLEARTOUR				0x00040000				/*!< 删除巡航 */
#define PTZ_SETPATTERN				0x00080000				/*!< 设置模式 */
#define PTZ_STARTPATTERN			0x00100000				/*!< 开始模式 */
#define PTZ_STOPPATTERN				0x00200000				/*!< 停止模式 */
#define PTZ_CLEARPATTERN			0x00400000				/*!< 清除模式 */
#define PTZ_POSITION				0x00800000				/*!< 快速定位 */
#define PTZ_AUX						0x01000000				/*!< 辅助开关 */
#define PTZ_MENU					0x02000000				/*!< 球机菜单 */
#define PTZ_EXIT					0x04000000				/*!< 退出球机菜单 */
#define PTZ_ENTER					0x08000000				/*!< 确认 */
#define PTZ_ESC						0x10000000				/*!< 取消 */
#define PTZ_MENUUPDOWN				0x20000000				/*!< 菜单上下操作 */
#define PTZ_MENULEFTRIGHT			0x40000000				/*!< 菜单左右操作 */
#define PTZ_OPT_NUM					0x80000000				//*< 操作的个数 */

	//hiMask
#define PTZ_DEV						0x00000001				/*!< 云台控制 */
#define PTZ_MATRIX					0x00000002				/*!< 矩阵控制 */

	//编码模式列表，用于与编码模式掩码进行与、或操作
#define RV_CAPTURE_COMP_DIVX_MPEG4	0x00000001
#define RV_CAPTURE_COMP_MS_MPEG4 	0x00000002
#define RV_CAPTURE_COMP_MPEG2		0x00000004
#define RV_CAPTURE_COMP_MPEG1		0x00000008
#define RV_CAPTURE_COMP_H263		0x00000010
#define RV_CAPTURE_COMP_MJPG		0x00000020
#define RV_CAPTURE_COMP_FCC_MPEG4	0x00000040
#define RV_CAPTURE_COMP_H264		0x00000080

	//码流控制列表
#define RV_CAPTURE_BRC_CBR			0
#define RV_CAPTURE_BRC_VBR			1
	//#define RV_CAPTURE_BRC_MBR			2

	//报警联动动作，用于与报警联动动作掩码进行与、或操作
#define RV_ALARM_UPLOAD		0x00000001
#define RV_ALARM_RECORD		0x00000002
#define RV_ALARM_PTZ		0x00000004
#define RV_ALARM_MAIL		0x00000008
#define RV_ALARM_TOUR		0x00000010
#define RV_ALARM_TIP		0x00000020
#define RV_ALARM_OUT		0x00000040
#define RV_ALARM_FTP_UL		0x00000080
#define RV_ALARM_BEEP		0x00000100
#define RV_ALARM_VOICE		0x00000200
#define RV_ALARM_SNAP		0x00000400

	//"恢复默认设置"掩码，多个设置可用或操作
#define RV_RESTORE_COMMON			0x00000001	//普通设置
#define RV_RESTORE_CODING			0x00000002	//编码设置
#define RV_RESTORE_VIDEO			0x00000004	//录像设置
#define RV_RESTORE_COMM				0x00000008	//串口设置
#define RV_RESTORE_NETWORK			0x00000010	//网络设置
#define RV_RESTORE_ALARM			0x00000020	//报警设置
#define RV_RESTORE_VIDEODETECT		0x00000040	//视频检测
#define RV_RESTORE_PTZ				0x00000080	//云台控制
#define RV_RESTORE_OUTPUTMODE		0x00000100	//输出模式
#define RV_RESTORE_CHANNELNAME		0x00000200	//通道名称
#define RV_RESTORE_ALL				0x80000000	//全部重置


	////////////////end of远程配置结构体相关常量///////////////////////


	/************************************************************************
	** 枚举定义
	***********************************************************************/
	//设备类型
	enum NET_DEVICE_TYPE 
	{
		NET_PRODUCT_NONE = 0,
		NET_DVR_NONREALTIME_MACE, /*!< 非实时MACE */
		NET_DVR_NONREALTIME, /*!< 非实时 */
		NET_NVS_MPEG1, /*!< 网络视频服务器 */
		NET_DVR_MPEG1_2, /*!< MPEG1二路录像机 */
		NET_DVR_MPEG1_8, /*!< MPEG1八路录像机 */
		NET_DVR_MPEG4_8, /*!< MPEG4八路录像机 */
		NET_DVR_MPEG4_16, /*!< MPEG4十六路录像机 */
		NET_DVR_MPEG4_SX2, /*!< LB系列录像机 */
		NET_DVR_MEPG4_ST2, /*!< GB系列录像机 */
		NET_DVR_MEPG4_SH2, /*!< HB系列录像机 */
		NET_DVR_MPEG4_GBE, /*!< GBE系列录像机 */
		NET_DVR_MPEG4_NVSII, /*!< II代网络视频服务器 */
		NET_DVR_STD_NEW, /*!< 新标准配置协议 */
		NET_DVR_DDNS, /*DDNS服务器*/
		NET_DVR_ATM,  /* ATM机 */
		NET_NB_SERIAL, /* 二代非实时NB系列机器 */
		NET_LN_SERIAL, /* LN系列产品 */
		NET_BAV_SERIAL, /* BAV系列产品 */
		NET_SDIP_SERIAL, /* SDIP系列产品 */
		NET_IPC_SERIAL, /* IPC系列产品 */
		NET_NVS_B,		/* NVS B系列 */
		NET_NVS_C,		/* NVS H系列 */
		NET_NVS_S,		/* NVS S系列 */
		NET_NVS_E,		/* NVS E系列 */
		NET_DVR_NEW_PROTOCOL /*从QueryDevState中查询设备类型,以字符串格式*/
	};

	//add by HY 2009-5-22 begin
#define DH_MAX_IPADDR_LEN			16			// IP地址字符串长度
#define DH_MACADDR_LEN				40			// MAC地址字符串长度
#define DH_DEV_TYPE_LEN				32			// 设备类型字符串长度
	// 配置文件类型，供CLIENT_ExportConfigFile接口使用
	typedef enum __DH_CONFIG_FILE_TYPE
	{
		DH_CONFIGFILE_ALL = 0,						// 全部配置文件
		DH_CONFIGFILE_LOCAL,						// 本地配置文件
		DH_CONFIGFILE_NETWORK,						// 网络配置文件
		DH_CONFIGFILE_USER,							// 用户配置文件
	} DH_CONFIG_FILE_TYPE;

	// 对应CLIENT_SearchDevices接口
	typedef struct 
	{
		char				szIP[DH_MAX_IPADDR_LEN];		// IP
		int					nPort;							// 端口
		char				szSubmask[DH_MAX_IPADDR_LEN];	// 子网掩码
		char				szGateway[DH_MAX_IPADDR_LEN];	// 网关
		char				szMac[DH_MACADDR_LEN];			// MAC地址
		char				szDeviceType[DH_DEV_TYPE_LEN];	// 设备类型
		BYTE				bReserved[32];					// 保留字节
	} DEVICE_NET_INFO;

	// 设置登入时的相关参数
	typedef struct  
	{
		int					nWaittime;				// 等待超时时间(毫秒为单位)，为0默认5000ms
		int					nConnectTime;			// 连接超时时间(毫秒为单位)，为0默认1500ms
		int					nConnectTryNum;			// 连接尝试次数，为0默认1次
		int					nSubConnectSpaceTime;	// 子连接之间的等待时间(毫秒为单位),为0默认10ms
		int					nGetDevInfoTime;		// 获取设备信息超时时间，为0默认500ms.
		int					nConnectBufSize;		// 每个连接接收数据缓冲大小(字节为单位)，为0默认250*1024
		BYTE				bReserved[28];			// 保留字段
	} NET_PARAM;
	//add by HY 2009-5-22 end

	//通用云台控制命令
	typedef enum _PTZ_ControlType
	{
		RV_PTZ_UP_CONTROL = 0,			//上
		RV_PTZ_DOWN_CONTROL,			//下
		RV_PTZ_LEFT_CONTROL,			//左
		RV_PTZ_RIGHT_CONTROL,			//右
		RV_PTZ_ZOOM_ADD_CONTROL,		//变倍+
		RV_PTZ_ZOOM_DEC_CONTROL,		//变倍-
		RV_PTZ_FOCUS_ADD_CONTROL,		//调焦+
		RV_PTZ_FOCUS_DEC_CONTROL,		//调焦-
		RV_PTZ_APERTURE_ADD_CONTROL,	//光圈+
		RV_PTZ_APERTURE_DEC_CONTROL,   //光圈-
		RV_PTZ_POINT_MOVE_CONTROL,     //转至预置点
		RV_PTZ_POINT_SET_CONTROL,      //设置
		RV_PTZ_POINT_DEL_CONTROL,      //删除
		RV_PTZ_POINT_LOOP_CONTROL,     //点间巡航
		RV_PTZ_LAMP_CONTROL            //灯光雨刷
	}RV_PTZ_ControlType;

	//云台控制扩展命令
	typedef enum _EXTPTZ_ControlType
	{
		RV_EXTPTZ_LEFTTOP = 0X20,		//左上
		RV_EXTPTZ_RIGHTTOP ,			//右上
		RV_EXTPTZ_LEFTDOWN ,			//左下
		RV_EXTPTZ_RIGHTDOWN,			//右下
		RV_EXTPTZ_ADDTOLOOP,			//加入预置点到巡航	巡航线路	预置点值	
		RV_EXTPTZ_DELFROMLOOP,			//删除巡航中预置点	巡航线路	预置点值	
		RV_EXTPTZ_CLOSELOOP,			//清除巡航	巡航线路		
		RV_EXTPTZ_STARTPANCRUISE,		//开始水平旋转			
		RV_EXTPTZ_STOPPANCRUISE,		//停止水平旋转			
		RV_EXTPTZ_SETLEFTBORDER,		//设置左边界			
		RV_EXTPTZ_SETRIGHTBORDER,		//设置右边界			
		RV_EXTPTZ_STARTLINESCAN,		//开始线扫			
		RV_EXTPTZ_CLOSELINESCAN,		//停止线扫			
		RV_EXTPTZ_SETMODESTART,		//设置模式开始	模式线路		
		RV_EXTPTZ_SETMODESTOP,			//设置模式结束	模式线路		
		RV_EXTPTZ_RUNMODE,				//运行模式	模式线路		
		RV_EXTPTZ_STOPMODE,			//停止模式	模式线路		
		RV_EXTPTZ_DELETEMODE,			//清除模式	模式线路		
		RV_EXTPTZ_REVERSECOMM,			//翻转命令			
		RV_EXTPTZ_FASTGOTO,			//快速定位	水平坐标(8192)	垂直坐标(8192)	变倍(4)
		RV_EXTPTZ_AUXIOPEN,			//x34	辅助开关开	辅助点		
		RV_EXTPTZ_AUXICLOSE,			//0x35	辅助开关关	辅助点
		RV_EXTPTZ_OPENMENU = 0X36,		//打开球机菜单
		RV_EXTPTZ_CLOSEMENU ,			//关闭菜单
		RV_EXTPTZ_MENUOK,				//菜单确定
		RV_EXTPTZ_MENUCANCEL,			//菜单取消
		RV_EXTPTZ_MENUUP,				//菜单上
		RV_EXTPTZ_MENUDOWN,			//菜单下
		RV_EXTPTZ_MENULEFT,			//菜单左
		RV_EXTPTZ_MENURIGHT,			//菜单右
		RV_EXTPTZ_ALARMHANDLE = 0x40,	//报警联动云台 parm1:报警输入通道 parm2:报警联动类型1-预置点2-线扫3-巡航 parm3:联动值，如预置点号
		RV_EXTPTZ_MATRIXSWITCH = 0x41,	//矩阵切换 parm1:监视器号(视频输出号) parm2:(视频输入号) parm3:矩阵号
		RV_EXTPTZ_LIGHTCONTROL,			//灯光控制器
		RV_EXTPTZ_UP_TELE = 0x70,		//上 + TELE，param1=速度（1-8），下同
		RV_EXTPTZ_DOWN_TELE,			//下 + TELE
		RV_EXTPTZ_LEFT_TELE,			//左 + TELE
		RV_EXTPTZ_RIGHT_TELE,			//右 + TELE
		RV_EXTPTZ_LEFTUP_TELE,			//左上 + TELE
		RV_EXTPTZ_LEFTDOWN_TELE,		//左下 + TELE
		RV_EXTPTZ_TIGHTUP_TELE,			//右上 + TELE
		RV_EXTPTZ_RIGHTDOWN_TELE,		//右下 + TELE
		RV_EXTPTZ_UP_WIDE,				//上 + WIDE，param1=速度（1-8），下同
		RV_EXTPTZ_DOWN_WIDE,			//下 + WIDE
		RV_EXTPTZ_LEFT_WIDE,			//左 + WIDE
		RV_EXTPTZ_RIGHT_WIDE,			//右 + WIDE
		RV_EXTPTZ_LEFTUP_WIDE,			//左上 + WIDE
		RV_EXTPTZ_LEFTDOWN_WIDE,		//左下 + WIDE
		RV_EXTPTZ_TIGHTUP_WIDE,			//右上 + WIDE
		RV_EXTPTZ_RIGHTDOWN_WIDE,		//右下 + WIDE
		RV_EXTPTZ_TOTAL,				//最大命令值
		//add by HY 2009-8-14 begin
		RV_EXTPTZ_RESET,				//复位
		//add by HY 2009-8-14 end
	}RV_EXTPTZ_ControlType;

	//IO控制命令
	typedef enum _IOTYPE
	{
		RV_ALARMINPUT = 1,				//报警输入
		RV_ALARMOUTPUT = 2,			//报警输出

		RV_DECODER_ALARMOUT = 3,	//报警解码器输出
		RV_WIRELESS_ALARMOUT =5,    //无线报警输出
		RV_ALARM_TRIGGER_MODE = 7,
	}RV_IOTYPE;

	//查询日志扩展接口参数：设备日志类型
	typedef enum _RV_LOG_QUERY_TYPE
	{
		DHLOG_ALL = 0,		//所有日志
		DHLOG_SYSTEM,		//系统日志
		DHLOG_CONFIG,		//配置日志
		DHLOG_STORAGE,		//存储相关
		DHLOG_ALARM,		//报警日志
		DHLOG_RECORD,		//录象相关
		DHLOG_ACCOUNT,		//帐号相关
		DHLOG_CLEAR,		//清除日志
		DHLOG_PLAYBACK		//回放相关
	}RV_LOG_QUERY_TYPE;

	//返回的日志信息结构体－日志类型
	typedef enum _RV_LOG_TYPE
	{
		//system logs
		RV_LOG_REBOOT = 0x0000,
		RV_LOG_SHUT,
		//config logs
		RV_LOG_CONFSAVE = 0x0100,
		RV_LOG_CONFLOAD,
		//fs logs
		RV_LOG_FSERROR = 0x0200,   //文件系统错误
		RV_LOG_HDD_WERR,           //硬盘写错误
		RV_LOG_HDD_RERR,           //硬盘读错误
		RV_LOG_HDD_TYPE,           //设置硬盘类型
		RV_LOG_HDD_FORMAT,         //格式化硬盘
		//alarm logs
		RV_LOG_ALM_IN = 0x0300,
		RV_LOG_ALM_END = 0x0302,
		RV_LOG_INFRAREDALM_IN = 0x03a0, //无线报警输入
		RV_LOG_INFRAREDALM_END,       //无线报警结束
		//record
		RV_LOG_MANUAL_RECORD = 0x0401,
		RV_LOG_CLOSED_RECORD,
		//user manager
		RV_LOG_LOGIN = 0x0500,
		RV_LOG_LOGOUT,
		RV_LOG_ADD_USER,
		RV_LOG_DELETE_USER,
		RV_LOG_MODIFY_USER,
		RV_LOG_ADD_GROUP,
		RV_LOG_DELETE_GROUP,
		RV_LOG_MODIFY_GROUP,
		//clear
		RV_LOG_CLEAR = 0x0600,
		RV_LOG_TYPE_NR = 7,
	}RV_LOG_TYPE;

	//add by HY 2009-7-14 begin
	typedef enum _RV_RESET_TYPE
	{	
		DEFAULT_CFG_GENERAL = 0,		
		DEFAULT_CFG_ENCODE,
		DEFAULT_CFG_RECORD,		
		DEFAULT_CFG_COMM,		
		DEFAULT_CFG_NET,		
		DEFAULT_CFG_ALARM = 5,		
		DEFAULT_CFG_DETECT,		
		DEFAULT_CFG_PTZ,		
		DEFAULT_CFG_AUT,		
		DEFAULT_CFG_CHANTITLE = 9,		
		DEFAULT_CFG_EXCEP,		
		DEFAULT_CFG_ALL = 31,		
		DEFAULT_CFG_END,		
	}_RV_RESET_TYPE;
	//add by HY 2009-7-14 end

	//接口CLIENT_QueryLogEx返回的日志信息结构体－日志类型
	//条件：int nType = 1;参数reserved = &nType.
	typedef enum _RV_NEWLOG_TYPE
	{
		//system logs
		RV_NEWLOG_REBOOT	 = 0x0000,		//系统重启
		RV_NEWLOG_SHUT,						//系统关机

		//config logs
		RV_NEWLOG_CONFSAVE = 0x0100,
		RV_NEWLOG_CONFLOAD,

		//fs logs
		RV_NEWLOG_FSERROR	 = 0x0200,		//文件系统错误
		RV_NEWLOG_HDD_WERR,					//硬盘写错误
		RV_NEWLOG_HDD_RERR,					//硬盘读错误
		RV_NEWLOG_HDD_TYPE,					//设置硬盘类型
		RV_NEWLOG_HDD_FORMAT,				//格式化硬盘
		RV_NEWLOG_HDD_NOSPACE,				//当前工作盘空间不足

		//alarm logs
		RV_NEWLOG_ALM_IN	 = 0x0300,
		RV_NEWLOG_NETALM_IN,
		RV_NEWLOG_ALM_END,
		RV_NEWLOG_LOSS_IN,
		RV_NEWLOG_LOSS_END,

		//record
		RV_NEWLOG_AUTOMATIC_RECORD = 0x0400,
		RV_NEWLOG_MANUAL_RECORD,
		RV_NEWLOG_CLOSED_RECORD,

		//user manager
		RV_NEWLOG_LOGIN    = 0x0500,
		RV_NEWLOG_LOGOUT,
		RV_NEWLOG_ADD_USER,
		RV_NEWLOG_DELETE_USER,
		RV_NEWLOG_MODIFY_USER,
		RV_NEWLOG_ADD_GROUP,
		RV_NEWLOG_DELETE_GROUP,
		RV_NEWLOG_MODIFY_GROUP,

		//clear
		RV_NEWLOG_CLEAR    = 0x0600,
		RV_NEWLOG_SEARCHLOG,				//查询日志

		//record operation
		RV_NEWLOG_SEARCH	 = 0x0700,		//录像查询
		RV_NEWLOG_DOWNLOAD,					//录像下载
		RV_NEWLOG_PLAYBACK,					//录像回放
		RV_NEWLOG_BACKUP,					/*!< 备份录像文件 */
		RV_NEWLOG_BACKUPERROR,				/*!< 备份录像文件失败 */

		RV_NEWLOG_TYPE_NR = 8,		
	}RV_NEWLOG_TYPE;


	//实时预览扩展接口增加的参数：预览类型
	typedef enum _RealPlayType
	{
		RV_RType_Realplay = 0,	//实时预览
		RV_RType_Multiplay,	//多画面预览

		RV_RType_Realplay_0,	//实时监视-主码流，等同于RV_RType_Realplay
		RV_RType_Realplay_1,	//实时监视-从码流1
		RV_RType_Realplay_2,	//实时监视-从码流2
		RV_RType_Realplay_3,	//实时监视-从码流3

		RV_RType_Multiplay_1,	//多画面预览－1画面
		RV_RType_Multiplay_4,	//多画面预览－4画面
		RV_RType_Multiplay_8,	//多画面预览－8画面
		RV_RType_Multiplay_9,	//多画面预览－9画面
		RV_RType_Multiplay_16,	//多画面预览－16画面

		RV_RType_Multiplay_6,	//多画面预览－6画面
		RV_RType_Multiplay_12,	//多画面预览－12画面
	} RV_RealPlayType;

	//设备控制接口参数：控制类型定义
	typedef enum _CtrlType
	{
		RV_CTRL_REBOOT = 0,	//重启设备	
		RV_CTRL_SHUTDOWN,	//关闭设备
		RV_CTRL_DISK,		//硬盘管理
		//网络键盘
		RV_KEYBOARD_POWER = 3,
		RV_KEYBOARD_ENTER,
		RV_KEYBOARD_ESC,
		RV_KEYBOARD_UP,
		RV_KEYBOARD_DOWN,
		RV_KEYBOARD_LEFT,
		RV_KEYBOARD_RIGHT,
		RV_KEYBOARD_BTN0,
		RV_KEYBOARD_BTN1,
		RV_KEYBOARD_BTN2,
		RV_KEYBOARD_BTN3,
		RV_KEYBOARD_BTN4,
		RV_KEYBOARD_BTN5,
		RV_KEYBOARD_BTN6,
		RV_KEYBOARD_BTN7,
		RV_KEYBOARD_BTN8,
		RV_KEYBOARD_BTN9,
		RV_KEYBOARD_BTN10,
		RV_KEYBOARD_BTN11,
		RV_KEYBOARD_BTN12,
		RV_KEYBOARD_BTN13,
		RV_KEYBOARD_BTN14,
		RV_KEYBOARD_BTN15,
		RV_KEYBOARD_BTN16,
		RV_KEYBOARD_SPLIT,
		RV_KEYBOARD_ONE,
		RV_KEYBOARD_NINE,
		RV_KEYBOARD_ADDR,
		RV_KEYBOARD_INFO,
		RV_KEYBOARD_REC,
		RV_KEYBOARD_FN1,
		RV_KEYBOARD_FN2,
		RV_KEYBOARD_PLAY,
		RV_KEYBOARD_STOP,
		RV_KEYBOARD_SLOW,
		RV_KEYBOARD_FAST,
		RV_KEYBOARD_PREW,
		RV_KEYBOARD_NEXT,
		RV_KEYBOARD_JMPDOWN,
		RV_KEYBOARD_JMPUP,
		//触发报警
		RV_TRIGGER_ALARM_IN = 100,
		RV_TRIGGER_ALARM_OUT,
		//矩阵控制
		RV_CTRL_MATRIX,
		//SD卡控制（ipc产品）参数同硬盘控制
		RV_CTRL_SDCARD,
		//刻录机控制
		RV_BURNING_START,		//开始刻录
		RV_BURNING_STOP,		//结束刻录

		RV_BURNING_ADDPWD,	//叠加密码(以0为结尾的字符串,最大长度8位)
		RV_BURNING_ADDHEAD,	//叠加片头(以0为结尾的字符串,最大长度1024字节,支持分行,行分隔符'\n')，
		RV_BURNING_ADDSIGN,	//叠加打点到刻录信息(参数无)
		RV_BURNING_ADDCURSTOMINFO,	//自定义叠加(以0为结尾的字符串,最大长度1024字节,支持分行,行分隔符'\n')

		//恢复设备的默认设置
		RV_CTRL_RESTOREDEFAULT,

		//设备触发DSP抓图
		RV_CTRL_CAPTURE_START,		//触发设备抓图

		//清除日志
		RV_CTRL_CLEARLOG,
		//无线报警
		RV_TRIGGER_ALARM_WIRELESS = 200,
	}CtrlType;

	//配置类型，目前使用该枚举的接口已淘汰，请不要使用
	typedef enum _CFG_INDEX
	{
		CFG_GENERAL =  0,		// 普通
		CFG_COMM   		,		// 串口
		CFG_NET    		,		// 网络
		CFG_RECORD 		,		// 录像
		CFG_CAPTURE		,		// 图像设置
		CFG_PTZ    		,		// 云台
		CFG_DETECT 		,		// 动态检测
		CFG_ALARM  		,		// 报警
		CFG_DISPLAY		,		// 显示
		CFG_RESERVED	,		// 保留，使类型连贯
		CFG_TITLE  		=10,	// 通道标题
		CFG_MAIL  		=11,	// 邮件功能
		CFG_EXCAPTURE	=12,	// 预览图像设置
		CFG_PPPOE		=13,	// pppoe设置
		CFG_DDNS		=14,	// DDNS设置
		CFG_SNIFFER		=15,	// 网络监视捕获设置
		CFG_DSPINFO		=16,	// 编码能力信息
		CFG_COLOR		=126,  //颜色配置信息
		CFG_ALL,				// 保留
	} CFG_INDEX;

	typedef enum _SNAP_TYPE
	{
		SNAP_TYP_TIMING = 0,
		SNAP_TYP_ALARM,
		SNAP_TYP_NUM,
	} SNAP_TYPE;

	//分辨率枚举，供RV_DSP_ENCODECAP使用
	typedef enum _CAPTURE_SIZE
	{
		CAPTURE_SIZE_D1,				// <  720*576(PAL)  720*480(NTSC)
		CAPTURE_SIZE_HD1,				// <  352*576(PAL)  352*480(NTSC)
		CAPTURE_SIZE_BCIF,				// <  720*288(PAL)  720*240(NTSC)
		CAPTURE_SIZE_CIF,				// <  352*288(PAL)  352*240(NTSC)
		CAPTURE_SIZE_QCIF,				// <  176*144(PAL)  176*120(NTSC)
		CAPTURE_SIZE_VGA,				// <  640*480
		CAPTURE_SIZE_QVGA,				// <  320*240
		CAPTURE_SIZE_SVCD,				// <  480*480
		CAPTURE_SIZE_QQVGA,				// <  160*128
		//2010-6-29 luhj add
		CAPTURE_SIZE_720P	= 9,		//	1280*720	720P
		CAPTURE_SIZE_1080P	= 10,		//	1920*1080	1080P
		//add end
		CAPTURE_SIZE_SVGA,				// <  800*592
		CAPTURE_SIZE_XVGA,				// <  1024*768	VGA输入编码，2800需要
		CAPTURE_SIZE_WXGA,				// <  1280*800
		CAPTURE_SIZE_SXGA,				// <  1280*1024  
		CAPTURE_SIZE_WSXGA,				// <  1600*1024  
		CAPTURE_SIZE_UXGA,				// <  1600*1200
		CAPTURE_SIZE_WUXGA,				// <  1920*1200
		CAPTURE_SIZE_NR  
	} CAPTURE_SIZE;

	typedef enum __EM_USEDEV_MODE
	{
		RV_TALK_CLIENT_MODE,		//设置客户端方式进行语音对讲
		RV_TALK_SERVER_MODE,		//设置服务器方式进行语音对讲
		RV_TALK_ENCODE_TYPE,		//设置语音对讲编码格式
		RV_ALARM_LISTEN_MODE        //设置报警订阅方式

	} EM_USEDEV_MODE;

	typedef enum __EM_UPGRADE_TYPE
	{
		RV_UPGRADE_BIOS_TYPE=1,		//BIOS升级
		RV_UPGRADE_WEB_TYPE,		//WEB升级
		RV_UPGRADE_BOOT_YPE,		//BOOT升级
		RV_UPGRADE_CHARACTER_TYPE,	//汉字库
		RV_UPGRADE_LOGO_TYPE,		//LOGO
		RV_UPGRADE_EXE_TYPE,		//EXE 例如播放器等
	} EM_UPGRADE_TYPE;

	//语言种类
	typedef enum __LANGUAGE_TYPE
	{
		RV_LANGUAGE_ENGLISH,				//英文	
		RV_LANGUAGE_CHINESE_SIMPLIFIED,		//简体中文	
		RV_LANGUAGE_CHINESE_TRADITIONAL,	//繁体中文	
		RV_LANGUAGE_ITALIAN,				//意大利文	
		RV_LANGUAGE_SPANISH,				//西班牙文
		RV_LANGUAGE_JAPANESE,				//日文版	
		RV_LANGUAGE_RUSSIAN,				//俄文版		
		RV_LANGUAGE_FRENCH,					//法文版		
		RV_LANGUAGE_GERMAN, 				//德文版		
		RV_LANGUAGE_PORTUGUESE,				//葡萄牙语	
		RV_LANGUAGE_TURKEY,					//土尔其语	
		RV_LANGUAGE_POLISH,					//波兰语	
		RV_LANGUAGE_ROMANIAN,				//罗马尼亚	
		RV_LANGUAGE_HUNGARIAN,              //匈牙利语	
		RV_LANGUAGE_FINNISH,				//芬兰语	
		RV_LANGUAGE_ESTONIAN,				//爱沙尼亚语	
		RV_LANGUAGE_KOREAN,					//韩语	
		RV_LANGUAGE_FARSI,					//波斯语 	
		RV_LANGUAGE_DANSK,					//丹麦语
		RV_LANGUAGE_CZECHISH,				//捷克文
		RV_LANGUAGE_BULGARIA,				//保加利亚文
	} RV_LANGUAGE_TYPE;

	//语音对讲格式
	typedef enum __TALK_CODING_TYPE
	{
		RV_TALK_DEFAULT = 0,
		RV_TALK_PCM = 1,					//PCM
		RV_TALK_G711a,						//G711a
		RV_TALK_AMR,						//AMR
		RV_TALK_G711u,						//G711u
		RV_TALK_G726,						//G726
	}RV_TALK_CODING_TYPE;

	/************************************************************************
	** 结构体定义
	***********************************************************************/
	//add by HY 2009-11-16 begin
	typedef struct
	{	
		BOOL	bEnable;
		int		dwWebPort;
		int		dwTCPPort;
		DWORD	dwReserved;	
	}NET_UPNP;
	//add by HY 2009-11-16 begin

	//时间
	typedef struct 
	{
		DWORD dwYear;		//年
		DWORD dwMonth;		//月
		DWORD dwDay;		//日
		DWORD dwHour;		//时
		DWORD dwMinute;		//分
		DWORD dwSecond;		//秒
	} NET_TIME,*LPNET_TIME;

	//设备信息
	typedef struct {
		BYTE sSerialNumber[RV_SERIALNO_LEN];	//序列号
		BYTE byAlarmInPortNum;				//DVR报警输入个数
		BYTE byAlarmOutPortNum;				//DVR报警输出个数
		BYTE byDiskNum;						//DVR 硬盘个数
		BYTE byDVRType;						//DVR类型, 见枚举DHDEV_DEVICE_TYPE
		BYTE byChanNum;						//DVR 通道个数
	} NET_DEVICEINFO, *LPNET_DEVICEINFO;

	//普通报警信息
	typedef struct {
		int channelcount;
		int alarminputcount;
		unsigned char alarm[16];			//外部报警
		unsigned char motiondection[16];	//动态检测
		unsigned char videolost[16];		//视频丢失
	} NET_CLIENT_STATE;

	//查询设备工作状态硬盘信息
	typedef struct {
		DWORD		dwVolume;		//硬盘的容量
		DWORD		dwFreeSpace;	//硬盘的剩余空间
		BYTE		dwStatus;		//硬盘的状态,0-休眠,1-活动,2-故障等；将DWORD拆成四个BYTE
		BYTE		bDiskNum;		//硬盘号
		BYTE		bSubareaNum;	//分区号
		BYTE		bSignal;		//标识，0为本地 1为远程
	}NET_DEV_DISKSTATE,*LPNET_DEV_DISKSTATE;

	//add by HY 2009-9-24 begin
	//查询设备硬盘类型
	typedef struct {
		int Type[32];
	}NET_DEV_DISKTYPE;
	//add by HY 2009-9-24 end

	//录像文件信息
	typedef struct {
		unsigned int     ch;              //通道号
		char             filename[128];   //文件名
		unsigned int     size;            //文件长度
		NET_TIME         starttime;       //开始时间
		NET_TIME         endtime;         //结束时间
		unsigned int     driveno;         //磁盘号
		unsigned int     startcluster;    //起始簇号
		int				 nRecordFileType; //录象文件类型  0：普通录象；1：报警录象；2：移动检测；3：卡号录象
	} NET_RECORDFILE_INFO, *LPNET_RECORDFILE_INFO;

	//录像文件信息
	typedef struct {
		unsigned int     ch;              //通道号
		char             filename[128];   //文件名
		unsigned int     size;            //文件长度
		NET_TIME         starttime;       //开始时间
		NET_TIME         endtime;         //结束时间
		unsigned int     driveno;         //磁盘号
		unsigned int     startcluster;    //起始簇号
		int				 nRecordFileType; //录象文件类型  0：普通录象；1：报警录象；2：移动检测；3：卡号录象
		unsigned int	 partno;		  //分区号
		unsigned int	 hint;			  
	} NET_RECORDFILE_INFO_EX, *LPNET_RECORDFILE_INFO_EX;

	//报警IO控制
	typedef struct 
	{
		unsigned short index;	//端口序号
		unsigned short state;	//端口状态
	}ALARM_CONTROL;


	typedef struct
	{
		unsigned short		index;					//???úDòo?
		unsigned short		mode;					//′￥·￠·?ê?(01?±?1ê??ˉ2×??ˉ);2?éè??μ?í¨μà￡?sdk??è???±￡3??-à′μ?éè???￡
		BYTE				bReserved[28];			
	} TRIGGER_MODE_CONTROL;

	typedef struct 
	{
		int	decoderNo;	//报警解码器号,从0开始
		unsigned short  alarmChn; //报警输出口,从0开始
		unsigned short  alarmState; //报警输出状态,1-打开　0-关闭
	}DECODER_ALARM_CONTROL;

	///////////////////用户帐号管理相关结构体/////////////////////////////////
	typedef struct _OPR_RIGHT
	{
		DWORD		dwID;
		char		name[RV_RIGHT_NAME_LENGTH];
		char		memo[RV_MEMO_LENGTH];
	}OPR_RIGHT;

	typedef struct _USER_INFO
	{
		DWORD		dwID;
		DWORD		dwGroupID;
		char		name[RV_USER_NAME_LENGTH];
		char		passWord[RV_USER_PSW_LENGTH];
		DWORD		dwRightNum;
		DWORD		rights[RV_MAX_RIGHT_NUM];
		char		memo[RV_MEMO_LENGTH];
		DWORD		dwReusable;		//本用户是否允许复用 1-复用，0-不复用

	}USER_INFO;

	typedef struct _USER_GROUP_INFO
	{
		DWORD		dwID;
		char		name[RV_USER_NAME_LENGTH];
		DWORD		dwRightNum;
		DWORD		rights[RV_MAX_RIGHT_NUM];
		char		memo[RV_MEMO_LENGTH];
	}USER_GROUP_INFO;

	//用户信息配置结构
	typedef struct _USER_MANAGE_INFO
	{
		DWORD				dwRightNum;
		OPR_RIGHT			rightList[RV_MAX_RIGHT_NUM];
		DWORD				dwGroupNum;
		USER_GROUP_INFO		groupList[RV_MAX_GROUP_NUM];
		DWORD				dwUserNum;
		USER_INFO			userList[RV_MAX_USER_NUM];
		DWORD				dwSpecial; //特殊信息， 1-支持用户复用，　0-不支持用户复用
	}USER_MANAGE_INFO;

	//////////////////////////////////////////////////////////////////////////////
	///////////////////用户帐号管理扩展相关结构体/////////////////////////////////
#define RV_USER_NAME_LENGTH_EX  16  //用户名长度
#define RV_USER_PSW_LENGTH_EX   16  //密码
	typedef struct _OPR_RIGHT_EX
	{
		DWORD  dwID;
		char  name[RV_RIGHT_NAME_LENGTH];
		char  memo[RV_MEMO_LENGTH];
	}OPR_RIGHT_EX;

	typedef struct _USER_INFO_EX
	{
		DWORD  dwID;
		DWORD  dwGroupID;
		char  name[RV_USER_NAME_LENGTH_EX];
		char  passWord[RV_USER_PSW_LENGTH_EX];
		DWORD  dwRightNum;
		DWORD  rights[RV_MAX_RIGHT_NUM];
		char  memo[RV_MEMO_LENGTH];
		DWORD   dwFouctionMask;//掩码

		/* 掩码,包括
		* 0x00000001 - 支持用户复用
		*/

		BYTE  byReserve[32];

	}USER_INFO_EX;

	typedef struct _USER_GROUP_INFO_EX
	{
		DWORD  dwID;
		char  name[RV_USER_NAME_LENGTH_EX];
		DWORD  dwRightNum;
		DWORD  rights[RV_MAX_RIGHT_NUM];
		char  memo[RV_MEMO_LENGTH];
	}USER_GROUP_INFO_EX;

	//用户信息配置结构
	typedef struct _USER_MANAGE_INFO_EX
	{

		DWORD				dwRightNum;
		OPR_RIGHT_EX		rightList[RV_MAX_RIGHT_NUM];
		DWORD				dwGroupNum;
		USER_GROUP_INFO_EX  groupList[RV_MAX_GROUP_NUM];
		DWORD				dwUserNum;
		USER_INFO_EX		userList[RV_MAX_USER_NUM];
		DWORD				dwFouctionMask;//掩码    
		/* 掩码,包括
		* 0x00000001 - 支持用户复用
		* 0x00000002 - 密码修改需要校验
		*/
		BYTE  byNameMaxLength;       //支持用户名的最大长度
		BYTE  byPSWMaxLength;       //支持的密码最大长度
		BYTE  byReserve[254];
	}USER_MANAGE_INFO_EX;
	///////////////////end of 用户帐号管理扩展相关结构体////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	////////////////////日志结构体相关///////////////////////////
	//日志时间结构
	typedef struct _DHDEVTIME
	{
		DWORD second		:6;					//	秒	1-60		
		DWORD minute		:6;					//	分	1-60		
		DWORD hour			:5;					//	时	1-24		
		DWORD day			:5;					//	日	1-31		
		DWORD month			:4;					//	月	1-12		
		DWORD year			:6;					//	年	2000-2063	
	}DHDEVTIME, *LPDHDEVTIME;

	typedef struct _RV_LOG_ITEM
	{
		DHDEVTIME time;					//日期
		unsigned short type;			//类型
		unsigned char reserved;			//保留
		unsigned char data;				//数据
		unsigned char context[8];		//内容
	}RV_LOG_ITEM, *LPRV_LOG_ITEM;

	//接口CLIENT_QueryLogEx返回的日志信息结构体
	//条件：int nType = 1;参数reserved = &nType.
	typedef struct _RV_NEWLOG_ITEM
	{
		DHDEVTIME	time; 				//日期
		WORD	type;					//类型
		WORD	data;					//数据
		char  	szOperator[8]; 			//用户名
		BYTE	context[16];		    //内容	
	}RV_NEWLOG_ITEM, *LPRV_NEWLOG_ITEM;

	typedef struct _LOG_ITEM_RECORD
	{
		DHDEVTIME	time;		//时间
		BYTE	channel;		//通道
		BYTE	type;		//录像类型
		BYTE	reserved[2];
	}LOG_ITEM_RECORD, *LPLOG_ITEM_RECORD;
	////////////////////end of 日志结构体相关///////////////////////////

	//回调视频数据帧的帧参数结构体
	typedef struct _tagVideoFrameParam
	{
		BYTE encode;		//编码类型
		BYTE frametype;		//I = 0, P = 1, B = 2...
		BYTE format;		//PAL - 0, NTSC - 1
		BYTE size;			//CIF - 0, HD1 - 1, 2CIF - 2, D1 - 3, VGA - 4, QCIF - 5, QVGA - 6 ,
		//SVCD - 7,QQVGA - 8, SVGA - 9, XVGA - 10,WXGA - 11,SXGA - 12,WSXGA - 13,UXGA - 14,WUXGA - 15,
		DWORD fourcc;		//如果是H264编码则总为0，MPEG4这里总是填写FOURCC('X','V','I','D');
		DWORD reserved;		//保留
		NET_TIME struTime;	//时间信息
	}tagVideoFrameParam;

	//回调音频数据帧的帧参数结构体
	typedef struct _tagCBPCMDataParam
	{
		BYTE channels;		// 声道数
		BYTE samples;			// 采样 0 - 8000, 1 - 11025, 2 - 16000, 3 - 22050, 4 - 32000, 5 - 44100, 6 - 48000
		BYTE depth;			// 采样深度 取值8或者16等。直接表示
		BYTE param1;			// 0 - 指示无符号，1-指示有符号
		DWORD reserved;		// 保留
	}tagCBPCMDataParam;

	//通道画面字幕叠加的数据结构
	typedef struct _RV_CHANNEL_OSDSTRING
	{
		BOOL bEnable;	//使能
		DWORD dwPosition[MAX_STRING_LINE_LEN];	//各行字符的位置　用1-9整数表示，与小键盘位置对应
		//		7左上	8上		9右上
		//		4左		5中		6右
		//		1左下	2下		3右下
		char szStrings[MAX_STRING_LINE_LEN][MAX_PER_STRING_LEN]; //最多六行字符，每行最多20个字节
	}RV_CHANNEL_OSDSTRING;

	////////////////////设备控制接口的控制参数定义//////////////////////
	typedef struct _DISKCTRL_PARAM
	{
		DWORD dwSize;		//size of this structure, for version control
		int nIndex;			//为硬盘信息结构体RV_HARDDISK_STATE里的数组stDisks下标，从0开始
		int ctrlType;		//control type.
		// 0 - 清除数据, 1 - 设为读写盘, 2 - 设为只读盘
		// 3 - 设为冗余盘, 4 - 恢复错误, 5 - 设为快照盘
	}DISKCTRL_PARAM;

	typedef struct _ALARMCTRL_PARAM
	{
		DWORD dwSize;
		int nAlarmNo;		//报警通道号，从0开始
		int nAction;		//0-触发报警，1-停止报警
	}ALARMCTRL_PARAM;

	typedef struct _MATRIXCTRL_PARAM
	{
		DWORD dwSize;
		int nChannelNo;		//视频输入号，从0开始
		int nMatrixNo;		//矩阵输出号，从0开始
	}MATRIXCTRL_PARAM;

	typedef struct _BURNING_PARM
	{
		int channelMask;	//通道号掩码，每位表示一个通道
		int	devMask;		//设备掩码，每位表示一台设备
	}BURNNG_PARM;


	////////////////////end of设备控制接口的控制参数定义//////////////////////

	////////////////////查询设备状态的参数定义////////////////////////////////
	typedef struct _RV_HARDDISK_STATE
	{
		DWORD	dwDiskNum;		//个数
		NET_DEV_DISKSTATE	stDisks[RV_MAX_DISKNUM]; //各硬盘或分区信息
	}RV_HARDDISK_STATE,*LPRV_HARDDISK_STATE;

	typedef RV_HARDDISK_STATE  RV_SDCARD_STATE;	//SD卡，数据结构同硬盘信息

	typedef struct _NET_DEV_BURNING
	{
		DWORD dwDriverType;	//刻录驱动器类型 0 -	DHFS, 1 - DISK, 2- CDRW
		DWORD dwBusType;	//总线类型　0- USB, 1-1394, 2-IDE
		DWORD dwTotalSpace;	//总容量(KB)
		DWORD dwRemainSpace;	//剩余容量(KB)
		BYTE  dwDriverName[RV_BURNING_DEV_NAMELEN]; //刻录驱动器名称
	}NET_DEV_BURNING,*LPNET_DEV_BURNING;

	typedef struct _RV_BURNING_DEVINFO
	{
		DWORD	dwDevNum;		//刻录设备个数
		NET_DEV_BURNING	stDevs[RV_MAX_BURNING_DEV_NUM]; //各刻录设备信息
	}RV_BURNING_DEVINFO, *LPRV_BURNING_DEVINFO;

	typedef struct _RV_BURNING_PROGRESS
	{
		BYTE	bBurning;		//旧刻录机表示是否正在刻录
		/*
		* 新刻录机的值：
		*0;   可以刻录
		*1;   非光盘设备,设备类型不正确
		*2;   没有有效的设备,即剩余空间为0
		*3;   有其他光盘在刻录
		*4;   设备为非空闲状态,即在备份、刻录或回放中
		*/
		BYTE	bRomType;		//才有盘片类型(新刻录机才有这个值)(0:RV文件系统；1：移动硬盘或u盘；2：光盘)
		BYTE	bOperateType;   //操作类型(新刻录机才有这个值)(0：空闲；1：正在备份中；2：正在刻录中；3：正在进行光盘回放)
		BYTE	bType;			//备份或刻录得过程状态(新刻录机才有这个值)(0：停止或结束 ；1：开始；2：出错；3：满;4：正在初始化)	
		//**************************
		NET_TIME stTimeStart;	//开始时间　
		DWORD	dwTimeElapse;	//已刻录时间(秒)
		DWORD	dwTotalSpace;	//光盘总容量
		DWORD	dwRemainSpace;	//光盘剩余容量
		DWORD	dwBurned;		//已刻录容量
		DWORD	dwStatus;		/*
								为了兼容老设备，保留起来的，新刻录机没有实际用处
								-1：表示新刻录机。如果不为-1，表示旧刻录机
								下面是老设备的刻录状态（老设备用的）如果不为-1，表示旧刻录机：
								0-正在初始化，
								1-正在刻录，以上各变量有效
								2-刻录出错
								3-盘满停止
								*/
	}RV_BURNING_PROGRESS, *LPRV_BURNING_PROGRESS;

	//设备支持语言结构体
	typedef struct _RV_LANGUAGE_DEVINFO
	{
		DWORD	dwLanguageNum;		    //支持语言个数
		BYTE    byLanguageType[252];    //枚举值,详见RV_LANGUAGE_TYPE
	}RV_DEV_LANGUAGE_INFO, *LPRV_DEV_LANGUAGE_INFO;


	typedef struct  
	{
		RV_TALK_CODING_TYPE		encodeType;		//编码类型
		int						nAudioBit;		//用实际的值表示，如8位 则填值为8
		DWORD					dwSampleRate;	//采样率，如16k 则填值为16000
		char					reserved[64];
	}DHDEV_TALKDECODE_INFO;

	//设备支持的语音对讲格式
	typedef struct 
	{
		int						nSupportNum;	//个数
		DHDEV_TALKDECODE_INFO	type[64];		//编码格式
		char					reserved[64];
	}DHDEV_TALKFORMAT_LIST;

	////////////////////end of 查询设备状态的参数定义////////////////////////////////


	//add by HY 2009-11-2 begin
	//DDNS 配置
	typedef struct 
	{
		BOOL	enable;
		int		DDNSKey;
		char	ServerName[64];
		int		ip;
		int		Port;
		char	UserName[64];
		char	Password[64];
		BOOL	Anonymity;
	}DDNS;

	typedef struct
	{
		DDNS Ddns[2];
	}NET_DEV_DDNS;

	typedef struct
	{
		int	 Count;
		DDNS Ddns[MAX_DDNS_COUNT];
	}NET_DEV_DDNS_EX;
	//add by HY 2009-11-2 end

	//GPS信息结构体
	typedef struct _GPS_Info
	{
		NET_TIME revTime;        //定位时间
		char DvrSerial[50];      //设备序列号
		double longitude;        //经度
		double latidude;         //纬度
		double height;           //高度（米）
		double angle;            //方向角（正北方向为原点，顺时针为正）
		double speed;            //速度（公里/小时）
		WORD starCount;          //定位星数
		BOOL antennaState;       //天线状态(true 好，false 坏)
		BOOL orientationState;    //定位状态（true 定位，false 不定位）
	}GPS_Info,*LPGPS_Info;

	//抓图参数结构体
	typedef struct _snap_param
	{
		UINT Channel;				//抓图的通道
		UINT Quality;				//画质1~6, 此参数只针对车载DVR设备有效
		UINT ImageSize;				//画面大小 0:QCIF,1:CIF,2:D1, 此参数只针对车载DVR设备有效
		UINT mode;					//抓图模式 0：表示请求一帧; 1：表示定时发送请求; 2：表示连续请求,车载DVR设备有效。
		UINT InterSnap;				//时间单位秒 若mode=1表示定时发送请求时，此时间有效，并且只有车载DVR有效。
		UINT CmdSerial;				//请求序列号
		UINT Reserved[4];
	}SNAP_PARAMS,*LPSNAP_PARAMS;

	//网络触发设备抓图结构体
	typedef struct __NET_SNAP_COMMANDINFO 
	{
		char			szCardInfo[16];		//卡号信息
		char			reserved[64];		//保留
	} NET_SNAP_COMMANDINFO, LPNET_SNAP_COMMANDINFO;

	//协议信息
	typedef struct 
	{
		char            protocolname[12]; //协议名
		unsigned int	baudbase;	      //波特率
		unsigned char	databits; 	      //数据位
		unsigned char	stopbits;	      //停止位
		unsigned char	parity;		      //校验位
		unsigned char   reserve;
	} PROTOCOL_INFO, *LPPROTOCOL_INFO;

	//音频格式信息
	typedef struct {
		BYTE	byFormatTag;				//编码类型，如0-PCM
		WORD	nChannels;				//声道数
		WORD	wBitsPerSample;			//采样深度			
		DWORD	nSamplesPerSec;			//采样率
	} RV_AUDIO_FORMAT, *LPRV_AUDIO_FORMAT;

	//查询设备工作状态通道信息:相关接口已淘汰，请不要使用
	typedef struct {
		BYTE byRecordStatic;		//通道是否在录像,0-不录像,1-手动录像，2-自动录象
		BYTE bySignalStatic;		//连接的信号状态,0-正常,1-信号丢失
		BYTE byHardwareStatic;		//通道硬件状态,0-正常,1-异常,例如DSP死掉
		char reserve;
		DWORD dwBitRate;			//实际码率
		DWORD dwLinkNum;			//客户端连接的个数
		DWORD dwClientIP[RV_MAX_LINK];	//客户端的IP地址
	}NET_DEV_CHANNELSTATE,*LPNET_DEV_CHANNELSTATE;

	//查询设备工作状态:相关接口已淘汰，请不要使用
	typedef struct{
		DWORD dwDeviceStatic; 	//设备的状态,0x00 正常,0x01 CPU占用过高,0x02 硬件错误
		NET_DEV_DISKSTATE  stHardDiskStatic[RV_MAX_DISKNUM]; 
		NET_DEV_CHANNELSTATE stChanStatic[RV_MAX_CHANNUM];//通道的状态
		BYTE  byAlarmInStatic[RV_MAX_ALARMIN]; //报警端口的状态,0-没有报警,1-有报警
		BYTE  byAlarmOutStatic[RV_MAX_ALARMOUT]; //报警输出端口的状态,0-没有输出,1-有报警输出
		DWORD  dwLocalDisplay;//本地显示状态,0-正常,1-不正常
	} NET_DEV_WORKSTATE,*LPNET_DEV_WORKSTATE;


	/************************************************************************
	** 侦听服务器回调原形
	***********************************************************************/
	typedef int (CALLBACK *fServiceCallBack)(LONG lHandle, char *pIp, WORD wPort, LONG lCommand, void *pParam, DWORD dwParamLen, DWORD dwUserData);

	/************************************************************************
	** 服务器断开回调原形
	***********************************************************************/
	typedef void (CALLBACK *fDisConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

	//	断线重连成功回调函数
	typedef void (CALLBACK *fHaveReConnect)(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

	/************************************************************************
	** 实时预览回调原形
	***********************************************************************/
	typedef void(CALLBACK *fRealDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

	/************************************************************************
	** 原始数据回调原形
	***********************************************************************/
	typedef int(CALLBACK *fDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser);

	/************************************************************************
	** 消息（报警）回调原形
	***********************************************************************/
	typedef BOOL (CALLBACK *fMessCallBack)(LONG lCommand, LONG lLoginID, char *pBuf,
		DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, DWORD dwUser);

	/************************************************************************
	** 屏幕叠加回调原形
	***********************************************************************/
	typedef void (CALLBACK *fDrawCallBack)(LONG lLoginID, LONG lPlayHandle, HDC hDC, DWORD dwUser);

	/************************************************************************
	** 实时预览回调原形-扩展
	***********************************************************************/
	typedef void(CALLBACK *fRealDataCallBackEx) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, DWORD dwUser);

	/************************************************************************
	** 回放录像进度回调原形
	***********************************************************************/
	typedef void(CALLBACK *fDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, DWORD dwUser);

	/************************************************************************
	** 通过时间下载录像进度回调原形
	现在按时间下载没有使用这个回调函数，使用的是回放录像进度回调原形. //by linjianyan
	***********************************************************************/
	typedef void(CALLBACK *fTimeDownLoadPosCallBack) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, DWORD dwUser);

	/************************************************************************
	** 通过时间下载录像进度回调原形
	现在按时间下载没有使用这个回调函数，使用的是回放录像进度回调原形. //by linjianyan
	***********************************************************************/
	typedef void(CALLBACK *fTimeDownLoadPosCallBackNew) (LONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, int index, NET_RECORDFILE_INFO_EX recordfileinfo, DWORD dwUser);

	/************************************************************************
	** 查询设备日志数据回调原形
	***********************************************************************/
	typedef void(CALLBACK *fLogDataCallBack) (LONG lLoginID, char *pBuffer, DWORD dwBufSize, DWORD nTotalSize, BOOL bEnd, DWORD dwUser);

	/************************************************************************
	** 透明串口回调原形
	***********************************************************************/
	typedef void(CALLBACK *fTransComCallBack) (LONG lLoginID, LONG lTransComChannel, char *pBuffer, DWORD dwBufSize, DWORD dwUser);

	/************************************************************************
	** 升级设备程序回调原形
	***********************************************************************/
	typedef void(CALLBACK *fUpgradeCallBack) (LONG lLoginID, LONG lUpgradechannel, int nTotalSize, int nSendSize, DWORD dwUser);

	/************************************************************************
	** 语音对讲的音频数据回调原形
	***********************************************************************/
	typedef void(CALLBACK *pfAudioDataCallBack) (LONG lTalkHandle, char *pDataBuf, DWORD dwBufSize, BYTE byAudioFlag, DWORD dwUser);

	/***********************************************************************
	** 抓图回调原形
	***********************************************************************/
	typedef void( CALLBACK *fSnapRev)(LONG lLoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, DWORD dwUser);

	/************************************************************************
	** SDK初始化
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_Init(fDisConnect cbDisConnect, DWORD dwUser);

	/************************************************************************
	** SDK退出清理
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_Cleanup();

	/************************************************************************
	** 设置断线重连成功回调函数，设置后SDK内部断线自动重连
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_SetAutoReconnect(fHaveReConnect cbAutoConnect, DWORD dwUser);

	/************************************************************************
	** 返回函数执行失败代码
	***********************************************************************/
	CLIENT_API DWORD CALL_METHOD CLIENT_GetLastError(void);

	/************************************************************************
	** 设置连接服务器超时时间和尝试次数
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);


	/************************************************************************
	** 获取SDK的版本信息
	***********************************************************************/
	CLIENT_API DWORD CALL_METHOD CLIENT_GetSDKVersion();


	/************************************************************************
	** 向设备注册
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_Login(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

	//	nSpecCap = 0为TCP方式下的登入；nSpecCap = 2为主动注册的登入；nSpecCap = 3为组播方式下的登入；nSpecCap = 4为UDP方式下的登入
	CLIENT_API LONG CALL_METHOD CLIENT_LoginEx(char *pchDVRIP, WORD wDVRPort, char *pchUserName, char *pchPassword, int nSpecCap, void* pCapParam, LPNET_DEVICEINFO lpDeviceInfo, int *error = 0);

	/************************************************************************
	** 向设备注销
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_Logout(LONG lLoginID);

	/************************************************************************
	** 
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_SetDVRMessCallBack(fMessCallBack cbMessage,DWORD dwUser);


	/************************************************************************
	** 设置语音对讲模式
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetDeviceMode(LONG lLoginID, EM_USEDEV_MODE emType, void* pValue);


	/************************************************************************
	** 开始帧听设备
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StartListen(LONG lLoginID);

	//对应LB设备程序版本
	CLIENT_API BOOL CALL_METHOD CLIENT_StartListenEx(LONG lLoginID);

	/************************************************************************
	** 停止帧听设备
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopListen(LONG lLoginID);

	/************************************************************************
	** 开始侦听服务器，侦听移动DVR连接
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_ListenServer(char* ip, WORD port, int nTimeout, fServiceCallBack cbListen, DWORD dwUserData);

	/************************************************************************
	** 停止侦听服务器，停止侦听移动DVR连接
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopListenServer(LONG lServerHandle);

	/************************************************************************
	** 响应设备的注册请求
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ResponseDevReg(char *devSerial, char* ip, WORD port, BOOL bAccept);

	/************************************************************************
	** 启动服务端口
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_StartService(WORD wPort, char *pIp = NULL, fServiceCallBack pfscb = NULL, 
		DWORD dwTimeOut = 0xffffffff, DWORD dwUserData = 0);

	/************************************************************************
	** 停止服务端口
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopService(LONG lHandle);

	/************************************************************************
	** 设置屏幕叠加回调
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_RigisterDrawFun(fDrawCallBack cbDraw, DWORD dwUser);


	/************************************************************************
	** 开始实时预览
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_RealPlay(LONG lLoginID, int nChannelID, HWND hWnd);
	CLIENT_API LONG CALL_METHOD CLIENT_RealPlayEx(LONG lLoginID, int nChannelID, HWND hWnd, RV_RealPlayType rType = RV_RType_Realplay);

	CLIENT_API BOOL	CALL_METHOD CLIENT_AdjustFluency(LONG lRealHandle, int nLevel);

	/************************************************************************
	** 停止实时预览
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlay(LONG lRealHandle);

	/************************************************************************
	** 停止实时预览_扩展
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopRealPlayEx(LONG lRealHandle);

	/************************************************************************
	** 保存数据为文件
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SaveRealData(LONG lRealHandle, const char *pchFileName);


	/************************************************************************
	** 结束保存数据为文件
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopSaveRealData(LONG lRealHandle);


	/************************************************************************
	** 设置图像属性
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ClientSetVideoEffect(LONG lPlayHandle, unsigned char nBrightness, unsigned char nContrast, unsigned char nHue, unsigned char nSaturation);

	/************************************************************************
	** 设置图像属性()
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ClientSetVideoEffectNew(LONG lPlayHandle, unsigned char nBrightness, unsigned char nContrast, unsigned char nHue, unsigned char nSaturation);

	/************************************************************************
	** 获取图像属性
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ClientGetVideoEffect(LONG lPlayHandle, unsigned char *nBrightness, unsigned char *nContrast, unsigned char *nHue, unsigned char *nSaturation);

	/************************************************************************
	** 获取图像属性()
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ClientGetVideoEffectNew(LONG lPlayHandle, unsigned char *nBrightness, unsigned char *nContrast, unsigned char *nHue, unsigned char *nSaturation);

	/************************************************************************
	** 打开声音
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_OpenSound(LONG hPlayHandle);

	/************************************************************************
	** 打开声音
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_OpenSoundNew(LONG hPlayHandle);

	/************************************************************************
	** 关闭声音
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_CloseSound();

	/************************************************************************
	** 关闭声音
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_CloseSoundNew();
	
	/************************************************************************
	** 打开语音对讲
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_StartTalkEx(LONG lLoginID, pfAudioDataCallBack pfcb, DWORD dwUser);

	/************************************************************************
	** 设置语音对讲的音量
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetAudioClientVolume(LONG lTalkHandle, WORD wVolume);

	/************************************************************************
	** 发送语音数据到设备
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_TalkSendData(LONG lTalkHandle, char *pSendBuf, DWORD dwBufSize);

	/************************************************************************
	** 开始PC端录音
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_RecordStart();

	/************************************************************************
	** 结束PC端录音
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_RecordStop();

	/************************************************************************
	** 解码音频数据
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_AudioDec(char *pAudioDataBuf, DWORD dwBufSize);

	/************************************************************************
	** 停止语音对讲
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopTalkEx(LONG lTalkHandle);

	/************************************************************************
	** 添加设备到广播组
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastAddDev(LONG lLoginID);

	/************************************************************************
	** 从广播组中删除设备
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_AudioBroadcastDelDev(LONG lLoginID);

	/************************************************************************
	** 设置音量
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetVolume(LONG lPlayHandle, int nVolume);

	/************************************************************************
	** 音频编码--初始化(特定标准格式->RV格式)
	***********************************************************************/
	CLIENT_API int  CALL_METHOD CLIENT_InitAudioEncode(RV_AUDIO_FORMAT aft);

	/************************************************************************
	** 音频编码--数据编码
	***********************************************************************/
	CLIENT_API int	CALL_METHOD	CLIENT_AudioEncode(LONG lTalkHandle, BYTE *lpInBuf, DWORD *lpInLen, BYTE *lpOutBuf, DWORD *lpOutLen);

	/************************************************************************
	** 音频编码--完成退出
	***********************************************************************/
	CLIENT_API int	CALL_METHOD	CLIENT_ReleaseAudioEncode();

	/************************************************************************
	** 设置实时预览回调
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBack(LONG lRealHandle, fRealDataCallBack cbRealData, DWORD dwUser);

	/************************************************************************
	** 设置实时预览回调-扩展
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetRealDataCallBackEx(LONG lRealHandle, fRealDataCallBackEx cbRealData, DWORD dwUser, DWORD dwFlag);

	/************************************************************************
	** 抓图
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_CapturePicture(LONG hPlayHandle, const char *pchPicFileName);

	/************************************************************************
	** 抓图
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_CapturePictureNew(LONG hPlayHandle, const char *pchPicFileName);

	/************************************************************************
	** 查询某段时间内是否有录像
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, BOOL *bResult, int waittime=1000);

	/************************************************************************
	** 通过时间查询录像文件
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordFile(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char* pchCardid, LPNET_RECORDFILE_INFO nriFileinfo, int maxlen, int *filecount, int waittime=1000, BOOL bTime = FALSE);

	/************************************************************************
	** 回放录像文件
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFile(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

	/************************************************************************
	** 回放录像文件扩展_增加了数据回调参数
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFileEx(LONG lLoginID, LPNET_RECORDFILE_INFO lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser, fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

	/************************************************************************
	** 回放录像文件
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByRecordFileNew(LONG lLoginID, LPNET_RECORDFILE_INFO_EX lpRecordFile, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

	/************************************************************************
	** 暂停录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_PausePlayBack(LONG lPlayHandle, BOOL bPause);

	/************************************************************************
	** 暂停录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_PausePlayBackNew(LONG lPlayHandle, BOOL bPause);

	/************************************************************************
	** 定位录像回放起始点
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SeekPlayBack(LONG lPlayHandle, unsigned int offsettime, unsigned int offsetbyte);

	/************************************************************************
	** 定位录像回放起始点
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SeekPlayBackNew(LONG lPlayHandle, unsigned int offsettime, unsigned int offsetbyte);

	/************************************************************************
	** 停止录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopPlayBack(LONG lPlayHandle);

	/************************************************************************
	** 停止录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopPlayBackNew(LONG lPlayHandle);

	/************************************************************************
	** 步进录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StepPlayBack(LONG lPlayHandle, BOOL bStop);

	/************************************************************************
	** 步进录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StepPlayBackNew(LONG lPlayHandle, BOOL bStop);

	/************************************************************************
	** 快进录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_FastPlayBack(LONG lPlayHandle);

	/************************************************************************
	** 快进录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_FastPlayBackNew(LONG lPlayHandle);

	/************************************************************************
	** 慢进录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SlowPlayBack(LONG lPlayHandle);

	/************************************************************************
	** 慢进录像回放
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SlowPlayBackNew(LONG lPlayHandle);

	/************************************************************************
	** 恢复正常回放速度
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_NormalPlayBack(LONG lPlayHandle);

	/************************************************************************
	** 恢复正常回放速度
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_NormalPlayBackNew(LONG lPlayHandle);

	/************************************************************************
	** 设置回放帧速
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetFramePlayBack(LONG lPlayHandle, int framerate);

	/************************************************************************
	** 获取回放帧速
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetFramePlayBack(LONG lPlayHandle, int *fileframerate, int *playframerate);

	/************************************************************************
	** 获取回放OSD时间
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetPlayBackOsdTime(LONG lPlayHandle, LPNET_TIME lpOsdTime, LPNET_TIME lpStartTime, LPNET_TIME lpEndTime);

	/************************************************************************
	** 下载录像文件
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_DownloadByRecordFile(LONG lLoginID,LPNET_RECORDFILE_INFO lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

	/************************************************************************
	** 下载录像文件
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_DownloadByRecordFileNew(LONG lLoginID,LPNET_RECORDFILE_INFO_EX lpRecordFile, char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

	/************************************************************************
	** 通过时间下载录像
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_DownloadByTime(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char *sSavedFileName, fTimeDownLoadPosCallBack cbTimeDownLoadPos, DWORD dwUserData);

	/************************************************************************
	** 通过时间下载录像
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_DownloadByTimeNew(LONG lLoginID, int nChannelId, int nRecordFileType, LPNET_TIME tmStart, LPNET_TIME tmEnd, char *sSavedFileName, fTimeDownLoadPosCallBackNew cbTimeDownLoadPos, DWORD dwUserData);

	/************************************************************************
	** 停止录像下载
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopDownload(LONG lFileHandle);

	/************************************************************************
	** 停止录像下载
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopDownloadNew(LONG lFileHandle);

	/************************************************************************
	** 查询录像下载进度
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetDownloadPos(LONG lFileHandle, int *nTotalSize, int *nDownLoadSize);

	/************************************************************************
	** 查询录像下载进度
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetDownloadPosNew(LONG lFileHandle, int *nTotalSize, int *nDownLoadSize);

	/************************************************************************
	** 多画面实时预览
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_MultiPlay(LONG lLoginID, HWND hWnd);

	/************************************************************************
	** 停止多画面实时预览
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopMultiPlay(LONG lMultiHandle);

	/************************************************************************
	** 查询系统服务器配置 －该接口已淘汰，请使用CLIENT_GetDevConfig
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int maxlen, int *nConfigbuflen, int waittime=1000);

	/************************************************************************
	** 设置系统服务器配置 －该接口已淘汰，请使用CLIENT_SetDevConfig
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetupConfig(LONG lLoginID, int nConfigType, char *pConfigbuf, int nConfigbuflen, int waittime=1000);

	/************************************************************************
	** 查询设备通道名称
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryChannelName(LONG lLoginID, char *pChannelName, int maxlen, int *nChannelCount, int waittime=1000);

	/************************************************************************
	** 设置设备通道名称
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelName(LONG lLoginID,char *pbuf, int nbuflen);

	/************************************************************************
	** 设置设备通道字符叠加
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_SetupChannelOsdString(LONG lLoginID, int nChannelNo, RV_CHANNEL_OSDSTRING* struOsdString, int nbuflen);

	/************************************************************************
	** 通用云台控制
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_PTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, DWORD dwStep, BOOL dwStop);

	/************************************************************************
	** RV云台控制
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControl(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, 
		unsigned char param1, unsigned char param2, unsigned char param3, BOOL dwStop);

	/************************************************************************
	** RV云台控制扩展接口，支持三维快速定位
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_DHPTZControlEx(LONG lLoginID, int nChannelID, DWORD dwPTZCommand, 
		LONG lParam1, LONG lParam2, LONG lParam3, BOOL dwStop);

	/************************************************************************
	** 异步查询设备日志
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogCallback(LONG lLoginID, fLogDataCallBack cbLogData, DWORD dwUser);

	/************************************************************************
	** 同步查询设备日志
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryLog(LONG lLoginID, char *pLogBuffer, int maxlen, int *nLogBufferlen, int waittime=3000);

	CLIENT_API BOOL CALL_METHOD CLIENT_QueryLogEx(LONG lLoginID, RV_LOG_QUERY_TYPE logType, char *pLogBuffer, int maxlen, int *nLogBufferlen, void* reserved, int waittime=3000);

	/************************************************************************
	** 创建透明串口通道
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_CreateTransComChannel(LONG lLoginID, int TransComType, 
		unsigned int baudrate, unsigned int databits,
		unsigned int stopbits, unsigned int parity,
		fTransComCallBack cbTransCom, DWORD dwUser);

	/************************************************************************
	** 透明串口发送数据
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SendTransComData(LONG lTransComChannel, char *pBuffer, DWORD dwBufSize);

	/************************************************************************
	** 释放通明串口通道
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_DestroyTransComChannel(LONG lTransComChannel);

	/************************************************************************
	** 该接口已淘汰，请不要使用
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_Reset(LONG lLoginID, BOOL bReset);

	//add by HY 2009-5-22 begin
	// 设置登陆网络环境
	CLIENT_API void CALL_METHOD CLIENT_SetNetworkParam(NET_PARAM *pNetParam);

	// 导入配置文件
	CLIENT_API LONG CALL_METHOD CLIENT_ImportConfigFile(LONG lLoginID, char *szFileName, fDownLoadPosCallBack cbUploadPos, DWORD dwUserData, DWORD param=0);

	// 停止导入配置文件
	CLIENT_API BOOL CALL_METHOD CLIENT_StopImportCfgFile(LONG lImportHandle);

	// 导出配置文件
	CLIENT_API LONG CALL_METHOD CLIENT_ExportConfigFile(LONG lLoginID, DH_CONFIG_FILE_TYPE emConfigFileType, char *szSavedFilePath, fDownLoadPosCallBack cbDownLoadPos, DWORD dwUserData);

	// 停止导出配置文件
	CLIENT_API BOOL CALL_METHOD CLIENT_StopExportCfgFile(LONG lExportHandle);

	// 搜索局域网内IPC、NVS等设备
	CLIENT_API BOOL CALL_METHOD CLIENT_SearchDevices(char* szBuf, int nBufLen, int* pRetLen, DWORD dwSearchTime);
	//add by HY 2009-5-22 end

	//add by HY 2009-9-24 begin
	CLIENT_API BOOL CALL_METHOD CLIENT_GetDiskType(LONG lLoginID, NET_DEV_DISKTYPE* Type, DWORD dwSearchTime = 2000);
	//add by HY 2009-9-24 end

	/************************************************************************
	** 开始升级设备程序
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_StartUpgrade(LONG lLoginID, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);

	CLIENT_API LONG CALL_METHOD CLIENT_StartUpgradeEx(LONG lLoginID, EM_UPGRADE_TYPE emType, char *pchFileName, fUpgradeCallBack cbUpgrade, DWORD dwUser);


	/************************************************************************
	** 发送数据
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SendUpgrade(LONG lUpgradeID);

	/************************************************************************
	** 结束升级设备程序
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopUpgrade(LONG lUpgradeID);

	/************************************************************************
	** 查道通道录像状态
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryRecordState(LONG lLoginID, char *pRSBuffer, int maxlen, int *nRSBufferlen, int waittime=1000);

	/************************************************************************
	** 设置通道录像状态
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetupRecordState(LONG lLoginID, char *pRSBuffer, int nRSBufferlen);

	/************************************************************************
	** 查询设备当前时间
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime, int waittime=1000);

	/************************************************************************
	** 设置设备当前时间
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetupDeviceTime(LONG lLoginID, LPNET_TIME pDeviceTime);

	/************************************************************************
	** 查询串口协议 － 该接口已淘汰，请使用CLIENT_GetDevConfig
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryComProtocol(LONG lLoginID, int nProtocolType, char *pProtocolBuffer, int maxlen, int *nProtocollen, int waittime=1000);

	/************************************************************************
	** 查询系统信息 － 该接口已淘汰，请使用CLIENT_GetDevConfig
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QuerySystemInfo(LONG lLoginID, int nSystemType, char *pSysInfoBuffer, int maxlen, int *nSysInfolen, int waittime=1000);

	//add by HY 2009-8-12 begin
	/************************************************************************
	查询用户锁定状态
	*************************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetUserLockState(LONG lLoginID, char* UserName, bool *bstate);

	/************************************************************************
	解除用户锁定状态
	*************************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_UnlockUserLockState(LONG lLoginID, char* UserName);
	//add by HY 2009-8-12 end

	/************************************************************************
	建议以后用扩展接口(有些设备用户名最大长度是16个字节的)
	*************************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfoEx(LONG lLoginID, USER_MANAGE_INFO_EX *info, int waittime=1000);

	/************************************************************************
	** 查询设备用户(老接口。支持8位用户名长度。)
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryUserInfo(LONG lLoginID, USER_MANAGE_INFO *info, int waittime=1000);

	/************************************************************************
	** 操作设备用户
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfo(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);
	/************************************************************************
	** 操作设备用户（新接口，支持8位和16位的用户名密码长度）。
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_OperateUserInfoEx(LONG lLoginID, int nOperateType, void *opParam, void *subParam, int waittime=1000);

	/************************************************************************
	** 开始对讲 － 该接口已淘汰，请使用CLIENT_StartTalkEx
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StartTalk(LONG lRealHandle, BOOL bCustomSend=false);

	/************************************************************************
	** 结束对讲 － 该接口已淘汰，请使用CLIENT_StopTalkEx
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_StopTalk(LONG lRealHandle);

	/************************************************************************
	** 发送自定义对讲数据 － 该接口已淘汰，请使用CLIENT_TalkSendData
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SendTalkData_Custom(LONG lRealHandle, char *pBuffer, DWORD dwBufSize);

	/************************************************************************
	** 获取通道码流
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_GetStatiscFlux(LONG lLoginID, LONG lPlayHandle);

	/************************************************************************
	** 查询IO状态
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryIOControlState(LONG lLoginID, RV_IOTYPE emType, 
		void *pState, int maxlen, int *nIOCount, int waittime=1000);

	/************************************************************************
	** IO控制
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_IOControl(LONG lLoginID, RV_IOTYPE emType, void *pState, int maxlen);

	/************************************************************************
	** 查询设备工作状态 该接口已淘汰，请使用CLIENT_QueryDevState
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetDEVWorkState(LONG lLoginID, LPNET_DEV_WORKSTATE lpWorkState, int waittime=1000);

	/************************************************************************
	** 强制I帧
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_MakeKeyFrame(LONG lLoginID, int nChannelID, int nSubChannel=0);

	/************************************************************************
	** 设置设备限制码流
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetMaxFlux(LONG lLoginID, WORD wFlux);

	/************************************************************************
	** 通过设备名字或者设备序列号向DDNS服务器查询设备IP
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetDVRIPByResolveSvr(char *pchDVRIP, WORD wDVRPort, 
		BYTE *sDVRName, WORD wDVRNameLen, 
		BYTE *sDVRSerialNumber, WORD wDVRSerialLen, 
		char* sGetIP);

	/************************************************************************
	** 通过时间回放录像
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTime(LONG lLoginID, int nChannelID, 
		LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser);

	/************************************************************************
	** 通过时间回放录像
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTimeNew(LONG lLoginID, int nChannelID, 
		LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser);

	/************************************************************************
	** 通过时间回放录像扩展_增加了数据回调参数
	***********************************************************************/
	CLIENT_API LONG CALL_METHOD CLIENT_PlayBackByTimeEx(LONG lLoginID, int nChannelID, 
		LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, HWND hWnd, fDownLoadPosCallBack cbDownLoadPos, DWORD dwPosUser,
		fDataCallBack fDownLoadDataCallBack, DWORD dwDataUser);

	/************************************************************************
	** 开始查找录像文件
	***********************************************************************/
	CLIENT_API LONG	CALL_METHOD CLIENT_FindFile(LONG lLoginID,
		int nChannelId,
		int nRecordFileType, 
		char* cardid, 
		LPNET_TIME time_start,
		LPNET_TIME time_end,
		BOOL bTime, 
		int waittime);

	/************************************************************************
	** 查找录像文件
	***********************************************************************/
	CLIENT_API int	CALL_METHOD CLIENT_FindNextFile(LONG lFindHandle,LPNET_RECORDFILE_INFO lpFindData);

	/************************************************************************
	** 结束录像文件查找
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_FindClose(LONG lFindHandle);

	/************************************************************************
	** 开始查找录像文件
	***********************************************************************/
	CLIENT_API LONG	CALL_METHOD CLIENT_FindFileNew(LONG lLoginID,
		int nChannelId,
		int nRecordFileType, 
		char* cardid, 
		LPNET_TIME time_start,
		LPNET_TIME time_end,
		BOOL bTime, 
		int waittime);

	/************************************************************************
	** 查找录像文件
	***********************************************************************/
	CLIENT_API int	CALL_METHOD CLIENT_FindNextFileNew(LONG lFindHandle,LPNET_RECORDFILE_INFO_EX lpFindData);

	/************************************************************************
	** 结束录像文件查找
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_FindCloseNew(LONG lFindHandle);

	/************************************************************************
	** 重启设备：返回值 TRUE 成功， FALSE 失败
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_RebootDev(LONG lLoginID);

	/************************************************************************
	** 恢复出厂设置：返回值 TRUE 成功， FALSE 失败
	***********************************************************************/
	//add by HY 2009-7-14 begin
	CLIENT_API BOOL CALL_METHOD CLIENT_ResetDeafault(LONG lLoginID, unsigned long type);
	//add by HY 2009-7-14 end

	/************************************************************************
	** 关闭设备：返回值 TRUE 成功， FALSE 失败
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ShutDownDev(LONG lLoginID);

	/************************************************************************
	** 设备控制：返回值 TRUE 成功， FALSE 失败
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_ControlDevice(LONG lLoginID, CtrlType type, void *param, int waittime = 1000);

	/************************************************************************
	** 主动查询设备状态，包括报警、硬盘、录象状态等。
	***********************************************************************/
	//	当 nType  = RV_DEVSTATE_BURNING_PROGRESS 时,*pRetLen表示查询设备的id号.这样做的原因是没有其它可以使用的入口参数。
	CLIENT_API BOOL CALL_METHOD CLIENT_QueryDevState(LONG lLoginID, int nType, char *pBuf, int nBufLen, int *pRetLen, int waittime=1000);

	///////////////以下是车载dvr相关的功能接口 /////////////////////////
	/************************************************************************
	** 设置抓图回调函数
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_SetSnapRevCallBack(fSnapRev OnSnapRevMessage, DWORD dwUser);

	/************************************************************************
	** 抓图请求
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SnapPicture(LONG lLoginID, SNAP_PARAMS par);

	/************************************************************************
	** GPS信息订阅回调
	***********************************************************************/
	typedef void( CALLBACK *fGPSRev)(LONG lLoginID,           //设备句柄
		GPS_Info  GpsInfo,       //GPS信息
		DWORD dwUserData);       //用户数据

	/************************************************************************
	** 设置GPS订阅回调函数
	***********************************************************************/
	CLIENT_API void CALL_METHOD CLIENT_SetSubcribeGPSCallBack(fGPSRev OnGPSMessage, DWORD dwUser);

	/************************************************************************
	** GPS信息订阅
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SubcribeGPS (
		LONG lLoginID,        //设备句柄
		BOOL bStart,		  //是否启动
		LONG KeepTime,        //订阅持续时间（单位秒）
		LONG InterTime);      //订阅时间内GPS发送频率（单位秒）  

	/////////////////////////end of 车载dvr相关的功能 ///////////////////////////////


	/////////////////////////未实现//////////////////////////
	/************************************************************************
	** 设置实时预览缓冲大小
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_SetPlayerBufNumber(LONG lRealHandle, DWORD dwBufNum);

	/************************************************************************
	** 通过时间下载文件，CLIENT_DownloadByTime已实现该功能
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_GetFileByTime(LONG lLoginID, int nChannelID, 
		LPNET_TIME lpStartTime, LPNET_TIME lpStopTime, char *sSavedFileName);

	/************************************************************************
	** 网络回放控制，已细化为各控制接口了
	***********************************************************************/
	CLIENT_API BOOL CALL_METHOD CLIENT_PlayBackControl(LONG lPlayHandle, 
		DWORD dwControlCode, DWORD dwInValue, DWORD *lpOutValue);

	//////////////////////////end of 未实现//////////////////////


	/************************************************************************
	** 远程配置设备接口 配置类型见RV_DEV_DEVICECFG 各宏
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_GetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
	CLIENT_API BOOL  CALL_METHOD CLIENT_SetDevConfig(LONG lLoginID, DWORD dwCommand, LONG lChannel, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);


	/************************************************************************
	** 平台接入接口
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_GetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpOutBuffer, DWORD dwOutBufferSize, LPDWORD lpBytesReturned,int waittime=500);
	CLIENT_API BOOL  CALL_METHOD CLIENT_SetPlatFormInfo(LONG lLoginID, DWORD dwCommand, int nSubCommand, int nParam, LPVOID lpInBuffer, DWORD dwInBufferSize, int waittime=500);

	/************************************************************************
	**  获取云台信息	
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_GetPtzOptAttr(LONG lLoginID,DWORD dwProtocolIndex,LPVOID lpOutBuffer,DWORD dwBufLen,DWORD *lpBytesReturned,int waittime=500);

	//chenfeng20090304 -s
	/************************************************************************
	**  获取远程回放解码句柄
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_GetPlayBackPlayerIndex(LONG lPlayHandle);

	/************************************************************************
	**  获取预览解码句柄
	***********************************************************************/
	CLIENT_API BOOL  CALL_METHOD CLIENT_GetRealPlayerIndex(LONG lPlayHandle);

	//chenfeng20090304 -e
	/******************************************************************************
	** 以下是各部分配置结构体
	*****************************************************************************/

	/************************************************************************
	*	系统信息
	************************************************************************/

	/*版本信息:高16位表示主版本号，低16位表示次版本号
	*关于时间的数据组织是：yyyymmdd */
	typedef struct 
	{
		DWORD	dwSoftwareVersion;
		DWORD	dwSoftwareBuildDate;
		DWORD	dwDspSoftwareVersion;
		DWORD	dwDspSoftwareBuildDate;
		DWORD	dwPanelVersion;
		DWORD	dwPanelSoftwareBuildDate;
		DWORD	dwHardwareVersion;
		DWORD	dwHardwareDate;
		DWORD	dwWebVersion;
		DWORD	dwWebBuildDate;
	} RV_VERSION_INFO, *LPRV_VERSION_INFO;

	/* DSP能力描述(CLIENT_GetDevConfig里的)，以后建议用CLIENT_QueryDevState接口 */
	typedef struct 
	{
		DWORD	dwVideoStandardMask;	//视频制式掩码，按位表示设备能够支持的视频制式
		DWORD	dwImageSizeMask;		//分辨率掩码，按位表示设备能够支持的分辨率设置
		DWORD	dwEncodeModeMask;		//编码模式掩码，按位表示设备能够支持的编码模式设置	

		DWORD	dwStreamCap;			//按位表示设备支持的多媒体功能，
		//第一位表示支持主码流
		//第二位表示支持辅码流1
		//第三位表示支持辅码流2
		//第五位表示支持jpg抓图
		DWORD	dwImageSizeMask_Assi[8];//表示主码流为各分辨率时，支持的辅码流分辨率掩码。

		DWORD	dwMaxEncodePower;		//- DSP 支持的最高编码能力 
		WORD	wMaxSupportChannel;		//- 每块 DSP 支持最多输入视频通道数 
		WORD	wChannelMaxSetSync;		//- DSP 每通道的最大编码设置是否同步 0-不同步, 1-同步
	} RV_DSP_ENCODECAP, *LPRV_DSP_ENCODECAP;

	/* 设备信息配置结构 */
	typedef struct 
	{
		DWORD dwSize;
		/*下面是设备的只读部分*/
		RV_VERSION_INFO			stVersion;
		RV_DSP_ENCODECAP		stDspEncodeCap;		//DSP能力描述
		BYTE	szDevSerialNo[RV_DEV_SERIALNO_LEN];	//序列号
		BYTE	byDevType;							//设备类型，见枚举DHDEV_DEVICE_TYPE
		BYTE	szDevType[RV_DEV_TYPE_LEN];			//设备详细型号，字符串格式，可能为空
		BYTE	byVideoCaptureNum;					//视频口数量
		BYTE	byAudioCaptureNum;					//音频口数量
		BYTE	byTalkInChanNum;					//NSP
		BYTE	byTalkOutChanNum;					//NSP
		BYTE	byDecodeChanNum;					//NSP
		BYTE	byAlarmInNum;						//报警输入口数
		BYTE	byAlarmOutNum;						//报警输出口数
		BYTE	byNetIONum;							//网络口数
		BYTE	byUsbIONum;							//USB口数量
		BYTE	byIdeIONum;							//IDE数量
		BYTE	byComIONum;							//串口数量
		BYTE	byLPTIONum;							//并口数量
		BYTE	byVgaIONum;							//NSP
		BYTE	byIdeControlNum;					//NSP
		BYTE	byIdeControlType;					//NSP
		BYTE	byCapability;						//NSP，扩展描述
		BYTE	byMatrixOutNum;						//视频矩阵输出口数

		/*下面是设备的可写部分*/
		BYTE	byOverWrite;						//硬盘满处理方式（覆盖、停止）
		BYTE	byRecordLen;						//录象打包长度
		BYTE	byStartChanNo;						//NSP
		WORD	wDevNo;								//设备编号，用于遥控
		BYTE	byVideoStandard;					//视频制式
		BYTE	byDateFormat;						//日期格式
		BYTE	byDateSprtr;						//日期分割符(0-"."， 1-"-"， 2-"/")
		BYTE	byTimeFmt;							//时间格式 (0-24小时，1－12小时)
		BYTE	byLanguage;						    //枚举值详见RV_LANGUAGE_TYPE
	} DHDEV_SYSTEM_ATTR_CFG, *LPDHDEV_SYSTEM_ATTR_CFG;

	/************************************************************************
	*	图像通道
	************************************************************************/

	/* 录象类型－定时、动态检测、报警 */
	typedef enum _REC_TYPE
	{
		RV_REC_TYPE_TIM = 0,
		RV_REC_TYPE_MTD,
		RV_REC_TYPE_ALM,
		RV_REC_TYPE_NUM,
	}REC_TYPE;

	/* 时间段结构 */															    
	typedef struct 
	{
		BOOL	bEnable;	//当表示录像时间段时，按位表示三个使能，从低位到高位分别表示动检录象、报警录象、普通录象
		int		iBeginHour;
		int		iBeginMin;
		int		iBeginSec;
		int		iEndHour;
		int		iEndMin;
		int		iEndSec;
	} RV_TSECT, *LPRV_TSECT;

	/* 区域:各边距按整长8192的比例 */
	typedef struct {
		long		left;
		long		top;
		long		right;
		long		bottom;
	} RV_RECT,*LPRV_RECT;

	/* OSD属性结构 */
	typedef struct  tagENCODE_WIDGET
	{
		DWORD		rgbaFrontground;		/*!< 物件的前景RGB，和透明度 */
		DWORD		rgbaBackground;			/*!< 物件的后景RGB，和透明度*/
		RV_RECT		rcRect;					/*!< 位置 */
		BYTE		bShow;					/*!< 物件显示 */
		BYTE		byReserved[3];
	} RV_ENCODE_WIDGET, *LPRV_ENCODE_WIDGET;

	/* 通道音视频属性 */
	typedef struct 
	{
		//视频参数
		BYTE	byVideoEnable;				/* 视频使能:1－打开，0－关闭 */
		BYTE	byBitRateControl;			/* 码流控制,参照常量定义 */
		BYTE	byFramesPerSec;				/* 帧率 */
		BYTE	byEncodeMode;				/* 编码模式,参照常量定义 */
		BYTE	byImageSize;				/* 分辨率参,参照常量定义 */
		BYTE	byImageQlty;				/* 档次1-6 */
		WORD	wLimitStream;				/* 限码流参数, 范围：50~4*1024 (k) */

		//音频参数
		BYTE	byAudioEnable;				/* 音频使能:1－打开，0－关闭 */
		BYTE	wFormatTag;					/* 编码类型，如PCM */
		WORD	nChannels;					/* 声道数 */
		WORD	wBitsPerSample;				/* 采样深度 */		
		DWORD	nSamplesPerSec;				/* 采样率 */

		BYTE	bIFrameInterval;			/* I帧间隔帧数量，描述两个I帧之间的P帧个数，0-149*/
		char	reserved;
	} RV_VIDEOENC_OPT, *LPRV_VIDEOENC_OPT;

	/* 画面颜色属性 */
	typedef struct 
	{
		RV_TSECT	stSect;
		BYTE		byBrightness;			/*! 亮度	0-100*/
		BYTE		byContrast;				/*! 对比度	0-100*/
		BYTE		bySaturation;			/*!	饱和度	0-100*/
		BYTE		byHue;					/*!	色度	0-100*/
		BYTE		byGainEn;				/*! 增益使能	 */ 
		BYTE		byGain;					/*! 增益	0-100*/
		BYTE		byReserved[2];
	} RV_COLOR_CFG, *LPRV_COLOR_CFG;

	/* 图像通道属性结构体 */
	typedef struct 
	{
		DWORD				dwSize;
		char				szChannelName[RV_CHAN_NAME_LEN];
		RV_VIDEOENC_OPT		stMainVideoEncOpt[RV_REC_TYPE_NUM];
		RV_VIDEOENC_OPT		stAssiVideoEncOpt[RV_N_ENCODE_AUX];		
		RV_COLOR_CFG		stColorCfg[RV_N_COL_TSECT];
		RV_ENCODE_WIDGET	stTimeOSD;
		RV_ENCODE_WIDGET	stChannelOSD;
		RV_ENCODE_WIDGET	stBlindCover[RV_N_COVERS];
		BYTE				byBlindEnable;			/*! 区域遮盖开关　0x00不使能遮盖，0x01仅遮盖设备本地预览，0x10仅遮盖录像（及网络预览），0x11都遮盖	*/
		BYTE				byBlindMask;			/*! 区域遮盖掩码  第一位：设备本地预览；第二位：录像(及网络预览) */
		BYTE				byReserved[2];
	} DHDEV_CHANNEL_CFG, *LPDHDEV_CHANNEL_CFG;


	/************************************************************************
	*	区域遮挡配置
	************************************************************************/
	typedef struct __VIDEO_COVER_ATTR
	{
		RV_RECT				rcBlock;			// 覆盖的区域坐标
		int					nColor;				// 覆盖的颜色
		BYTE				bBlockType;			// 覆盖方式：0-黑块，1-马赛克
		BYTE				bEncode;			// 编码级遮挡, 1—生效, 0—不生郊
		BYTE				bPriview;           // 预览遮挡, 1—生效, 0—不生效
		char				reserved[29];		// 保留
	}VIDEO_COVER_ATTR;

	typedef struct __DHDEV_VIDEOCOVER_CFG 
	{
		DWORD				dwSize;
		char				szChannelName[RV_CHAN_NAME_LEN];	// 只读
		BYTE				bTotalBlocks;		// 支持的遮挡块数
		BYTE				bCoverCount;		// 已设置的遮挡块数
		VIDEO_COVER_ATTR	CoverBlock[RV_MAX_VIDEO_COVER_NUM];		// 覆盖的区域	
		char				reserved[30];		// 保留
	}DHDEV_VIDEOCOVER_CFG;


	/************************************************************************
	*	预览图像参数
	************************************************************************/

	/* 预览图像参数 */
	typedef struct 
	{
		DWORD dwSize;
		RV_VIDEOENC_OPT	stPreView;
		RV_COLOR_CFG  stColorCfg[RV_N_COL_TSECT];
	}DHDEV_PREVIEW_CFG;

	/************************************************************************
	* 语音对讲音频属性结构体－未实现，待扩展 *
	************************************************************************/

	typedef struct 
	{
		//音频输入参数
		BYTE	byInFormatTag;					//编码类型，如PCM
		BYTE	byInChannels;					//声道数
		WORD	wInBitsPerSample;				//采样深度					
		DWORD	dwInSamplesPerSec;				//采样率

		//音频输出参数
		BYTE	byOutFormatTag;					//编码类型，如PCM
		BYTE	byOutChannels;					//声道数
		WORD	wOutBitsPerSample;				//采样深度				
		DWORD	dwOutSamplesPerSec;				//采样率
	} DHDEV_TALK_CFG, *LPDHDEV_TALK_CFG;

	/************************************************************************
	*	定时录象
	************************************************************************/

	typedef struct 
	{
		DWORD		dwSize;
		RV_TSECT	stSect[RV_N_WEEKS][RV_N_REC_TSECT];
		BYTE		byPreRecordLen;					/*! 预录时间,单位是s,0表示不预录*/
		BYTE		byRedundancyEn;					/*! 录像冗余开关*/
		BYTE		byReserved[2];
	} DHDEV_RECORD_CFG, *LPRV_RECORD_CFG;

	/************************************************************************
	*	报警配置
	************************************************************************/

	typedef struct 
	{
		int iType;
		int iValue;
	} RV_PTZ_LINK, LPRV_PTZ_LINK;

	typedef struct 
	{
		/******************************************************************************
		** 消息触发配置
		**
		** 当设备产生消息,譬如产生一个输入报警,或者视频丢失时,需要做相应的处理.
		** 此处定义如何处理一个消息
		*****************************************************************************/

		/* 消息处理方式,可以同时多种处理方式,包括
		* 0x00000001 - 网络:上传管理服务器
		* 0x00000002 - 录像:触发
		* 0x00000004 - 云台联动
		* 0x00000008 - 发送邮件
		* 0x00000010 - 设备本地报警轮巡
		* 0x00000020 - 设备提示使能
		* 0x00000040 - 设备报警输出使能
		* 0x00000080 - Ftp上传使能
		* 0x00000100 - 蜂鸣
		* 0x00000200 - 语音提示
		* 0x00000400 - 抓图使能*/

		/*当前报警所支持的处理方式，按位掩码表示*/
		DWORD		dwActionMask;

		/* 触发动作，按位掩码表示，具体动作所需要的参数在各自的配置中体现*/
		DWORD		dwActionFlag;

		/* 报警触发的输出通道,报警触发的输出,为 1 表示触发该输出  */ 
		BYTE		byRelAlarmOut[RV_MAX_ALARMOUT_NUM];
		DWORD		dwDuration;		/* 报警持续时间 */

		/*联动录象*/
		BYTE		byRecordChannel[RV_MAX_VIDEO_IN_NUM];		/* 报警触发的录象通道,为1表示触发该通道 */
		DWORD		dwRecLatch;		/* 录象持续时间 */

		/*抓图通道*/
		BYTE		bySnap[RV_MAX_VIDEO_IN_NUM];
		/*轮巡通道*/
		BYTE		byTour[RV_MAX_VIDEO_IN_NUM];

		/*  云台联动: */
		RV_PTZ_LINK	struPtzLink[RV_MAX_VIDEO_IN_NUM];
		DWORD		dwEventLatch;   /*!< 联动开始延时时间，s为单位 范围是0－－15 默认值是0*/
		/* 报警触发的无线输出通道,报警触发的输出,为 1 表示触发该输出  */ 
		BYTE		byRelWIAlarmOut[RV_MAX_ALARMOUT_NUM];
		BYTE		bMessageToNet;	//消息上传给网络使能
		BYTE		byReserved[111];       
	} RV_MSG_HANDLE;

	/* 外部报警 */
	typedef struct
	{
		BYTE		byAlarmType;						/* 报警器类型,0：常闭,1：常开  */
		BYTE		byAlarmEn;							/*报警使能*/
		BYTE		byReserved[2];						
		RV_TSECT	stSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE	struHandle;	/* 处理方式  */
	} RV_ALARMIN_CFG,*LPDHDEV_ALARMIN_CFG; 

	/* 动态检测报警 */
	typedef struct 
	{
		BYTE			byMotionEn;							/* 动态检测报警使能*/
		BYTE			byReserved;
		WORD			wSenseLevel;						/*灵敏度*/
		WORD			wMotionRow;							/* 动态检测区域的行数 */
		WORD			wMotionCol;							/* 动态检测区域的列数 */
		BYTE			byDetected[RV_MOTION_ROW][RV_MOTION_COL];	/*检测区域，最多32*32块区域*/
		RV_TSECT		stSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE	struHandle;	/*处理方式*/
	}RV_MOTION_DETECT_CFG;

	/* 视频丢失报警 */
	typedef struct
	{
		BYTE			byAlarmEn;							/*视频丢失报警使能*/
		BYTE			byReserved[3];
		RV_TSECT		stSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE	struHandle;	/* 处理方式  */
	} RV_VIDEO_LOST_CFG;

	/* 图像遮挡报警 */
	typedef struct
	{
		BYTE			byBlindEnable;						/* 使能 */
		BYTE			byBlindLevel;						/* 灵敏度1-6 */
		BYTE			byReserved[2];
		RV_TSECT	stSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE	struHandle;	/* 处理方式  */
	} RV_BLIND_CFG;

	/* 硬盘消息(内部报警) */
	typedef struct 
	{
		BYTE				byNoDiskEn;				/* 无硬盘时报警 */
		BYTE				byReserved_1[3];
		RV_TSECT			stNDSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE		struNDHandle;				/* 处理方式  */

		BYTE				byLowCapEn;				/* 硬盘低容量时报警*/
		BYTE				byLowerLimit;			/* 容量阀值 0-99*/
		BYTE				byReserved_2[2];
		RV_TSECT			stLCSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE		struLCHandle;			/* 处理方式  */

		BYTE				byDiskErrEn;			/* 硬盘故障报警*/
		BYTE				byReserved_3[3];
		RV_TSECT			stEDSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE		struEDHandle;	/* 处理方式  */

	} RV_DISK_ALARM_CFG;

	typedef struct
	{
		BYTE				byEnable;
		BYTE				byReserved[3];
		RV_MSG_HANDLE		struHandle;
	} RV_NETBROKEN_ALARM_CFG;

	//2010-3-18 luhj-0002 增加IP冲突报警结构体
	typedef struct  
	{
		BYTE				byEnable;
		BYTE				byReserved[3];
		RV_MSG_HANDLE		struHandle;
	}RV_IPCONFLICT_ALARM_CFG;
	//luhj-0002 end

	///*音频报警*/
	//typedef struct 
	//{
	//	BYTE		bySoundAlarmEn;			/*音频报警使能*/
	//	BYTE		byVol;					/*报警音量阀值*/
	//	BYTE		byReserved[2];			/*保留字节*/
	//	RV_TSECT	stSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
	//	RV_MSG_HANDLE	struHandle;	/* 处理方式  */
	//}RV_SOUND_ALARM_CFG;


	/* 报警布防 */
	typedef struct
	{
		DWORD dwSize;

		RV_ALARMIN_CFG 		struLocalAlmIn[RV_MAX_ALARM_IN_NUM];
		RV_ALARMIN_CFG		struNetAlmIn[RV_MAX_ALARM_IN_NUM];
		RV_MOTION_DETECT_CFG struMotion[RV_MAX_VIDEO_IN_NUM];
		RV_VIDEO_LOST_CFG	struVideoLost[RV_MAX_VIDEO_IN_NUM];
		RV_BLIND_CFG		struBlind[RV_MAX_VIDEO_IN_NUM];
		RV_DISK_ALARM_CFG	struDiskAlarm;
		RV_NETBROKEN_ALARM_CFG	struNetBrokenAlarm;
		//	RV_SOUND_ALARM_CFG	struSoundAlarm[RV_MAX_VIDEO_IN_NUM];
		//可能还有设备异常
	} DHDEV_ALARM_SCHEDULE;
	
	//2010-3-18 luhj-008
	typedef struct  
	{
		DWORD dwSize;
		RV_IPCONFLICT_ALARM_CFG	struIPConflict;
	} DHDEV_ALARM_IPCONFLICT;
	//luhj-0008 end

	//2009-12-14 刘阳 修改 start
	/* 智能DVR SAVIA报警 */
	typedef struct
	{
		long			iIntelEnable;						/* 使能 */
		unsigned long	iIntelIPaddr;					/* ip地址 */
		BYTE			byReserved[4];
		RV_TSECT		stSect[RV_N_WEEKS][RV_N_REC_TSECT];		//NSP
		RV_MSG_HANDLE	struHandle;	/* 处理方式  */
	} RV_SAVIA_ALARM;
	typedef struct
	{
		long			iSaviaNum;							//Savia数目
		RV_SAVIA_ALARM	stSaviaAlarm[RV_MAX_SAVIA_NUM];		//Savia报警设置
	} RV_SAVIA_ALARM_SCHEDULE;
	//2009-12-14 刘阳 修改 end

	//2009-12-28 刘阳 修改 start
	/* 黑白名单 */
	typedef struct
	{
		DWORD			dwSize;								//结构体大小
		BOOL			bEnable;							//IP权限设置使能
		LONG			iBlackCount;						//黑名单数目
		DWORD			dwBlackIP[RV_MAX_BLACK_IP_NUM];		//黑名单IP
		LONG			iWhiteCount;						//白名单数目
		DWORD			dwWhiteIP[RV_MAX_WHITE_IP_NUM];		//白名单IP
	} RV_BLACKWHITE_CFG;
	//2009-12-28 刘阳 修改 end

	//2010-1-21 刘阳 修改 start
	/* 解码器配置 */
	typedef struct
	{
		int				channel;		//!本地解码通道号, 从0开始
		unsigned short	device_type;	//!设备类型, 0-威乾设备 ,4-VS300接入,3-RTSP
		unsigned short	device_port;	//连接前端设备端口
		unsigned int	device_ip;		//网络字节序储存, 连接的前端设备ip
		unsigned int	device_channel;	//!远程通道号
		unsigned int	enable;			//!使能开关
		unsigned int	stream_type;	//!连接码流类型,0-主码流
		char			username[128];	//!连接前端用户名
		char			password[128];	//!连接密码
		//
		unsigned char	ucAVEnable;       //0,只解视频 1，只解音频 ，2 解音视频 ，不允许什么都不解
		unsigned char	ucRemotePtzPreset;//远程预置点
		unsigned char	ucRemotePtzPresetEnable;//是否一连接上就设置远程预置点   
		unsigned char	ucRes;
		unsigned char	ucUrl[256];       //rtsp模式下此配置有效,格式如：rtsp://ip:port/user=xxx&password=xxx&id=xx&type=xx
		int				iDecodePolicy;    //解码策略 	(-2, 最实时 -1 实时 0 默认情况 1 流畅 2,	最流畅 ) 以后可能扩展到自定义具体的延时时间
		unsigned char	ucRes1[124];
	}RV_CFG_DECODE_T;
	typedef struct
	{
		DWORD			dwSize;
		long			iDecoderNumber;
		RV_CFG_DECODE_T	struDecoder[RV_MAX_DECODER_CHAN_NUM];
	}RV_DECODER_CFG;
	//2010-1-21 刘阳 修改 end

	/************************************************************************
	*	网络配置
	************************************************************************/
	/*                                                                      */
	/*以太网配置*/
	typedef struct 
	{
		char	sDevIPAddr[RV_MAX_IPADDR_LEN];			/* DVR IP 地址  */ 
		char	sDevIPMask[RV_MAX_IPADDR_LEN];			/* DVR IP 地址掩码  */ 
		char	sGatewayIP[RV_MAX_IPADDR_LEN];			/* 网关地址  */ 

		/* 10M/100M  自适应,索引 
		* 1-10MBase - T
		* 2-10MBase-T 全双工 
		* 3-100MBase - TX
		* 4-100M 全双工
		* 5-10M/100M  自适应 
		*/
		//为了扩展将DWORD拆成四个
		BYTE	dwNetInterface;							/* NSP */
		BYTE	bTranMedia;								/* 0：有线 1：无线 */
		BYTE	bValid;									/* 按位表示，第一位：1：有效 0：无效；第二位：0：DHCP关闭 1：DHCP使能；第三位：0：不支持DHCP 1：支持DHCP */
		BYTE	bDefaultEth;							/* 是否作为默认的网卡 1：默认 0：非默认 */
		char	byMACAddr[RV_MACADDR_LEN];				/* MAC地址，只读 */
	} RV_ETHERNET; 

	/* 远程主机配置 */
	typedef struct 
	{
		BYTE	byEnable;								/* 连接使能  */
		BYTE	byReserved;
		WORD	wHostPort;								/* 远程主机 端口  */
		char	sHostIPAddr[RV_MAX_IPADDR_LEN];			/* 远程主机 IP 地址  */ 		
		char	sHostUser[RV_MAX_HOST_NAMELEN];			/* 远程主机 用户名 */ 
		char	sHostPassword[RV_MAX_HOST_PSWLEN];		/* 远程主机 密码 */ 
	}RV_REMOTE_HOST;

	/* 远程主机配置(密码64字节) */
	typedef struct 
	{
		BYTE	byEnable;								/* 连接使能  */
		BYTE	byReserved;
		WORD	wHostPort;								/* 远程主机 端口  */
		char	sHostIPAddr[RV_MAX_IPADDR_LEN];			/* 远程主机 IP 地址  */ 		
		char	sHostUser[RV_MAX_HOST_NAMELEN];			/* 远程主机 用户名 */ 
		char	sHostPassword[RV_MAX_HOST_PSWLEN_EX];		/* 远程主机 密码 */ 
	}RV_REMOTE_HOST_EX;

	/* 邮件配置 */
	typedef struct 
	{
		char	sMailIPAddr[RV_MAX_IPADDR_LEN];			/*邮件服务器IP地址*/
		WORD	wMailPort;								/*邮件服务器端口*/
		WORD	wReserved;								/* 保留 */
		char	sSenderAddr[RV_MAX_MAIL_ADDR_LEN];		/*发送地址*/
		char	sUserName[RV_MAX_NAME_LEN];			/* 用户名 */
		char	sUserPsw[RV_MAX_NAME_LEN];				/* 用户密码 */
		char	sDestAddr[RV_MAX_MAIL_ADDR_LEN];			/*目的地址*/
		char	sCcAddr[RV_MAX_MAIL_ADDR_LEN];			/*抄送地址*/
		char	sBccAddr[RV_MAX_MAIL_ADDR_LEN];			/*暗抄地址*/
		char	sSubject[RV_MAX_MAIL_SUBJECT_LEN];		/*标题*/
	}RV_MAIL_CFG;

	/*网络配置结构体*/
	typedef struct
	{ 
		DWORD				dwSize; 

		char				sDevName[RV_MAX_NAME_LEN];				/* 设备主机名 */

		WORD				wTcpMaxConnectNum;						/* TCP最大连接数(一般指视频数据请求数) */
		WORD				wTcpPort;								/* TCP帧听端口 */
		WORD				wUdpPort;								/* UDP侦听端口 */
		WORD				wHttpPort;								/* HTTP端口号 */
		WORD				wHttpsPort;							/* HTTPS端口号 */
		WORD				wSslPort;								/* SSL端口号*/
		RV_ETHERNET			stEtherNet[RV_MAX_ETHERNET_NUM];/* 以太网口  */ 

		RV_REMOTE_HOST		struAlarmHost;		/* 报警服务器 */
		RV_REMOTE_HOST		struLogHost;		/* 日志服务器 */
		RV_REMOTE_HOST		struSmtpHost;		/* SMTP服务器 */
		RV_REMOTE_HOST		struMultiCast;		/* 多播组 */
		RV_REMOTE_HOST		struNfs;			/* NFS服务器 */
		RV_REMOTE_HOST		struPppoe;			/* PPPoE服务器 */
		char				sPppoeIP[RV_MAX_IPADDR_LEN]; /* PPPoE注册返回的IP */
		RV_REMOTE_HOST		struDdns;			/* DDNS服务器 */
		char				sDdnsHostName[RV_MAX_HOST_NAMELEN];		/* DDNS主机名*/
		RV_REMOTE_HOST		struDns;			/* DNS服务器*/
		RV_MAIL_CFG			struMail;			/* 邮件配置 */
	} DHDEV_NET_CFG;

	/*网络配置结构体*/
	typedef struct
	{ 
		DWORD				dwSize; 
		
		char				sDevName[RV_MAX_NAME_LEN];				/* 设备主机名 */
		
		WORD				wTcpMaxConnectNum;						/* TCP最大连接数(一般指视频数据请求数) */
		WORD				wTcpPort;								/* TCP帧听端口 */
		WORD				wUdpPort;								/* UDP侦听端口 */
		WORD				wHttpPort;								/* HTTP端口号 */
		WORD				wHttpsPort;							/* HTTPS端口号 */
		WORD				wSslPort;								/* SSL端口号*/
		RV_ETHERNET			stEtherNet[RV_MAX_ETHERNET_NUM];/* 以太网口  */ 
		
		RV_REMOTE_HOST_EX		struAlarmHost;		/* 报警服务器 */
		RV_REMOTE_HOST_EX		struLogHost;		/* 日志服务器 */
		RV_REMOTE_HOST_EX		struSmtpHost;		/* SMTP服务器 */
		RV_REMOTE_HOST_EX		struMultiCast;		/* 多播组 */
		RV_REMOTE_HOST_EX		struNfs;			/* NFS服务器 */
		RV_REMOTE_HOST_EX		struPppoe;			/* PPPoE服务器 */
		char				sPppoeIP[RV_MAX_IPADDR_LEN]; /* PPPoE注册返回的IP */
		RV_REMOTE_HOST_EX		struDdns;			/* DDNS服务器 */
		char				sDdnsHostName[RV_MAX_HOST_NAMELEN];		/* DDNS主机名*/
		RV_REMOTE_HOST_EX		struDns;			/* DNS服务器*/
		RV_MAIL_CFG			struMail;			/* 邮件配置 */
	} DHDEV_NET_CFG_RHEX;

	/************************************************************************
	*	串口配置
	************************************************************************/

	/* 串口基本属性 */
	typedef struct
	{
		BYTE	byDataBit;		//数据位 0-5,1-6,2-7,3-8
		BYTE	byStopBit;		//停止位 0-1位, 1-1.5位, 2-2位 
		BYTE	byParity;		//校验位 0-no, 1-odd, 2-even 
		BYTE	byBaudRate;	//{0-300,1-600,2-1200,3-2400,4-4800
		// 5-9600,6-19200,7-38400,8-57600,9-115200} 
	} RV_COMM_PROP;

	/* 485解码器配置 */
	typedef struct
	{ 
		RV_COMM_PROP	struComm;

		BYTE			wProtocol;			/*!< 协议类型 保存协议的下标，动态变化 */
		BYTE			byReserved;			/*!保留  */	
		BYTE			wDecoderAddress;	/*解码器地址:0 - 255*/ 
		BYTE 			byMartixID;			/*矩阵号*/
	} RV_485_CFG; 

	/* 232串口配置 */
	typedef struct
	{
		RV_COMM_PROP	struComm;
		BYTE			byFunction;/*!< 串口功能，对应串口配置取到的功能名列表 */
		BYTE			byReserved[3];
	} RV_RS232_CFG;

	/* 串口配置结构体 */
	typedef struct
	{
		DWORD			dwSize;
		//解码器协议
		DWORD			dwDecProListNum;				//协议个数
		char			DecProName[RV_MAX_DECPRO_LIST_SIZE][RV_MAX_NAME_LEN]; //协议名列表
		RV_485_CFG		stDecoder[RV_MAX_DECODER_NUM];		//各解码器当前属性

		DWORD			dw232FuncNameNum;				//232功能个数
		char			s232FuncName[RV_MAX_232FUNCS][RV_MAX_NAME_LEN];	//功能名列表
		RV_RS232_CFG	st232[RV_MAX_232_NUM];		//各232串口当前属性
	} DHDEV_COMM_CFG;


	/************************************************************************
	*	自动维护配置
	***********************************************************************/
	/* 自动维护属性 */
	typedef struct
	{
		DWORD dwSize;

		//自动重启
		BYTE byAutoRebootDay; // 0-从不, 1-每天, 2-每星期日, 3-每星期一,.....
		BYTE byAutoRebootTime;  // 0-0:00 1-1:00,........23-:23:00 */
		//自动删除文件
		BYTE byAutoDeleteFilesTime; /*! 0-从不, 1-24H, 2-48H, 3-72H, 4-96H, 5-ONE WEEK, 6-ONE MONTH */

		BYTE reserved[13]; // 保留位
	} DHDEV_AUTOMT_CFG;

	/************************************************************************
	* 本机矩阵控制策略配置
	************************************************************************/

	typedef struct
	{
		BOOL		bTourEnable;			//	轮巡使能
		int			nTourPeriod;			//	轮巡间隔，单位秒, 5-300 
		DWORD		dwChannelMask;			//	轮巡的通道，掩码形式表示
		char		reserved[64];
	}RV_VIDEOGROUP_CFG;

	typedef struct
	{
		DWORD	dwSize;
		int		nMatrixNum;			//	矩阵个数(注：不允许修改)
		RV_VIDEOGROUP_CFG struVideoGroup[RV_MATRIX_MAXOUT];
		char	reserved[32];
	}DHDEV_VIDEO_MATRIX_CFG;   

	/************************************************************************
	*	多ddns配置结构体
	************************************************************************/

	typedef struct
	{
		DWORD	dwId;				//ddns服务器id号
		BOOL	bEnable;			//使能，同一时间只能有一个ddns服务器处于使能状态
		char	szServerType[RV_MAX_SERVER_TYPE_LEN];	//服务器类型，希网..
		char	szServerIp[RV_MAX_DOMAIN_NAME_LEN];		//服务器ip或者域名
		DWORD	dwServerPort;						//服务器端口
		char	szDomainName[RV_MAX_DOMAIN_NAME_LEN];	//dvr域名，如jeckean.3322.org
		char	szUserName[RV_MAX_HOST_NAMELEN];	//用户名
		char	szUserPsw[RV_MAX_HOST_PSWLEN];		//密码
		char	szAlias[RV_MAX_DDNS_ALIAS_LEN];		//服务器别名，如"RV inter ddns"
		DWORD	dwAlivePeriod;						//DDNS 保活时间
		char	reserved[256];
	}RV_DDNS_SERVER_CFG;

	typedef struct
	{
		DWORD	dwSize;
		DWORD	dwDdnsServerNum;	
		RV_DDNS_SERVER_CFG struDdnsServer[RV_MAX_DDNS_NUM];	
	}DHDEV_MULTI_DDNS_CFG;


	/************************************************************************
	*   邮件配置结构体
	************************************************************************/
	typedef struct 
	{
		char	sMailIPAddr[RV_MAX_DOMAIN_NAME_LEN];	/*邮件服务器地址(IP或者域名)*/
		char	sSubMailIPAddr[RV_MAX_DOMAIN_NAME_LEN];
		WORD	wMailPort;								/*邮件服务器端口*/
		WORD	wSubMailPort;
		WORD	wReserved;								/*保留*/
		char	sSenderAddr[RV_MAX_MAIL_ADDR_LEN];		/*发送地址*/
		char	sUserName[RV_MAX_NAME_LEN];				/*用户名*/
		char	sUserPsw[RV_MAX_NAME_LEN];				/*用户密码*/
		char	sDestAddr[RV_MAX_MAIL_ADDR_LEN];		/*目的地址*/
		char	sCcAddr[RV_MAX_MAIL_ADDR_LEN];			/*抄送地址*/
		char	sBccAddr[RV_MAX_MAIL_ADDR_LEN];			/*暗抄地址*/
		char	sSubject[RV_MAX_MAIL_SUBJECT_LEN];		/*标题*/
		BYTE	bEnable;//使能0:false,	1:true
		char	reserved[255];
	} DHDEV_MAIL_CFG;

	/************************************************************************
	*   邮件配置结构体扩展
	************************************************************************/
	typedef struct 
	{
		char	sMailIPAddr[RV_MAX_DOMAIN_NAME_LEN];	/*邮件服务器地址(IP或者域名)*/
		char	sSubMailIPAddr[RV_MAX_DOMAIN_NAME_LEN];
		WORD	wMailPort;								/*邮件服务器端口*/
		WORD	wSubMailPort;
		WORD	wReserved;								/*保留*/
		char	sSenderAddr[RV_MAX_MAIL_ADDR_LEN];		/*发送地址*/
		char	sUserName[RV_MAX_NAME_LEN_EX];			/*用户名	luhj-0015 从16修改至32*/
		char	sUserPsw[RV_MAX_NAME_LEN];				/*用户密码*/
		char	sDestAddr[RV_MAX_MAIL_ADDR_LEN];		/*目的地址*/
		char	sCcAddr[RV_MAX_MAIL_ADDR_LEN];			/*抄送地址*/
		char	sBccAddr[RV_MAX_MAIL_ADDR_LEN];			/*暗抄地址*/
		char	sSubject[RV_MAX_MAIL_SUBJECT_LEN];		/*标题*/
		BYTE	bEnable;//使能0:false,	1:true
		char	reserved[255];
	} DHDEV_MAIL_CFG_EX;

	/************************************************************************
	*   设备软件版本信息
	************************************************************************/
	typedef struct  
	{
		char	szDevSerialNo[RV_DEV_SERIALNO_LEN];		//序列号
		char	byDevType;								//设备类型，见枚举DHDEV_DEVICE_TYPE
		char	szDevType[RV_DEV_TYPE_LEN];				//设备详细型号，字符串格式，可能为空
		int		nProtocalVer;							//协议版本号
		char	szSoftWareVersion[RV_MAX_URL_LEN];
		DWORD	dwSoftwareBuildDate;
		char	szDspSoftwareVersion[RV_MAX_URL_LEN];
		DWORD	dwDspSoftwareBuildDate;
		char	szPanelVersion[RV_MAX_URL_LEN];
		DWORD	dwPanelSoftwareBuildDate;
		char	szHardwareVersion[RV_MAX_URL_LEN];
		DWORD	dwHardwareDate;
		char	szWebVersion[RV_MAX_URL_LEN];
		DWORD	dwWebBuildDate;
		char	reserved[256];
	} DHDEV_VERSION_INFO;


	/************************************************************************
	*   DSP能力描述
	************************************************************************/
	typedef struct 
	{
		DWORD	dwVideoStandardMask;	//视频制式掩码，按位表示设备能够支持的视频制式
		DWORD	dwImageSizeMask;		//分辨率掩码，按位表示设备能够支持的分辨率设置
		DWORD	dwEncodeModeMask;		//编码模式掩码，按位表示设备能够支持的编码模式设置	

		DWORD	dwStreamCap;			//按位表示设备支持的多媒体功能，
		//第一位表示支持主码流
		//第二位表示支持辅码流1
		//第三位表示支持辅码流2
		//第五位表示支持jpg抓图
		DWORD	dwImageSizeMask_Assi[32];//表示主码流为各分辨率时，支持的辅码流分辨率掩码。

		DWORD	dwMaxEncodePower;		//- DSP 支持的最高编码能力 
		WORD	wMaxSupportChannel;		//- 每块 DSP 支持最多输入视频通道数 
		WORD	wChannelMaxSetSync;		//- DSP 每通道的最大编码设置是否同步 0-不同步, 1-同步

		BYTE	bMaxFrameOfImageSize[32];//不同分辨率下的最大采集帧率，与dwVideoStandardMask按位对应
		BYTE	bEncodeCap;				//0：主码流的编码能力+辅码流的编码能力 <= 设备的编码能力；
		//1：主码流的编码能力+辅码流的编码能力 <= 设备的编码能力，
		//辅码流的编码能力 <= 主码流的编码能力，
		//辅码流的分辨率 <= 主码流的分辨率，
		//主码流和辅码流的帧率 <= 前端视频采集帧率

		char	reserved[95];
	} DHDEV_DSP_ENCODECAP, *LPDHDEV_DSP_ENCODECAP;


	/************************************************************************
	*   抓图功能配置
	************************************************************************/
	typedef struct 
	{
		DWORD	dwSize;
		BOOL	bTimingEnable;		//定时抓图开关（报警抓图开关在各报警联动配置中体现）
		RV_VIDEOENC_OPT struSnapEnc[SNAP_TYP_NUM];	//抓图编码配置，现支持其中的分辨率、画质、帧率设置，帧率在这里是负数，表示一秒抓图的次数。
	}DHDEV_SNAP_CFG;

	/************************************************************************
	*   web路径配置
	************************************************************************/
	typedef struct 
	{
		DWORD	dwSize;
		BOOL	bSnapEnable;	//是否抓图
		int		iSnapInterval;	//抓图周期
		char	szHostIp[RV_MAX_IPADDR_LEN]; /* HTTP主机IP */
		WORD	wHostPort;
		int		iMsgInterval;	//状态消息发送间隔
		char	szUrlState[RV_MAX_URL_LEN];		//状态消息上传URL
		char	szUrlImage[RV_MAX_URL_LEN];		//图片上传Url
		char	szDevId[RV_MAX_DEV_ID_LEN];		//机器的web编号
		BYTE	byReserved[2];
	}DHDEV_URL_CFG;

	/************************************************************************
	*   图象水印配置
	************************************************************************/
	typedef struct __DHDEV_WATERMAKE_CFG 
	{
		DWORD				dwSize;
		int					nEnable;			//	使能
		int					nStream;			//	码流（1～n）0-所有码流
		int					nKey;				//	数据类型(1-文字，2-图片)
		char				szLetterData[RV_MAX_WATERMAKE_LETTER];	//	文字
		char				szData[RV_MAX_WATERMAKE_DATA];		//	图片数据
		BYTE				bReserved[512];		//	保留
	} DHDEV_WATERMAKE_CFG;

	/************************************************************************
	*   FTP上传配置
	************************************************************************/
	typedef struct
	{
		struct
		{
			RV_TSECT	struSect;	//该时间段内的“使能”无效，可忽略
			BOOL		bMdEn;		//上传动态检测录象
			BOOL		bAlarmEn;	//上传外部报警录象
			BOOL		bTimerEn;	//上传普通定时录像
			DWORD		dwRev[4];
		} struPeriod[RV_TIME_SECTION];
	}RV_FTP_UPLOAD_CFG;

	typedef struct
	{
		DWORD				dwSize;
		BOOL				bEnable;							//是否启用
		char				szHostIp[RV_MAX_IPADDR_LEN];		//主机IP
		WORD				wHostPort;							//主机端口
		char				szDirName[RV_FTP_MAX_PATH];			//FTP目录路径
		char				szUserName[RV_FTP_USERNAME_LEN];	//用户名
		char				szPassword[RV_FTP_PASSWORD_LEN];	//密码
		int					iFileLen;							//文件长度
		int					iInterval;							//相邻文件时间间隔
		RV_FTP_UPLOAD_CFG	struUploadCfg[RV_MAX_CHANNUM][RV_N_WEEKS];
		char 				protocol;							//0-FTP 1-SMB
		char				NASVer;								//网络存储服务器版本0=老的FTP（界面上显示时间段）,1=NAS存储（界面上屏蔽时间段）
		BYTE 				reserved[128];
	}DHDEV_FTP_PROTO_CFG;

	// DNS服务器配置
	typedef struct  
	{
		char				szPrimaryIp[RV_MAX_IPADDR_LEN];
		char				szSecondaryIp[RV_MAX_IPADDR_LEN];
		char				reserved[256];
	}DHDEV_DNS_CFG;


	/************************************************************************
	*   录象下载策略配置
	************************************************************************/
	typedef struct
	{
		DWORD dwSize;

		BOOL bEnable;	//TRUE－高速下载，FALSE-普通下载
	}DHDEV_DOWNLOAD_STRATEGY_CFG;

	/************************************************************************
	*   网络传输策略配置
	************************************************************************/
	typedef struct
	{
		DWORD dwSize;

		BOOL bEnable;
		int  iStrategy;	//0-画质优先， 1-流畅性优先， 2-自动。
	}DHDEV_TRANSFER_STRATEGY_CFG;


	/************************************************************************
	*	IPC的一些配置
	************************************************************************/
	//	配置无线网络信息
	typedef struct 
	{
		int					nEnable;			// 无线使能
		char				szSSID[36];			// SSID
		int					nLinkMode;			// 连接模式 0:auto 1:adhoc 2:Infrastructure
		int					nEncryption;		// 加密 0: off 2:WEP64bit 3: WEP128bit
		int					nKeyType;			// 0:Hex 1:ASCII
		int					nKeyID;				// 序号
		char				szKeys[4][32];		// 四组密码
		int					nKeyFlag;
		char				reserved[12];
	} DHDEV_WLAN_INFO;

	//	选择使用某个无线设备
	typedef struct  
	{
		char				szSSID[36];
		int					nLinkMode;			// 连接模式 0:adhoc 1:Infrastructure
		int 				nEncryption;		// 加密 0: off  2:WEP64bit 3: WEP128bit
		char				reserved[48];
	} DHDEV_WLAN_DEVICE;

	//	搜索到的无线设备列表
	typedef struct  
	{
		DWORD				dwSize;
		BYTE				bWlanDevCount;		// 搜索到的无线设备个数
		DHDEV_WLAN_DEVICE	lstWlanDev[RV_MAX_WLANDEVICE_NUM];
		char				reserved[255];
	} DHDEV_WLAN_DEVICE_LIST;

	//	主动注册参数配置
	typedef struct  
	{
		char				szServerIp[32];		//注册服务器IP
		int					nServerPort;		//端口号
		char				reserved[64];
	} DHDEV_SERVER_INFO;

	typedef struct  
	{
		DWORD				dwSize;
		BYTE				bServerNum;         //支持的最大ip数
		DHDEV_SERVER_INFO	lstServer[RV_MAX_REGISTER_SERVER_NUM];
		BYTE				bEnable;			//使能
		char				szDeviceID[32];		//设备id。
		char				reserved[94];
	} DHDEV_REGISTER_SERVER;

	//	摄像头属性
	typedef struct __DHDEV_CAMERA_INFO
	{
		BYTE				bBrightnessEn;		// 亮度可调 1：可；0：不可
		BYTE				bContrastEn;		// 对比度可调
		BYTE				bColorEn;			// 色度可调
		BYTE				bGainEn;			// 增益可调
		BYTE				bSaturationEn;		// 饱和度可调
		BYTE				bBacklightEn;		// 背光补偿可调
		BYTE				bExposureEn;		// 曝光选择可调
		BYTE				bColorConvEn;		// 自动彩黑转换可调
		BYTE				bAttrEn;			// 属性选项 1：可；0：不可
		BYTE				bMirrorEn;			// 镜像 1：支持；0：不支持
		BYTE				bFlipEn;			// 翻转 1：支持；0：不支持
		BYTE				bRev[125];			// 保留
	} DHDEV_CAMERA_INFO;

	//	摄像头属性配置
	typedef struct __DHDEV_CAMERA_CFG 
	{
		DWORD				dwSize;
		BYTE				bExposure;			//曝光模式 1-9:手动曝光等级; 0:自动曝光
		BYTE				bBacklight;			//背光补偿 3:强；2：中；1：弱； 0:关
		BYTE				bAutoColor2BW;		//日/夜模式 2:开；1：自动; 0:关
		BYTE				bMirror;			//镜像  1 开， 0关
		BYTE				bFlip;				//翻转  1 开， 0关
		char				bRev[123];			//保留
	} DHDEV_CAMERA_CFG;

#define ALARM_MAX_NAME 64
	//(无线)红外报警配置
	typedef struct
	{
		BOOL				bEnable;							//报警输入使能
		char				szAlarmName[RV_MAX_ALARM_NAME];		//报警输入名称
		int					nAlarmInPattern;					//报警器输入波形
		int					nAlarmOutPattern;					//报警输出波形
		char				szAlarmInAddress[RV_MAX_ALARM_NAME];//报警输入地址
		int					nSensorType;						//外部设备传感器类型常开 or 常闭
		int					nDefendEfectTime;					//布撤防延时时间，在此时间后该报警输入有效
		int					nDefendAreaType;					//防区类型 
		int					nAlarmSmoothTime;					//报警平滑时间，即在此时间内如果只有一个报警输入连续输入两次则忽略掉后面一次
		char				reserved[128];

		RV_TSECT			stSect[RV_N_WEEKS][RV_N_REC_TSECT];
		RV_MSG_HANDLE		struHandle;	/* 处理方式  */
	} RV_INFRARED_INFO;

	//无线遥控器配置
	typedef struct 
	{
		BYTE  address[ALARM_MAX_NAME];							// 遥控器地址
		BYTE  name[ALARM_MAX_NAME];								// 遥控器名称
		BYTE  reserved[32];										// 保留字段
	}RV_WI_CONFIG_ROBOT;

	//无线报警输出配置
	typedef struct 
	{
		BYTE  address[ALARM_MAX_NAME];							//报警输出地址
		BYTE  name[ALARM_MAX_NAME];								//报警输出名称
		BYTE  reserved[32];										//保留字段
	}RV_WI_CONFIG_ALARM_OUT;

	typedef struct  
	{
		DWORD				dwSize;
		BYTE				bAlarmInNum;						//无线报警输入数
		BYTE				bAlarmOutNum;						//无线报警输出数
		RV_WI_CONFIG_ALARM_OUT AlarmOutAddr[16];				//报警输出地址
		BYTE				bRobotNum;							//遥控器个数
		RV_WI_CONFIG_ROBOT RobotAddr[16];						//遥控器地址
		RV_INFRARED_INFO	InfraredAlarm[16];
		char				reserved[256];
	} RV_INFRARED_CFG;

	//新音频检测报警信息
	typedef struct {
		int channel;						//报警通道号
		int alarmType;						//报警类型0--音频值过低 1---音频值过高
		unsigned int volume;				//音量值
		char 	reserved[256];
	} NET_NEW_SOUND_ALARM_STATE;

	typedef struct  
	{
		int channelcount;                   //报警的通道个数
		NET_NEW_SOUND_ALARM_STATE  SoundAlarmInfo[RV_MAX_ALARM_IN_NUM];
	}RV_NEW_SOUND_ALARM_STATE;

	//新音频检测报警配置
	typedef struct
	{
		BOOL				bEnable;							//报警输入使能
		int					Volume_min;							//音量的最小值
		int					Volume_max;							//音量的最大值
		char				reserved[128];	
		RV_TSECT			stSect[RV_N_WEEKS][RV_N_REC_TSECT];	
		RV_MSG_HANDLE		struHandle;	/* 处理方式  */
	} RV_AUDIO_DETECT_INFO;

	typedef struct  
	{
		DWORD					dwSize;
		int						AlarmNum;
		RV_AUDIO_DETECT_INFO	AudioDetectAlarm[RV_MAX_ALARM_IN_NUM];
		char					reserved[256];
	} RV_AUDIO_DETECT_CFG;

	//存储位置设置结构体，每通道独立设置,每通道可以选择各种存储类型, 目前包括本地, 可移动, 远程存储.
	typedef struct 
	{
		DWORD 	dwSize;
		DWORD 	dwLocalMask;      //本地存储掩码  按位表示：
		//第一位	系统预录
		//第二位	定时录像
		//第三位	动检录像
		//第四位	报警录像
		//第五位	卡号录像
		//第六位	手动录像

		DWORD 	dwMobileMask;				//可移动存储掩码 存储掩码如本地存储掩码
		int  	RemoteType;					//远程存储类型 0: Ftp  1: Smb 
		DWORD 	dwRemoteMask;				//远程存储掩码 存储掩码如本地存储掩码
		DWORD 	dwRemoteSecondSelLocal;		//远程异常时本地存储掩码
		DWORD 	dwRemoteSecondSelMobile;	//远程异常时可移动存储掩码
		char 	SubRemotePath[MAX_PATH_STOR];  //远程目录, 其中长度为260
		char	reserved[128];
	}RV_STORAGE_STATION_CFG;

	/************************************************************************
	*   网络抓包配置
	************************************************************************/
	typedef struct
	{
		int					Offset;			//标志位的位偏移
		int					Length;			//标志位的长度
		char				Key[16];		//标志位的值
	} RV_SNIFFER_FRAMEID;

	typedef struct 
	{
		int					Offset;			//标志位的位偏移
		int					Offset2;		//目前没有应用
		int					Length;			//标志位的长度
		int					Length2;		//目前没有应用
		char				KeyTitle[24];	//标题的值
	} RV_SNIFFER_CONTENT;

	typedef struct 
	{
		RV_SNIFFER_FRAMEID	snifferFrameId;								//每个FRAME ID 选项
		RV_SNIFFER_CONTENT	snifferContent[RV_SNIFFER_CONTENT_NUM];		//每个FRAME对应的4个抓包内容
	} RV_SNIFFER_FRAME;

	//每组抓包对应的配置结构
	typedef struct
	{
		char				SnifferSrcIP[RV_MAX_IPADDR_LEN];			//抓包源地址		
		int					SnifferSrcPort;								//抓包源端口
		char				SnifferDestIP[RV_MAX_IPADDR_LEN]; 			//抓包目标地址
		int					SnifferDestPort;							//抓包目标端口
		char				reserved[28];								//保留字段
		RV_SNIFFER_FRAME	snifferFrame[RV_SNIFFER_FRAMEID_NUM];		//6个FRAME 选项
		int					displayPosition;							//显示位置
		int					recdChannelMask;							//通道掩码
	} RV_ATM_SNIFFER_CFG;

	typedef struct  
	{
		DWORD dwSize;
		RV_ATM_SNIFFER_CFG SnifferConfig[4];
		char	reserved[256];								//保留字段
	}DHDEV_SNIFFER_CFG;


	/************************************************************************
	*   OEM查询
	************************************************************************/
	typedef struct  
	{
		char				szVendor[RV_MAX_STRING_LEN];
		char				szType[RV_MAX_STRING_LEN];
		char				reserved[128];
	}DHDEV_OEM_INFO;


	/************************************************************************
	*   NTP配置
	************************************************************************/

	typedef enum __RV_TIME_ZONE_TYPE
	{
		RV_TIME_ZONE_0,				// {0, 0*3600,"GMT+00:00"}
		RV_TIME_ZONE_1,				// {1, 1*3600,"GMT+01:00"}
		RV_TIME_ZONE_2,				// {2, 2*3600,"GMT+02:00"}
		RV_TIME_ZONE_3,				// {3, 3*3600,"GMT+03:00"}
		RV_TIME_ZONE_4,				// {4, 3*3600+1800,"GMT+03:30"}
		RV_TIME_ZONE_5,				// {5, 4*3600,"GMT+04:00"}
		RV_TIME_ZONE_6,				// {6, 4*3600+1800,"GMT+04:30"}
		RV_TIME_ZONE_7,				// {7, 5*3600,"GMT+05:00"}
		RV_TIME_ZONE_8,				// {8, 5*3600+1800,"GMT+05:30"}
		RV_TIME_ZONE_9,				// {9, 5*3600+1800+900,"GMT+05:45"}
		RV_TIME_ZONE_10,			// {10, 6*3600,"GMT+06:00"}
		RV_TIME_ZONE_11,			// {11, 6*3600+1800,"GMT+06:30"}
		RV_TIME_ZONE_12,			// {12, 7*3600,"GMT+07:00"}
		RV_TIME_ZONE_13,			// {13, 8*3600,"GMT+08:00"}
		RV_TIME_ZONE_14,			// {14, 9*3600,"GMT+09:00"}
		RV_TIME_ZONE_15,			// {15, 9*3600+1800,"GMT+09:30"}
		RV_TIME_ZONE_16,			// {16, 10*3600,"GMT+10:00"}
		RV_TIME_ZONE_17,			// {17, 11*3600,"GMT+11:00"}
		RV_TIME_ZONE_18,			// {18, 12*3600,"GMT+12:00"}
		RV_TIME_ZONE_19,			// {19, 13*3600,"GMT+13:00"}
		RV_TIME_ZONE_20,			// {20, -1*3600,"GMT-01:00"}
		RV_TIME_ZONE_21,			// {21, -2*3600,"GMT-02:00"}
		RV_TIME_ZONE_22,			// {22, -3*3600,"GMT-03:00"}
		RV_TIME_ZONE_23,			// {23, -3*3600-1800,"GMT-03:30"}
		RV_TIME_ZONE_24,			// {24, -4*3600,"GMT-04:00"}
		RV_TIME_ZONE_25,			// {25, -5*3600,"GMT-05:00"}
		RV_TIME_ZONE_26,			// {26, -6*3600,"GMT-06:00"}
		RV_TIME_ZONE_27,			// {27, -7*3600,"GMT-07:00"}
		RV_TIME_ZONE_28,			// {28, -8*3600,"GMT-08:00"}
		RV_TIME_ZONE_29,			// {29, -9*3600,"GMT-09:00"}
		RV_TIME_ZONE_30,			// {30, -10*3600,"GMT-10:00"}
		RV_TIME_ZONE_31,			// {31, -11*3600,"GMT-11:00"}
		RV_TIME_ZONE_32,			// {32, -12*3600,"GMT-12:00"}
	}RV_TIME_ZONE_TYPE;

	typedef struct  
	{
		BOOL				bEnable;			//	是否启用
		int					nHostPort;			//	NTP服务器默认端口为123
		char				szHostIp[32];		//	主机IP
		char				szDomainName[128];	//	域名
		int					nType;				//	不可设置 0：表示IP，1：表示域名，2：表示IP和域名
		int					nUpdateInterval;	//	更新时间(分钟)
		int					nTimeZone;			//	见RV_TIME_ZONE_TYPE
		char				reserved[128];
	}DHDEV_NTP_CFG;


	/************************************************************************
	*   平台接入配置 － U网通
	************************************************************************/
	typedef struct
	{
		BOOL bChnEn;
		char szChnId[RV_INTERVIDEO_UCOM_CHANID];
	}RV_INTERVIDEO_UCOM_CHN_CFG;

	typedef struct
	{
		DWORD dwSize;
		BOOL bFuncEnable;		// 接入功能使能与否 0 - 使能
		BOOL bAliveEnable;		// 心跳使能与否
		DWORD dwAlivePeriod;	// 心跳周期，单位秒，0-3600
		char szServerIp[RV_MAX_IPADDR_LEN];		// CMS的IP
		WORD wServerPort;						// CMS的Port
		char szRegPwd[RV_INTERVIDEO_UCOM_REGPSW]; //注册密码
		char szDeviceId[RV_INTERVIDEO_UCOM_DEVID];	//设备id
		char szUserName[RV_INTERVIDEO_UCOM_USERNAME];
		char szPassWord[RV_INTERVIDEO_UCOM_USERPSW];
		RV_INTERVIDEO_UCOM_CHN_CFG  struChnInfo[RV_MAX_CHANNUM];//通道id,en
	}DHDEV_INTERVIDEO_UCOM_CFG;

	/************************************************************************
	*   平台接入配置 － 阿尔卡特
	************************************************************************/
	typedef struct
	{
		DWORD dwSize;
		unsigned short usCompanyID[2];  /* 公司ID,数值,不同的第三方服务公司,考虑4字节对齐,目前只用数组第一个 */
		char szDeviceNO[32];  /* 前端设备序列号，字符串, 包括'\0'结束符共32byte */
		char szVSName[32];  /* 前端设备名称，字符串, 包括'\0'结束符共16byte */
		char szVapPath[32];  /* VAP路径 */
		unsigned short usTcpPort;  /* TCP 端口,数值: 数值 1~65535 */    
		unsigned short usUdpPort;  /* UDP 端口,数值: 数值 1~65535 */    
		bool bCsEnable[4];  /* 中心服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
		char szCsIP[16];  /* 中心服务器IP地址, 字符串, 包括'\0'结束符共16byte */
		unsigned short usCsPort[2];  /* 中心服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */    
		bool bHsEnable[4];  /* 心跳服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
		char szHsIP[16];  /* 心跳服务器IP地址, 字符串, 包括'\0'结束符共16byte */
		unsigned short usHsPort[2];  /* 心跳服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */ 
		int iHsIntervalTime;  /* 心跳服务器间隔周期,数值(单位:秒) */ 
		bool bRsEnable[4];  /* 注册服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
		char szRsIP[16];  /* 注册服务器IP地址, 字符串, 包括'\0'结束符共16byte */
		unsigned short usRsPort[2];  /* 注册服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */
		int iRsAgedTime;  /* 注册服务器有效时间,数值(单位:小时) */
		char szAuthorizeServerIp[16];  /* 鉴权服务器IP */
		unsigned short usAuthorizePort[2];  /* 鉴权服务器端口,考虑4字节对齐,目前只用数组第一个 */
		char szAuthorizeUsername[32];  /* 鉴权服务器帐号 */
		char szAuthorizePassword[36];  /* 鉴权服务器密码 */

		char szIpACS[16];  /* ACS(自动注册服务器) IP */
		unsigned short usPortACS[2];  /* ACS Port,考虑4字节对齐,目前只用数组第一个 */
		char szUsernameACS[32];  /* ACS用户名 */
		char szPasswordACS[36];  /* ACS密码 */
		bool bVideoMonitorEnabled[4];  /* DVS是否定期上报前端视频信号监控信息, 数值: true使能, false不使能 */
		int iVideoMonitorInterval;  /* 上报周期（分钟） */

		char szCoordinateGPS[64];  /* GPS坐标 */
		char szPosition[32];  /* 设备位置 */
		char szConnPass[36];  /* 设备接入码 */
	}DHDEV_INTERVIDEO_BELL_CFG;

	/************************************************************************
	*   平台接入配置 － 中兴力维
	************************************************************************/
	typedef struct  
	{
		DWORD dwSize;
		unsigned short		nSevPort;							//服务器端口, 数值, 数值1~65535
		char				szSevIp[RV_INTERVIDEO_NSS_IP];		//服务器IP地址, 字符串, 包括'\0'结束符共32byte
		char				szDevSerial[RV_INTERVIDEO_NSS_SERIAL];	//前端设备序列号，字符串,包括'\0'结束符共32byte
		char				szUserName[RV_INTERVIDEO_NSS_USER];
		char				szPwd[RV_INTERVIDEO_NSS_PWD];
	}DHDEV_INTERVIDEO_NSS_CFG;

	/************************************************************************
	*   网络运行状态信息
	************************************************************************/

	typedef struct
	{
		int					nChannelNum;//通道号
		char				szUseType[32];//通道用途
		DWORD				dwStreamSize;//流量大小(单位:kb/s)
		char				reserved[32];//保留
	}DHDEV_USE_CHANNEL_STATE;

	typedef struct 
	{
		char						szUserName[32];//用户名
		char						szUserGroup[32];//用户组
		NET_TIME					time;//登入时间
		int							nOpenedChannelNum;//开启的通道个数
		DHDEV_USE_CHANNEL_STATE		channelInfo[RV_MAX_CHANNUM];
		char						reserved[64];
	}DHDEV_USER_NET_INFO;

	typedef	struct 
	{
		int							nUserCount;//用户数量
		DHDEV_USER_NET_INFO			stuUserInfo[32];
		char						reserved[256];
	}DHDEV_TOTAL_NET_STATE;
	/************************************************************************
	*   云台属性信息
	************************************************************************/
#define  NAME_MAX_LEN 16
	typedef struct 
	{
		DWORD		dwHighMask; /*!< 操作的掩码高位 */
		DWORD		dwLowMask; /*!< 操作的掩码低位 */
		char		szName[NAME_MAX_LEN]; /*!< 操作的协议名NAME_MAX_LEN=16 */
		WORD		wCamAddrMin; /*!< 通道地址的最小值 */
		WORD		wCamAddrMax; /*!< 通道地址的最大值 */
		WORD		wMonAddrMin; /*!< 监视地址的最小值 */
		WORD		wMonAddrMax; /*!< 监视地址的最大值 */
		BYTE		bPresetMin; /*!< 预置点的最小值 */
		BYTE		bPresetMax; /*!< 预置点的最大值 */
		BYTE		bTourMin; /*!< 自动巡航线路的最小值 */
		BYTE		bTourMax; /*!< 自动巡航线路的最大值 */
		BYTE		bPatternMin; /*!< 轨迹线路的最小值 */
		BYTE		bPatternMax; /*!< 轨迹线路的最大值 */
		BYTE		bTileSpeedMin; /*!< 垂直速度的最小值 */
		BYTE		bTileSpeedMax; /*!< 垂直速度的最大值 */
		BYTE		bPanSpeedMin; /*!< 水平速度的最小值 */
		BYTE		bPanSpeedMax; /*!< 水平速度的最大值 */
		BYTE		bAuxMin; /*!< 辅助功能的最小值 */
		BYTE		bAuxMax; /*!< 辅助功能的最大值 */
		int			nInternal; /*!< 发送命令的时间间隔 */
		char		cType; /*!< 协议的类型 */
		char		Reserved[7];
	}PTZ_OPT_ATTR;
	/************************************************************************
	*   设置设备返信息
	************************************************************************/
	typedef struct
	{
		DWORD		dwType;	 //类型(即GetDevConfig SetDevConfig的类型)
		WORD		wResultCode;//返回码：
		/*		0:成功 
		*		1:失败
		*		2:数据不合法
		*		3:暂时无法设置	
		*		4:没有权限
		*/

		WORD   		wRebootSign;//重启标志：
		/*
		*	0:不需要重启 1:需要重启才生效
		*/
		DWORD		dwReserved[2];//保留	
	}DEV_SET_RESULT;

	/******************************************************************************
	* 抓图功能属性结构体
	*****************************************************************************/

	typedef struct 
	{
		int			nChannelNum;//通道号

		DWORD		dwVideoStandardMask	;//分辨率(按位) 具体查看枚举 CAPTURE_SIZE						
		int			nFramesCount;//Frequence[128]数组的有效长度
		char 		Frames[128];	//帧率（按数值）
		//-25:25秒1帧
		//-24:24秒1帧
		//-23:23秒1帧
		//-22:22秒1帧
		//……
		//0:无效
		//1：1秒1帧
		//2：1秒2帧
		//3：1秒3帧
		//4：1秒4帧
		//5：1秒5帧
		//17：1秒17帧
		//18：1秒18帧
		//19：1秒19帧
		//20：1秒20帧
		//……
		//25: 1秒25帧
		int			nSnapModeCount;//SnapMode[16]数组的有效长度
		char		SnapMode[16];//（按数值） 0：定时触发抓图 1：手动触发抓图
		int			nPicFormatCount;//Format[16]数组的有效长度
		char 		PictureFormat[16];//（按数值）0:BMP格式 1:JPG格式
		int			nPicQualityCount;//Quality[32]数组的有效长度
		char 		PictureQuality[32];//（按数值）	
		//100:图象质量100%
		//80:图象质量80%
		//60:图象质量60%
		//50:图象质量50%
		//30:图象质量30%
		//10:图象质量10%
		char 		nReserved[128];//保留
	} RV_QUERY_SNAP_INFO;

	typedef struct 
	{
		int			nChannelCount; //通道个数
		RV_QUERY_SNAP_INFO  stuSnap[RV_MAX_CHANNUM];
	}RV_SNAP_ATTR_EN;

	/******************************************************************************
	* 主动注册功能属性结构体
	*****************************************************************************/

	typedef struct
	{
		DWORD	 dwSize;
		LONG     iEnable;   //使能
		LONG     iConnectType; //连接方式，0：单连接（使用原有的信令媒体单一连接模式） 1：多连接 （采取信令为主动注册连接，媒体需要在信令控制如视频请求情况下进行新的动态连接方式）
		char	 strServerIp[16]; //注册服务器ip		
		LONG     iServerPort;   //注册服务器端口
		char     strUsr[64];    //用户名
		char     strPwd[64];   //密码
		char     strPuId[32];   //设备统一标识，32位标识，由平台服务器约定
		/*
		设备统一标识规则 
		现有使用了32字节，具体信息安排，由平台服务器约定规则
		Xx          yy          zz                    a      				b     
		2字节		2字节       2字节					1字节     			1字节
		省级编号    市级编号     市级级别编号保留字     前端 0，客户端1		设备类型
		cccccccc cccccccc ccccccccc
		24字节 设备序号

		其中设备类型 01： DVR； 02 ：IPC ；03：NVS ； 04 ：解码器；05：车载DVR
		*/
	}RV_CONFIG_REG_SERVER;

	/******************************************************************************
	* 轮巡功能属性结构体
	*****************************************************************************/
#define V_TOUR_COMBINE_NUM 10
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
	}RV_SPLIT_MODE;
	typedef struct
	{
		unsigned char bTourEnable;//使能
		unsigned char ucRes;	//
		unsigned short usIntervalTime;//轮巡时间间隔
	}RV_DISPLAY_TOUR_CFG;
	typedef struct
	{
		DWORD	 dwSize;
		RV_DISPLAY_TOUR_CFG stDisplayTour;
		RV_SPLIT_COMBINE stSplitCombine[V_TOUR_COMBINE_NUM];
	}RV_DISPLAY_TOUR;

	/******************************************************************************
	* 画面分割功能属性结构体
	*****************************************************************************/
	typedef struct
	{
		RV_SPLIT_MODE stSplitMode;//模式
		RV_SPLIT_COMBINE_CH stCombineCH;//通道组合
	}RV_DISPLAY_SPLIT_CFG;

	/******************************************************************************
	* 解码器画面分组通道组合以及轮询结构体
	*****************************************************************************/
	typedef struct
	{
		DWORD	 dwSize;
		RV_DISPLAY_SPLIT_CFG stSplitCfg;
		RV_DISPLAY_TOUR_CFG stTourCfg;
		LONG iCombineNum;
		RV_SPLIT_COMBINE stSplitCombine[V_TOUR_COMBINE_NUM];
	}RV_WSCC;

	/******************************************************************************
	* 抓图配置结构体
	*****************************************************************************/
#define MAX_SNAP_COUNT 32
	typedef struct
	{   	    	
		int iChannel;
	    int Enable;/*定时抓拍使能*/
		int SnapQuality;/*抓拍图像质量，初步分6档， 1 差 2 一般 3普通， 4好， 5很好， 6最好*/
							/*!<抓图策略
							 usSnapMode 抓拍模式， 0 表示1秒几张模式
												   1 表示几秒1张模式
												   2 表示几分钟1张模式
							 usCounts   对应抓拍模式下的数值
							 */
		unsigned short usSnapMode;
		unsigned short usCounts;/*抓拍张数（1秒几张模式）或者抓拍时间（几秒1张模式）*/
	    int SnapCount;
	}RV_CONFIG_SNAP;
	typedef struct
	{
		DWORD dwSize;
		DWORD dwCount;								//设置时：==通道数为所有，>=0&&<通道数为哪个通道配置
													//读取时：返回个数
		RV_CONFIG_SNAP stSnapCfg[MAX_SNAP_COUNT];
	}RV_SNAP;

	typedef struct
	{
		unsigned short usTotal;  //总通道数目
		unsigned short usIndex;  //第几个通道信息
	}RV_DEV_CHANNEL;

	/******************************************************************************
	* 解码器报警配置结构体
	*****************************************************************************/
	#define RV_N_UI_TSECT					6
	#define RV_N_WEEKS						7
	typedef struct
	{
		int				enable;		     //!使能		
		unsigned char	startHour; 			//!开始时间:小时		
		unsigned char	startMinute;		//!开始时间:分钟		
		unsigned char	startSecond; 		//!开始时间:秒钟		
		unsigned char	endHour;			 //!结束时间:小时		
		unsigned char	endMinute; 		//!结束时间:分钟		
		unsigned char	endSecond; 		//!结束时间:秒钟		
		unsigned char   ucReserved[2];    //保留
	}RV_NETSECTION;		
	typedef struct
	{
		int				iName;
		RV_NETSECTION	tsSchedule[RV_N_WEEKS][RV_N_UI_TSECT];	/*!< 时间段 */
	}RV_NET_WORKSHEET;
	#define RV_N_SYS_CH 16
	typedef struct
	{		
		unsigned int		dwRecord;				/*!< 录象掩码 */
		int					iRecordLatch;				/*!< 录像延时：10～300 sec */ 	
		unsigned int		dwTour;					/*!< 轮巡掩码 */	
		unsigned int		dwSnapShot;				/*!< 抓图掩码 */
		unsigned int		dwAlarmOut;				/*!< 报警输出通道掩码 */
		int					iAOLatch;				/*!< 报警输出延时：10～300 sec */ 
		RV_PTZ_LINK			PtzLink[RV_N_SYS_CH];		/*!< 云台联动项 */		
		int					bRecordEn;					/*!< 录像使能 */
		int					bTourEn;						/*!< 轮巡使能 */
		int					bSnapEn;						/*!< 抓图使能 */	
		int					bAlarmOutEn;					/*!< 报警使能 */
		int					bPtzEn;						/*!< 云台联动使能 */
		int					bTip;						/*!< 屏幕提示使能 */	
		int					bMail;						/*!< 发送邮件 */	
		int					bMessage;					/*!< 发送消息到报警中心 */	
		int					bBeep;						/*!< 蜂鸣 */	
		int					bVoice;						/*!< 语音提示 */		
		int					bFTP;						/*!< 启动FTP传输 */		
		int					iWsName;				/*!< 时间表的选择，由于时间表里使用数字做索引，且不会更改 */
		unsigned int		dwMatrix;					/*!< 矩阵掩码 */
		int					bMatrixEn;					/*!< 矩阵使能 */
		int					bLog;						/*!< 日志使能，目前只有在WTN动态检测中使用 */
		int					iEventLatch;				/*!< 联动开始延时时间，s为单位 */
		int					bMessagetoNet;				/*!< 消息上传给网络使能 */
		unsigned int		dwReserved[7]; 				/*!< 保留字节 */
	}RV_EVENT_HANDLER;
	typedef struct
	{
		int iEnable;  //告警布防撤防 0撤防，1布防
		int iReserved; ////通用配置信息，具体定义如下
						//告警输入标识传感器类型常开 or 常闭；0常开，1常闭
						//视频丢失和动检为灵敏度；0~6档次，6档次为最灵敏
						//磁盘为硬盘剩余容量下限, 百分数
						//智能红色报警为灵敏度；0~6档次，6档次为最灵敏
 	}RV_ALARM_GUARD;
	typedef struct
	{
		RV_ALARM_GUARD			stGuard;
		RV_NET_WORKSHEET		stWorkSheet;
		RV_EVENT_HANDLER		struHandle;	/* 处理方式  */
	} RV_DECODER_ALARM,*LPRV_DECODER_ALARM; 
	typedef struct
	{
		DWORD dwSize;		
		RV_DECODER_ALARM 		struDecoderAlarm[RV_MAX_CHANNUM];
	}RV_DECODER_ALARM_SCHEDULE;

	//////////////////////////////////////////////////////////////////////
	//	其他报警结构
	//////////////////////////////////////////////////////////////////////
	typedef struct
	{
		RV_ALARM_GUARD			stGuard;
		RV_NET_WORKSHEET		stWorkSheet;
		RV_EVENT_HANDLER		struHandle;	/* 处理方式  */
	} RV_GEN_ALARM; 
	#define RV_MD_REGION_ROW	32
	typedef struct
	{
		int iRowNum;
		int iColNum;
		BYTE byRegion[32][32];
	}RV_REGION;
	typedef struct
	{
		RV_ALARM_GUARD			stGuard;
		RV_REGION				stRegion;
		RV_NET_WORKSHEET		stWorkSheet;
		RV_EVENT_HANDLER		struHandle;	/* 处理方式  */
	} RV_MOTION_ALARM; 
	typedef struct
	{
		DWORD dwSize;		
		RV_GEN_ALARM 		struLocalAlarmIn[RV_MAX_CHANNUM];
		//RV_GEN_ALARM 		struNetAlarmIn[RV_MAX_CHANNUM];
		RV_MOTION_ALARM		struMotionAlarm[RV_MAX_CHANNUM];
		RV_GEN_ALARM 		struVideoLossAlarm[RV_MAX_CHANNUM];
		RV_GEN_ALARM 		struVideoBindAlarm[RV_MAX_CHANNUM];
		RV_GEN_ALARM 		struNoDiskAlarm;
		RV_GEN_ALARM 		struDiskErrAlarm;
		RV_GEN_ALARM 		struDiskFullAlarm;
		RV_GEN_ALARM 		struNetBrokenAlarm;
		RV_GEN_ALARM 		struIPConflictAlarm;
	}RV_ALARM_SCHEDULE;

	//////////////////////////////////////////////////////////////////////
	//	TV调节结构
	//////////////////////////////////////////////////////////////////////
	typedef struct 
	{
		int left;//[0,100]
		int top;//[0,100]
		int right;//[0,100]
		int bottom;//[0,100]
	}RV_VD_RECT;
	//!TV调节设置
	typedef struct
	{
		DWORD dwSize;
		//!TV缩小比率，各分量取值相同   [0,100]
		RV_VD_RECT rctMargin;		//!亮度   [0,100]	
		int iBrightness;		//!对比度[0,100]	
		int iContrast;		//!去抖动[0,100]	
		int iAntiDither;	
	}RV_TVADJUST;

	//////////////////////////////////////////////////////////////////////
	//	普通配置
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//	夏令时结构体
	typedef struct
	{
		int Hour;
		int Minute;
	}RV_DSTTIMES;
	typedef struct
	{
		int iYear;		//!year [2000,2037]
		int iMonth;		//!month from January=1 [1,12]
		int iWeek;		//!周1:first  to2 3 4 -1:last one   0:表示使用按日计算的方法[-1,4]
		union
		{
			int iWeekDay;	//!weekday from sunday=0 [0, 6]
			int iDays;		//!day from one=1[0, 31]
		};	
		RV_DSTTIMES time;
	}RV_DST_POINT;
	//周模式选择
	//iMonth，iWeek(-1,1,2,3,4)， iWeekDay，Hour，Min
	//日模式选择 iWeek=0
	//iYear ,iMonth,iDays，Hour，Min
	typedef struct
	{
		RV_DST_POINT iDST[2];
		unsigned char ucEnable;//0,不使能，1使能
		unsigned char ucRes[3];//保留 
	}RV_DST_TIME;
	typedef struct
	{ 		
		int  iCtrlMask;/*!< 按位掩码形式
						0 硬盘满时处理 1覆盖"OverWrite", 0停止"StopRecord" 
						1 是否静音
						2 LCD屏幕自动关闭
						3 是否TV输出
						4 是否VGA输出
						5 是否HDMI输出
						*/		
		int iLocalNo;/*!<?本机编号:[0,?998]?*/ 
		unsigned short usScreenSaveTime;	//!屏保时间(分钟) [0, 120]	
		unsigned short usAutoLogout;//!本地菜单自动注销(分钟) [0, 120]
		unsigned char ucDateFormat;//日期格式/*!< 日期格式:“YYMMDD”, “MMDDYY”, “DDMMYY” */
		unsigned char ucDateSprtr;//日期分隔符/*!< 日期分割符:“.”, “-”, “/” */
		unsigned char ucTimeFmt;//时间格式/*!< 时间格式:1：“12”, 0：“24” */
		unsigned char ucRes;
		unsigned char ucSupportVideoFmt;/*设备可以支持的制式 ， 第0位 PAL ，第1位 NTSC， 第2位 SECAM*/
		unsigned char ucCurVideoFmt;/*设备使用的制式 ，0 PAL ，1 NTSC， 2 SECAM*/
		unsigned char ucRes1[2];
		unsigned int uiSupportLanguage;/*设备可以支持的语言 ， 
								第0位 English，第1位 SimpChinese， 第2位 TradChinese，
								第3位 “Italian”,4 “Spanish”, 5“Japanese”, 6“Russian”, 7“French”, 8“German” 
								*/						
		unsigned int uiCurLanguage;/*设备使用的语言 ，
								0 English，1 SimpChinese，2 TradChinese，3“Italian”,4 “Spanish”, 
								5“Japanese”, 6“Russian”, 7“French”, 8“German” */
		unsigned int uiRes[3];//保留字节						
	}RV_NET_GENERAL;
	/// 系统时间结构
	typedef struct
	{
		int  year;///< 年。[2000,2037]   
		int  month;///< 月，January = 1, February = 2, and so on.   
		int  day;///< 日。   
		int  wday;///< 星期，Sunday = 0, Monday = 1, and so on   
		int  hour;///< 时。   
		int  minute;///< 分。   
		int  second;///< 秒。   
		int  isdst;///< 夏令时标识。   
	}RV_SYSTEM_TIME;
	#define RV_MAX_DRIVERINFO 64
	typedef  struct
	{
		DWORD dwSize;
		RV_NET_GENERAL stGeneral;
		RV_SYSTEM_TIME stCurTime;
		RV_DST_TIME stDst;
	}RV_COMM_CFG;
//////////////////////////////////////////////////////////////////////
//	平台接入配置
//////////////////////////////////////////////////////////////////////
//互信互通平台接入配置
typedef struct
{
	DWORD dwSize;
	BOOL			Enable;   //使能
	int				MaxConn;  //视频最大连接数	
	char			ServerIp[40]; //服务器IP	
	unsigned short	ServerPort; //服务器端口
	unsigned short	MegaPort; //监听端口	
	char			DeviceId[32];//设备ID	
	char			UserName[32];  //用户名
	char			PassWords[32]; //密码	
	unsigned short	DownLoadMode; //录像下载方式按位0 TCP 1 HTXT
	unsigned short	FunMask;	/*功能掩码，按位操作
								第0位是否支持预案
								第1位是否支持网管
								第2位是否支持预录
								第3位是否支持NAT
								*/	
	unsigned short	AlarmInterVal; /*报警间隔*/
	unsigned short	Reserve;
}RV_CONFIG_NET_MEGA;
/************************************************************************
*   平台接入配置 － 阿尔卡特
************************************************************************/
typedef struct
{
	DWORD dwSize;
	unsigned short usCompanyID[2];  /* 公司ID,数值,不同的第三方服务公司,考虑4字节对齐,目前只用数组第一个 */
	char szDeviceNO[32];  /* 前端设备序列号，字符串, 包括'\0'结束符共32byte */
	char szVSName[32];  /* 前端设备名称，字符串, 包括'\0'结束符共16byte */
	char szVapPath[32];  /* VAP路径 */
	unsigned short usTcpPort;  /* TCP 端口,数值: 数值 1~65535 */    
	unsigned short usUdpPort;  /* UDP 端口,数值: 数值 1~65535 */    
	bool bCsEnable[4];  /* 中心服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
	char szCsIP[64];  /* 中心服务器IP地址, 字符串, 包括'\0'结束符共16byte */
	unsigned short usCsPort[2];  /* 中心服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */    
	bool bHsEnable[4];  /* 心跳服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
	char szHsIP[64];  /* 心跳服务器IP地址, 字符串, 包括'\0'结束符共16byte */
	unsigned short usHsPort[2];  /* 心跳服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */ 
	int iHsIntervalTime;  /* 心跳服务器间隔周期,数值(单位:秒) */ 
	bool bRsEnable[4];  /* 注册服务器使能标志, 数值: true使能, false不使能,考虑4字节对齐,目前只用数组第一个 */
	char szRsIP[64];  /* 注册服务器IP地址, 字符串, 包括'\0'结束符共16byte */
	unsigned short usRsPort[2];  /* 注册服务器端口,数值: 数值 1~65535,考虑4字节对齐,目前只用数组第一个 */
	int iRsAgedTime;  /* 注册服务器有效时间,数值(单位:小时) */
	char szAuthorizeServerIp[64];  /* 鉴权服务器IP */
	unsigned short usAuthorizePort[2];  /* 鉴权服务器端口,考虑4字节对齐,目前只用数组第一个 */
	char szAuthorizeUsername[32];  /* 鉴权服务器帐号 */
	char szAuthorizePassword[36];  /* 鉴权服务器密码 */

	char szIpACS[64];  /* ACS(自动注册服务器) IP */
	unsigned short usPortACS[2];  /* ACS Port,考虑4字节对齐,目前只用数组第一个 */
	char szUsernameACS[32];  /* ACS用户名 */
	char szPasswordACS[36];  /* ACS密码 */
	bool bVideoMonitorEnabled[4];  /* DVS是否定期上报前端视频信号监控信息, 数值: true使能, false不使能 */
	int iVideoMonitorInterval;  /* 上报周期（分钟） */

	char szCoordinateGPS[64];  /* GPS坐标 */
	char szPosition[32];  /* 设备位置 */
	char szConnPass[36];  /* 设备接入码 */
}RV_INTERVIDEO_BELL_CFG;
//VS300平台配置
typedef struct
{
	DWORD dwSize;
	BOOL    BEnalbe;//使能 4字节 0 不使能 1 使能
	char    acServerIp[32];//服务器ip
	long	iServerPort;//服务器侦听端口
    char    acUserName[32];//用户名
    char    acPassWords[32];//密码
	long	iType;//协议类型 0 tcp 1 udp
    char    acReserve[80];//保留位
}RV_CONFIG_VS300_NET;
//神眼平台接入配置
typedef struct
{ 	
	DWORD dwSize;
	BOOL Enable;//!是否开启
	char ServerIp[64];//神眼服务器IP
	int ServerPort;//!神眼服务器端口号
	char PUId[32];//!设备ID
	int  usTcpPort;  /* 对外的TCP 端口,数值: 数值 1~65535 */    
	int  usUdpPort;  /* 对外的UDP 端口,数值: 数值 1~65535 */    
	char Reserved[64];//!保留
}RV_CONFIG_ZXSHENYAN_NET;
//////////////////////////////////////////////////////////////////////
//	磁盘配置
//////////////////////////////////////////////////////////////////////
typedef struct
{	
	unsigned char ucUse;			//此分区是否有效	
	unsigned char ucPartion;		//分区号；	
	unsigned char ucFSType;			//分区支持的文件系统	
									// 0 WFS文件系统  现有最大支持2分区，且分区又约束如下三种方式	
									// WFS2分区的话，默认第1分区为快照分区，第2分区为录像分区	
									//  如果是WFS 1分区的话，根据ucPartType决定	
									//  ucPartType==0，录像分区；ucPartType==1快照分区	
									//  此时快照（存储图片）后者录像，设备只能进行一种媒体的存储	
									//  WFS文件系统约束只作百分比2分区模式	
									// 1 FAT32	
									// 2 EXT3	
	unsigned char ucPartType;		//WFS文件系统分区专用 0 录像分区  1快照分区；标准文件系统，不需要，无限制	
	unsigned int uiTotalSpace;		//< 总容量，其单位参考ucSpaceType说明	
	unsigned int uiRemainSpace;		//< 剩余容量，其单位参考ucSpaceType说明	
	unsigned char ucSpaceType;		//标识硬盘容量单位，方便比较准确的统计	
									//  后4位（7  6 5 4 ） 总大小单位  0 KB，1MB，2GB，3TB								
									//  前4位（3 2 1 0） 剩余大小单位  0 KB，1MB，2GB，3TB
	unsigned char ucReserve[3];
}RV_DISK_PARTION_INFO;
typedef struct
{
	unsigned char ucDiskNo;				//盘符,第几个硬盘 最多255个
	unsigned char ucCurrentUse;			//是否当前使用工作盘
	unsigned char ucLocalStorage;		//本地存储还是远程存储介质 0本地 1远程
	unsigned char ucDiskType;			//硬盘介质类型 0 sata硬盘 1，U盘，3 sd卡，4，移动硬盘，5，dvd光盘
	unsigned char ucErrorFlag;			//工作盘错误标志
	unsigned char ucPartMethod;			//支持分区模式 
										// 第0位表示是否支持分区 0 不支持 1支持
										//  后七位 按位标识支持分区方式 
										//  第1位 按照百分比分区 0 不支持 1支持
										//   第2位 按照容量分区 MB  0 不支持 1支持
										//   第3位 按照容量分区 GB  0 不支持 1支持
										//   第4位 按照容量分区 TB 0 不支持 1支持
	unsigned char ucSupportPartionMax;  //支持硬盘最大分区个数
	unsigned char ucSupportFileSystem;  //支持的文件系统0 WFS 1 FAT32 2 EXT3
	unsigned int uiTotalSpace;			//< 总容量，其单位参考ucSpaceType说明
	unsigned int uiRemainSpace;			//< 剩余容量，其单位参考ucSpaceType说明
	unsigned char ucSpaceType;			//标识硬盘容量单位，方便比较准确的统计
										//  后4位（7  6 5 4 ） 总大小单位  0 KB，1MB，2GB，3TB
										//  前4位（3 2 1 0） 剩余大小单位  0 KB，1MB，2GB，3TB
	unsigned char ucAttr;       //0 可读写 （默认），1只读， 2 冗余备份
	unsigned char ucRes1[2];				//保留
	RV_DISK_PARTION_INFO szPartion[4];
	unsigned char ucRes2[64];			//保留
}RV_DISK_INFO;
#define RV_MAX_DISK_NUM	16
typedef struct
{
	DWORD dwSize;
	DWORD dwCount;
	RV_DISK_INFO stDiskInfo[RV_MAX_DISK_NUM];
}RV_DISKS;
//硬盘操作
typedef struct
{
	unsigned char ucDiskNo;         //盘符,第几个硬盘 最多255个
	unsigned char ucCtrlType;       //0 可读写 （默认），1只读， 2 冗余备份 ，3 格式化 
									//4 扇区检测错误恢复
									// 5重新分区，重新分区后面数据才有效
	unsigned char ucRes[2];  

	/////重新分区有效数据----------------------
	unsigned char ucFSType;	// 0 WFS文件系统  现有最大支持2分区，且分区又约束如下三种方式
							// WFS2分区的话，默认第1分区为快照分区，第2分区为录像分区
							//  如果是WFS 1分区的话，ucPartType标示录像分区和快照（存储图片）
							//  此时后者录像，设备只能进行一种媒体的存储
							//  WFS文件系统约束只作百分比2分区模式
							// 1 FAT32
							// 2 EXT3
	unsigned char ucPartType;			//WFS文件系统分区专用 只有一个分区的时候才使用 0 录像分区  1快照分区
										//WFS  两分区时候，默认第1分区为快照分区，第2分区为录像分区，此值无效
	unsigned char ucPartNum; //分区个数
	unsigned char ucPartMethod;	//硬盘分区方式， 0 百分比，1 KB 2MB 3 GB 4 TB
	unsigned int uiPartSpace[4];	// 按照分区方式，依次分区所使用的容量
									//如果按照百分比，例如 50%-50% 2分区， 
									//内部填充50，50, 0, 0；ucPartCtrlType== 2 ，ucPartNo ==2
									//如果按照容量MB分区方式，例如1G的硬盘，200MB-300MB-100MB-400MB，
									//那么依次填充 200，300，100，400；ucPartCtrlType== 2 ，ucPartNo ==4
	unsigned int uiRes[6];  
}RV_CTRL_DISK_INFO;
//硬盘分区进行格式化以及修复操作
typedef struct
{
	unsigned char ucDiskNo;          //盘符,第几个硬盘 最多255个
	unsigned char ucPartCtrlType;   //0 格式化，1分区扇区检测修复
	unsigned char ucPartNo;       //ucPartCtrlType==0、1，对第ucPartNo个分区进行格式化、扇区检测修复；
	unsigned char ucRes;	
	unsigned int uiRes[7];       //保留
}RV_CTRL_DISK_PARTION_INFO;
typedef struct
{
	DWORD dwSize;
	RV_CTRL_DISK_INFO stDiskInfo;
}RV_CTRL_DISK_OP;
typedef struct
{
	DWORD dwSize;
	RV_CTRL_DISK_PARTION_INFO stDiskPartionInfo;
}RV_CTRL_DISK_PARTION_OP;
//////////////////////////////////////////////////////////////////////
//	通道配置
//////////////////////////////////////////////////////////////////////
typedef struct
{
	int enable;//!使能
	int startHour;//!开始时间:小时
	int	startMinute;//!开始时间:分钟
	int	startSecond;//!开始时间:秒钟
	int	endHour;//!结束时间:小时
	int	endMinute;//!结束时间:分钟
	int	endSecond;//!结束时间:秒钟
}RV_TIMESECTION;
//////////////////////////////////////////////////////////////////////
//	网络配置
//////////////////////////////////////////////////////////////////////
#define RV_NAME_PASSWORD_LEN		64	
#define RV_EMAIL_ADDR_LEN  32
#define RV_N_MIN_TSECT 2
typedef struct
{
	char HostName[RV_NAME_PASSWORD_LEN]; //!设备名	
	int HttpPort; //!HTTP服务端口	
	int TCPPort; //!TCP侦听端口		
	int SSLPort; //!SSL侦听端口	
	int UDPPort; //!UDP侦听端口	
	int MaxConn; //!最大连接数	
	unsigned char ucMonMode; //!监视协议 {"TCP","UDP","MCAST",…}  0|1|2	
	unsigned char ucUseTransPlan; //!是否启用网络传输策略
	unsigned char ucTransPlan; //!传输策略 2自动 1流畅性优先 0质量优先，默认1
	unsigned char ucHighSpeedDownload; //!是否启用高速录像下载测率
    int iRes[3];
}RV_CONFIG_NET_APP;
typedef struct
{
	char strEthName[16]; //不同网卡名称，以字符串标示"eth0"; 此不可以改
	char strMacAddr[32];//xx.xx.xx.xx.xx.xx(字符串形式)
	char HostIP[20]; //!主机IP	
	char Submask[20]; //!子网掩码
	char Gateway[20]; //!网关IP
	unsigned char  ucTranMedia;   // 0：有线，1：无线
	unsigned char  ucDefaultEth; //是否作为默认的网卡, 1：默认 0：非默认
	unsigned char  ucValid;	// 按位表示，第一位：1：有效 0：无效；
							//第二位：0：DHCP关闭 1：DHCP使能；
							//第三位：0：不支持DHCP 1：支持DHCP
	unsigned char  ucReserve;	
	unsigned char  ucRes[16]; //保留字
}RV_CONFIG_NET_ETH;
typedef struct
{
	DWORD dwSize;
	RV_CONFIG_NET_APP stNetApp;
	RV_CONFIG_NET_ETH stNetEth;
}RV_CONFIG_NET_COMM;
//!服务器结构定义
typedef struct
{	
	char ServerName[RV_NAME_PASSWORD_LEN];//!服务名,ip域名形式或者是xxx.xxx.xxx.xxx形式，最长64字节
	int iReserved;//!原始为IP地址，为了扩展保留此字段；	
	int Port;//!端口号
	char UserName[RV_NAME_PASSWORD_LEN];//!用户名	
	char Password[RV_NAME_PASSWORD_LEN];	//!密码	
	int  Anonymity;//!是否匿名登录
}RV_REMOTE_SERVER;
typedef struct
{
	int Enable;	//!是否开启 0 不开启，1开启	
	int iKey;//!类型名称，如果是ddns，对应ddnskey;可以扩展服务类型使用
	RV_REMOTE_SERVER Server;		//!服务器 信息
}RV_IPADDR_SERVER;
#define  RV_MAX_EMAIL_TITLE_LEN 64
#define  RV_MAX_EMAIL_RECIEVERS  5
//!EMAIL设置
typedef struct
{
	//!可以填ip,也可以填域名
	RV_IPADDR_SERVER Server;
	int bUseSSL;		
	char SendAddr[RV_EMAIL_ADDR_LEN];//!发送地址		
	char Recievers[RV_MAX_EMAIL_RECIEVERS][RV_EMAIL_ADDR_LEN];//!接收人地址		
	char Title[RV_MAX_EMAIL_TITLE_LEN];//!邮件主题	
	RV_TIMESECTION Schedule[RV_N_MIN_TSECT];//!email有效时间	
}RV_CONFIG_EMAIL;
typedef struct
{
	DWORD dwSize;
	RV_CONFIG_EMAIL stEmail;
}RV_EMAIL;
//DNS
typedef struct
{
	char strPrimaryIp[16]; //xxx. xxx. xxx. xxx (字符串形式)
	char strSecondIp[16];
}RV_DNS_IP_LIST;
typedef struct
{
	DWORD dwSize;
	RV_DNS_IP_LIST stDNSIpList;
}RV_DNS;
//UPNP
typedef struct
{	
	int Enable; //是否开启	
	int WebPort;// web端口	
	int TCPPort; // tcp端口	
	int iRervered;
}RV_CONFIG_UPNP;
typedef struct
{
	DWORD dwSize;
	RV_CONFIG_UPNP stUpnp;
}RV_UPNP;
//PPPOE
typedef struct 
{
	int iEnable; //0 不使能，1 使能
	char strip[32];
	char strUser[64];
	char strPwd[64];
}RV_PPPOE_IP_SRV;	//注明：其中，拨号获取设置的时候只有使能，ip（拨号ip），用户名密码有效；
				//设置的时候，只有使能，用户名密码有效；
typedef struct
{
	DWORD dwSize;
	RV_PPPOE_IP_SRV stPppoe;
}RV_PPPOE;
//DDNS
#define RV_MAX_DDNS_SERVER_NUM 16
typedef struct
{
	DWORD dwSize;
	int iCount;
	RV_IPADDR_SERVER stDDNSServer[RV_MAX_DDNS_SERVER_NUM];
}RV_MDDNS;
//////////////////////////////////////////////////////////////////////
//	编码配置
//////////////////////////////////////////////////////////////////////
typedef struct
{
	int		iCompression;			/*!< 压缩模式 */	
	int		iResolution;			/*!< 分辨率 参照枚举capture_size_t(DVRAPI.H) */	
	int		iBitRateControl;		/*!< 码流控制 参照枚举capture_brc_t(DVRAPI.H) */	
	int		iQuality;				/*!< 码流的画质 档次1-6（对应的是最差，差，一般，好，很好，最好*/	
	int		nFPS;					/*!< 帧率值（0~30）（-n~0）负数表示多秒一帧，NTSC/PAL不区分 */	
	int		nBitRate;				/*!< 0-4096k */
	int		iGOP;					/*!< 描述两个I帧之间的间隔时间s，2-12 */
}RV_VIDEO_FORMAT;
typedef struct
{
	int		nBitRate;				/*!< 码流kbps*/	
	int		nFrequency;				/*!< 采样频率*/	
	//int		nMaxVolume;				/*!< 最大音量阈值*/
	unsigned char	ucLAudioVolumn;	//左声道音量
	unsigned char	ucRAudioVolumn;	//右声道音量
	unsigned char	ucRes[2];		//保留
}RV_AUDIO_FORMAT_T;
//!媒体格式
typedef struct
{
	RV_VIDEO_FORMAT vfFormat;			/*!< 视频格式定义 */			
	RV_AUDIO_FORMAT_T afFormat;			/*!< 音频格式定义 */
	int	bVideoEnable;				/*!< 开启视频编码 */
	int	bAudioEnable;				/*!< 开启音频编码 */	
}RV_MEDIA_FORMAT;
//!编码设置
typedef struct
{
	int iSteamIndex;				/*!<码流格式 
										主码流 参考枚举参考ENCODE_TYPE_BY_RECORD ，现有产品默认只有一个主码流，填充为0；
										副码流 参考 ENCODE_TYPE_BY_SUBSTREAM，最多4种子码流，现在产品最多支持一种副码流，填充为0；
										捉图码流 参考 ENCODE_TYPE_BY_SUBSTREAM，最多从4种码流中进行捉图配置
									*/	
	RV_MEDIA_FORMAT dstFmt;			/*!<码流格式 */	
}RV_NET_ENCODE;
//
#define RV_N_COLOR_SECTION 2
//! 视频颜色结构
typedef struct
{
	int	nBrightness;				/*!< 亮度	0-100 */
	int	nContrast;					/*!< 对比度	0-100 */
	int	nSaturation;				/*!< 饱和度	0-100 */
	int	nHue;						/*!< 色度	0-100 */
	int	mGain;						/*!< 增益	0-100 第７位置1表示自动增益　*/		
	int	mWhitebalance;				/*!<自动白电平控制，bit7置位表示开启自动控制.0x0,0x1,0x2分别代表低,中,高等级*/
}RV_VIDEOCOLOR_PARAM; 
typedef struct
{
	RV_TIMESECTION		TimeSection;		/*!< 时间段 */
	RV_VIDEOCOLOR_PARAM	dstColor;			/*!< 颜色定义 */
	int					iEnable;             /*!< 使能定义 */ /* 现有sdk暂时没有此部分，默认使能*/
}RV_VIDEOCOLOR;
typedef struct
{
	RV_VIDEOCOLOR dstVideoColor[RV_N_COLOR_SECTION];
}RV_CONFIG_VIDEOCOLOR;
//! 查询视频区域遮挡属性信息
typedef void *						VD_HANDLE;
typedef int							VD_BOOL;
typedef unsigned int				VD_COLORREF;//0x00bbggrr
typedef unsigned int				VD_COLORDEV;//0x????????
//!视频物件结构
typedef struct
{
	VD_COLORREF rgbaFrontground;		/*!< 物件的前景RGB，和透明度 */	
	VD_COLORREF rgbaBackground;			/*!< 物件的后景RGB，和透明度*/	
	RV_VD_RECT	rcRelativePos;				/*!< 物件边距与整长的比例*8191 */	
	VD_BOOL	bPreviewBlend;				/*!< 预览叠加 */	
	VD_BOOL	bEncodeBlend;				/*!< 编码叠加 */
}RV_VIDEO_WIDGET;
//区域遮挡
#define RV_COVERNUM 8
typedef struct
{
	unsigned int	uiCoverNum;			/*!< 当前该通道有几个叠加的区域 */
	RV_VIDEO_WIDGET	dstCovers[RV_COVERNUM];
}RV_NET_VIDEO_COVER;
//
#define RV_CHANNEL_NAME_SIZE 32
typedef struct
{
	DWORD dwSize;	
	char cName[RV_MAX_CHANNUM][RV_CHANNEL_NAME_SIZE];
	RV_NET_ENCODE stEncodeMain[RV_MAX_CHANNUM];
	RV_NET_ENCODE stEncodeAssist[RV_MAX_CHANNUM];
	RV_CONFIG_VIDEOCOLOR stColorCfg[RV_MAX_CHANNUM];
	RV_VIDEO_WIDGET stTimeOSD[RV_MAX_CHANNUM];
	RV_VIDEO_WIDGET stTitleOSD[RV_MAX_CHANNUM];
	RV_NET_VIDEO_COVER stCovor[RV_MAX_CHANNUM];
	RV_NET_GENERAL stGeneral;
}RV_CODE_CFG;

//2010-7-20 luhj add
// typedef struct  
// {
// 	DWORD dwSize;
// 	NET_CHANNEL_NAME stChannelName[32];
// }RV_CHANNEL_NAME;
//add end
//2010-3-29 luhj-0016
typedef struct
{
	unsigned short 	iChannel;		//通道 从0开始
	unsigned short	iIndex;			//预置点或者轨迹下标号 从1开始
}RV_PTZ_CHANNEL;

#define NAME_LEN			32		
#define PTZ_PRESETNUM		64
#define PTZ_CHANNELS		8

typedef struct 
{
	unsigned char 	ucChannel;					//通道
	unsigned char 	ucPresetID;					//预置点号
	unsigned char	iSpeed;						//预置点速度 速度1~15等级
	unsigned char	iDWellTime;					//预置点停留时间 1~255s
	unsigned char	szPresetName[NAME_LEN];		//预置点名称
}RV_PRESET_INFO;

typedef struct
{
	unsigned char 	ucChannel;					//通道
	unsigned char	ucTourIndex;				//巡航号
	unsigned char	ucPresetCnt;				//一个巡航组中的预置点数 0~128
	unsigned char	uRes;						//保留
	unsigned char	uiPresetNum[PTZ_PRESETNUM];	//巡航组中的预置点号
}RV_TOUR_INFO;

typedef struct  
{
	DWORD dwSize;
	DWORD dwCount;
	RV_PRESET_INFO	struPreset[PTZ_PRESETNUM];
}RV_PTZ_PRESET_SCHEDULE;

typedef struct  
{
	DWORD dwsize;
	DWORD dwCount;
	RV_TOUR_INFO	struTour[PTZ_CHANNELS];
}RV_PTZ_TOUR_SCHEDULE;

typedef struct  
{
	RV_PTZ_CHANNEL struChannel;
	RV_PRESET_INFO	struPreset;
}RV_PTZ_TOUR_PRESET;
//luhj-0016 end
//NTP结构体
typedef struct
{	
	RV_IPADDR_SERVER szNtpServer;//!服务器	
	int UpdatePeriod;//!更新周期	
	int TimeZone;//!时区
}RV_CONFIG_NTP;
typedef struct
{
	DWORD dwSize;
	RV_CONFIG_NTP stNtp;
}RV_NTP;

//////////////////////////////////////////////////////////////////////////
//	输出模式结构体
typedef struct
{
    unsigned char ucOSDShow;//0位 是否显示时间 1位 是否显示通道名
    unsigned char ucRes[15];//保留
}RV_OUT_MODE_OSD_CFG;
typedef struct
{
	DWORD dwSize;
    RV_OUT_MODE_OSD_CFG stOSD;
}RV_OUT_MODE_CFG;

//////////////////////////////////////////////////////////////////////////
//	ftp结构体
typedef struct
{
	RV_IPADDR_SERVER	stServer;						//ftp服务器
	char            cDirName[RV_FTP_MAX_PATH];            //FTP目录路径 240
    int             iReserved;                   
    int             iFileLen;                    //文件长度               M为单位
    int             iInterval;                    //相邻文件时间间隔    秒为单位  //这个先保留不处理吧
    char            cRev[128];
}RV_CONFIG_FTP_SERVER_SET;
typedef struct
{
    RV_TSECT				stSect[RV_N_WEEKS][RV_TIME_SECTION];
}RV_FTP_APP_TIME_SET;
typedef struct
{
	RV_CONFIG_FTP_SERVER_SET	stFtpServerSet;	
	RV_FTP_APP_TIME_SET			stFtpAppTimeSet[RV_MAX_CHANNUM];
}RV_FTP_SERVER;
typedef struct
{
	DWORD dwSize;
	RV_FTP_SERVER stFtpRecord;//录像
	RV_FTP_SERVER stFtpPicture;//图片
}RV_FTP;

/////////////////////////////////////////////////////////////////////////
//	rtsp结构体
typedef struct
{
    unsigned short    usEnable;           //使能 
    unsigned short    usListernPort;     //RTSP侦听端口
    unsigned short    usUdpStartPort;  //UDP起始端口
    unsigned short    usUdpPortNum;   //UDP端口数量
    unsigned short    usReserve[36];   //保留
}RV_CONFIG_RTSP_SET;
typedef struct
{
	DWORD dwSize;
	RV_CONFIG_RTSP_SET stRTSP;
}RV_RTSP;

//////////////////////////////////////////////////////////////////////
//	能红色报警结构
//////////////////////////////////////////////////////////////////////
//区域遮挡
#define RV_REDREGIONNUM 8
typedef struct
{
	long lX;
	long lY;
}RV_POINT;
typedef struct
{
	int				iRectNum;	/*!< 当前该通道有几个区域 */
	RV_POINT		szPointArray[RV_REDREGIONNUM][4];
}RV_REDON_RECT;
typedef struct
{
	RV_ALARM_GUARD			stGuard;
	RV_NET_WORKSHEET		stWorkSheet;
	RV_EVENT_HANDLER		struHandle;	/* 处理方式  */
	RV_REDON_RECT			stRedonRect;
} RV_INTERED_ALARM; 
typedef struct
{
	DWORD dwSize;		
	RV_INTERED_ALARM		struInteRedAlarm[RV_MAX_CHANNUM];
}RV_INTERED_ALARM_SCHEDULE;

//////////////////////////////////////////////////////////////////////
//录像控制
typedef struct
{
	unsigned char ucChannelCount;//通道数目
	unsigned char ucRes[3];
	unsigned char ucState[128];//第0个到第127个依次为第1到第128通道（0: 为不录像;1: 手动录像;2: 为自动录像）
}RV_RECCTL; 
typedef struct
{
	DWORD dwSize;  
	RV_RECCTL struRecCtl;
}RV_RECCTL_CFG;

/* 串口配置结构体 超过16通道 */
typedef struct
{
	DWORD			dwSize;
	//解码器协议
	DWORD			dwDecProListNum;				//协议个数
	char			DecProName[RV_MAX_DECPRO_LIST_SIZE][RV_MAX_NAME_LEN]; //协议名列表
	RV_485_CFG		stDecoder[RV_MAX_DECODER_NUM_EX];		//各解码器当前属性
	
	DWORD			dw232FuncNameNum;				//232功能个数
	char			s232FuncName[RV_MAX_232FUNCS][RV_MAX_NAME_LEN];	//功能名列表
	RV_RS232_CFG	st232[RV_MAX_232_NUM];		//各232串口当前属性
} DHDEV_COMM_CFG_EX;

//2010-7-20 luhj add
typedef struct  
{
    int iChannel;
    char strChannelName[MAX_HHCHANNELNAME_LEN]; 
}RV_CHANNEL_NAME;

typedef struct  
{
	DWORD dwSize;
	RV_CHANNEL_NAME stChannelName[RV_MAX_DECODER_NUM_EX];		
}RV_CHANNELNAME_CFG;
//add end

//////////////////////////////////////////////////////////////////////////
//音频配置
typedef struct
{
  unsigned int uiTalkAudioSourceType;    //!< 声音源方式，0为线性输入，1为mic
  unsigned int uiOutSilence;        ///< 静音 0--非静音 1--静音
  unsigned int uiOutLAudioVolumn;   //左声道音量
  unsigned int uiOutRAudioVolumn;   //右声道音量，单声道的设备左右值一样
  unsigned int uiReserverd[32];//保留
}RV_DEV_AUDIO;
typedef struct  
{
	DWORD			dwSize;
	RV_DEV_AUDIO	stDevAudio;		
}RV_DEV_AUDIO_CFG;

//////////////////////////////////////////////////////////////////////////
// 数字通道轮训配置
typedef struct
{
	BOOL			BEnable;                  //使能开关
    char			cChName[32];                         /* 本配置名称,便于识别*/
    char			cDeviceIP[64];                            ///< 待连接设备的ip地址
    LONG			iDevicePort;                        ///< 待连接设备的端口号
    char			cDeviceUserName[32]; ///< 用户名
    char			cDevicePassword[32]; ///< 密码    
    LONG			iDevType;                            ///< 设备类型，按枚举表示，即协议类型,0表示tcp直连，3 rtsp+udp 4 vs300.
    LONG			iRemoteChannelNo;                    ///< 远程通道号,从0开始算起  
    LONG			iStreamType;                         ///< 要连接的码流类行?0:主码流， 1:辅码流
    LONG			iRemotePtzPreset;//远程预置点
    LONG			iRemotePtzPresetEnable;//是否一连接上就设置远程预置点   
    LONG			reserverd[5];                        ///< 保留字节	
}RV_REMOTE_CH_CFG;
typedef struct
{
    BOOL			BEnable;//使能开关
    LONG			iDeviceChannelType;                    ///< 通道类型，0表示数字通道，1表示模拟通道，默认为数字通道,暂时不可以切换
    LONG			iMode;//0 单连接 1 多连接
    unsigned long   uiTourTime;/* 多连接轮巡时间 */
    unsigned long   uiNotTourCfgIndex; /* 单连接时的采用的通道配置,在轮巡列表中的位置*/
    LONG			iDecodePolicy;   // //解码策略     (-2, 最实时 -1 实时 0 默认情况 1 流畅 2,    最流畅 ) 以后可能扩展到自定义具体的延时时间
    LONG			iAVEnable;       //0,只解视频 1，只解音频 ，2 解音视频 
    LONG			iReserverd[5];          ///< 保留字节
}RV_LOCAL_CH_CFG;
typedef struct
{
	RV_LOCAL_CH_CFG		stLocalChCfg;		//通道配置
	DWORD				dwRemoteChanCount;	//远程通道数目
	RV_REMOTE_CH_CFG	stRemoteChanCfg[64]; //远程通道配置
}RV_LOCAL_CH;
typedef struct
{
	DWORD dwSize;
	RV_LOCAL_CH stLocalChan[64];
}RV_DIGI_CH_TOUR_CFG;

	/******************************************************************************
	* end of 配置结构体
	*****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // DHNETSDK_H










































