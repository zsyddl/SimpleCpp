#include "StdAfx.h"
#include "SourceIndexFile.h"
#include "FilterManager.h"

CSourceIndexFile g_regSourceIndexFile;
CSourceIndexFile::CSourceIndexFile(void)
:CSourceFilter()
, m_pFile(NULL)
, m_pBufferRead(NULL)
, m_pVideoInfo(NULL)
{
	strcpy(m_szType, "SourceIndexFile");
	strcpy(m_szName, "SourceIndexFile");

	m_pBufferRead = new CBuffer();
	m_pBufferRead->SetMaxSize(1024*1024);
	m_pBufferRead->SetAllocSize(1024*1024);
	m_pBufferRead->SetUsed(0);

	CFilterManager::RegistFilter(&g_regSourceIndexFile);
}

CSourceIndexFile::~CSourceIndexFile(void)
{
	if (m_pBufferRead)
	{
		delete m_pBufferRead;
		m_pBufferRead = NULL;
	}
	if (m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}
	if (m_pVideoInfo)
	{
		delete m_pVideoInfo;
	}
}

int CSourceIndexFile::Open(char *pSzParam/* =0 */)
{
	if (m_pFile)
		return 1;
	CSourceFilter::Open(pSzParam);

	m_pFile = new CIndexFile();
	string strFile;
	strFile = m_mapParamValue["file"];

	char szFile[256];
	strcpy(szFile, strFile.c_str());
	int nRet = 0;
	nRet = m_pFile->Open(szFile, eIFOM_Read);

	if (nRet < 0)
	{
		return -1;
	}
	m_pVideoInfo = new sVideoFrameInfo;
	m_pFile->ReadHeader((char*)m_pVideoInfo, sizeof(sVideoFrameInfo));

	return 0;
}
int CSourceIndexFile::DoFilter()
{
	CFilterPin *pPinOut = 0;
	pPinOut = m_lstPinOut.at(0);
	CFilterPinBuffer *pPinBuffer = 0;
	if (!pPinOut)
	{
		return -1;
	}
	if (pPinOut->GetFreeBufferCount() <= 0)
	{
		goto ret;
	}
	ReadFrame();
	if (m_pBufferRead->GetUsedSize() <= 0)
	{//гаЪ§Он
		goto ret;
	}

/*
	FILE *pFile = 0;
	pFile = fopen("D:\\tmp\\hk.h264", "a+b");
	if (pFile)
	{
		fwrite(m_pBufferRead->GetData(), 1, m_pBufferRead->GetLength(), pFile);
		fclose(pFile);
		pFile = NULL;
	}
	*/
	pPinOut->GetFreeBuffer(&pPinBuffer);
	if (!pPinBuffer)
	{
		goto ret;
	}
	pPinBuffer->Clear();
	pPinBuffer->Append((char*)m_pBufferRead->GetData(), m_pBufferRead->GetUsedSize());
	CBuffer *pExtData = pPinBuffer->GetExtData();
	pExtData->SetUsed(sizeof(sVideoFrameInfo));
	memcpy(pExtData->GetData(), m_pVideoInfo, sizeof(sVideoFrameInfo));

	pPinOut->PutPreparedBuffer(pPinBuffer);
	m_pBufferRead->Clear();

ret:
	return CSourceFilter::DoFilter();
}
CMediaFilter *CSourceIndexFile::CreateObject()
{
	return new CSourceIndexFile();
}

int CSourceIndexFile::ReadFrame()
{
	if (m_pFile == NULL)
		return 0;
	
	m_pBufferRead->Clear();
	int nMaxSize = 1024*1024;
	int nRead = 0;
	nRead = m_pFile->ReadBlock((char*)m_pBufferRead->GetData(), nMaxSize);
	if (nRead < 0)
		nRead = 0;

	m_pBufferRead->SetUsed(nRead);
	m_pVideoInfo->nPts++;

	return 0;
}
