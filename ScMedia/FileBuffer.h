#pragma once
/*
* 文件缓存,对应内存缓存
* 缓存的内容存储在文件里面
*/
typedef struct _iobuf FILE;
namespace SimpleCpp{
	class CBuffer;
}
using namespace SimpleCpp;

typedef enum _eFileBufferMode
{
	eFBM_Memory,
	eFBM_File,
}eFileBufferMode;
class CFileBuffer
{
public:
	CFileBuffer(char *pFile=0, int nMaxSize=10*1024*1024);
	~CFileBuffer(void);

	int Append(char *pData, int nLen);
	void Clear();
	int GetData(char *pData, int &nLen);
	int GetData( int nPos, char *pData, int &nLen);
	int GetSize();

	int SetFilePath(char *pFilePath);
	void SetBufferMode();
protected:
	int CreateFile(char *pSzFile);
protected:
	char *m_pSzFile;
	FILE *m_pFile;
	int   m_nMaxSize;
	int	  m_nUsedSize;
	CBuffer *m_pMemBuffer;
	int   m_nBufferMode;
};
