
#ifndef VIVS_MEDIA_DEF_H
#define VIVS_MEDIA_DEF_H

#ifdef WIN32
#ifdef VIVSPLAYSDK_EXPORTS
#define MEDIASDK_API __declspec(dllexport)
#else
#define MEDIASDK_API __declspec(dllimport)
#endif
#else
#define MEDIASDK_API
#endif
//#define QT_OPENGL
/*
#define FTMV_FLAG ('F'<<0|'T'<<8|'M'<<16|'V'<<24)
#define FTMV_SIZE 4

#define GW_AV_STREAM_END_FLAG  "avdatastreamends0101"
#define GW_MAX_AV_STREAM_END_FLAG_LEN 20

typedef struct _sAvStreamInfo  //sizeof:48
{
	unsigned char    aucAvStreamEndFlag[GW_MAX_AV_STREAM_END_FLAG_LEN];
	unsigned char    ucMediaType;			//			音频还是视频  0为音频，非0为视频
	unsigned char    ucBoardType;			//		单板类型
	unsigned char    ucVideoCodecType;		//视频编码类型
	unsigned char    ucResolution;			//		视频分辨率
	unsigned char    ucIIntrval;			//			视频I帧间隔
	unsigned int   ulBitrate;				//		视频比特率
	unsigned int    ulFramerate;			//		视频帧率
	unsigned char    ucAudioCodecType;
	unsigned int    ulAudioSamplerate;		//音频采样频率
	unsigned int    ulAudioBitrate;			//	音频比特率
}sAvStreamInfo;
#define GW_AV_DATA_TYPE_AUDIO 0
#define GW_AV_DATA_TYPE_VIDEO 1

typedef struct _sVivsAbsoluteTime
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;

	unsigned short wDayofWeek;
	unsigned short wDayofYear;
}sVivsAbsoluteTime;

#define GW_AV_END_FLAG_LENGTH 13
#define GW_MAX_AV_STREAM_END_FLAG_LEN 20
typedef struct _sAvDataEnd
{//20
	char     aucAvDataEndFlag[GW_MAX_AV_STREAM_END_FLAG_LEN];
	sVivsAbsoluteTime stOsdTime; //16
	unsigned int   ulSeq;//4
	unsigned int    ulSensorNo;   
}sAvDataEnd;
#define GW_AV_END_FLAG  "avdataend0101"
*/
typedef enum _eFrameType
{
	eFT_Unkown,
	eFT_Video=0,
	eFT_I ,
	eFT_P ,
	eFT_B ,
	eFT_SPS,
	eFT_PPS,
	eFT_Audio=100,
}eFrameType;


//编码
typedef enum _eVivsStreamType{
	eVST_Unkown,
	eVST_HK, //HK
	eVST_DH, //DH
	eVST_VivsH264, //vivs打包的 h264, "VH26"
	eVST_VivsPSH264,   //vivs打包的 ps h264 "VPH2"
	eVST_StdH264,  //标准h264 裸码流 "H264"

}eVivsStreamType;
typedef enum _eVivsCodecId{
	eCVID_Unkown,
	eVCID_HK,
	eVCID_DH,
	eVCID_H264,
	eVCID_Mpeg4,

	eVCID_Audio=1000,
	eVCID_ALaw,
	eVCID_ULaw,
	eVCID_G729,
}eVivsCodecId;
//

typedef struct _sVivsVideoInfo{
	int m_nWidth;
	int m_nHeight;
	int m_nPixFormat;
	int m_nBitRate;
	int m_nGopSize;
	int m_nCodecId;
}sVivsVideoInfo;
typedef struct _sVivsAudioInfo{
	int m_nSampleRate;
	int m_nChannels;
	int m_nSampleFormat;
	int m_nBitRate;
	int m_nCodecId;
}sVivsAudioInfo;
typedef struct _sVivsStreamInfo{
	int m_nStreamType; //// HK, DH, HH, VL, STRH, JCO 
	int m_nDataType;   // video, audio , av, ...
	unsigned int m_nStreamTag;// HK, DH, HH, VL, STRH, JCO ,标记厂家. 
	sVivsAudioInfo m_audioInfo;
	sVivsVideoInfo m_videoInfo;
}sVivsStreamInfo;


