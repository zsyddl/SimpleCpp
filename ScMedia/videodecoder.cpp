#include "stdafx.h"
#include <windows.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
//#include "../stdafx2.h"
#include ".\videodecoder.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <time.h>

#define  vivs_warning
//static GStaticMutex codec_lock = G_STATIC_MUTEX_INIT;
bool VideoCodec::m_first_instance = TRUE;
VideoCodec::CodecDll VideoCodec::dll;

static void av_util_log_set_callback(void* ptr, int level, const char* fmt, va_list vl)
{
    return;
}

void VideoCodec::first_instance()
{
    //g_static_mutex_lock(&codec_lock);
    dll.init();

    // must be called before using avcodec 
    dll.avcodec_init();

    // register all the codecs (you can also register only the codec you wish to have smaller code
    dll.avcodec_register_all();

    dll.av_log_set_callback(av_util_log_set_callback);
    //g_static_mutex_unlock(&codec_lock);	
}

VideoCodec::CodecDll::CodecDll()
{ 

}

void VideoCodec::CodecDll::freeDll()
{
    ::FreeLibrary(m_dll);
    ::FreeLibrary(m_dll2);
}

bool VideoCodec::CodecDll::init()
{
 	LPTSTR strDLLPath;

	strDLLPath = new TCHAR[128 + 1];

	GetModuleFileName((HINSTANCE)&__ImageBase, strDLLPath, (128 + 1)); 

	size_t len = wcslen(strDLLPath);
	for(; len > 0; len--)
	{
		if(strDLLPath[len - 1] == '\\')
		{
			break;
		}
	}

	strDLLPath[len] = '\0';
	wcscat_s(strDLLPath, 128, L"swscale");

	wcscat_s(strDLLPath, 128, L".dll"); 

	m_dll2  = ::LoadLibrary(strDLLPath);

    if(!m_dll2)
        return false;
    av_log_set_callback = reinterpret_cast<dll_av_log_set_callback>(::GetProcAddress(m_dll2, "av_log_set_callback"));

    if(!av_log_set_callback)
        return false;

    av_free = reinterpret_cast<dll_av_free>(::GetProcAddress(m_dll2, "av_free"));
    if (!av_free)
        return false;

    av_malloc = reinterpret_cast<dll_av_malloc>(::GetProcAddress(m_dll2, "av_malloc"));
    if (!av_malloc)
        return false;

    sws_freeContext = reinterpret_cast<dll_sws_freeContext>(::GetProcAddress(m_dll2, "sws_freeContext"));
    if (!sws_freeContext)
        return false;

    sws_getContext = reinterpret_cast<dll_sws_getContext>(::GetProcAddress(m_dll2, "sws_getContext"));
    if (!sws_getContext)
        return false;

    sws_scale = reinterpret_cast<dll_sws_scale>(::GetProcAddress(m_dll2, "sws_scale"));
    if (!sws_scale)
        return false;

	GetModuleFileName((HINSTANCE)&__ImageBase, strDLLPath, (128 + 1)); 

	len = wcslen(strDLLPath);
	for(; len > 0; len--)
	{
		if(strDLLPath[len - 1] == '\\')
		{
			break;
		}
	}

	strDLLPath[len] = '\0';
#ifdef FFMPEG_52
	wcscat_s(strDLLPath, 128, L"avcodec-52");
#elif defined(FFMPEG_80)
	wcscat_s(strDLLPath, 128, L"avcodec-54");
#endif
	wcscat_s(strDLLPath, 128, L".dll"); 

	m_dll  = ::LoadLibrary(strDLLPath);

	delete[] strDLLPath;

	if(!m_dll)
		return false;
#ifdef FFMPEG_52
	avcodec_init = reinterpret_cast<dll_avcodec_init>(::GetProcAddress(m_dll, "avcodec_init"));
	if(!avcodec_init)
		return false;

	avcodec_find_decoder = reinterpret_cast<dll_avcodec_find_decoder>(::GetProcAddress(m_dll, "avcodec_find_decoder"));
	if(!avcodec_find_decoder)
		return false;

	avcodec_register_all = reinterpret_cast<dll_avcodec_register_all>(::GetProcAddress(m_dll, "avcodec_register_all"));
	if (!avcodec_register_all)
		return false;


	avcodec_alloc_context  = reinterpret_cast<dll_avcodec_alloc_context>(::GetProcAddress(m_dll, "avcodec_alloc_context"));
	if (!avcodec_alloc_context)
		return false;

	avcodec_alloc_frame  = reinterpret_cast<dll_avcodec_alloc_frame>(::GetProcAddress(m_dll, "avcodec_alloc_frame"));
	if (!avcodec_alloc_frame)
		return false;

// 	img_convert = reinterpret_cast<dll_img_convert>(::GetProcAddress(m_dll, "img_convert"));
// 	if (!img_convert)
// 		return false;

	avpicture_get_size = reinterpret_cast<dll_avpicture_get_size>(::GetProcAddress(m_dll, "avpicture_get_size"));
	if (!avpicture_get_size)
		return false;


	avcodec_open  = reinterpret_cast<dll_avcodec_open>(::GetProcAddress(m_dll, "avcodec_open"));
	if (!avcodec_open)
		return false;

	avcodec_close  = reinterpret_cast<dll_avcodec_close>(::GetProcAddress(m_dll, "avcodec_close"));
	if (!avcodec_close)
		return false;	

	avcodec_decode_video = reinterpret_cast<dll_avcodec_decode_video>(::GetProcAddress(m_dll, "avcodec_decode_video"));
	if (!avcodec_decode_video)
        return false;

    av_parser_parse = reinterpret_cast<dll_av_parser_parse>(::GetProcAddress(m_dll, "av_parser_parse"));
    if (!av_parser_parse)
        return false;

    av_parser_init = reinterpret_cast<dll_av_parser_init>(::GetProcAddress(m_dll, "av_parser_init"));
    if (!av_parser_init)
        return false;

    av_parser_close = reinterpret_cast<dll_av_parser_close>(::GetProcAddress(m_dll, "av_parser_close"));
    if (!av_parser_close)
        return false;
#elif defined (FFMPEG_80)
// 	avcodec_init = reinterpret_cast<dll_avcodec_init>(::GetProcAddress(m_dll, "avcodec_register_all"));
// 	if(!avcodec_init)
// 		return false;

	avcodec_find_decoder = reinterpret_cast<dll_avcodec_find_decoder>(::GetProcAddress(m_dll, "avcodec_find_decoder"));
	if(!avcodec_find_decoder)
		return false;

	avcodec_register_all = reinterpret_cast<dll_avcodec_register_all>(::GetProcAddress(m_dll, "avcodec_register_all"));
	if (!avcodec_register_all)
		return false;


	avcodec_alloc_context  = reinterpret_cast<dll_avcodec_alloc_context>(::GetProcAddress(m_dll, "avcodec_alloc_context"));
	if (!avcodec_alloc_context)
		return false;

	avcodec_alloc_frame  = reinterpret_cast<dll_avcodec_alloc_frame>(::GetProcAddress(m_dll, "avcodec_alloc_frame"));
	if (!avcodec_alloc_frame)
		return false;

	// 	img_convert = reinterpret_cast<dll_img_convert>(::GetProcAddress(m_dll, "img_convert"));
	// 	if (!img_convert)
	// 		return false;

	avpicture_get_size = reinterpret_cast<dll_avpicture_get_size>(::GetProcAddress(m_dll, "avpicture_get_size"));
	if (!avpicture_get_size)
		return false;


	avcodec_open  = reinterpret_cast<dll_avcodec_open>(::GetProcAddress(m_dll, "avcodec_open"));
	if (!avcodec_open)
		return false;

	avcodec_close  = reinterpret_cast<dll_avcodec_close>(::GetProcAddress(m_dll, "avcodec_close"));
	if (!avcodec_close)
		return false;	

	avcodec_decode_video = reinterpret_cast<dll_avcodec_decode_video>(::GetProcAddress(m_dll, "avcodec_decode_video"));
	if (!avcodec_decode_video)
		return false;

	av_parser_parse = reinterpret_cast<dll_av_parser_parse>(::GetProcAddress(m_dll, "av_parser_parse"));
	if (!av_parser_parse)
		return false;

	av_parser_init = reinterpret_cast<dll_av_parser_init>(::GetProcAddress(m_dll, "av_parser_init"));
	if (!av_parser_init)
		return false;

	av_parser_close = reinterpret_cast<dll_av_parser_close>(::GetProcAddress(m_dll, "av_parser_close"));
	if (!av_parser_close)
		return false;
#endif
    return true;
}
VideoCodec::CodecDll::~CodecDll()
{
}

