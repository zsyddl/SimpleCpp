#pragma once
/*
* 流 打包器 
* 
*/
#define BYTE_ALIGNMENT       8       //字节对齐
#define STREAM_TYPE_VIDEO_H264 0x1B
#define STREAM_TYPE_AUDIO_G711 0x90
#define OPTIONAL_FIELDS_ONLY_PTS (1 << 7) 
#define SPLIT_PACKET_LENGTH 60000 

#define OPTIONAL_FIELDS_PTS_DTS (1 << 6) 

#define OPTIONAL_FIELDS_ESCR (1 << 5)

#define OPTIONAL_FIELDS_ES_RATE (1 << 4)

#define OPTIONAL_FIELDS_DSM_TRICK_MODE (1 << 3)

#define OPTIONAL_FIELDS_ADDITIONAL_COPY_INFO (1 << 2)

#define OPTIONAL_FIELDS_PREVIOUS_PES_CRC (1 << 1)

#define OPTIONAL_FIELDS_PES_EXTENSION (1 << 0)
//Ps header flag;
#define PS_HEADER_FLAG_LEN 4
// ps header 16 bytes
#define PS_HEADER_FLAG (0xba010000)
#define PS_HEADER_LEN  16
#define PS_SYS_FLAG    (0xbb010000)
#define PS_SYS_LEN     18
#define PS_PSM_FLAG    (0xbc010000)
#define PS_PSM_LEN     24
#define PS_PES_HEADER_FLAG (0x01000000)
//长度不确定,根据参数解析
#define PS_PES_HEADER_LEN 0

typedef struct _StPSInfo
{
	unsigned long long u64SCR;
	unsigned int u32MuxRate;
	int s32MediaType;          //媒体类型。0x01视频I帧，0x02视频P帧，0x08视频B帧，0x10音频G711
	int s32Flag;                //0x01加PSM头标志位，0x02加SYS头标志位
}StPSInfo;//封装PS时需要的信息

typedef struct _StPESInfo
{
	unsigned char u8StreamID;
	unsigned int u32Flag;
	unsigned long long u64Pts;
	unsigned long long u64Dts;
	unsigned long long u64ESCRBase;
	unsigned short u16ESCRExtention;
	unsigned int u32ESRate;
}StPESInfo;//封装PES时需要的信息
typedef struct _StMediaInfo
{
	int s32MediaType;		   //媒体类型。0x01视频I帧，0x02视频P帧，0x08视频B帧，0x10音频G711
	unsigned int u32TimeStamp; //时间戳
	//视频相关参数
	int s32VideoWidth;         //视频宽度
	int s32VideoHeight;        //视频高度
	int s32VideoFPS;           //视频帧率
	//音频相关参数
	int s32AudioSample;        //音频采样率
	int s32AudioRate;          //音频码率
	//朗驰网板
	int s32LaunchFlag;          //朗驰网板的标志信息。0x01有ETI_DVR_FILE_HEAD头
}StMediaInfo;//去除头信息过程中得到的一些关于媒体数据的信息，用这个结构体做下汇总，以后的处理过程可能用得到

#define IN 
#define OUT

namespace SimpleCpp{
	class CBuffer;
}
using namespace SimpleCpp;
typedef enum _eFrameType
{
	eFT_I =0,
	eFT_P = 1,
	eFT_B = 2,
}eFrameType;

#define MAX_PS_SIZE (256*1024)
class CPsPackage
{
public:
	CPsPackage(int nW, int nH, int nFPS, int nBitRate);
	virtual ~CPsPackage(void);
	