#ifndef int64_t
typedef long long int64_t;
#endif

typedef char int8 ;
typedef unsigned char uint8 ;
typedef short int int16 ;
typedef unsigned short int uint16;
typedef int  int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

#ifndef WIN32
#define HWND void*
#endif

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif

//视频帧信息
typedef struct _sVideoFrameInfo{
	int nWidth;
	int nHeight;
	int nCodecId;
	int nPixFormat;
	int nFps;
	int nBps;
	int nGop;
	int nPts;
	int nDts;
}sVideoFrameInfo;
//音频帧信息
typedef struct _sAduioFrameInfo{

}sAudioFrameInfo;
//



//

#define FTMV_FLAG ('F'<<0|'T'<<8|'M'<<16|'V'<<24)
#define FTMV_SIZE 4

#define GW_AV_STREAM_END_FLAG  "avdatastreamends0101"
#define GW_MAX_AV_STREAM_END_FLAG_LEN 20

typedef struct _sAvStreamInfo  //sizeof:48
{
	unsigned char    aucAvStreamEndFlag[GW_MAX_AV_STREAM_END_FLAG_LEN];
	unsigned char    ucMediaType;			//			音频还是视频  0为音频，非0为视频
	unsigned char    ucBoardType;			//		单板类型
	unsigned char    ucVideoCodecType;		//视频编码类型
	unsigned char    ucResolution;			//		视频分辨率
	unsigned char    ucIIntrval;			//			视频I帧间隔
	unsigned int   ulBitrate;				//		视频比特率
	unsigned int    ulFramerate;			//		视频帧率
	unsigned char    ucAudioCodecType;
	unsigned int    ulAudioSamplerate;		//音频采样频率
	unsigned int    ulAudioBitrate;			//	音频比特率
}sAvStreamInfo;
#define GW_AV_DATA_TYPE_AUDIO 0
#define GW_AV_DATA_TYPE_VIDEO 1

typedef struct _sVivsAbsoluteTime
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;

	unsigned short wDayofWeek;
	unsigned short wDayofYear;
}sVivsAbsoluteTime;

#define GW_AV_END_FLAG_LENGTH 13
#define GW_MAX_AV_STREAM_END_FLAG_LEN 20
typedef struct _sAvDataEnd
{//20
	char     aucAvDataEndFlag[GW_MAX_AV_STREAM_END_FLAG_LEN];
	sVivsAbsoluteTime stOsdTime; //16
	unsigned int   ulSeq;//4
	unsigned int    ulSensorNo;   
}sAvDataEnd;
#define GW_AV_END_FLAG  "avdataend0101"

#define VIVS_TIME_FLAG ('z'<<0|'x'<<8|'e'<<16|'l'<<24)
typedef union _sVivsTime
{
	int  time;
	struct time_64
	{
		int time_low;
		int time_high;
	}time64;
}sVivsTime;

/* Record Content */
typedef struct _sVivsTimeStamp
{
	unsigned int  uKey;   // before each data package, is a magic words, should be SFUNIT_PACKAGE_HEAD
	unsigned short  uLen;   // the data package length, the data len must less than 65535.
	unsigned short   u16MilliSecond; // the data reveived time offset in second, in millisecond
	sVivsTime  u32Seconds;     // the data reveived time, it is a absult time, in second
}sVivsTimeStamp;

typedef enum _eAvDataEndType{
	eADET_NotEnd,
	eADET_Video,
	eADET_Audio,
}eAvDataEndType;
//
// typedef void (__stdcall *FilterCB_Progress)(float fSpeed, int64_t s64CurPos,  int64_t s64Total, void *pUserData, void *pReversed);
// typedef void (__stdcall *FilterCB_Finished)(void *pUserData, void *pReversed);
// typedef void (__stdcall *FilterCB_Event)(void *pEvent, int nEventLen, void *pUserData, void *pReversed);

//
#endif
