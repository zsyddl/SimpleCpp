#include "StdAfx.h"
#include "MediaFileIndex.h"
#include "FfmpegCodec.h"
#include <stdlib.h>
#include "Charset.h"
#include "MediaStream.h"

extern "C" {
#include "sqlite3.h"
};

CMediaFileIndex::CMediaFileIndex(void)
: m_pSqliteDb(NULL)
, m_pStmtRead(NULL)
, m_nFps(25)
, m_nTimebaseDen(25)
, m_nTimebaseNum(1)
, m_s64StartTime(0)
, m_nWriteTmp(0)
, m_pStmtWrite(NULL)
, m_bComplete(false)
, m_s64TotalTime(0)
, m_s64LastWriteTimestamp(0)
, m_s64FileSize(0)
, m_s64EndTimestamp(-1)
, m_s64BeginTimestamp(-1)
, m_nFpsDen(1)
, m_nFpsNum(25)
{
	//debug("%s\n", __FUNCTION__);
}

CMediaFileIndex::~CMediaFileIndex(void)
{
	//debug("%s\n", __FUNCTION__);
	if (m_pStmtRead)
	{
		sqlite3_finalize(m_pStmtRead);
		m_pStmtRead = NULL;
	}
	if (m_pStmtWrite)
	{
		sqlite3_finalize(m_pStmtWrite);
		m_pStmtWrite = NULL;
	}

	if (m_pSqliteDb)
	{
		char *pErrorMsg;
		sqlite3_exec(m_pSqliteDb, "commit", 0, 0, &pErrorMsg);
		sqlite3_close(m_pSqliteDb);
		m_pSqliteDb = NULL;
	}
}
//都是内存索引。
//
int CMediaFileIndex::Open(char *pSzFile)
{

	if (m_pSqliteDb)
	{
		return -2;
	}
	char szDbUrl[256];
	memset(szDbUrl, 0, sizeof(szDbUrl));
	if (pSzFile == NULL
		|| strlen(pSzFile) <= 0)
	{
		sprintf(szDbUrl, "file:%d?mode=memory", this );
	}
	else{
		strcpy(szDbUrl, pSzFile);
		//remove(szDbUrl);
	}
	//m_pSqliteDb = new sqlite3;
	//不存在：创建，存在：打开
	int nRet = 0;
	CCharset charset;

	char szFileUtf8[1024];
	memset(szFileUtf8, 0, sizeof(szFileUtf8));
	charset.From(szDbUrl, "gb18030");
	nRet = charset.To(szFileUtf8, "utf-8");
	if (nRet < 0)
		return -3;
	
	nRet = sqlite3_open(szFileUtf8, &m_pSqliteDb);
	if (nRet != SQLITE_OK)
		return -1;

	char szSql[256];
	memset(szSql, 0, sizeof(szSql));
	//创建参数表
	sprintf(szSql, "create table Param (ID integer primary key autoincrement, name nvarchar(64), value nvarchar(64) )");
	char *pError = 0;
	sqlite3_exec(m_pSqliteDb, szSql, NULL, NULL, &pError);
	if (pError)
	{
		sqlite3_free(pError);
	}
	//数据表 stream0, stream1 stream2 ....
	memset(szSql, 0, sizeof(szSql));
	sprintf(szSql, "create table Stream0 (ID integer primary key autoincrement,  \
		Timestamp BIGINT,		   \
		StreamId integer, \
		FrameType integer,\
		Position BIGINT, \
		FrameLen integer, \
		FrameData blob ) \
		");

	sqlite3_exec(m_pSqliteDb, szSql, NULL, NULL, &pError);
	if (pError)
	{
		sqlite3_free(pError);
	}


	
	char *zErrorMsg;
	sqlite3_exec(m_pSqliteDb, "begin", 0 , 0 ,& zErrorMsg);

	m_nWriteTmp = 0;

	LoadParams();

	sqlite3_prepare(m_pSqliteDb, "select * from Stream0", -1, &m_pStmtRead, 0);

	return 0;
}

int CMediaFileIndex::Close()
{

	SaveParams();

	if (m_pStmtRead)
	{
		sqlite3_finalize(m_pStmtRead);
		m_pStmtRead = NULL;
	}
	if (m_pStmtWrite)
	{

		sqlite3_finalize(m_pStmtWrite);
		m_pStmtWrite = NULL;
	}
	if (m_pSqliteDb)
	{
		char *pErrorMsg=0;
		sqlite3_exec(m_pSqliteDb, "commit", 0, 0, &pErrorMsg);
		sqlite3_close(m_pSqliteDb);
		m_pSqliteDb = NULL;
	}
	return 0;
}
int CMediaFileIndex::Save(char *pSzFile)
{
	if (!pSzFile || strlen(pSzFile) <= 0)
	{
		
	}
	SaveParams();

	return 0;
}

