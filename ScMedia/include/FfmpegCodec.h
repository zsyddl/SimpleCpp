#pragma once



#ifndef UINT64_C
#define UINT64_C unsigned long long
#endif
extern "C"
{
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
}
#ifndef SAMPLE_FMT_S16
#define SAMPLE_FMT_S16 1
#endif

#ifndef INT64_C
#define INT64_C long long
#endif

#define CODEC_BPS_K (1024)
#define CODEC_BPS_M (1024*1024)

#define SIZE_K (1024)
#define SIZE_M (1024*SIZE_K)
#define SIZE_G (1024*SIZE_M)

typedef long long int64;

typedef enum _eFfmpegDataType{
	eFDT_Video,
	eFDT_Audio,
	eFDT_Text,
}eFfmpegDataType;
typedef enum _eFfmpegFrameType{
	eFFT_I = 0,
	eFFT_B ,
	eFFT_P ,
	eFFT_Param,
	eFFT_Unknown,
}eFfmpegFrameType;

typedef struct _sFfmpegVideoInfo{
	int m_nCodecId;
	int m_nWidth;
	int m_nHeight;
	int m_nPixFormat;
	int m_nBitRate;
	int m_nGopSize;
	int m_nFps;
}sFfmpegVideoInfo;
typedef struct _sFfmpegAudioInfo{
	int m_nCodecId;
	int m_nSampleRate;
	int m_nChannels;
	int m_nSampleFormat;
	int m_nBitRate;
}sFfmpegAudioInfo;
typedef struct _sFfmpegStreamInfo{
	int m_nStreamId;
	int m_nStreamType; // hk h264 dh
	int m_nCodecId;
	int m_nDataType;
	int64 m_s64StartTimestamp;//起始时间戳
	unsigned int m_nCodecTag;
	sFfmpegAudioInfo m_audioInfo;
	sFfmpegVideoInfo m_videoInfo;
	int m_nTimebaseNum;
	int m_nTimebaseDen;
	char m_szName[64];
}sFfmpegStreamInfo;

typedef struct _sFfmpegFrameInfo{
	int m_nDataType;

	int64 m_s64Pts;
	int64 m_s64Dts;
	int64 m_s64Bts; //开始的时间戳
	int   m_nFrameIndex;
	int   m_nFrameType;// I B P;
	int   m_nTimeDen;
	int   m_nTimeNum;
	sFfmpegVideoInfo m_videoInfo;
	sFfmpegAudioInfo m_audioInfo;
	
}sFfmpegFrameInfo;

namespace SimpleCpp{
	class CMutex;
}
using namespace SimpleCpp;

#define FFMPEG_NEW

#ifdef WIN32
#ifdef VIVSMEDIABOX_EXPORTS
#define MEDIASDK_API __declspec(dllexport)
#else
#define MEDIASDK_API __declspec(dllimport)
#endif
#else
#define MEDIASDK_API
#define __stdcall
#endif

class MEDIASDK_API CFfmpegCodec
{
public:
	CFfmpegCodec(void);
	virtual ~CFfmpegCodec(void);
	// nCodecId defined as below , (followed this class )
	int	Open(/*int nCodecId, */bool bDecodeOrEncode=true);
	void Close();
	//
	int SetVideoParam(int nWith, int nHeight, int nPixFormat, int nBitRates);
	int SetVideoParam(sFfmpegVideoInfo &videoInfo);
	int SetAudioParam(sFfmpegAudioInfo &audioInfo);
	sFfmpegVideoInfo &GetVideoParam();
	sFfmpegAudioInfo &GetAudioParam();
	//int InputData(uint8_t *pData, int nLen);

	int Encode(uint8_t *pData, int nLen);
	int Encode(uint8_t *pData, int nLen, uint8_t *pEncodeData, int &nEncodeLen);

	int Decode(uint8_t *pData, int nLen);
	int Decode(uint8_t *pData, int nLen, uint8_t *pDecodeData, int &nDecodeLen);
	bool HasNewPicture();
	int GetVideoFrameSize();
	int GetDecodeData(uint8_t *pData);
	int GetFameInfo(int &nLen, int &nW, int &nH, int &nType);
	int GetTimeBase(int &nTimeBaseNum, int &nTimeBaseDen);
	int GetVideoSize(int &nW, int &nH);
	AVFrame *GetFrame();
	int GetFrameType();
	//复制到对应的 缓存，读音的 pix format nw nH
	int CopyFrame(uint8_t *pImageOut, int nImageLen, int nW, int nH, AVPixelFormat pix_fmt);

