#include "StdAfx.h"
#include "Render2IndexFile.h"
#include "MediaSdkDef.h"
#include "Buffer.h"
#include "FilterPinBuffer.h"
#include "FilterManager.h"

CRender2IndexFile g_regRender2IndexFile;
CRender2IndexFile::CRender2IndexFile(void)
: CRender()
, m_pFile(NULL)
, m_pVideoFrameInfo(NULL)
{
	strcpy(m_szType, "Render2IndexFile");
	strcpy(m_szName, "Render2IndexFile");

	CFilterManager::RegistFilter(&g_regRender2IndexFile);
}

CRender2IndexFile::~CRender2IndexFile(void)
{
	if (m_pFile)
	{
		m_pFile->WriteHeader((char*)m_pVideoFrameInfo, sizeof(sVideoFrameInfo));
		m_pFile->Close();
		delete m_pFile;
	}
	if (m_pVideoFrameInfo)
	{
		delete m_pVideoFrameInfo;
		m_pVideoFrameInfo = NULL;
	}
}

CMediaFilter *CRender2IndexFile::CreateObject()
{
	return new CRender2IndexFile;
}
int CRender2IndexFile::Open(char *pParam)
{
	CRender::Open(pParam);
	
	string strFile;
	strFile = m_mapParamValue["file"];

	if (m_pFile)
	{
		m_pFile->Close();
	}
	else
	{
		m_pFile = new CIndexFile();
	}

	int nRet = 0;
	char szFile[256];
	strcpy(szFile, strFile.data());
	nRet = m_pFile->Open(szFile, eIFOM_ReadWrite);
	if (nRet >= 0)
	{
		//m_pFile->Seek(m_pFile->GetMaxIndex());
	}

	return nRet;
}
int CRender2IndexFile::DoFilter()
{
	
	CFilterPin *pPinIn = NULL;
	pPinIn = m_lstPinIn.at(0);
	CFilterPinBuffer *pBufferIn = NULL;
	if (pPinIn->GetPreparedBufferConut() <= 0)
		return -2;
	pPinIn->GetPreparedBuffer(&pBufferIn);
	if (!pBufferIn)
		return -3;
/*
	if(m_pRawFile != NULL)
	{
		fwrite(pBufferIn->GetData(), pBufferIn->GetLength(), 1, m_pRawFile);
	}
*/
	sVideoFrameInfo *pVideoFrameInfo = NULL;
	CBuffer *pBufferEx = pBufferIn->GetExtData();
	if (pBufferEx || pBufferEx->GetLength() != sizeof(sVideoFrameInfo))
	{
		pVideoFrameInfo = (sVideoFrameInfo *) pBufferEx->GetData();
		if (CheckStreamChanged(*pVideoFrameInfo))
		{
			ReCreate();
		}
	}


	if (!m_pFile)
	{
		pPinIn->PutFreeBuffer(pBufferIn);

		return -1;
	}
	int nWriten = 0;
	if (m_pFile && pBufferIn->GetLength() > 0)
	{
		int nToWrite = 0;
		nToWrite = pBufferIn->GetLength();
		nWriten = m_pFile->WriteBlock(pBufferIn->GetData(), nToWrite/*, pVideoFrameInfo->nPts*/);
	}

	pPinIn->PutFreeBuffer(pBufferIn);

	return CRender::DoFilter();
}

bool  CRender2IndexFile::CheckStreamChanged(sVideoFrameInfo &videoFrameInfo)
{
	if (!m_pVideoFrameInfo)
	{
		m_pVideoFrameInfo = new sVideoFrameInfo;
		*m_pVideoFrameInfo = videoFrameInfo;
	 
		return true;
	}
	return false;
}
int   CRender2IndexFile::ReCreate()
{
	if (m_pFile)
	{
		m_pFile->WriteHeader((char*)m_pVideoFrameInfo, sizeof(sVideoFrameInfo));
	}

	return 0;
}