

IfFunction()
{
	if (!m_pProcessor)
		return -1;
	if (!m_pSession)
	{
		m_pSession = m_pProcessor->GetSession();
		if (!m_pSession)
		{
			return -2;
		}
	}
	//
	int nRet = 0;
	CRpcMessage rpcCall;
	//{build message
	/*rpcCall.SetService(m_strName);
	rpcCall.SetFunction(string("Function"));
	//组包
	CFunctionParams *pParams = rpcCall.GetFunctionParams();
	CVariant* pVar(NULL) ;
	//返回值
	pVar = new CVariant(eVariantType_int32);
	pParams->SetReturn(pVar);
	//参数
	CParam*	pParam = new CParam();
	pVar = pParam->m_pVariant;
	*pVar = CVariant(nCount);
	pParams->Append(pParam);
	*/
	//} 
	//发送
	CRpcMessage *pRpcRet(NULL);
 
	nRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, 20*1000);
	if (nRet < 0)
	{
		debug("%s: false\n", __FUNCTION__);
		return -1;
	}
	else
	{
		if (pRpcRet)
		{
			nRet = pRpcRet->GetFunctionParams()->GetReturn()->Value<int>();
			debug("%s: ok: ret [%d]\n", __FUNCTION__, nRetValue);
		}
	}

	return nRet;
};
IfSlot()
{
	//
	CVariant *pVarRet = pParams->GetReturn();
	CParam* pParam = pParams->GetParam().at(0);
	int nRet = 0 ;
	//{build OnXXX call;

	//int nCount = pParam->m_pVariant->Value<int>();
	//int nRet = OnFunction(nCount);
	//}
	//pVarRet->SetValue(nRet);
	*pVarRet = CVariant(nRet);
};
