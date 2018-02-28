#include "StdAfx.h"
#include "AvFromatMp4.h"

CAvFromatMp4::CAvFromatMp4(void)
{
}

CAvFromatMp4::~CAvFromatMp4(void)
{

}
#define EINVAL          22
#define ENOMEM          12
#define AV_RB16(x)  ((((const uint8_t*)(x))[0] << 8) | ((const uint8_t*)(x))[1])
#define AV_RB32(x)  ((((const uint8_t*)(x))[0] << 24) | \
(((const uint8_t*)(x))[1] << 16)| \
(((const uint8_t*)(x))[2] <<  8)| \
((const uint8_t*)(x))[3])

#define AV_RB32(x)  ((((const uint8_t*)(x))[0] << 24) | \
(((const uint8_t*)(x))[1] << 16) | \
(((const uint8_t*)(x))[2] <<  8) | \
((const uint8_t*)(x))[3])

#define AV_WB32(p, d) do {    \
((uint8_t*)(p))[3] = (d);     \
((uint8_t*)(p))[2] = (d)>>8;  \
((uint8_t*)(p))[1] = (d)>>16; \
((uint8_t*)(p))[0] = (d)>>24; } while(0)





static void alloc_and_copy(uint8_t **poutbuf,          int *poutbuf_size,
						   const uint8_t *sps_pps, uint32_t sps_pps_size,
						   const uint8_t *in,      uint32_t in_size)
{
	uint32_t offset = *poutbuf_size;
	uint8_t nal_header_size =  4;

	*poutbuf_size += sps_pps_size+in_size+nal_header_size;

	*poutbuf = (unsigned char *)av_realloc(*poutbuf, *poutbuf_size);
	if (*poutbuf==NULL)
		return ;

	if (sps_pps)
		memcpy(*poutbuf+offset, sps_pps, sps_pps_size);
	memcpy(*poutbuf+sps_pps_size+nal_header_size+offset, in, in_size);
	//if (!offset)
	AV_WB32(*poutbuf+offset+sps_pps_size, 1);
	//else
	//{
	// (*poutbuf+offset+sps_pps_size)[0] = (*poutbuf+offset+sps_pps_size)[1] = 0;
	// (*poutbuf+offset+sps_pps_size)[2] = 1;
	//}
}

int CAvFromatMp4::Filter(H264BSFContext *ctx,  AVCodecContext *avctx, const char *args,
						 uint8_t  **poutbuf, int *poutbuf_size, 
						 const uint8_t *buf, int      buf_size,int keyframe)
{

		uint8_t unit_type;
		uint32_t nal_size, cumul_size = 0;

		if (!avctx->extradata || avctx->extradata_size < 6)
		{
			*poutbuf = (uint8_t*) buf;
			*poutbuf_size = buf_size;
			return 0;
		}

		if (!ctx->sps_pps_data)
		{
			uint16_t unit_size;
			uint32_t total_size = 0;
			uint8_t *out = NULL, unit_nb, sps_done = 0;
			const uint8_t *extradata = avctx->extradata+4;
			static const uint8_t nalu_header[4] = {0, 0, 0, 1};


			ctx->length_size = (*extradata++ & 0x3) + 1;
			if (ctx->length_size == 3)
				return AVERROR(EINVAL);


			unit_nb = *extradata++ & 0x1f;
			if (!unit_nb)
			{
				unit_nb = *extradata++;
				sps_done++;
			}
			while (unit_nb--)
			{
				unit_size = AV_RB16(extradata);
				total_size += unit_size+4;
				if (extradata+2+unit_size > avctx->extradata+avctx->extradata_size)
				{
					av_free(out);
					return AVERROR(EINVAL);
				}
				out = (unsigned char *)av_realloc(out, total_size);
				if (!out)
					return AVERROR(ENOMEM);
				memcpy(out+total_size-unit_size-4, nalu_header, 4);
				memcpy(out+total_size-unit_size,   extradata+2, unit_size);
				extradata += 2+unit_size;

				if (!unit_nb && !sps_done++)
					unit_nb = *extradata++;
			}

			ctx->sps_pps_data = out;
			ctx->size = total_size;
			ctx->first_idr = 1;
			//char str[512]={0};
			//                           memcpy(str, out, total_size);
		}
		*poutbuf_size = 0;
		*poutbuf = NULL;

		do
		{
			if (ctx->length_size == 1)
				nal_size = buf[0];
			else if (ctx->length_size == 2)
				nal_size = AV_RB16(buf);
			else
			{
				nal_size = AV_RB32(buf);


			}

			buf += ctx->length_size;
			unit_type = *buf & 0x1f;


			if ( unit_type != 6 )
			{
				//zsyddl
				//alloc_and_copy(poutbuf, poutbuf_size, ctx->sps_pps_data, ctx->size, buf, 0);
				ctx->first_idr = 0;
			}
			//else
			//{
			// alloc_and_copy(poutbuf, poutbuf_size, NULL, 0,  buf, nal_size);
			// if (!ctx->first_idr && unit_type == 1)
			//  ctx->first_idr = 1;
			//}

			buf += nal_size;
			cumul_size += nal_size + ctx->length_size;
		} while (cumul_size < buf_size);

		return 1;

}