	int GetCodecType();
	int GetCodecType(int nCodecId);
	AVCodecContext *GetCodecContext(){return m_pAvCodecContext;};

	void Flush();
	
	void Reset();
	//在编码的时候 才有效
	int64_t GetPts();
	void SetDahuaFlag();
	bool IsKeyFrame();
protected:
	AVCodec *m_pAvCodec;
	AVCodecContext *m_pAvCodecContext;
	//输出的解压缩图片, //输入的未压缩 图片
	AVFrame *m_pAvPicture;
	
	//AVPicture *m_pAvInputPicture;
	bool m_bDecodeOrEncode;
	bool m_bDahuaFlag;
	//
	bool m_bKeyFrame;
	//
	bool m_bNewPicture;

	sFfmpegVideoInfo m_videoInfo;
	sFfmpegAudioInfo m_audioInfo;

	FILE  *m_pFileTest;
	//int m_nCodecId;
	//调用 ffmpeg 库函数的时候 进行 互斥， ffmpeg对多线程 的操作 不好
	//static CMutex *m_pMutexFfmpeg;
};
class CFfmpeg{
public:
	CFfmpeg();
	~CFfmpeg();

	void Lock();
	void Unlock();
protected:
	CMutex *m_pMutexFfmpeg;
};
extern CFfmpeg g_ffmpeg;
/*
audio 
enum AVSampleFormat {
	AV_SAMPLE_FMT_NONE = -1,
	AV_SAMPLE_FMT_U8,          ///< unsigned 8 bits
	AV_SAMPLE_FMT_S16,         ///< signed 16 bits
	AV_SAMPLE_FMT_S32,         ///< signed 32 bits
	AV_SAMPLE_FMT_FLT,         ///< float
	AV_SAMPLE_FMT_DBL,         ///< double

	AV_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
	AV_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
	AV_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
	AV_SAMPLE_FMT_FLTP,        ///< float, planar
	AV_SAMPLE_FMT_DBLP,        ///< double, planar

	AV_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
};*/
/* video  pix format
enum PixelFormat {
	PIX_FMT_NONE= -1,
	PIX_FMT_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
	PIX_FMT_YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
	PIX_FMT_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
	PIX_FMT_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
	PIX_FMT_YUV422P,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
	PIX_FMT_YUV444P,   ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
	PIX_FMT_YUV410P,   ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
	PIX_FMT_YUV411P,   ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
	PIX_FMT_GRAY8,     ///<        Y        ,  8bpp
	PIX_FMT_MONOWHITE, ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
	PIX_FMT_MONOBLACK, ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb
	PIX_FMT_PAL8,      ///< 8 bit with PIX_FMT_RGB32 palette
	PIX_FMT_YUVJ420P,  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV420P and setting color_range
	PIX_FMT_YUVJ422P,  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV422P and setting color_range
	PIX_FMT_YUVJ444P,  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of PIX_FMT_YUV444P and setting color_range
	PIX_FMT_XVMC_MPEG2_MC,///< XVideo Motion Acceleration via common packet passing
	PIX_FMT_XVMC_MPEG2_IDCT,
	PIX_FMT_UYVY422,   ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
	PIX_FMT_UYYVYY411, ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
	PIX_FMT_BGR8,      ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
	PIX_FMT_BGR4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
	PIX_FMT_BGR4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
	PIX_FMT_RGB8,      ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
	PIX_FMT_RGB4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
	PIX_FMT_RGB4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)
	PIX_FMT_NV12,      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
	PIX_FMT_NV21,      ///< as above, but U and V bytes are swapped

	PIX_FMT_ARGB,      ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
	PIX_FMT_RGBA,      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
	PIX_FMT_ABGR,      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
	PIX_FMT_BGRA,      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...

	PIX_FMT_GRAY16BE,  ///<        Y        , 16bpp, big-endian
	PIX_FMT_GRAY16LE,  ///<        Y        , 16bpp, little-endian
	PIX_FMT_YUV440P,   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
	PIX_FMT_YUVJ440P,  ///< planar YUV 4:4:0 full scale (JPEG), deprecated in favor of PIX_FMT_YUV440P and setting color_range
	PIX_FMT_YUVA420P,  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)
	PIX_FMT_VDPAU_H264,///< H.264 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
	PIX_FMT_VDPAU_MPEG1,///< MPEG-1 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
	PIX_FMT_VDPAU_MPEG2,///< MPEG-2 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
	PIX_FMT_VDPAU_WMV3,///< WMV3 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
	PIX_FMT_VDPAU_VC1, ///< VC-1 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
	PIX_FMT_RGB48BE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian
	PIX_FMT_RGB48LE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian

	PIX_FMT_RGB565BE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
	PIX_FMT_RGB565LE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
	PIX_FMT_RGB555BE,  ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0
	PIX_FMT_RGB555LE,  ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0

	PIX_FMT_BGR565BE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
	PIX_FMT_BGR565LE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
	PIX_FMT_BGR555BE,  ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), big-endian, most significant bit to 1
	PIX_FMT_BGR555LE,  ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), little-endian, most significant bit to 1

	PIX_FMT_VAAPI_MOCO, ///< HW acceleration through VA API at motion compensation entry-point, Picture.data[3] contains a vaapi_render_state struct which contains macroblocks as well as various fields extracted from headers
	PIX_FMT_VAAPI_IDCT, ///< HW acceleration through VA API at IDCT entry-point, Picture.data[3] contains a vaapi_render_state struct which contains fields extracted from headers
	PIX_FMT_VAAPI_VLD,  ///< HW decoding through VA API, Picture.data[3] contains a vaapi_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers

	PIX_FMT_YUV420P16LE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
	PIX_FMT_YUV420P16BE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
	PIX_FMT_YUV422P16LE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
	PIX_FMT_YUV422P16BE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
	PIX_FMT_YUV444P16LE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
	PIX_FMT_YUV444P16BE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
	PIX_FMT_VDPAU_MPEG4,  ///< MPEG4 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
	PIX_FMT_DXVA2_VLD,    ///< HW decoding through DXVA2, Picture.data[3] contains a LPDIRECT3DSURFACE9 pointer

	PIX_FMT_RGB444LE,  ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), little-endian, most significant bits to 0
	PIX_FMT_RGB444BE,  ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), big-endian, most significant bits to 0
	PIX_FMT_BGR444LE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), little-endian, most significant bits to 1
	PIX_FMT_BGR444BE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), big-endian, most significant bits to 1
	PIX_FMT_GRAY8A,    ///< 8bit gray, 8bit alpha
	PIX_FMT_BGR48BE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian
	PIX_FMT_BGR48LE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian

	//the following 10 formats have the disadvantage of needing 1 format for each bit depth, thus
	//If you want to support multiple bit depths, then using PIX_FMT_YUV420P16* with the bpp stored seperately
	//is better
	PIX_FMT_YUV420P9BE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
	PIX_FMT_YUV420P9LE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
	PIX_FMT_YUV420P10BE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
	PIX_FMT_YUV420P10LE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
	PIX_FMT_YUV422P10BE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
	PIX_FMT_YUV422P10LE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
	PIX_FMT_YUV444P9BE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
	PIX_FMT_YUV444P9LE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
	PIX_FMT_YUV444P10BE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
	PIX_FMT_YUV444P10LE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
	PIX_FMT_YUV422P9BE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
	PIX_FMT_YUV422P9LE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
	PIX_FMT_VDA_VLD,    ///< hardware decoding through VDA

#ifdef AV_PIX_FMT_ABI_GIT_MASTER
	PIX_FMT_RGBA64BE,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
	PIX_FMT_RGBA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
	PIX_FMT_BGRA64BE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
	PIX_FMT_BGRA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
#endif
	PIX_FMT_GBRP,      ///< planar GBR 4:4:4 24bpp
	PIX_FMT_GBRP9BE,   ///< planar GBR 4:4:4 27bpp, big endian
	PIX_FMT_GBRP9LE,   ///< planar GBR 4:4:4 27bpp, little endian
	PIX_FMT_GBRP10BE,  ///< planar GBR 4:4:4 30bpp, big endian
	PIX_FMT_GBRP10LE,  ///< planar GBR 4:4:4 30bpp, little endian
	PIX_FMT_GBRP16BE,  ///< planar GBR 4:4:4 48bpp, big endian
	PIX_FMT_GBRP16LE,  ///< planar GBR 4:4:4 48bpp, little endian

#ifndef AV_PIX_FMT_ABI_GIT_MASTER
	PIX_FMT_RGBA64BE=0x123,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
	PIX_FMT_RGBA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
	PIX_FMT_BGRA64BE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
	PIX_FMT_BGRA64LE,  ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
#endif
	PIX_FMT_0RGB=0x123+4,      ///< packed RGB 8:8:8, 32bpp, 0RGB0RGB...
	PIX_FMT_RGB0,      ///< packed RGB 8:8:8, 32bpp, RGB0RGB0...
	PIX_FMT_0BGR,      ///< packed BGR 8:8:8, 32bpp, 0BGR0BGR...
	PIX_FMT_BGR0,      ///< packed BGR 8:8:8, 32bpp, BGR0BGR0...
	PIX_FMT_YUVA444P,  ///< planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)

	PIX_FMT_NB,        ///< number of pixel formats, DO NOT USE THIS if you want to link with shared libav* because the number of formats might differ between versions
};
*/
// enum CodecID {
//     CODEC_ID_NONE,
// 
//     /* video codecs */
//     CODEC_ID_MPEG1VIDEO,
//     CODEC_ID_MPEG2VIDEO, ///< preferred ID for MPEG-1/2 video decoding
//     CODEC_ID_MPEG2VIDEO_XVMC,
//     CODEC_ID_H261,
//     CODEC_ID_H263,
//     CODEC_ID_RV10,
//     CODEC_ID_RV20,
//     CODEC_ID_MJPEG,
//     CODEC_ID_MJPEGB,
//     CODEC_ID_LJPEG,
//     CODEC_ID_SP5X,
//     CODEC_ID_JPEGLS,
//     CODEC_ID_MPEG4,
//     CODEC_ID_RAWVIDEO,
//     CODEC_ID_MSMPEG4V1,
//     CODEC_ID_MSMPEG4V2,
//     CODEC_ID_MSMPEG4V3,
//     CODEC_ID_WMV1,
//     CODEC_ID_WMV2,
//     CODEC_ID_H263P,
//     CODEC_ID_H263I,
//     CODEC_ID_FLV1,
//     CODEC_ID_SVQ1,
//     CODEC_ID_SVQ3,
//     CODEC_ID_DVVIDEO,
//     CODEC_ID_HUFFYUV,
//     CODEC_ID_CYUV,
//     CODEC_ID_H264,
//     CODEC_ID_INDEO3,
//     CODEC_ID_VP3,
//     CODEC_ID_THEORA,
//     CODEC_ID_ASV1,
//     CODEC_ID_ASV2,
//     CODEC_ID_FFV1,
//     CODEC_ID_4XM,
//     CODEC_ID_VCR1,
//     CODEC_ID_CLJR,
//     CODEC_ID_MDEC,
//     CODEC_ID_ROQ,
//     CODEC_ID_INTERPLAY_VIDEO,
//     CODEC_ID_XAN_WC3,
//     CODEC_ID_XAN_WC4,
//     CODEC_ID_RPZA,
//     CODEC_ID_CINEPAK,
//     CODEC_ID_WS_VQA,
//     CODEC_ID_MSRLE,
//     CODEC_ID_MSVIDEO1,
//     CODEC_ID_IDCIN,
//     CODEC_ID_8BPS,
//     CODEC_ID_SMC,
//     CODEC_ID_FLIC,
//     CODEC_ID_TRUEMOTION1,
//     CODEC_ID_VMDVIDEO,
//     CODEC_ID_MSZH,
//     CODEC_ID_ZLIB,
//     CODEC_ID_QTRLE,
//     CODEC_ID_SNOW,
//     CODEC_ID_TSCC,
//     CODEC_ID_ULTI,
//     CODEC_ID_QDRAW,
//     CODEC_ID_VIXL,
//     CODEC_ID_QPEG,
//     CODEC_ID_PNG,
//     CODEC_ID_PPM,
//     CODEC_ID_PBM,
//     CODEC_ID_PGM,
//     CODEC_ID_PGMYUV,
//     CODEC_ID_PAM,
//     CODEC_ID_FFVHUFF,
//     CODEC_ID_RV30,
//     CODEC_ID_RV40,
//     CODEC_ID_VC1,
//     CODEC_ID_WMV3,
//     CODEC_ID_LOCO,
//     CODEC_ID_WNV1,
//     CODEC_ID_AASC,
//     CODEC_ID_INDEO2,
//     CODEC_ID_FRAPS,
//     CODEC_ID_TRUEMOTION2,
//     CODEC_ID_BMP,
//     CODEC_ID_CSCD,
//     CODEC_ID_MMVIDEO,
//     CODEC_ID_ZMBV,
//     CODEC_ID_AVS,
//     CODEC_ID_SMACKVIDEO,
//     CODEC_ID_NUV,
//     CODEC_ID_KMVC,
//     CODEC_ID_FLASHSV,
//     CODEC_ID_CAVS,
//     CODEC_ID_JPEG2000,
//     CODEC_ID_VMNC,
//     CODEC_ID_VP5,
//     CODEC_ID_VP6,
//     CODEC_ID_VP6F,
//     CODEC_ID_TARGA,
//     CODEC_ID_DSICINVIDEO,
//     CODEC_ID_TIERTEXSEQVIDEO,
//     CODEC_ID_TIFF,
//     CODEC_ID_GIF,
//     CODEC_ID_DXA,
//     CODEC_ID_DNXHD,
//     CODEC_ID_THP,
//     CODEC_ID_SGI,
//     CODEC_ID_C93,
//     CODEC_ID_BETHSOFTVID,
//     CODEC_ID_PTX,
//     CODEC_ID_TXD,
//     CODEC_ID_VP6A,
//     CODEC_ID_AMV,
//     CODEC_ID_VB,
//     CODEC_ID_PCX,
//     CODEC_ID_SUNRAST,
//     CODEC_ID_INDEO4,
//     CODEC_ID_INDEO5,
//     CODEC_ID_MIMIC,
//     CODEC_ID_RL2,
//     CODEC_ID_ESCAPE124,
//     CODEC_ID_DIRAC,
//     CODEC_ID_BFI,
//     CODEC_ID_CMV,
//     CODEC_ID_MOTIONPIXELS,
//     CODEC_ID_TGV,
//     CODEC_ID_TGQ,
//     CODEC_ID_TQI,
//     CODEC_ID_AURA,
//     CODEC_ID_AURA2,
//     CODEC_ID_V210X,
//     CODEC_ID_TMV,
//     CODEC_ID_V210,
//     CODEC_ID_DPX,
//     CODEC_ID_MAD,
//     CODEC_ID_FRWU,
//     CODEC_ID_FLASHSV2,
//     CODEC_ID_CDGRAPHICS,
//     CODEC_ID_R210,
//     CODEC_ID_ANM,
//     CODEC_ID_BINKVIDEO,
//     CODEC_ID_IFF_ILBM,
//     CODEC_ID_IFF_BYTERUN1,
//     CODEC_ID_KGV1,
//     CODEC_ID_YOP,
//     CODEC_ID_VP8,
//     CODEC_ID_PICTOR,
//     CODEC_ID_ANSI,
//     CODEC_ID_A64_MULTI,
//     CODEC_ID_A64_MULTI5,
//     CODEC_ID_R10K,
//     CODEC_ID_MXPEG,
//     CODEC_ID_LAGARITH,
//     CODEC_ID_PRORES,
//     CODEC_ID_JV,
//     CODEC_ID_DFA,
//     CODEC_ID_WMV3IMAGE,
//     CODEC_ID_VC1IMAGE,
//     CODEC_ID_UTVIDEO,
//     CODEC_ID_BMV_VIDEO,
//     CODEC_ID_VBLE,
//     CODEC_ID_DXTORY,
//     CODEC_ID_V410,
//     CODEC_ID_XWD,
//     CODEC_ID_CDXL,
//     CODEC_ID_Y41P       = MKBETAG('Y','4','1','P'),
//     CODEC_ID_ESCAPE130  = MKBETAG('E','1','3','0'),
//     CODEC_ID_AVRP       = MKBETAG('A','V','R','P'),
// 
//     CODEC_ID_G2M        = MKBETAG( 0 ,'G','2','M'),
//     CODEC_ID_AYUV       = MKBETAG('A','Y','U','V'),
//     CODEC_ID_V308       = MKBETAG('V','3','0','8'),
//     CODEC_ID_V408       = MKBETAG('V','4','0','8'),
//     CODEC_ID_YUV4       = MKBETAG('Y','U','V','4'),
// 
//     /* various PCM "codecs" */
//     CODEC_ID_FIRST_AUDIO = 0x10000,     ///< A dummy id pointing at the start of audio codecs
//     CODEC_ID_PCM_S16LE = 0x10000,
//     CODEC_ID_PCM_S16BE,
//     CODEC_ID_PCM_U16LE,
//     CODEC_ID_PCM_U16BE,
//     CODEC_ID_PCM_S8,
//     CODEC_ID_PCM_U8,
//     CODEC_ID_PCM_MULAW,
//     CODEC_ID_PCM_ALAW,
//     CODEC_ID_PCM_S32LE,
//     CODEC_ID_PCM_S32BE,
//     CODEC_ID_PCM_U32LE,
//     CODEC_ID_PCM_U32BE,
//     CODEC_ID_PCM_S24LE,
//     CODEC_ID_PCM_S24BE,
//     CODEC_ID_PCM_U24LE,
//     CODEC_ID_PCM_U24BE,
//     CODEC_ID_PCM_S24DAUD,
//     CODEC_ID_PCM_ZORK,
//     CODEC_ID_PCM_S16LE_PLANAR,
//     CODEC_ID_PCM_DVD,
//     CODEC_ID_PCM_F32BE,
//     CODEC_ID_PCM_F32LE,
//     CODEC_ID_PCM_F64BE,
//     CODEC_ID_PCM_F64LE,
//     CODEC_ID_PCM_BLURAY,
//     CODEC_ID_PCM_LXF,
//     CODEC_ID_S302M,
//     CODEC_ID_PCM_S8_PLANAR,
// 
//     /* various ADPCM codecs */
//     CODEC_ID_ADPCM_IMA_QT = 0x11000,
//     CODEC_ID_ADPCM_IMA_WAV,
//     CODEC_ID_ADPCM_IMA_DK3,
//     CODEC_ID_ADPCM_IMA_DK4,
//     CODEC_ID_ADPCM_IMA_WS,
//     CODEC_ID_ADPCM_IMA_SMJPEG,
//     CODEC_ID_ADPCM_MS,
//     CODEC_ID_ADPCM_4XM,
//     CODEC_ID_ADPCM_XA,
//     CODEC_ID_ADPCM_ADX,
//     CODEC_ID_ADPCM_EA,
//     CODEC_ID_ADPCM_G726,
//     CODEC_ID_ADPCM_CT,
//     CODEC_ID_ADPCM_SWF,
//     CODEC_ID_ADPCM_YAMAHA,
//     CODEC_ID_ADPCM_SBPRO_4,
//     CODEC_ID_ADPCM_SBPRO_3,
//     CODEC_ID_ADPCM_SBPRO_2,
//     CODEC_ID_ADPCM_THP,
//     CODEC_ID_ADPCM_IMA_AMV,
//     CODEC_ID_ADPCM_EA_R1,
//     CODEC_ID_ADPCM_EA_R3,
//     CODEC_ID_ADPCM_EA_R2,
//     CODEC_ID_ADPCM_IMA_EA_SEAD,
//     CODEC_ID_ADPCM_IMA_EA_EACS,
//     CODEC_ID_ADPCM_EA_XAS,
//     CODEC_ID_ADPCM_EA_MAXIS_XA,
//     CODEC_ID_ADPCM_IMA_ISS,
//     CODEC_ID_ADPCM_G722,
//     CODEC_ID_ADPCM_IMA_APC,
// 
//     /* AMR */
//     CODEC_ID_AMR_NB = 0x12000,
//     CODEC_ID_AMR_WB,
// 
//     /* RealAudio codecs*/
//     CODEC_ID_RA_144 = 0x13000,
//     CODEC_ID_RA_288,
// 
//     /* various DPCM codecs */
//     CODEC_ID_ROQ_DPCM = 0x14000,
//     CODEC_ID_INTERPLAY_DPCM,
//     CODEC_ID_XAN_DPCM,
//     CODEC_ID_SOL_DPCM,
// 
//     /* audio codecs */
//     CODEC_ID_MP2 = 0x15000,
//     CODEC_ID_MP3, ///< preferred ID for decoding MPEG audio layer 1, 2 or 3
//     CODEC_ID_AAC,
//     CODEC_ID_AC3,
//     CODEC_ID_DTS,
//     CODEC_ID_VORBIS,
//     CODEC_ID_DVAUDIO,
//     CODEC_ID_WMAV1,
//     CODEC_ID_WMAV2,
//     CODEC_ID_MACE3,
//     CODEC_ID_MACE6,
//     CODEC_ID_VMDAUDIO,
//     CODEC_ID_FLAC,
//     CODEC_ID_MP3ADU,
//     CODEC_ID_MP3ON4,
//     CODEC_ID_SHORTEN,
//     CODEC_ID_ALAC,
//     CODEC_ID_WESTWOOD_SND1,
//     CODEC_ID_GSM, ///< as in Berlin toast format
//     CODEC_ID_QDM2,
//     CODEC_ID_COOK,
//     CODEC_ID_TRUESPEECH,
//     CODEC_ID_TTA,
//     CODEC_ID_SMACKAUDIO,
//     CODEC_ID_QCELP,
//     CODEC_ID_WAVPACK,
//     CODEC_ID_DSICINAUDIO,
//     CODEC_ID_IMC,
//     CODEC_ID_MUSEPACK7,
//     CODEC_ID_MLP,
//     CODEC_ID_GSM_MS, /* as found in WAV */
//     CODEC_ID_ATRAC3,
//     CODEC_ID_VOXWARE,
//     CODEC_ID_APE,
//     CODEC_ID_NELLYMOSER,
//     CODEC_ID_MUSEPACK8,
//     CODEC_ID_SPEEX,
//     CODEC_ID_WMAVOICE,
//     CODEC_ID_WMAPRO,
//     CODEC_ID_WMALOSSLESS,
//     CODEC_ID_ATRAC3P,
//     CODEC_ID_EAC3,
//     CODEC_ID_SIPR,
//     CODEC_ID_MP1,
//     CODEC_ID_TWINVQ,
//     CODEC_ID_TRUEHD,
//     CODEC_ID_MP4ALS,
//     CODEC_ID_ATRAC1,
//     CODEC_ID_BINKAUDIO_RDFT,
//     CODEC_ID_BINKAUDIO_DCT,
//     CODEC_ID_AAC_LATM,
//     CODEC_ID_QDMC,
//     CODEC_ID_CELT,
//     CODEC_ID_G723_1,
//     CODEC_ID_G729,
//     CODEC_ID_8SVX_EXP,
//     CODEC_ID_8SVX_FIB,
//     CODEC_ID_BMV_AUDIO,
//     CODEC_ID_FFWAVESYNTH = MKBETAG('F','F','W','S'),
//     CODEC_ID_8SVX_RAW   = MKBETAG('8','S','V','X'),
//     CODEC_ID_SONIC       = MKBETAG('S','O','N','C'),
//     CODEC_ID_SONIC_LS    = MKBETAG('S','O','N','L'),
// 
//     /* subtitle codecs */
//     CODEC_ID_FIRST_SUBTITLE = 0x17000,          ///< A dummy ID pointing at the start of subtitle codecs.
//     CODEC_ID_DVD_SUBTITLE = 0x17000,
//     CODEC_ID_DVB_SUBTITLE,
//     CODEC_ID_TEXT,  ///< raw UTF-8 text
//     CODEC_ID_XSUB,
//     CODEC_ID_SSA,
//     CODEC_ID_MOV_TEXT,
//     CODEC_ID_HDMV_PGS_SUBTITLE,
//     CODEC_ID_DVB_TELETEXT,
//     CODEC_ID_SRT,
//     CODEC_ID_MICRODVD   = MKBETAG('m','D','V','D'),
// 
//     /* other specific kind of codecs (generally used for attachments) */
//     CODEC_ID_FIRST_UNKNOWN = 0x18000,           ///< A dummy ID pointing at the start of various fake codecs.
//     CODEC_ID_TTF = 0x18000,
//     CODEC_ID_BINTEXT    = MKBETAG('B','T','X','T'),
//     CODEC_ID_XBIN       = MKBETAG('X','B','I','N'),
//     CODEC_ID_IDF        = MKBETAG( 0 ,'I','D','F'),
// 
//     CODEC_ID_PROBE = 0x19000, ///< codec_id is not known (like CODEC_ID_NONE) but lavf should attempt to identify it
// 
//     CODEC_ID_MPEG2TS = 0x20000, /**< _FAKE_ codec to indicate a raw MPEG-2 TS
//                                 * stream (only used by libavformat) */
//     CODEC_ID_MPEG4SYSTEMS = 0x20001, /**< _FAKE_ codec to indicate a MPEG-4 Systems
//                                 * stream (only used by libavformat) */
//     CODEC_ID_FFMETADATA = 0x21000,   ///< Dummy codec for streams containing only metadata information.
// };

