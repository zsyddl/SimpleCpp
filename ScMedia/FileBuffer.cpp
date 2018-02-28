#include "StdAfx.h"
#include "FileBuffer.h"
#include <stdio.h>
#include "Buffer.h"

CFileBuffer::CFileBuffer(char *pFile, int nMaxSize/*=10*1024*1024*/)
: m_nMaxSize(nMaxSize)
, m_pFile(0)
, m_pSzFile(0)
, m_nUsedSize(0)
, m_pMemBuffer(0)
, m_nBufferMode(eFBM_Memory)
{
	m_pSzFile = new char[256];
	memset(m_pSzFile, 0, 256);

	if (pFile != NULL)
	{
		strncpy(m_pSzFile, pFile, 256);
	}
	
	if (m_nBufferMode == eFBM_Memory)
		m_pMemBuffer = new CBuffer();
	else
	{
		CreateFile(pFile);
	}
}

CFileBuffer::~CFileBuffer(void)
{
	
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;

		remove(m_pSzFile);
	}
	if (m_pSzFile)
	{
		delete m_pSzFile;
		m_pSzFile = NULL;
	}

	if (m_pMemBuffer)
	{
		delete m_pMemBuffer;
		m_pMemBuffer = NULL;
	}
}

int CFileBuffer::CreateFile( char *pSzFile )
{
	if (pSzFile != NULL)
	{
		m_pFile = fopen(pSzFile, "w+b");
	}
	if (!m_pFile)
	{
		
		sprintf(m_pSzFile, "./FileBuffer/%X.fb", this);

		m_pFile = fopen(m_pSzFile, "w+b");
	}
	if (!m_pFile)
	{

		sprintf(m_pSzFile, "%X.fb", this);

		m_pFile = fopen(m_pSzFile, "w+b");
	}

	return 0;
}
int CFileBuffer::Append(char *pData, int nLen)
{
 
	if (m_nBufferMode == eFBM_Memory)
	{
		if (m_pMemBuffer)
		{
			m_pMemBuffer->Append((unsigned char*)pData, nLen);
		}
	}
	else
	{
		if (m_pFile)
			fwrite(pData, nLen, 1, m_pFile);
	}

	m_nUsedSize = nLen;

	return 0;
}
void CFileBuffer::Clear()
{
 
	m_nUsedSize = 0;
	if (m_pMemBuffer)
	{
		m_pMemBuffer->SetUsed(0);
	}
	if (m_pFile)
		fseek(m_pFile, 0, SEEK_SET);
}
int CFileBuffer::GetData(char *pData, int &nLen)
{
	if (m_nBufferMode == eFBM_Memory)
	{

		if (m_pMemBuffer)
		{
			memcpy(pData, m_pMemBuffer->GetData(), nLen);
		}
	}
	else
	{
		if (m_pFile)
		{
			fseek(m_pFile, 0, SEEK_SET);
			nLen = fread(pData, nLen, 1, m_pFile);
		}
	}

	return nLen;
}

int CFileBuffer::GetData( int nPos, char *pData, int &nLen )
{
	if (m_nBufferMode == eFBM_Memory)
	{

		if (m_pMemBuffer)
		{
			memcpy(pData, m_pMemBuffer->GetData()+nPos, nLen);
		}
	}
	else{
		if (m_pFile){
			fseek(m_pFile, nPos, SEEK_SET);
			int nSize = nLen;
			int nCount = 1;
			nLen = fread(pData, nSize, nCount, m_pFile);

			nLen = nLen * nSize;
		}
	}

	return nLen;
}

int CFileBuffer::GetSize()
{
	return m_nUsedSize;
}

int CFileBuffer::SetFilePath( char *pFilePath )
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;

		remove(m_pSzFile);
	}
	sprintf(m_pSzFile, "%s/%X.fb", pFilePath, this);

	m_pFile = fopen(m_pSzFile, "w+b");

	return 0;
}
