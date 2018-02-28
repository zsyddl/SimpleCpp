#pragma once

class CDwt1
{
public:
	CDwt1(void);
	~CDwt1(void);
	
	//输入 char数据，一般为图像， 输出 是有符号的 dwt 系数，包括 LL图像也是 带符号的系数
	void Dwt53(unsigned char *pSrc, int nW, int nH, int nLevels, int nTh, int nQ, char *pDwtCof);
	void IDwt53(unsigned char *pDwtCof, int nW, int nH, int nLevels, int nTh, int nQ, char *pSrc);
	//使用小波进行缩小图像
	void DwtZoomIn53(unsigned char *pSrc, int nW, int nH, int nLevels, unsigned char *pDes);
	//一维 53小波 1次变换, 不重排数据 , 左右边界分别 预留 2 个数据。
	void Dwt1D53(char *pSrc, int nCount, char *pDwtCof);
protected:
	
};
