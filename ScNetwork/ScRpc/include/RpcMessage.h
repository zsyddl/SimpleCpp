#pragma once

#include <string>
#include "SimpleMessage.h"
#include "Types.h"

using namespace std;

namespace Scp{
class CFunctionParams;
class CVariant;

class SCP_API CRpcMessage :
	public CSimpleMessage
{
	friend class CRpcDispatcher;
public:
	CRpcMessage(void);
	CRpcMessage(eMessageType type);
	CRpcMessage(CRpcMessage *pRequest);
	virtual ~CRpcMessage(void);

	//
	//int		AppendI32(int nValue);
	
	//
	void		SetService(string &strServiceName);;
	void		SetFunction(string &strFunction);;
	CFunctionParams* GetFunctionParams();
	//
	int64	GetObjectIdC() const { return m_iObjectIdFrom; }
	void	SetObjectIdFrom(int64 val) { m_iObjectIdFrom = val; }
	int64	GetObjectIdS() const { return m_iObjectIdTo; }
	void	SetObjectIdTo(int64 val) { m_iObjectIdTo = val; }
	//
	//void		AppendParam(CVariant *pVar);
	//void		SetReturn(CVariant* pVar);;
	virtual int Build();
	virtual eMessageParseState Parse(CBuffer *pBuffer, int &nPassedLen);
	//
protected:
	string			m_strService;
	string			m_strFunction;
	string			m_strResponse; //"ret";标记为回复

	int64			m_iObjectIdFrom;	//客户端的objectId
	int64			m_iObjectIdTo; //服务端的objectId

	CFunctionParams* m_pParams;
};


}