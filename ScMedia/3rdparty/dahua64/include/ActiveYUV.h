#ifndef ACTIVEYUV_H
#define ACTIVEYUV_H
#define DLL_ACTIVEYUV_API	extern "C" __declspec(dllexport)

DLL_ACTIVEYUV_API void yv12_to_bgra_mmx( unsigned char * pDestRect,
								int DestRectPitch,
								unsigned char * psrc_y,
								unsigned char * psrc_u,
								unsigned char * psrc_v,
								int YPitch,
								int UVPitch,
								int ImageWidth,
								int ImageHeight,
								int flip);
								
DLL_ACTIVEYUV_API void yv12_to_bgr_mmx( unsigned char * pDestRect,
								int DestRectPitch,
								unsigned char* psrc_y,
								unsigned char* psrc_u,
								unsigned char* psrc_v,
								int YPitch,
								int UVPitch,
								int ImageWidth,
								int ImageHeight,
								int flip);
DLL_ACTIVEYUV_API void emms_mmx();
#endif