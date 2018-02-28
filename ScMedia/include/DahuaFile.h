#pragma once
#include "mediafile.h"

namespace SimpleCpp{
	class CBuffer;
}
using namespace SimpleCpp;

class CMediaFileIndex;
class CDahuaFile :
	public CMediaFile
{
public:
	CDahuaFile(void);
	virtual ~CDahuaFile(void);

	virtual int Open(char *pSzFile, int nOpenMode);
	virtual int Close();

	virtual int Write(char *pData, int nDataLen);
	virtual int Read(char *pData, int &nBufferLen, int64 &s64Timestamp, int &nStreamId, int &nFrameType);

	virtual int Seek(int64 s64Time, int nSeekFlag);
	virtual int SeekToTimestamp(int64 s64Time, int nSeekFlag);

	virtual int GenIndex();
	virtual  int DoFixFile(int64 &s64Total, int64 &s64Pos);
protected:
	int CheckIndex(char *pSzMediaFile, char *pSzIndexFile);
	int GetFrameType(unsigned char *pFrame, int nLen);

	//用于修复建立索引 的读取
	CBuffer *m_pBufferFixRead;
	FILE	*m_pFileFixRead;
	int64	 m_s64FixTimestamp;
	int64	m_s64FixFrameCount;
	int64	m_s64FileTotalSize;

	FILE			*m_pFile;
	CMediaFileIndex *m_pIndex;
	FILE			*m_pFileRead;

	bool m_bExit;
	char m_szFile[256];
};
