#include "StdAfx.h"
#include "RpcMessage.h"
#include "RpcService.h"
#include "Buffer.h"
#include "Variant.h"

namespace SimpleCpp{
CRpcMessage::CRpcMessage(void)
: m_pParams(NULL)
, m_iObjectIdFrom(0)
, m_iObjectIdTo(0)
{
	m_nType = eMessageType_Request;
	m_pParams = new CFunctionParams();
}

CRpcMessage::CRpcMessage( CRpcMessage *pRequest )
: m_pParams(NULL)
, m_iObjectIdFrom(0)
, m_iObjectIdTo(0)
{
	m_nType = eMessageType_Response;
	m_pParams = new CFunctionParams();
	m_pSession = pRequest->m_pSession;

	m_strService = pRequest->m_strService;
	m_strFunction = pRequest->m_strFunction + " Ret";
	
	m_nSessionId = pRequest->m_nSessionId;
	m_nMessageId = pRequest->m_nMessageId;
	//id 需要交换
	m_iObjectIdFrom = pRequest->m_iObjectIdTo;
	m_iObjectIdTo = pRequest->m_iObjectIdFrom;
	//
	CFunctionParams* pParams = pRequest->GetFunctionParams();
	m_pParams->GetReturn()->CopyFrom(pParams->GetReturn());
	//复制需要返回的数值
	m_pParams->CopyOutParam(pParams);
	//m_pParams->
	//
}

CRpcMessage::CRpcMessage(eMessageType type)
: m_pParams(NULL)
, m_iObjectIdFrom(0)
, m_iObjectIdTo(0)
{
	m_nType = type;
	m_pParams = new CFunctionParams();
}
CRpcMessage::~CRpcMessage(void)
{
	if (m_pParams)
	{
		delete m_pParams;
		m_pParams = NULL;
	}
}

void CRpcMessage::SetService( string &strServiceName )
{
	m_strService = strServiceName;
}

void CRpcMessage::SetFunction( string &strFunction )
{
	m_strFunction = strFunction;
}

CFunctionParams* CRpcMessage::GetFunctionParams()
{
	return m_pParams;
}

int CRpcMessage::Build()
{
	//
	m_pBuffer->Clear();
	CSimpleMessage::Build();
	//service name
	m_pBuffer->Write(m_strService);
	//function name
	m_pBuffer->Write(m_strFunction);
	//
	m_pBuffer->Write(m_iObjectIdFrom);
	m_pBuffer->Write(m_iObjectIdTo);
	//return value
	CBuffer* pParamBuffer = m_pParams->GetBuffer();
	m_pBuffer->Append(pParamBuffer);
	/*CVariant *pVar = m_pParams->GetReturn();
	pVar->WriteTo(m_pBuffer);
	//params;
	vector<CParam*> &lstParams = m_pParams->GetParam();
	//
	char nParamCount = lstParams.size();
	m_pBuffer->Write(&nParamCount, sizeof(nParamCount));
	//
	for (int i=0; i<lstParams.size();
		i++)
	{
		CParam* pParam = lstParams.at(i);
		//CVariant *pVar = pParam->m_pVariant;
		pParam->WriteTo(m_pBuffer);
	}*/

	m_nPackageLength = m_pBuffer->GetUsedSize();
	
	ReFillLength(m_nPackageLength);
 
	return 0;
}

eMessageParseState CRpcMessage::Parse( CBuffer *pBuffer , int &nPassedLen)
{
	eMessageParseState state(eMessageParseState_Success);
	//
	int nPassedTmp = 0;
	state = CSimpleMessage::Parse(pBuffer, nPassedTmp);
	nPassedLen = nPassedTmp;

	if (state != eMessageParseState_Success)
	{
		return state;
	}
	
	int nHeaderLen = CSimpleMessage::GetHeaderLength();
	m_pBuffer->Seek(nHeaderLen);
	//service name
	m_pBuffer->Read(m_strService);
	//function name
	m_pBuffer->Read(m_strFunction);
	//
	m_pBuffer->Read(m_iObjectIdFrom);
	m_pBuffer->Read(m_iObjectIdTo);

	//放入buffer 不做进一步解析了 在用户扩展地方进行序列化操作
	//nPassedLen = pBuffer->GetPos();
	CBuffer* pParamBuffer = m_pParams->GetBuffer();

	pParamBuffer->Append(m_pBuffer->GetData()+m_pBuffer->GetPos(),
						 m_nPackageLength-m_pBuffer->GetPos());
	

	//nPassedLen = m_nPackageLength;
	return state;
	//
	//return value
	
	//
	CVariant *pVar = new CVariant();
	pVar->ReadFrom(pBuffer);
	m_pParams->SetReturn(pVar);
	//

	//params;
	//
	char nParamCount;
	pBuffer->Read(&nParamCount, sizeof(nParamCount));
	//
	//vector<CVariant*> &lstParams = m_pParams->GetParam();
	for (int i=0;
		i<nParamCount;
		i++)
	{
		CParam*	pParam = new CParam();
		 
		pParam->ReadFrom(pBuffer);
		//
		m_pParams->Append(pParam);
	}
	nPassedLen = pBuffer->GetPos();

	//
	return state;
}

// 
// void CRpcMessage::AppendParam( CVariant *pVar )
// {
// 	 
// 	m_pParams->Append(pVar);
// }
// 
// void CRpcMessage::SetReturn( CVariant* pVar )
// {
// 	m_pParams->SetReturn(pVar);
// }

// int CRpcMessage::AppendI32( int nValue )
// {
// 
// 	return 0;
// }
// 

}