void VideoCodec::avcodec_dll_init(CodecID ID, bool parse_flag)
{
   // g_static_mutex_lock(&codec_lock);

    codec = dll.avcodec_find_decoder(ID);

    c = dll.avcodec_alloc_context();
    picture= dll.avcodec_alloc_frame();

    if(parse_flag)
    {
        parser = dll.av_parser_init(ID);
    }

    if(ID != CODEC_ID_MPEG4)
    {
        if(codec->capabilities&CODEC_CAP_TRUNCATED)
            c->flags|= CODEC_FLAG_TRUNCATED;
    }

    dll.avcodec_open(c, codec);

    //c->workaround_bugs = 1;
    //c->error_resilience = FF_ER_CAREFUL;
    //c->error_concealment = 3;

   // g_static_mutex_unlock(&codec_lock);	
}

void VideoCodec::avcodec_dll_free(bool parse_flag)
{
    bool restart_codec = FALSE;

    //g_static_mutex_lock(&codec_lock);

    try
    {
        dll.avcodec_close(c);
    }
    catch(...)
    {
        vivs_warning("avcodec_close fail, assert.");
        restart_codec = TRUE;
    }

    try
    {
        dll.av_free(c);
    }
    catch(...)
    {
        vivs_warning("av_free c fail, assert.");
    }
    try
    {
        dll.av_free(picture);
    }
    catch(...)
    {
        vivs_warning("av_free picture fail, assert.");
    }

    if(parse_flag)
    {
        try
        {
            dll.av_parser_close(parser);
        }
        catch(...)
        {
            vivs_warning("av_parser_close fail, assert.");
        }
    }

#if 0
     if(restart_codec)
     {
         if(!parse_flag)
         {
             try
             {
                 dll.av_parser_close(parser);
             }
             catch(...)
             {
                 vivs_warning("av_parser_close fail, assert.");
             }
         }
 
         dll.freeDll();
         first_instance();
         avcodec_dll_init(id, TRUE);

         //vivs_warning("freeDll");
     }
#endif
   // g_static_mutex_unlock(&codec_lock);
}

