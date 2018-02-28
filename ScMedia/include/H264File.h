#pragma once
#include "mediafile.h"
class CMediaFileIndex;

#define H264_NAL_FLAG (0x01000000)

class CH264File :
	public CMediaFile
{
public:
	CH264File(void);
	virtual ~CH264File(void);

	virtual int Open(char *pSzFile, int nOpenMode);
	virtual int Close();

	virtual int Write(char *pData, int nDataLen);
	virtual int Read(char *pData, int &nBufferLen, int64 &s64Timestamp, int &nStreamId, int &nFrameType);

	virtual int Seek(int64 s64Time, int nSeekFlag);
	virtual int SeekToTimestamp(int64 s64Time, int nSeekFlag);
	virtual  int DoFixFile(int64 &s64Total, int64 &s64Pos);
protected:
	int ProbInfo(char *pData, int nLen);
	int DoFixFile();
	int CheckIndex(char *pSzMediaFile, char *pSzIndexFile);


	//用于修复建立索引 的读取
	CBuffer *m_pBufferFixRead;
	FILE	*m_pFileFixRead;
	int64	 m_s64FixTimestamp;
	int64	m_s64FixFrameCount;
	int64	m_s64FileTotalSize;

	FILE			*m_pFile;
	FILE			*m_pFileRead;
	CMediaFileIndex *m_pIndex;
	bool m_bExit;
	char m_szFile[256];
};
