
#include "stdafx.h"
#include "Charset.h"

#include <string.h>

/*#define libiconv_open iconv_open*/
CCharset::CCharset(void)
: m_iconv(0)
{
	memset(m_szFromCodec, 0, sizeof(m_szFromCodec));
	memset(m_szToCodec, 0, sizeof(m_szToCodec));
}

CCharset::~CCharset(void)
{
	Close();
}
// asiic
int CCharset::Open(char *pSrcCodec, char *pDesCodec)
{
	if (m_iconv)
		return 1;
	//char *to = "UTF-16";
	//char *from = "UTF-8";
	m_iconv = iconv_open(pSrcCodec, pDesCodec);
	if (!m_iconv){
		return -1;
	}

	return 0;
}
void CCharset::Close()
{
	if (!m_iconv)
		return ;
	iconv_close(m_iconv);

	m_iconv = NULL;
}
int CCharset::Trans(char *pSrc, char *pDes)
{
	if (!m_iconv)
		return -1;

	int nRet = 0 ;
	size_t inputLen = strlen(pSrc);
	int nSrcLenW = 0;

	size_t outlen = inputLen*2;
	
	int charOutPutLen = 0;
	iconv(m_iconv, (const char **)&pSrc, (size_t *)&inputLen, &pDes, (size_t *)&charOutPutLen);

	return nRet;
}

int CCharset::From(char *pSrc, char *pCodecName)
{
	Close();
	int nRet = 0;
	strncpy(m_szFromCodec, pCodecName, sizeof(m_szFromCodec));
	m_pSrc = pSrc;

	return nRet;
}
int CCharset::To(char *pDes, char *pCodecName)
{ 
	Close();
	int nRet = 0;
	strncpy(m_szToCodec, pCodecName, sizeof(m_szToCodec));
	m_iconv = iconv_open(m_szToCodec, m_szFromCodec);
	if (!m_iconv || m_iconv == (void*)-1){
		m_iconv = 0;
		return -1;
	}
	m_pDes = pDes;
	size_t nSrcLen = 0;
	if (strstr(m_szFromCodec, "16") != 0)
	{
		nSrcLen = wcslen((const wchar_t*)m_pSrc)*2;
	}
	else
	{
		nSrcLen = strlen(m_pSrc);
	}
	//必须用这个 size_t 否则数据会错误，主要是 符号引起的 数值错误
	size_t nDesLen = nSrcLen*6;
	nRet = iconv(m_iconv, (const char **)&m_pSrc, (size_t *)&nSrcLen, &pDes, (size_t *)&nDesLen);
	if (nRet == -1)
	{
		return nRet;
	}

	nRet = nDesLen;
	return nRet;
}