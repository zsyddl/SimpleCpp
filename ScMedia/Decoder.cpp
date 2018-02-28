#include "StdAfx.h"
#include "Decoder.h"
#include "VideoFramePinBuffer.h"

CDecoder::CDecoder(void)
:m_nPixOutFormat(0)
{
	strcpy(m_szKind, FILTER_KIND_DECODER);
	strcpy(m_szName, FILTER_KIND_DECODER);
	strcpy(m_szType, FILTER_KIND_DECODER);
}

CDecoder::~CDecoder(void)
{
}
int CDecoder::Open(char *pParam)
{
	CMediaFilter::Open();

	CFilterPin *pIn = m_lstPinIn.at(0);
	pIn->ResizeBuffer<CFilterPinBuffer>(10);

	CFilterPin *pOut = m_lstPinOut.at(0);
	pOut->ResizeBuffer<CFilterPinBuffer>(2);
	pOut->SetAutoFreeMemory(0);
	return 0;
}
int CDecoder::DoFilter()
{

	// 默认 直接 将 输入 copy 到 输出
	/*CFilterPin *pPinIn = m_lstPinIn.at(0);
	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut || !pPinIn)
		return -1;
	CFilterPinBuffer *pBufferCodec = 0;
	CFilterPinBuffer *pBufferDecode = 0;
	pPinIn->GetPreparedBuffer(&pBufferCodec);
	if (!pBufferCodec)
		return -1;
	pPinOut->GetFreeBuffer(&pBufferDecode);
	if (!pBufferDecode)
		return -1;
	pBufferDecode->Clear();
	pBufferDecode->Append(pBufferCodec->GetData(), pBufferCodec->GetLength());
	
	pPinOut->PutPreparedBuffer(pBufferDecode);
	pPinIn->PutFreeBuffer(pBufferCodec);
	*/
	return CMediaFilter::DoFilter();
}
void  CDecoder::SetPixOutFormat(int nPixFormat)
{
	m_nPixOutFormat = nPixFormat;

	return ;
}
//复制到对应的 缓存，读音的 pix format nw nH
int CDecoder::CopyFrame(unsigned char *pImageOut, int nImageLen, int nW, int nH, int pix_fmt)
{

	return 0;
}
int64 CDecoder::GetTimestamp()
{

	return 0;
}