#ifdef WIN32
#include "StdAfx.h"
#endif
#include "MultiSelectPumper.h"
#include "AsyncSocket.h"
#include "SelectPumper.h"

using namespace SimpleCpp;
namespace SimpleCpp{


CMultiSelectPumper::CMultiSelectPumper(void)
:m_pSelectedPumper(NULL)
{
}

CMultiSelectPumper::~CMultiSelectPumper(void)
{
}

int CMultiSelectPumper::Append( CAsyncSocket *pSocket )
{
	//ÅÐ¶ÏÊÇ·ñÓÐ¿ÕÏÐµÄ pumpler¡£

	CSelectPumper *pSelectedPumper = NULL;
	pSelectedPumper = GetFreePumper();
	if (pSelectedPumper == NULL)
		return -1;

	pSocket->SetPumper(pSelectedPumper);
	pSocket->SetData((void *)pSelectedPumper);
	//
	return 0;
}

int CMultiSelectPumper::Remove( CAsyncSocket *pSocket )
{
	CSocketPumper *pPumper = (CSocketPumper *) pSocket->GetData();
	if (pPumper)
	{
		pPumper->Remove(pSocket);
	}

	return 0;
}

CSelectPumper* CMultiSelectPumper::GetFreePumper()
{
	CSelectPumper *pstPumper = NULL;
	vector<CSelectPumper *>::iterator iter;

	pstPumper = *(m_vecSelectPumper.begin());
	for (iter = m_vecSelectPumper.begin(); iter != m_vecSelectPumper.end(); iter++)
	{
		if (pstPumper->SocketCount() > (*iter)->SocketCount())
		{
			pstPumper = *iter;
		}
	}

	return pstPumper;
}

int CMultiSelectPumper::Open()
{
	int nRet = 0;

	nRet = CreatePumper();
	return nRet;
}

int CMultiSelectPumper::CreatePumper(int nCount)
{
	CSelectPumper *pSelectPumper = NULL;

	for (int i = 0; i < nCount; i++)
	{
		pSelectPumper = new CSelectPumper;
		if (NULL != pSelectPumper)
		{
			m_vecSelectPumper.push_back(pSelectPumper);
			pSelectPumper->Open();
#ifdef WIN32
			Sleep(2000);
#else
            sleep(2);
#endif
        }
	}

	return 0;
}

int CMultiSelectPumper::DestroyPumper()
{
	CSelectPumper *pstPumper = NULL;
	vector<CSelectPumper *>::iterator iter;

	for(iter = m_vecSelectPumper.begin(); iter != m_vecSelectPumper.end(); )
	{
		pstPumper = *iter;
		iter++;
		pstPumper->Close();
		delete pstPumper;
	}

	return 0;
}

}
