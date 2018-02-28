#include "StdAfx.h"
#include "VideoFramePinBuffer.h"
#include "Buffer.h"

CVideoFramePinBufer::CVideoFramePinBufer(void)
: CFilterPinBuffer(1*1024*1024, 20*1024*1024)
, m_pY(0)
, m_pV(0)
, m_pU(0)
, m_nHeight(0)
, m_nWidth(0)
, m_nPixFormat(ePF_YV420)
, m_nFps(25)
, m_nGop(25)
, m_nPts(0)
{
	
	m_pY = (unsigned char*)m_pMemory;
}

CVideoFramePinBufer::~CVideoFramePinBufer(void)
{
}
int CVideoFramePinBufer::SetFrameSize(int nW, int nH)
{

	if (m_nWidth == nW
		&& m_nHeight == nH)
		return 0;
	if (nW*nH*3 > m_nMaxSize)
	{
		SetMaxSize(nW*nH*3);
	}
	Resize(nW*nH*3);
	m_pY = (unsigned char*)m_pMemory;
	m_pU = m_pY + nW*nH;
	m_pV = m_pU + nW*nH/4;

	m_nHeight = nH;
	m_nWidth = nW;
	m_nUsedSize = nW*nH*3;

	//m_nReversePinBufferMemorySize = m_nAllocSize;

	return 0;
}
int CVideoFramePinBufer::CopyFrom(CFilterPinBuffer *pBuffer)
{
	//CFilterPinBuffer::CopyFrom(pBuffer);

	CVideoFramePinBufer *pVideoSrc = 0;
	pVideoSrc = dynamic_cast<CVideoFramePinBufer*>(pBuffer);
	if (pVideoSrc)
	{
		int nOldW = m_nWidth;
		int nOldH = m_nHeight;
		int nOldAlloc = GetAllocSize();
		int nOldUsed = GetUsedSize();
		int nOldMaxSize = GetMaxSize();

		unsigned char *pOldY = m_pY;

		m_nWidth = pVideoSrc->m_nWidth;
		m_nHeight = pVideoSrc->m_nHeight;

		m_pMemory = (char*)pVideoSrc->m_pY;
		m_pY = pVideoSrc->m_pY; //(unsigned char*)m_pMemory;
		//m_pY = (unsigned char*)m_pMemory;;
		m_pU = m_pY + m_nWidth*m_nHeight;
		m_pV = m_pU + m_nWidth*m_nHeight/4;
		//SetMaxSize(pVideoSrc->GetMaxSize());
		//SetAllocSize(pVideoSrc->GetAllocSize());
		//SetUsedSize(pVideoSrc->GetUsedSize());
		m_nMaxSize = pVideoSrc->GetMaxSize();
		m_nAllocSize = pVideoSrc->GetAllocSize();
		m_nUsedSize = pVideoSrc->GetUsedSize();


		pVideoSrc->m_nWidth = nOldW;
		pVideoSrc->m_nHeight = nOldH;
		pVideoSrc->m_pY = pOldY;
		pVideoSrc->m_pU = pVideoSrc->m_pY + nOldH*nOldW;
		pVideoSrc->m_pV = pVideoSrc->m_pU + nOldH*nOldW/4;
		pVideoSrc->m_pMemory = (char*)pOldY;
		pVideoSrc->SetMaxSize(nOldMaxSize);
		pVideoSrc->SetAllocSize(nOldAlloc);
		pVideoSrc->SetUsedSize(nOldUsed);
		//
		m_pExtData->CopyFrom(pVideoSrc->GetExtData());
	}
	else
	{
		CFilterPinBuffer::CopyFrom(pBuffer);
	}


	return 0;
}

void CVideoFramePinBufer::Clear( int nReversedMemoryLen/*=0*/ )
{
	m_nUsedSize = 0;
}
