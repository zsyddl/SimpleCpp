#include "StdAfx.h"
#include "IndexFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

CIndexFile::CIndexFile(void)
: m_pFile(NULL)
, m_u32OpenMode(0)
, m_pFileIndex(NULL)
, m_s64Index(0)
{
	memset(m_szFile, 0, sizeof(m_szFile));
}

CIndexFile::~CIndexFile(void)
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pFileIndex)
	{
		fclose(m_pFileIndex);
		m_pFileIndex = NULL;
	}
}
int64 CIndexFile::GetFileLength(FILE *pFile)
{
	if (!pFile)
		return -1;
	struct   _stat64   stat;    
	if (_fstat64(fileno(pFile), &stat) != 0)
	{
		return -1;
	}
	/*if(_fstat64((int)m_pFile, &stat) != 0)
	{
		return Std2FileError(errno);
	}*/
	int64   iFileSize   =   stat.st_size;     //获取文件大小   
	return iFileSize;
}
int CIndexFile::Open(char *pSzFile, int nOpenMode)
{
	if (m_pFile)
	{
		return 1;
	}
	int nRet = 0;
	char szOpenMode[32];
	memset(szOpenMode, 0, sizeof(szOpenMode));
	if (nOpenMode == eIFOM_Read)
	{
		strcpy(szOpenMode, "r+b");
	}
	else
	{//不删除文件直接写
		//如果文件不存在，需要创建文件
		bool bRet = false;

		if (_access(pSzFile, 0) == -1)
		{//不存在
			bRet = false;
			strcpy(szOpenMode, "w+b");
		}
		else
		{
			bRet = true;
			strcpy(szOpenMode, "r+b");
		}
		
	}

	m_pFile = fopen(pSzFile, szOpenMode);
	if (!m_pFile)
	{
		nRet = -1;
	}
	char szIndexFile[256];
	memset(szIndexFile, 0, sizeof(szIndexFile));
	sprintf(szIndexFile, "%s.index", pSzFile);
	m_pFileIndex = fopen(szIndexFile, szOpenMode);
	if (!m_pFileIndex)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		nRet = -1;
	}
	
	//移动到 文件末尾
	if (nOpenMode == eIFOM_Read)
	{
		Seek(0x00);
	}
	else
	{//写文件，需要到 末尾
		Seek(0xfffffffffffffff);
	}

	
	return nRet;
}
int CIndexFile::Close()
{
	int nRet = 0;
	
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pFileIndex)
	{
		fclose(m_pFileIndex);
		m_pFileIndex = NULL;
	}

	return nRet;
}

int64 CIndexFile::GetMaxIndex()
{
	int nRet = 0;
	struct   _stat64   stat;    
	if (_fstat64(fileno(m_pFileIndex), &stat) != 0)
	{
		return -1;
	}
	/*if(_fstat64((int)m_pFile, &stat) != 0)
	{
		return Std2FileError(errno);
	}*/
	int64   iFileSize   =   stat.st_size;     //获取文件大小   
	int nMaxIndex = 0;
	int nIndexDataSize = iFileSize - INDEX_FILE_HEAD_LEN;
	if (nIndexDataSize <= 0)
	{
		nIndexDataSize = 0;
	}
	nMaxIndex = (nIndexDataSize)/sizeof(sIndex);

	return nMaxIndex;
}