VideoCodec::VideoCodec(CodecID ID):
c(0),
m_width(0),
m_height(0),
m_id(ID)
, picture(0)
, parser(0)
{
	if (m_first_instance)
	{
		m_first_instance = FALSE;
        first_instance();
	}

    this->id = ID;

    avcodec_dll_init(ID, TRUE);
    //extended_buf = g_new0(guchar, EXT_BUFF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);
}

VideoCodec::~VideoCodec(void)
{
    avcodec_dll_free(TRUE);
    //g_free(extended_buf);
}

void VideoCodec::resart_decoder(bool parse_flag)
{
    avcodec_dll_free(parse_flag);
    avcodec_dll_init(m_id, parse_flag);
}

#if 0
void VideoCodec::SaveAsBMP(AVFrame *pFrameRGB, int width, int height, char* path, int bpp)
{
	char buf[5] = {0};
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	FILE *fp;

	/*char filename[20] = "c:\\test";
	_itoa (index, buf, 10);
	strcat (filename, buf);
	strcat (filename, ".bmp");*/

	if ( (fp=fopen(path, "wb+")) == NULL )
	{
		g_message("open bmpfile fail");
		return;
	}

	bmpheader.bfType = 0x4d42;
	bmpheader.bfReserved1 = 0;
	bmpheader.bfReserved2 = 0;
	bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8;

	bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.biWidth = width;
	bmpinfo.biHeight = height;
	bmpinfo.biPlanes = 1;
	bmpinfo.biBitCount = bpp;
	bmpinfo.biCompression = BI_RGB;
	bmpinfo.biSizeImage = (width*bpp+31)/32*4*height;
	bmpinfo.biXPelsPerMeter = 100;
	bmpinfo.biYPelsPerMeter = 100;
	bmpinfo.biClrUsed = 0;
	bmpinfo.biClrImportant = 0;

	fwrite (&bmpheader, sizeof(bmpheader), 1, fp);
	fwrite (&bmpinfo, sizeof(bmpinfo), 1, fp);

	for(int j = height - 1; j >= 0; j--)
	{
		fwrite (pFrameRGB->data[0]+ width*j*bpp/8, width*bpp/8, 1, fp);
	}

	fclose(fp);
}
#endif

bool VideoCodec::decode(const unsigned char* inbuf_ptr, int len)
{
	int got_picture = 0;

    try
    {
        dll.avcodec_decode_video(c, picture, &got_picture,(uint8_t *)inbuf_ptr, len);
    }
    catch(...)
    {
        //vivs_warning("decode fail, assert.");
        resart_decoder(FALSE);
        return FALSE;
    }

    if (!got_picture)
    {
        //vivs_warning("len %d", len);
        return FALSE;
    }

	return TRUE;	
}

int VideoCodec::parse(uint8_t **poutbuf, int *poutbuf_size, const uint8_t *buf, int buf_size, int64_t pts, int64_t dts)
{
    int ret = 0;

    //memcpy(extended_buf, buf, buf_size);
    //memset(extended_buf+buf_size, 0 , FF_INPUT_BUFFER_PADDING_SIZE);

    try
    {
        ret = dll.av_parser_parse(parser, c, poutbuf, poutbuf_size, buf, buf_size, 0, 0);
    }
    catch(...)
    {
        vivs_warning("parse fail, assert.");
        ret = -1;
        resart_decoder(TRUE);
    }

    return ret;
}



