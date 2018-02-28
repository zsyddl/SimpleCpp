#pragma once

extern "C"
{

#include "libavutil\mem.h"
#include "libavcodec\avcodec.h"

}

typedef struct H264BSFContext {
	uint8_t  length_size;
	uint8_t  first_idr;
	uint8_t *sps_pps_data;
	uint32_t size;
} H264BSFContext;



class CAvFromatMp4
{
public:
	CAvFromatMp4(void);
	~CAvFromatMp4(void);

	static int Filter(H264BSFContext *ctx,  AVCodecContext *avctx, const char *args,
		uint8_t  **poutbuf, int *poutbuf_size,
		const uint8_t *buf, int      buf_size,
		int keyframe);
};