	//对任意长度的 h264 数据打包,内部自动分帧
	int Package(unsigned char *pH264, int nLen, unsigned char *pPsStream, int &nLenOut);
	int Package(unsigned char *pH264, int nLen, CBuffer *pBufferPsPkg);
	//对一帧 h264 打包
	//nFrameType: eFT_I, eFT_P
	int PackageFrame(unsigned char *pH264Frame, int nLen, int nFrame,CBuffer *pBufferPsPkg);
	//对ps 流解包, 输出为标准 h264流.
	//pPsStream, 输入, nLen: 输入
	//pH264Stream: 输入输出.需要用户分配好足够的空间.一般 512kB足够. 
	int UnPackage(unsigned char *pPsStream, int nLen, unsigned char *pH264Stream, int &nLenOut);
	int UnPackage(unsigned char *pPsStream, int nLen, CBuffer *pBufferH264);
protected:
	/*功能 h264 分帧
	* @ pBufferH264  : 输入的 h264 流
	* @ pBufferFrame :    解析的一阵 h264
	* @ return       : passed pos, 
	* @ 每次调用 只能解析一帧数据，需要反复调用, 5 7 8 都为 I
	*/
	int ParseH264Frame(CBuffer *pBufferH264, CBuffer *pBufferFrame, int &nFrameType);
	//
	int PackageH2642Ps(CBuffer *pBufferFrame, CBuffer *pBufferPsPkg, int nFrameType);
	// nFrameType: eFT_I, eFT_P
	int PackageH2642Ps(unsigned char *pBufferFrame, int nLen,int nFrameType, CBuffer *pBufferPsPkg);
	//
	/*
	* 简单实现流程
		*
		* |写PS头|-->|I帧前加PSM头|-->|Media2PESStream|-->|分包操作|-->|Media2PESPacket|
		*
		* 功能：将给定的一帧媒体数据封装成一个PS包
		* 参数1：pMediaBuf        [in]    一帧媒体数据
		* 参数2：s32MediaBufSize  [in]    一帧媒体数据的长度
		* 参数3：pstPsInfo        [in]    封装PS需要的信息
		* 参数4：pPSBuf           [out]   封装后的PS包
		* 参数5：pPSBufSize       [out]   封装后PS包的长度
		* 返回值：成功返回0，失败返回-1
		*/
	int Media2PSPacket(IN unsigned char *pMediaBuf, IN int s32MediaBufSize, IN StPSInfo *pstPsInfo,
		OUT unsigned char *pPSBuf, OUT int *pPSBufSize);

	/*
	* 功能	:按照大端字节序写16位数据
	* 参数1	：pBuf		【out】	    要写入的buf
	* 参数2	：u16Value	【in】		小端字节序的16位数据
	* 返回值	：成功返回 0， 失败返回 -1
	*/
	unsigned short Write16bit(OUT unsigned char *pBuf, IN unsigned short u16Value);

	/*
	* 功能	:按照大端字节序写32位数据
	* 参数1	：pBuf		【out】	    要写入的buf
	* 参数2	：u32Value	【in】		小端字节序的32位数据
	* 返回值	：成功返回 0， 失败返回 -1
	*/
	unsigned short Write32bit(OUT unsigned char *pBuf, IN unsigned int u32Value);
	/*
	* 功能：写system_clock_reference_base 6字节
	* 参数1：pBuf		【out】	空间指针
	* 参数2：u64SCR		【in】	SCR值
	* 返回值：成功返回0，失败返回-1
	*/
	int WriteSCR(OUT unsigned char *pBuf, IN unsigned long long u64SCR);
	/*
	*功能：写system_header字段 
	* 参数1：pBuf 			【out】	用来保存生成的系统头(SystemHeader字段) 
	* 参数2：pu16BufLen		【out】	系统头的长度 
	* 参数3：u32SysHeadFlag	【in】	系统头的标志位 
	* 参数4：u16HeaderLength	【in】	header_length的值 
	* 参数5：u32RateBound		【in】	rate_bound的值 
	* 参数6：u8AudioBound		【in】	audio_bound的值 
	* 参数7：u8Videobound		【in】	video_bound的值
	* 参数8：u8StreamID		【in】	stream_id的值 
	* 参数8：u8AudioBound		【in】	P-STD_buffer_size_bound的值 
	* 返回值：成功返回0， 失败返回-1 
	*/
	int WriteSystemHeader(unsigned char *pBuf, unsigned short *pu16bufLen, unsigned int u32SysHeadFlag, \
		unsigned short u16HeaderLength, unsigned int u32RateBound, unsigned char u8AudioBound, \
		unsigned char u8Videobound, unsigned char u8StreamID, unsigned short u16PSTDBufSize);


