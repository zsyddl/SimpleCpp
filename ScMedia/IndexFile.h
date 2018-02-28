#pragma once


#include <stdio.h>
#include <stdlib.h>

typedef long long int64;
typedef int int32;

typedef enum _eIndexFileOpenMode{
	eIFOM_Read = 0x000000001,
	eIFOM_Write = 0x00000002,
	eIFOM_ReadWrite = (eIFOM_Read|eIFOM_Write),
}eIndexFileOpenMode;
#define INDEX_FILE_HEAD_LEN  128
#define DATA_FILE_HEAD_LEN   1024

typedef struct _sIndex{
	int64 s64Pos;
	int64 s64Len;
}sIndex;



class CIndexFile
{
public:
	CIndexFile(void);
	virtual ~CIndexFile(void);

	int Open(char *pSzFile, int nOpenMode);
	int Close();

	//int Seek(char *pSzIndex);
	int Seek(int64 nIndex);
	int64 GetMaxIndex();

	int ReadBlock(char *pSzBuffer, int &nReadMax);
	int WriteBlock(char *pSzBuffer, int &nWrite);

	int WriteHeader(char *pHeader, int nSize);
	int ReadHeader(char *pHeader, int nSize);
protected:
	int64 GetFileLength(FILE *pFile);

	unsigned int  m_u32OpenMode;
	FILE *m_pFile; //Êý¾Ý
	FILE *m_pFileIndex;//Ë÷Òý
	char m_szFile[256];
	int64 m_s64Index;
};
