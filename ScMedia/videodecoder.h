#ifndef H264_h_2326
#define H264_h_2326


#include <windows.h>
#include <stdio.h>
#include "frame.h"
#define FFMPEG_52
#ifdef FFMPEG_52
#include "libavcodec/avcodec-52.h"
#include "libswscale/swscale.h"
#elif defined(FFMPEG_80)
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#endif

#define EXT_BUFF_SIZE (1024 *1024)

typedef unsigned char uint8_t;
class VideoCodec
{
public:
	VideoCodec(CodecID id);
    bool decode(const unsigned char* data, int len);
	void SaveAsBMP(AVFrame *pFrameRGB, int width, int height, char* path, int bpp);
    int parse(uint8_t **poutbuf, int *poutbuf_size,
        const uint8_t *buf, int buf_size,
        int64_t pts, int64_t dts);
	~VideoCodec();
    CodecID id;

	class CodecDll
	{
	public:
		CodecDll();
         bool  init();
         void freeDll();
		~CodecDll();

		//============dll =======
        typedef void (*dll_av_log_set_callback)(void (*)(void*, int, const char*, va_list));
		typedef void (*dll_avcodec_init)(void);
		typedef AVCodec* (*dll_avcodec_find_decoder)(enum CodecID id);

		typedef void (*dll_avcodec_register_all)(void);
		typedef AVCodecContext* (*dll_avcodec_alloc_context)(void) ;
		typedef AVFrame*(*dll_avcodec_alloc_frame)(void);
		typedef int (*dll_avcodec_open)(AVCodecContext *avctx, AVCodec *codec);
		typedef int (*dll_avcodec_close)(AVCodecContext *avctx);
		typedef void (*dll_av_free)(void *ptr);
		typedef int (*dll_avcodec_decode_video)(AVCodecContext *avctx, AVFrame *picture, int *got_picture_ptr, uint8_t *buf, int buf_size) ;
		//typedef int (*dll_img_convert)(AVPicture *dst, int dst_pix_fmt,	const AVPicture *src, int pix_fmt, int width, int height);
		typedef void* (*dll_av_malloc)(unsigned int size);
		typedef int (*dll_avpicture_get_size)(int pix_fmt, int width, int height);
        typedef int (*dll_av_parser_parse)(AVCodecParserContext *s,
            AVCodecContext *avctx,
            uint8_t **poutbuf, int *poutbuf_size,
            const uint8_t *buf, int buf_size,
            int64_t pts, int64_t dts);
        typedef AVCodecParserContext* (*dll_av_parser_init)(int codec_id);
        typedef void* (*dll_av_parser_close)(AVCodecParserContext *s);
        typedef void* (*dll_sws_freeContext)(struct SwsContext *swsContext);

         typedef struct SwsContext *(*dll_sws_getContext)(int srcW, int srcH, int srcFormat, int dstW, int dstH, int dstFormat, int flags,
             SwsFilter *srcFilter, SwsFilter *dstFilter, double *param);
         typedef int (*dll_sws_scale)(struct SwsContext *context, uint8_t* src[], int srcStride[], int srcSliceY,
             int srcSliceH, uint8_t* dst[], int dstStride[]);
 
         dll_sws_freeContext sws_freeContext;
         dll_sws_getContext sws_getContext;
         dll_sws_scale sws_scale;
        dll_av_log_set_callback av_log_set_callback;
		dll_avcodec_init avcodec_init;
		dll_avcodec_find_decoder avcodec_find_decoder;
		dll_avcodec_register_all avcodec_register_all;
		dll_avcodec_alloc_context avcodec_alloc_context ;
		dll_avcodec_alloc_frame avcodec_alloc_frame;
		//dll_img_convert	img_convert;
		dll_avcodec_open avcodec_open;
		dll_avcodec_close avcodec_close;
		dll_av_free av_free;
		dll_av_malloc av_malloc;
		dll_avpicture_get_size avpicture_get_size;
		dll_avcodec_decode_video avcodec_decode_video;
        dll_av_parser_parse av_parser_parse;
        dll_av_parser_init av_parser_init;
        dll_av_parser_close  av_parser_close;

	private:
		HINSTANCE m_dll, m_dll2;
	};

    AVCodecContext *c;
    AVFrame *picture;
    AVCodecParserContext *parser;

    int m_width;
    int m_height;

	//guchar* extended_buf;

    static CodecDll dll;

private:

    static bool m_first_instance;
    void resart_decoder(bool parse_flag);
    void first_instance();
    void VideoCodec::avcodec_dll_init(CodecID ID, bool parse_flag);
    void avcodec_dll_free(bool parse_flag);

	AVCodec *codec;

	CodecID m_id;
};



#endif //H264_h_2326