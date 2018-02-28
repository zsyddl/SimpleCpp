#include "StdAfx.h"
#include "DahuaFile.h"
#include "MediaFileIndex.h"
#include "ScBuffer.h"
#include "FfmpegCodec.h"
#include <sys/stat.h>
#include <io.h>

#define MPEG4_VOSH_START		0xB0010000
#define MPEG4_VO_START			0xB5010000
#define MPEG4_VOL_START			0x20010000
#define MPEG4_USER_DATA_START	0xB2010000
#define MPEG4_VOP_START			0xB6010000

CDahuaFile::CDahuaFile(void)
: m_pFile(NULL)
, m_pIndex(NULL)
, m_pFileFixRead(NULL)
, m_pBufferFixRead(NULL)
, m_s64FixFrameCount(0)
, m_s64FixTimestamp(0)
, m_s64FileTotalSize(0)
{
}

CDahuaFile::~CDahuaFile(void)
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pIndex)
	{
		m_pIndex->Close();
		delete m_pIndex;
		m_pIndex = NULL;
	}
	if (m_pFileFixRead)
	{
		fclose(m_pFileFixRead);
		m_pFileFixRead = NULL;
	}
	if (m_pBufferFixRead)
	{
		delete m_pBufferFixRead;
		m_pBufferFixRead = NULL;
	}
}
int CDahuaFile::Open(char *pSzFile, int nOpenMode)
{
	if (m_pFile)
	{
		return -1;
	}
	char szOpenFlag[32];
	memset(szOpenFlag, 0, sizeof(szOpenFlag));

	sprintf(szOpenFlag, "r+b");
	m_pFile = fopen(pSzFile, szOpenFlag);
	/*

	if (nOpenMode & AVIO_FLAG_WRITE)
	{
		sprintf(szOpenFlag, "w+b");
	}
	else
	{
		sprintf(szOpenFlag, "r+b");
	}
	m_pFile = fopen(pSzFile, szOpenFlag);*/
	if (!m_pFile)
		return -4;
	m_pFileRead = fopen(pSzFile, szOpenFlag);
	int nRet = 0;
	/*
	int nParseMaxLen = 1024*1024;
	char *pParseData = new char[nParseMaxLen];
	int nParseDatalen = 0;

	nParseDatalen = fread(pParseData, 1, nParseMaxLen, m_pFile);

	
	nRet = ProbInfo(pParseData, nParseDatalen);
	delete pParseData;
	if (nRet < 0)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	else
	{*/
		strcpy(m_szFile, pSzFile);
		_fseeki64(m_pFile, 0, SEEK_SET);
		m_pIndex = new CMediaFileIndex();
		char szIndexFile[256];
		memset(szIndexFile, 0, sizeof(szIndexFile));
		sprintf(szIndexFile, "%s.idx",pSzFile);
		int nCheckIndex = 0;
		nCheckIndex = CheckIndex(pSzFile, szIndexFile);
		if (nCheckIndex < 0)
		{
			//删除文件
			remove(szIndexFile);
			m_bNeedFix = true;
		}
		nRet = m_pIndex->Open( szIndexFile /*"f:\\1.index.db" */);
		if (nRet < 0)
		{
			debug("%s, open index false\n", __FUNCTION__);
		}
		else
		{
			debug("%s, open index success\n", __FUNCTION__);
		}
		if (nCheckIndex < 0)
		{
			//GenIndex();
			//m_pIndex->Flush();
		}
		m_s64TotalTime = m_pIndex->GetTotalTime();
	//}

	return nRet;

}
int CDahuaFile::Close()
{

	return 0;
}
int CDahuaFile::Write(char *pData, int nDataLen)
{

	return 0;
}
int CDahuaFile::Read(char *pData, int &nBufferLen, int64 &s64Timestamp, int &nStreamId, int &nFrameType)
{
	if (!m_pIndex)
	{
		return -1;
	}
	int nId(0);
	int64   s64FramePosition(0);
	int nFrameLen(0);
	int nRet = 0;
	int nTries = 5;
	while (nTries--)
	{
		nRet = m_pIndex->Read(nId, s64Timestamp, nStreamId, nFrameType, s64FramePosition, NULL, nFrameLen);
		if (nRet < 0)
		{
			return -2;
		}
		else if (nRet == 0)
		{

		}
		else
		{
			break;
		}
	}
	if (nBufferLen < nFrameLen)
	{
		nBufferLen = nFrameLen;
		return -3;
	}
	_fseeki64(m_pFileRead, s64FramePosition, SEEK_SET);

	nBufferLen = fread(pData, 1, nFrameLen, m_pFileRead);

	m_s64CurrTimestamp = s64Timestamp;

	return nBufferLen;
}