	/*
	* 功能：生成program_stream_map节目流映射.目前按两个节目流来处理(一个H264的视频，一个G711的音频)。以后需要扩展成可配置的。
	* 参数1：u32VideoType  【in】       视频类型
	* 参数2：u32AudioType  【in】       音频类型
	* 参数3：pBuf          【out】      空间指针
	* 参数4：u32BufSize	   【out】      生成的PSM的长度
	* 返回值：成功返回0，失败返回-1
	*/
	int CreatePSM(IN unsigned int u32VideoType, IN unsigned int u32AudioType, OUT unsigned char *pBuf, OUT unsigned int *pBufSize);
	/*
	* 功能：将给定的媒体数据封装成PES数据(如果媒体数据长度大于一包PES长度要求，会进行分包处理，
	*      这时一帧媒体数据会返回多个PES包组成的PES流)
	* 参数1：pMediaBuf        [in]    一帧媒体数据(可能是视频可能是音频)
	* 参数2：s32MediaBufSize  [in]    一帧媒体数据的长度
	* 参数3：pstPesInfo       [in]    封装PES需要的信息
	* 参数4：pPESBuf          [out]   封装后的PES流
	* 参数5：pPESBufSize      [out]   封装后PES流的长度
	* 返回值：成功返回0，失败返回-1
	*/
	int Media2PESStream(IN unsigned char *pMediaBuf, IN int s32MediaBufSize, IN StPESInfo *pstPesInfo, \
		OUT unsigned char *pPESBuf, OUT int *pPESBufSize);
	/*
	* 功能：将分包后的媒体数据(此时的媒体数据不一定时一帧，可能时一帧拆成的多个部分)封装成PES包。
	* 参数1：pMediaBuf        [in]    分包后的媒体数据
	* 参数2：s32MediaBufSize  [in]    分包后媒体数据的长度
	* 参数3：pstPesInfo       [in]    封装PES需要的信息
	* 参数4：pPESBuf          [out]   封装后的PES包
	* 参数5：pPESBufSize      [out]   封装后PES包的长度
	* 返回值：成功返回0，失败返回-1
	*/
	int Media2PESPacket(IN unsigned char *pMediaBuf, IN int s32MediaBufSize, IN StPESInfo *pstPesInfo, \
		OUT unsigned char *pPESBuf, OUT int *pPESBufSize);

	/*
	* 功能：计算PES包头中PES_header_data_length字段的长度
	* 参数1：pstPesInfo       [in]    封装PES需要的信息
	* 返回值：成功返回PES_header_data_length字段的长度，失败返回-1
	*/
	int CalculatePESHeaderDataLength(IN StPESInfo *pstPesInfo);
	/*
	* 功能	：将64位的时间戳按照PES的协议要求，写入给定的空间
	* 参数1	：pBuf			【out】	    要写入的buf
	* 参数2	：u8ID			【in】		时间戳ID，取值为0x01,0x02,0x03.不同的取值见协议写时间戳部分
	* 参数3	：u64TimeStamp	【in】		64位表示的时间戳
	* 返回值	：成功返回 0， 失败返回 -1
	*/
	unsigned short WriteTimeStamp(OUT unsigned char *pBuf, IN unsigned char u8ID, IN unsigned long long u64TimeStamp);

protected:
	CBuffer *m_pBufferH264;

	int m_nTimeStamp;
	int m_nW;
	int m_nH;
	int m_nFPS;
	int m_nBitRate;
};