int CMediaFileIndex::Write(int nId, int64 s64Timestamp , int nStreamId, int nFrameType, int64 s64Pos, char* pData, int &nDataLen)
{

	if (!m_pSqliteDb)
		return -1;
	if (m_s64BeginTimestamp < 0)
	{
		m_s64BeginTimestamp = s64Timestamp;
	}
	 

	sqlite3_stmt  *pStmt = NULL;//写二进制数据时要用的结构

	sqlite3_prepare( m_pSqliteDb, "insert into Stream0( timestamp, StreamId,  FrameType, Position, FrameLen) values( ?, ?, ?, ?, ? )",
		-1, &pStmt, 0 );//准备插入数据
	
	sqlite3_bind_int64(pStmt, 1, s64Timestamp);
	sqlite3_bind_int(pStmt, 2, nStreamId);
	sqlite3_bind_int(pStmt, 3, nFrameType);
	sqlite3_bind_int64(pStmt, 4, s64Pos);
	sqlite3_bind_int64(pStmt, 5, nDataLen);

	//sqlite3_bind_blob( pStmt, 2, pFrame, nLen, NULL ); //把内容和字段绑定

	int nRet = 0;
	nRet = sqlite3_step( pStmt );//执行

	sqlite3_finalize( pStmt );//释放内存
	pStmt = NULL;

	if (nRet != SQLITE_DONE)
	{
		const char *pErrorMsg = 0;
		pErrorMsg = sqlite3_errmsg(m_pSqliteDb);
		debug("%s, error: %s\n", __FUNCTION__, pErrorMsg);
		return -1;
	}
	m_nWriteTmp++;

	if (m_nWriteTmp %5000 == 0)
	{
		char *pErrorMsg = 0;
		sqlite3_exec(m_pSqliteDb, "commit", 0, 0, &pErrorMsg);
		sqlite3_exec(m_pSqliteDb, "begin", 0, 0, &pErrorMsg);
		m_nWriteTmp = 0;
	}

	m_s64LastWriteTimestamp = s64Timestamp;
	m_s64TotalTime = m_s64LastWriteTimestamp * 1000* m_nTimebaseNum/m_nTimebaseDen;

	m_s64EndTimestamp = s64Timestamp;

	return 0;
}
int CMediaFileIndex::Read(int &nId, int64 &s64Timestamp , int &nStreamId, int &nFrameType, int64 &s64Pos, char* pData, int &nDataLen)
{
	if (!m_pSqliteDb )
		return -1;

	if (!m_pStmtRead)
	{//没有， 重新 读取
		return -1;
		
	}

	if (!m_pStmtRead)
		return -1;
	//一次取一行
	int result = sqlite3_step ( m_pStmtRead );

	if (result != SQLITE_ROW)
	{
		if (result == SQLITE_DONE)
		{
			sqlite3_finalize(m_pStmtRead);
			m_pStmtRead = NULL;
			//sqlite3_reset(m_pStmtRead);
		}
		return -2;
	}
	s64Timestamp = sqlite3_column_int64( m_pStmtRead, 1 );

	nStreamId = sqlite3_column_int( m_pStmtRead, 2 );
	nFrameType = sqlite3_column_int( m_pStmtRead, 3 );
	s64Pos = sqlite3_column_int( m_pStmtRead, 4 );
	nDataLen = sqlite3_column_int( m_pStmtRead, 5 );
	/*
	const void * pData = sqlite3_column_blob ( m_pStmtRead, 2 );
	int nDataLen = sqlite3_column_bytes ( m_pStmtRead, 2 );
	memcpy(pFrame, pData, nDataLen);
	nLen = nDataLen;
	*/
	//debug("%s: timestamp: %I64d, Type: %d, Pos: %I64d, len: %d\n", __FUNCTION__, s64Timestamp, nFrameType, s64Pos, nDataLen);
	return nDataLen;
}