int CDahuaFile::Seek(int64 s64Time, int nSeekFlag)
{
	if (!m_pIndex)
	{
		return -1;
	}
	m_pIndex->Seek(s64Time, nSeekFlag);
	return 0;
}
int CDahuaFile::SeekToTimestamp(int64 s64Timestamp, int nSeekFlag)
{
	if (!m_pIndex)
	{
		return -1;
	}
	m_pIndex->SeekToTimestamp(s64Timestamp, nSeekFlag);
	return 0;
}

int CDahuaFile::GenIndex()
{
	if (!m_pFile)
		return -1;

	debug("%s++\n", __FUNCTION__);
	CBuffer bufferTmp(4*1024*1024, 4*1024*1024);
	bufferTmp.SetAllocSize(4*1024*1024);
	int nFrameCount = 0;
	bool bPrevIsParamNal = false;
	int64 s64FilePos = 0;
	int64 s64Timestamp = 0;
	int64 s64LastFramePos = -1;
	int   nFrameLen = 0;
	int   nLastFramType = eFFT_I;
	m_bExit = false;
	while (!m_bExit)
	{
		int nBufferLen = bufferTmp.GetLength()-5;
		unsigned char *pDataTmp = 0;
		int nPassed = 0;
		for (int i=0; i<nBufferLen; i++)
		{
			pDataTmp = bufferTmp.GetData() + i;
			if (*(int*)pDataTmp == MPEG4_VOSH_START
				|| *(int*)pDataTmp == MPEG4_VOP_START)
			{

				int nLen=0;
				
				//m_pIndex->Write(nFrameCount, s64Timestamp, 0, eFFT_I, s64FilePos+i, 0, nLen);
				//s64Timestamp = nFrameCount;
				//写入上一个 帧
				if (s64LastFramePos != -1)
				{
					nFrameLen = s64FilePos+i - s64LastFramePos;
					m_pIndex->Write(nFrameCount, s64Timestamp, 0, nLastFramType, s64LastFramePos, 0, nFrameLen);
				}

				s64LastFramePos = s64FilePos+i;
				nLastFramType = GetFrameType(pDataTmp, 5);
				nFrameCount++;// nFrameCount*1000*m_nTimebaseNum/m_nTimebaseDen;
				s64Timestamp = nFrameCount;

				nPassed = i;
			}

		}
		bufferTmp.Clear(0, nPassed);

		int nRead =0;
		s64FilePos = _ftelli64(m_pFile) - bufferTmp.GetLength();
		nRead = fread(bufferTmp.GetData()+bufferTmp.GetLength(), 1, bufferTmp.GetAllocSize()-bufferTmp.GetLength(), m_pFile );

		if (nRead <= 0)
		{
			break;
		}
		bufferTmp.SetUsed(bufferTmp.GetLength() + nRead);
	}
	m_pIndex->SetComplete(true);
	struct _stat64  buf;
	int result;
	char timebuf[26];
	errno_t err;
	// 多媒体文件长度不相等
	result = _stati64( m_szFile, &buf );
	m_pIndex->SetFileSize(buf.st_size);

	m_s64TotalTime = s64Timestamp*1000*m_nTimebaseNum/m_nTimebaseDen;
	debug("%s--, total frames: %d\n", __FUNCTION__, nFrameCount);
	return 0;
}

int CDahuaFile::CheckIndex( char *pSzMediaFile, char *pSzIndexFile )
{
	if (pSzIndexFile == NULL
		|| strlen(pSzIndexFile) <= 0)
	{
		return -1;
	}
	int nRet = 0;
	nRet = access(pSzIndexFile, 0);
	if (nRet < 0)
	{//不存在
		return -1;
	}
	//以及对应的文件时间

	//检查数据库完整性，以及对应的文件时间
	CMediaFileIndex indexFile;
	indexFile.Open(pSzIndexFile);
	if (!indexFile.IsComplete())
	{
		return -1;
	}
	//
	//文件长度
	struct _stat64 buf;
	int result;
	char timebuf[26];
	errno_t err;
	// 多媒体文件长度不相等
	result = _stati64( pSzMediaFile, &buf );
	if (buf.st_size != indexFile.GetFileSize())
	{
		return -2;
	}


	return 0;
}

