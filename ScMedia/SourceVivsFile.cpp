#include "StdAfx.h"
#include "SourceVivsFile.h"
#include "Buffer.h"
#include "VivsMediaFile.h"

#ifdef WIN32
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#else
#include <time.h>
#endif

CSourceVivsFile::CSourceVivsFile(void)
:CSourceFilter()
, m_pVivsMediaFile(NULL)
, m_bEndOfFile(true)
{
	strcpy(m_szName, "vivs file");
	strcpy(m_szType, "vivs file");
}

CSourceVivsFile::~CSourceVivsFile(void)
{
	if (m_pBufferRead)
	{
		delete m_pBufferRead;
		m_pBufferRead = NULL;
	}
}

int CSourceVivsFile::DoFilter()
{
	//读取文件放入到 outpin

	CFilterPin *pPinOut = NULL;
	pPinOut = m_lstPinOut.at(0);
	if (!pPinOut)
		return -1;
	CFilterPinBuffer *pPinBuffer = NULL;
	//读取数据
	//ReadFrame();
	ReadOneFrame();
	//
	// 取所有去一个 连接的 pin 
	if (m_pBufferRead->GetLength()>0)
	{
		pPinOut->GetFreeBuffer(&pPinBuffer);
		if (pPinBuffer)
		{
			int nAppend = 0;
			if (pPinBuffer->GetMaxSize() < m_pBufferRead->GetLength())
			{
				pPinBuffer->SetMaxSize(m_pBufferRead->GetLength());
			}
			nAppend = pPinBuffer->Append((char*)m_pBufferRead->GetData(), m_pBufferRead->GetLength());
			if (nAppend <= 0)
			{
				debug("source append to out pin <= 0\n");
			}
			m_pBufferRead->Clear(0, nAppend);
			pPinOut->PutPreparedBuffer(pPinBuffer);
		}
		else
		{
			debug("source filter, out buffer busy\n");
		}
	}
	else
	{
		debug("source filter, read buffer is empty\n");
	}
	Sleep(40);
	//
	return CSourceFilter::DoFilter();
}

