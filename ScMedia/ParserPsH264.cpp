#include "StdAfx.h"
#include "ParserPsH264.h"
#include "FilterPinBuffer.h"
using namespace SimpleCpp;

CParserPsH264::CParserPsH264(void)
: m_pBufferStream(NULL)
{
}

CParserPsH264::~CParserPsH264(void)
{
	if (m_pBufferStream)
	{
		delete m_pBufferStream;
		m_pBufferStream = NULL;
	}
}
 int CParserPsH264::Open(int nBufferCount)
{

	m_pBufferStream = new CFilterPinBuffer();

	return 0;
}

 int CParserPsH264::InputData(char *pData, int nLen)
{
	if (!pData)
		return -1;
	if (!m_pBufferStream)
		return -2;
	m_pBufferStream->Append(pData, nLen);
	//½âÎöÂëÁ÷¡£¡£¡£


	//

	return nLen;
}