/*
int CIndexFile::Seek(char *pSzIndex)
{
	int nRet = 0;

	return nRet;
}*/
int CIndexFile::Seek(int64 nIndex)
{
	int nRet = 0;
	if (nIndex <= 0)
	{
		m_s64Index = 0;
	}
	else if (nIndex > GetMaxIndex())
	{
		m_s64Index = GetMaxIndex();
	}
	else
	{
		m_s64Index = nIndex;
	}
	//新建的文件
	//

	if (GetFileLength(m_pFile) < DATA_FILE_HEAD_LEN)
	{//移动到末尾, 0长度文件 ， 数据文件  DATA_FILE_HEAD_LEN位置
		int64 s64IndexPos = 0;
		s64IndexPos = m_s64Index*sizeof(sIndex) + INDEX_FILE_HEAD_LEN;
		sIndex index;
		
		_fseeki64(m_pFileIndex, s64IndexPos, SEEK_SET);
		//至少加一个头的长度
		//_fseeki64(m_pFile, DATA_FILE_HEAD_LEN, SEEK_SET);
		char szTmp[DATA_FILE_HEAD_LEN];
		memset(szTmp, 0, sizeof(szTmp));
		fwrite(szTmp, 1, sizeof(szTmp), m_pFile);
		fflush(m_pFile);
		char szIndexHead[INDEX_FILE_HEAD_LEN];
		memset(szIndexHead, 0, sizeof(szIndexHead));
		fwrite(szIndexHead, 1, sizeof(szIndexHead), m_pFileIndex);
		//_fseeki64(m_pFile, -1, SEEK_CUR);
	}
	else{//移动  数据文件位置
		int64 s64IndexPos = 0;
		s64IndexPos = m_s64Index*sizeof(sIndex) + INDEX_FILE_HEAD_LEN;
		sIndex index;

		_fseeki64(m_pFileIndex, s64IndexPos, SEEK_SET);
		int nRead =0;
		nRead =	fread(&index, 1, sizeof(sIndex), m_pFileIndex);
		if (nRead != sizeof(sIndex) )
		{//移动到末尾
			_fseeki64(m_pFile, DATA_FILE_HEAD_LEN, SEEK_END);
			return -1;
		}

		_fseeki64(m_pFile, index.s64Pos, SEEK_SET);
	}

	return nRet;
}
int CIndexFile::ReadBlock(char *pSzBuffer, int &nReadMax)
{
	if (!m_pFileIndex || !m_pFile)
		return -1;

	if (m_s64Index >= GetMaxIndex())
	{
		return -1;
	}

	int nRet = 0;
	int64 s64IndexPos = 0;
	s64IndexPos = m_s64Index*sizeof(sIndex) + INDEX_FILE_HEAD_LEN;
	sIndex index;

	_fseeki64(m_pFileIndex, s64IndexPos, SEEK_SET);
	int nRead =0;
	nRead =	fread(&index, 1, sizeof(sIndex), m_pFileIndex);
	if (nRead != sizeof(sIndex) )
		return -1;

	int64 s64DataPos = 0;
	_fseeki64(m_pFile, index.s64Pos, SEEK_SET);
	int nToRead = 0;
	nToRead = (nReadMax<index.s64Len) ?nReadMax:index.s64Len ;
	nRead = fread(pSzBuffer, 1, nToRead, m_pFile);

	nReadMax = nRead;
	
	m_s64Index++;
	if (m_s64Index >= GetMaxIndex())
	{
		m_s64Index = GetMaxIndex();
	}

	return nRead;
}
int CIndexFile::WriteHeader(char *pHeader, int nSize)
{
	if (!m_pFileIndex|| !m_pFile)
	{
		return -1;
	}
	int nRet = 0;
	int64 nPosCur = 0;
	nPosCur = _ftelli64(m_pFile);;//获取当前数据文件的位置

	_fseeki64(m_pFile, 0, SEEK_SET);
	int nToWrite = nSize;
	int nWriten = 0;
	nWriten = fwrite(pHeader, 1, nToWrite, m_pFile);
	if (nWriten > 0)
	{
		nRet = nWriten;
	}
	fflush(m_pFile);
	//放回到先前的位置
	_fseeki64(m_pFile, nPosCur, SEEK_SET);

	return nRet;
}
int CIndexFile::ReadHeader(char *pHeader, int nSize)
{
	if (!m_pFileIndex|| !m_pFile)
	{
		return -1;
	}
	int nRet = 0;
	int64 nPosCur = 0;
	nPosCur = _ftelli64(m_pFile);;//获取当前数据文件的位置

	_fseeki64(m_pFile, 0, SEEK_SET);
	int nToRead = nSize;
	int nRead = 0;
	nRead = fread(pHeader, 1, nToRead, m_pFile);
	if (nRead > 0)
	{
		nRet = nRead;
	}
	//放回到先前的位置
	_fseeki64(m_pFile, nPosCur, SEEK_SET);

	return nRet;
}
int CIndexFile::WriteBlock(char *pSzBuffer, int &nWrite)
{
	int nRet = 0;

	if (!m_pFileIndex || !m_pFile)
		return -1;

	int64 s64IndexPos = 0;
	s64IndexPos = m_s64Index*sizeof(sIndex) + INDEX_FILE_HEAD_LEN;
	sIndex index;
	index.s64Pos = _ftelli64(m_pFile);;//获取当前数据文件的位置
	index.s64Len = nWrite;
	_fseeki64(m_pFileIndex, s64IndexPos, SEEK_SET);
	int nWriten =0;
	nWriten =	fwrite(&index, 1, sizeof(sIndex), m_pFileIndex);
	if (nWriten != sizeof(sIndex) )
		return -1;

	int64 s64DataPos = 0;
	_fseeki64(m_pFile, index.s64Pos, SEEK_SET);
	int nToWrite = nWrite;
	 
	nWriten = fwrite(pSzBuffer, 1, nToWrite, m_pFile);
	nWrite = nWriten;

	m_s64Index++;

	return nWriten;

}