int CMediaFileIndex::Seek(int64 s64Time, int nFlag/*=0*/)
{
	int64 s64Timestamp = 0;
	s64Timestamp = TimeToTimestamp(s64Time);
	SeekToTimestamp(s64Timestamp, nFlag);

	return 0;
}
int CMediaFileIndex::SeekToTimestamp(int64 s64Timestamp, int nFlag/*=0*/)
{
	if (!m_pSqliteDb)
		return -1;
	
	char *pError = 0;
	char **pResult = 0;
	int nRows = 0, nCols = 0;
	if (s64Timestamp < m_s64BeginTimestamp)
	{
		s64Timestamp = m_s64BeginTimestamp;
	}
	else if (s64Timestamp > m_s64EndTimestamp)
	{
		s64Timestamp = m_s64EndTimestamp;
	}

	char szSql[1024];
	memset(szSql, 0, sizeof(szSql));
	if ( !(nFlag & eSSF_Any) )
	{
		if ( nFlag &eSSF_Backward)
		{
			sprintf(szSql, "select * from Stream0 where timestamp >= \
				(select timestamp from Stream0 where timestamp <= %I64d and FrameType=%d order by Timestamp Desc limit 1)", 
				s64Timestamp, eFFT_I);
		}
		else
		{
			sprintf(szSql, "select * from Stream0 where timestamp >= \
				(select timestamp from Stream0 where timestamp >= %I64d and FrameType=%d limi 1)", 
				s64Timestamp, eFFT_I);
		}
	}
	else
	{
		sprintf(szSql, "select * from Stream0 where timestamp >= %I64d ", s64Timestamp);
	}
	if (m_pStmtRead)
	{
		sqlite3_finalize(m_pStmtRead);
	}
	sqlite3_prepare(m_pSqliteDb,  szSql, -1, &m_pStmtRead, 0);

	return 0;
}

int CMediaFileIndex::SetFps(int nFps)
{
	m_nFps = nFps;

	return m_nFps;
}
int CMediaFileIndex::GetFps()
{
	return m_nFps;
}
int64 CMediaFileIndex::TimeToTimestamp(int64 s64Time)
{
	int64 s64Timestamp = 0;
	
	s64Timestamp = s64Time*m_nTimebaseDen/1000.0/m_nTimebaseNum;

	return s64Timestamp;
}
int64 CMediaFileIndex::TimestampToTime(int64 s64Timestamp)
{
	int64 s64Time=0;
	
	s64Time = s64Timestamp*(1000*m_nTimebaseNum/m_nTimebaseDen);

	return s64Time;
}

