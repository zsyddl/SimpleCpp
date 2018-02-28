#ifndef frame_h_1454
#define frame_h_1454

enum YUV_FORMAT {yuv_420, yuv_422};

struct Frame
{
	unsigned char* Y;
	unsigned char* U;
	unsigned char* V;

	int width;
	int height;

	int strideY;
	int strideCRCB;


	YUV_FORMAT format;
};

#endif //Stream_h_1454