int CDahuaFile::GetFrameType(unsigned char *pFrame, int nLen )
{
	int nFrameType = eFFT_I;
	unsigned char nFlag = (unsigned char)pFrame[4];
	switch(nFlag & (BYTE)0xc0)
	{
	case 0x00:
		//I Frame
		nFrameType = eFFT_I;
		break;
	case 0x40:
		//P Frame
		nFrameType = eFFT_P;
		break;
	case 0x80:
		//B Frame
		nFrameType = eFFT_B;
		break;
	default:
		break;
	}

	return nFrameType;
}

int CDahuaFile::DoFixFile( int64 &s64Total, int64 &s64Pos )
{
	if (!m_bNeedFix)
	{
		return -3;
	}
	if (!m_pFileFixRead)
	{
		m_pFileFixRead = fopen(m_szFile, "r+b");
	}
	if (!m_pFileFixRead)
		return -1;

	if (!m_pIndex)
	{
		return -2;
	}

	if (!m_pBufferFixRead)
	{
		m_pBufferFixRead = new CBuffer(4*1024*1024, 4*1024*1024);
		m_pBufferFixRead->SetAllocSize(4*1024*1024);
	}
	if (feof(m_pFileFixRead))
	{//文件结束了
		return -4;
	}
	

	bool bPrevIsParamNal = false;
	int64 s64FilePos = 0;
	int64 s64Timestamp = 0;
	int64 s64LastFramePos = -1;
	int   nFrameLen = 0;
	int   nLastFramType = eFFT_I;
	bool  bFileEnd = false;

	int nPassed = 0;
	while (1)
	{
		int nRead =0;
		s64FilePos = _ftelli64(m_pFile) - m_pBufferFixRead->GetLength();
		nRead = fread(m_pBufferFixRead->GetData()+m_pBufferFixRead->GetLength(), 1, m_pBufferFixRead->GetAllocSize()-m_pBufferFixRead->GetLength(), m_pFile );

		m_pBufferFixRead->SetUsed(m_pBufferFixRead->GetLength() + nRead);

		int nBufferLen = m_pBufferFixRead->GetLength()-5;
		unsigned char *pDataTmp = 0;
		nPassed = 0;
		for (int i=0; i<nBufferLen; i++)
		{
			pDataTmp = m_pBufferFixRead->GetData() + i;
			if (*(int*)pDataTmp == MPEG4_VOSH_START
				|| *(int*)pDataTmp == MPEG4_VOP_START)
			{
				int nLen=0;

				//m_pIndex->Write(nFrameCount, s64Timestamp, 0, eFFT_I, s64FilePos+i, 0, nLen);
				//s64Timestamp = nFrameCount;
				//写入上一个 帧
				if (s64LastFramePos != -1)
				{
					nFrameLen = s64FilePos+i - s64LastFramePos;
					m_pIndex->Write(m_s64FixFrameCount, s64Timestamp, 0, nLastFramType, s64LastFramePos, 0, nFrameLen);
				}

				s64LastFramePos = s64FilePos+i;
				nLastFramType = GetFrameType(pDataTmp, 5);
				m_s64FixFrameCount++;// nFrameCount*1000*m_nTimebaseNum/m_nTimebaseDen;
				s64Timestamp = m_s64FixFrameCount;

				nPassed = i;
			}
		}
		m_pBufferFixRead->Clear(0, nPassed);

		if (nRead <= 0 )
		{
			bFileEnd = true;
		}
		break;
	}
	struct _stat64  buf;
	// 多媒体文件长度不相等
	int nRet(0);
	nRet = _stati64( m_szFile, &buf );
	m_s64FileTotalSize = buf.st_size;

	if (bFileEnd)
	{
		m_pIndex->SetComplete(true);
		m_pIndex->SetFileSize(buf.st_size);

		m_pIndex->Flush();
		m_pIndex->Save();

		s64Total = m_s64FileTotalSize;
		s64Pos = s64Total;
	}
	else
	{
		s64Total = m_s64FileTotalSize;
		s64Pos = (s64FilePos + nPassed);
	}
	m_s64TotalTime = s64Timestamp*1000*m_nTimebaseNum/m_nTimebaseDen;

	return 0;
}
