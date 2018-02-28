#pragma once

//用于存储媒体文件的索引。有的文件 ffmpeg 无法获得索引 则需要使用这个类建立索引
//
typedef long long int64;
class CMediaFileIndex
{
public:
	CMediaFileIndex(void);
	~CMediaFileIndex(void);

	int Open(char *pSzFile);
	int Close();
	int Save(char *pSzFile=0);

	int Write(int nId,  int64 s64Timestamp , int nStreamId, int nFrameType, int64 s64Pos, char* pData, int &nDataLen);
	int Flush();

	int Read(int &nId,  int64 &s64Timestamp , int &nStreamId, int &nFrameType, int64 &s64Pos, char* pData, int &nDataLen);

	int Seek(int64 s64Time, int nFlag=0);
	int SeekToTimestamp(int64 s64Timestamp, int nFlag=0);
	int64 TimeToTimestamp(int64 s64Time);
	int64 TimestampToTime(int64 s64Timestamp);

	void SetTimebase(int nNum, int nDen);
	int64 GetTotalTime();
	int SetFps(int nFps);
	int GetFps();
	void SetComplete(bool bComplete=false);
	bool IsComplete();
	int64 GetFileSize();
	void  SetFileSize(int64 s64Size);
protected:
	void SaveParams();
	void LoadParams();
	int SaveParam(char *pSzName, char *pSzValue);
	int LoadParam(char *pSzName, char *pSzValue);
	bool IsParamExist(char *pSzName);
	int InsertParam(char *pSzName, char *pSzValue);
	int UpdateParam(char *pSzName, char *pSzValue);

	struct	sqlite3				*m_pSqliteDb;
	struct	sqlite3_stmt		*m_pStmtRead;
	struct	sqlite3_stmt		*m_pStmtWrite;
	int m_nFps;
	int m_nFpsNum;
	int m_nFpsDen;
	int m_nTimebaseNum;
	int m_nTimebaseDen;
	int64 m_s64StartTime;
	int64 m_s64TotalTime;
	int64 m_s64LastWriteTimestamp;
	int64 m_s64BeginTimestamp;
	int64 m_s64EndTimestamp;

	int  m_nWriteTmp;
	bool	m_bComplete;
	int64	m_s64FileSize;
};
