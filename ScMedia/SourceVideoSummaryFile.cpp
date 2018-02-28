#include "StdAfx.h"
#include "SourceVideoSummaryFile.h"
#include "VideoSummaryFile.h"
#include "FilterManager.h"

CSourceVideoSummaryFile g_regSourceVideoSummaryFile;
CSourceVideoSummaryFile::CSourceVideoSummaryFile(void)
:CSourceFilter()
, m_pFile(NULL)
, m_nTimePerFrame(40)
{

	strcpy(m_szName, "SourceVideoSummaryFile");
	strcpy(m_szType, "SourceVideoSummaryFile");
	CFilterManager::RegistFilter(&g_regSourceVideoSummaryFile);
}

CSourceVideoSummaryFile::~CSourceVideoSummaryFile(void)
{
}
int CSourceVideoSummaryFile::Init(char *pSzParam)
{
	CSourceFilter::Init(pSzParam);


	//////////////////////////////////////////////////////////////////////////
	if (m_pFile)
	{
		return 1;
	}
	 
	m_pFile = new CVideoSummaryFile;


	string strFile ;
	strFile = m_mapParamValue["source"];

	int nRet = 0;
	char szFile[256];

	strcpy(szFile, strFile.data());

	nRet = m_pFile->Open(szFile);

	if (nRet < 0)
		return -1;

	int nNum=0, nDen=0;
	m_pFile->GetTimeBase(nNum, nDen);
	if (nNum != 0 && nDen != 0)
	{
		m_nTimePerFrame = 1000.0*nNum/nDen;
	}

	return 0;
}
int CSourceVideoSummaryFile::DoFilter()
{
	//读取一帧数据
	if (! m_pFile)
		goto ret;

	CFilterPin *pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		goto ret;

	// 循环 读取 直到 读取的 时间 减去 上一次循环的 读取 的时间 > 实际流失的时间
	
	if (pPinOut->GetFreeBufferCount() <= 0)
	{
		goto ret;
	}
	CFilterPinBuffer *pBufferOut = 0;
	pPinOut->GetFreeBuffer(&pBufferOut);
	if(!pBufferOut)
	{
		goto ret;
	}

	vector<CVideoSummaryFilePicture*> *lstPicture(0);

	int nRet = 0;
	nRet = m_pFile->ReadFrame(&lstPicture, eVSFMC_Next);
	if (nRet< 0 || !lstPicture)
	{//结束
		pPinOut->PutFreeBuffer(pBufferOut);

		if (m_cbFinished)
		{
			m_cbFinished(m_pCbFinishedUserData, m_pCbProgressReversed);
		}
		goto ret;
	}
	if (m_cbProgress)
	{
		m_cbProgress(1, m_pFile->GetCurrTime(), m_pFile->GetTotalTime(), m_pCbProgressUserData, m_pCbProgressReversed);
		
	}
	debug("%s, [%d/%d]\n", __FUNCTION__, (int)m_pFile->GetCurrTime(), (int)m_pFile->GetTotalTime() );
	//将 pic 信息放入到输出pin
	//如果有新的背景图片也放入，没有则 render 不更新背景图片
	//根据当前背景 图片判断 是否 有新的背景图片
	//数据结构？ bkg=xxx\npic=xx\n;
	if (pBufferOut->GetAllocSize() < (lstPicture->size() + 1)* 100)
	{
		pBufferOut->SetAllocSize(lstPicture->size() * 100);
	}
	memset(pBufferOut->GetData(), 0, pBufferOut->GetAllocSize());
	int nOutBufferPos = 0;
	//char *pSzLine = new char[1024];
	if (lstPicture->size() > 0)
	{//bkg
		int nSize = 0;
		CVideoSummaryFilePicture *pPicture = 0;
		pPicture = lstPicture->at(0);
		int nW(0), nH(0);
		m_pFile->GetBkgSize(nW, nH);
		nSize = sprintf(pBufferOut->GetData(), "bkg=%s\\%s\\%s;w=%d;h=%d;\n", 
			m_pFile->GetFilePath(), PIC_FOLDER, pPicture->GetBkgFile(), nW, nH);
		nOutBufferPos += nSize;
	}

	for (int i=1; i<lstPicture->size(); i++)
	{
		CVideoSummaryFilePicture *pPicture = 0;
		pPicture = lstPicture->at(i);
		//
		int nSize = 0;
		int nX(0), nY(0), nW(0), nH(0);
		pPicture->GetPos(nX, nY);
		pPicture->GetSize(nW, nH);
		nSize = sprintf(pBufferOut->GetData()+nOutBufferPos, 
			"pic=%s;x=%d;y=%d;w=%d;h=%d;pts=%I64d;\n", 
			pPicture->GetFullPath(), nX, nY, nW, nH, pPicture->m_s64TimeStamp);
		nOutBufferPos += nSize;
		//
	}
	pBufferOut->SetUsedSize(nOutBufferPos);
	//
	//delete pSzLine;

	pPinOut->PutPreparedBuffer(pBufferOut);

ret:

	Sleep(m_nTimePerFrame);

	return CSourceFilter::DoFilter();
}

CMediaFilter* CSourceVideoSummaryFile::CreateObject()
{

	return new CSourceVideoSummaryFile;
}

int CSourceVideoSummaryFile::PlayNextFrame()
{
	
	return 0;
}