int CMediaFileIndex::Flush()
{
	if (!m_pSqliteDb)
		return -1;

	if (m_pStmtWrite)
	{
		char *pErrorMsg=0;
		sqlite3_exec(m_pSqliteDb, "commit", 0, 0, &pErrorMsg);
		sqlite3_exec(m_pSqliteDb, "begin", 0, 0, &pErrorMsg);
	}

	//SaveParams();

	return 0;
}
void CMediaFileIndex::SaveParams()
{
	char szValue[1024];
	//m_s64TotalTime = m_s64LastWriteTimestamp*1000*m_nTimebaseNum/m_nTimebaseDen;
	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%I64d", m_s64TotalTime);
	SaveParam("TotalTime", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%d", m_bComplete);
	SaveParam("Complete", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%I64d", m_s64FileSize);
	SaveParam("FileSize", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%I64d", m_s64BeginTimestamp);
	SaveParam("BeginTimestamp", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%I64d", m_s64EndTimestamp);
	SaveParam("EndTimestamp", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%d", m_nTimebaseNum);
	SaveParam("TimebaseNum", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%d", m_nTimebaseDen);
	SaveParam("TimebaseDen", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%d", m_nFpsNum);
	SaveParam("FpsNum", szValue);

	memset(szValue, 0, sizeof(szValue));
	sprintf(szValue, "%d", m_nFpsDen);
	SaveParam("FpsDen", szValue);
}

void CMediaFileIndex::LoadParams()
{
	char szValue[1024];
	memset(szValue, 0, sizeof(szValue));
	LoadParam("TotalTime", szValue);
	sscanf(szValue, "%I64d", &m_s64TotalTime);
	memset(szValue, 0, sizeof(szValue));
	LoadParam("Complete", szValue);
	sscanf(szValue, "%d", &m_bComplete);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("FileSize", szValue);
	sscanf(szValue, "%I64d", &m_s64FileSize);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("BeginTimestamp", szValue);
	sscanf(szValue, "%I64d", &m_s64BeginTimestamp);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("EndTimestamp", szValue);
	sscanf(szValue, "%I64d", &m_s64EndTimestamp);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("TimebaseNum", szValue);
	sscanf(szValue, "%d", &m_nTimebaseNum);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("TimebaseDen", szValue);
	sscanf(szValue, "%d", &m_nTimebaseDen);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("FpsDen", szValue);
	sscanf(szValue, "%d", &m_nFpsDen);

	memset(szValue, 0, sizeof(szValue));
	LoadParam("FpsNum", szValue);
	sscanf(szValue, "%d", &m_nFpsNum);
}
int CMediaFileIndex::SaveParam(char *pSzName, char *pSzValue)
{
	if (!m_pSqliteDb)
		return -1;
	int nRet = 0;
	if (IsParamExist(pSzName))
	{
		nRet = UpdateParam(pSzName, pSzValue);
	}
	else
	{
		nRet = InsertParam(pSzName, pSzValue);
	}
	return nRet;
}
int CMediaFileIndex::LoadParam(char *pSzName, char *pSzValue)
{
	if (!m_pSqliteDb)
		return -1;
	int nRet = 0;

	bool bExist = false;
	char *pError = 0;
	char **pResult = 0;
	int nRows = 0, nCols = 0;
	char *pSzSql = new char[1024];
	memset(pSzSql, 0, 1024);
	sprintf(pSzSql, "select * from param where name='%s' ", pSzName );
	sqlite3_get_table(m_pSqliteDb, pSzSql,  &pResult, &nRows, &nCols, &pError);
	if (pError)
	{
		sqlite3_free(pError);
	}
	//TRACE("rows: %d, cols: %d, \n", nRows, nCols );
	if (nRows > 0)
	{
		nRet = 0;
		strcpy(pSzValue, pResult[3+2]);
	}
	else
	{
		nRet = -1;
	}
	sqlite3_free_table(pResult);

	delete pSzSql;

	//TRACE("%s: %s, %s \n", __FUNCTION__, pSzName, pSzValue);
	return nRet;
}
bool CMediaFileIndex::IsParamExist(char *pSzName)
{
	if (!m_pSqliteDb)
	{
		return false;
	}
	bool bExist = false;
	char *pError = 0;
	char **pResult = 0;
	int nRows = 0, nCols = 0;
	char *pSzSql = new char[1024];
	memset(pSzSql, 0, 1024);
	sprintf(pSzSql, "select * from param where name='%s' ", pSzName );
	sqlite3_get_table(m_pSqliteDb, pSzSql,  &pResult, &nRows, &nCols, &pError);
	if (pError)
	{
		sqlite3_free(pError);
	}
	//TRACE("rows: %d, cols: %d, \n", nRows, nCols );
	if (nRows > 0)
	{
		bExist = true;
	}
	sqlite3_free_table(pResult);

	delete pSzSql;

	return bExist;
}
int CMediaFileIndex::InsertParam(char *pSzName, char *pSzValue)
{
	if (!m_pSqliteDb)
		return -1;

	int nRet = 0;
	char *pError = 0;
	char *pSzSql = new char[1024];
	memset(pSzSql, 0, 1024);

	sprintf(pSzSql, "insert into param (name, value) values('%s', '%s')", pSzName, pSzValue);

	sqlite3_exec(m_pSqliteDb, pSzSql, NULL, NULL, &pError);
	if (pError)
	{
		sqlite3_free(pError);
		nRet = -1;
	}
	delete pSzSql;

	//TRACE("%s: %s, %s \n", __FUNCTION__, pSzName, pSzValue);

	return nRet;
}
int CMediaFileIndex::UpdateParam(char *pSzName, char *pSzValue)
{
	if (!m_pSqliteDb)
		return -1;

	int nRet = 0;
	char *pError = 0;
	char *pSzSql = new char[1024];
	memset(pSzSql, 0, 1024);

	sprintf(pSzSql, "update  param set value='%s' where name='%s'", pSzValue, pSzName );

	sqlite3_exec(m_pSqliteDb, pSzSql, NULL, NULL, &pError);
	if (pError)
	{
		sqlite3_free(pError);
		nRet = -1;
	}
	delete pSzSql;

	//TRACE("%s: %s, %s \n", __FUNCTION__, pSzName, pSzValue);

	return nRet;
}
void CMediaFileIndex::SetComplete( bool bComplete/*=false*/ )
{
	m_bComplete = bComplete;
}

int64 CMediaFileIndex::GetTotalTime()
{
	return m_s64TotalTime;
}

bool CMediaFileIndex::IsComplete()
{
	return m_bComplete;
}
int64 CMediaFileIndex::GetFileSize()
{
	return m_s64FileSize;
}
void  CMediaFileIndex::SetFileSize(int64 s64Size)
{
	m_s64FileSize = s64Size;
}

void CMediaFileIndex::SetTimebase( int nNum, int nDen )
{
	m_nTimebaseNum = nNum;
	m_nTimebaseDen = nDen;
}
