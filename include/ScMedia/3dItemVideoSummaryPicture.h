#pragma once

#include "3dItemImage.h"

typedef long long int64;
typedef unsigned char uint8;

class C3dItemText2d;
class MEDIASDK_API C3dItemVideoSummaryPicture:
	public C3dItemImage
{
public:
	C3dItemVideoSummaryPicture(C3dGraphics *p3dGraphic, int nTexturePixW=600, int nTexturePixH=480, e3dPixFormat pixFormat=e3dPF_YV420 );
	virtual ~C3dItemVideoSummaryPicture(void);

	virtual int Create(float fX, float fY, float fZ);
	virtual  int Render(C3dGraphics *pGraphic);

	virtual int Release();
	virtual int	ReCreate(C3dGraphics *p3dGraphics);

	virtual int SetImageData(char *pData, int nW, int nH, int nPixFormat = e3dPF_RGBA);
	virtual int SetImageFromFile(char *pData, int nLen);
	virtual int SetImageMask(uint8 *pMask, int nBlockSize);

	virtual int SetTimeStamp(int64 s64TimeStamp);
	virtual bool HitTest(float fX, float fY);
	int64 GetTimeStamp(){return m_s64TimeStamp;};
	void SetTimeBase(int nTimeBaseNum, int nTimeBaseDen);
	void SetStartTimestamp(int64 s64Timestamp);
	void SetObjectId(int64 s64ObjectId);
	int64 GetObjectId();
protected:
	void DrawLine(int x0, int y0, int x1, int y1);
	void MaskPicture(uint8 *pMask);
	//根据mask重新创建
	void ReCreateGeometry();

	C3dItemText2d *m_p3dItem2dText;
	int64   m_s64StartTimestamp;
	int64	m_s64TimeStamp; 
	int64   m_s64ObjectId;
	int m_nTimeBaseNum;
	int m_nTimeBaseDen;
	int		m_nCornerLineLen;
	uint8 *m_pMask;
	int    m_nMaskBlockSize;
	int    m_nBlockCols;
	int    m_nBlockRows;
	bool   m_bEnableMask ;
};