int CSourceVivsFile::Init()
{
	m_pBufferRead = new CBuffer();
	return CSourceFilter::Init();
}
int CSourceVivsFile::Open(char *pUrl)
{
	if (m_pVivsMediaFile)
		return -1;
	m_pVivsMediaFile = new CVivsMediaFile();
	if (m_pVivsMediaFile->Open(pUrl, true) <0)
	{
		return -1;
	}
	m_bEndOfFile = false;
	return 0;
}
int CSourceVivsFile::Close()
{
	if (m_pVivsMediaFile)
	{
		m_pVivsMediaFile->Close();
		delete m_pVivsMediaFile;
		m_pVivsMediaFile = NULL;
	}
	return 0;
}
int CSourceVivsFile::ReadOneFrame()
{
	if (!m_pVivsMediaFile)
		return -1;
	if (m_pBufferRead->GetMaxSize()-m_pBufferRead->GetLength() <10* 1024 )
		return 0;
	int nFrameLen = 1024*1024;
	unsigned char *pSzFrameTmp =new unsigned char [nFrameLen];

	int nRet = m_pVivsMediaFile->ReadFrame((char*)pSzFrameTmp, nFrameLen, m_s64TimeStamp);
	if (nFrameLen <= 0 || nRet<0)
	{//读取到文件结尾了。
		m_bEndOfFile = true;
		//break;
	}

	//pStream->m_pMutexInputBuffer->Lock();
	if (nFrameLen > 0)
	{
		m_pBufferRead->Append((unsigned char*)pSzFrameTmp, nFrameLen);
	}

#ifdef debug_source
	static FILE *pfileSource = 0;
	if (pfileSource == NULL)
	{
		pfileSource = fopen("F:\\tmp\\vivs_h264-source.vd", "w+b");
	}
	if (pfileSource)
	{
		fwrite(pSzFrameTmp, nFrameLen, 1, pfileSource);
		fflush(pfileSource);
	}
#endif
	//debug("%s, input data buffer len: %d\n", __FUNCTION__, m_pInputDataBuffer->GetLength());
	//pStream->m_pMutexInputBuffer->Unlock();

	delete pSzFrameTmp;

	return 0;
}
int CSourceVivsFile::ReadFrame()
{
	if (!m_pVivsMediaFile)
		return -1;
	if (m_bSeek)
	{//执行跳转
		m_pVivsMediaFile->SeekTo(m_s64SeekTime);

		m_s64TimeStamp = m_s64SeekTime;
		m_s64TimeLastTimeStamp = m_s64TimeStamp;
		m_s64TimeLastRead = timeGetTime();

		m_bSeek = false;
		m_bEndOfFile = false;
	}
	if (m_bEndOfFile)
	{
		return -1;;
	}
	if (m_state == eFS_Pause)
	{//暂停
		//pStream->m_pThreadReadFile->Sleep(100);
		m_s64TimeLastRead = timeGetTime();
		return -1;
	}
	//计算播放时间

	long lTimeNow = timeGetTime();
	if (m_s64TimeLastRead <= 0)
	{
		m_s64TimeLastRead = lTimeNow;
	}
	long lTimeElapse = lTimeNow - (long)m_s64TimeLastRead;
	if (lTimeElapse <= (long)m_s64imeNeedToWait)
	{//需要休眠，保证 时间走到 文件时间的前面去
		return -1;
	}
	//文件需要 读取的时间
	long lFileTimeNeedTo = (lTimeElapse-(long)m_s64imeNeedToWait )*m_fSpeed;

	//debug("time stamp: %d, need to %d\n", (long)lFileTimeStamp, (long)lFileTimeNeedTo);

	m_s64TimeLastRead = timeGetTime();
	//读取 直到 读的媒体文件的时间 比需要的 播放的时间更多。
	int nFrameLen = 1024*1024;
	unsigned char *pSzFrameTmp =new unsigned char [nFrameLen];
	while (m_state != eFS_Stop &&
		m_state != eFS_Pause && 
		(m_s64TimeStamp-m_s64TimeLastTimeStamp) < lFileTimeNeedTo 
		&& !m_bEndOfFile)
	{
		int nRet = m_pVivsMediaFile->ReadFrame((char*)pSzFrameTmp, nFrameLen, m_s64TimeStamp);
		if (nFrameLen <= 0 || nRet<0)
		{//读取到文件结尾了。
			m_bEndOfFile = true;
			debug("%s: read frame: %d end..... \n", __FUNCTION__, nFrameLen);
			break;
		}
		debug("%s: read frame: %d \n", __FUNCTION__, nFrameLen);
		//pStream->m_pMutexInputBuffer->Lock();
		m_pBufferRead->Append((unsigned char*)pSzFrameTmp, nFrameLen);
		//debug("%s, input data buffer len: %d\n", __FUNCTION__, m_pInputDataBuffer->GetLength());
		//pStream->m_pMutexInputBuffer->Unlock();
	}
	delete pSzFrameTmp;
	if (m_s64TimeLastTimeStamp<=0)
	{
		m_s64TimeLastTimeStamp = m_s64TimeStamp;
	}
	//需要等待的时间
	m_s64imeNeedToWait = (m_s64TimeStamp-m_s64TimeLastTimeStamp-lFileTimeNeedTo)/m_fSpeed;
	if (m_s64imeNeedToWait < 0)
		m_s64imeNeedToWait = 0;
	//时间回调 通知
	int64_t s64StartTime=0, s64EndTime=0;
	m_pVivsMediaFile->GetTime(s64StartTime, s64EndTime);
	if (m_s64TimeLastTimeStamp/1000 != m_s64TimeStamp/1000)
	{//上报时间
		debug("play file time stamp : %d, %d-%d second\n", (long)m_s64TimeStamp/1000, (long)s64StartTime/1000, (long)s64EndTime/1000);
		/*if (m_cbPlayFileTimeStamp)
		{

			(pStream->m_cbPlayFileTimeStamp)((long)pStream, lFileTimeStamp, pStream->m_fPlaySpeed, pStream->m_pCbPlayFileUserData, pStream->m_pCbPlayFileReversed);
		}*/
	}
	else if (m_bEndOfFile)
	{//播放文件结束
		debug("play file time stamp : %d, second\n", (long)m_s64TimeStamp/1000);
		/*if (m_cbPlayFileTimeStamp)
		{
			m_cbPlayFileTimeStamp((long)pStream, -1, pStream->m_fPlaySpeed, pStream->m_pCbPlayFileUserData, pStream->m_pCbPlayFileReversed);
		}
		*/
	}

	m_s64TimeLastTimeStamp = m_s64TimeStamp;

	return 0;
}