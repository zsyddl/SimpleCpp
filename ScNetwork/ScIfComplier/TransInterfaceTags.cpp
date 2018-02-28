#include "StdAfx.h"
#include "TransInterfaceTags.h"
#include "Tag.h"
#include "FileTags.h"
#include "RpcService.h"

using namespace SimpleCpp;
CTransInterfaceTags::CTransInterfaceTags(void)
{
}

CTransInterfaceTags::~CTransInterfaceTags(void)
{
}

int CTransInterfaceTags::Trans(CFileTags* pFileTags)
{
	TransTag2Source(pFileTags, m_pFileH, m_pFileCPP, m_strFileName);
	return 0;
}
//完全通过 file tag 转换到源码
int CTransInterfaceTags::TransTag2Source(CFileTags* pFileTags, FILE* pFileH, FILE* pFileCPP, string& strHeaderFile)
{
	//
	int nRet = 0;
	string strTmp;
	//写include头文件
	strTmp = "\/\/cpp file, user can not modify this file \r\n" \
		"#include \"" + strHeaderFile + ".h"  + "\" \r\n";
	strTmp = strTmp + 
		"#include \"" + "RpcMessage.h" + "\" \r\n";
	strTmp = strTmp + 
		"#include \"" + "Variant.h" + "\" \r\n";
	strTmp = strTmp + 
		"#include \"" + "Session.h" + "\" \r\n";
	strTmp = strTmp + 
		"#include \"" + "RpcDispatcher.h" + "\" \r\n";

	strTmp = strTmp + "\r\n";

	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//所有的都需要写头文件和 cpp
	list<CTag*>& lstTopTags = pFileTags->GetTopTags();
	list<CTag*>::iterator item;
	for (item = lstTopTags.begin();
		item != lstTopTags.end();
		item++)
	{
		CTag* pTopTag = *item;
		//只有clas的If 才转换

		if (pTopTag->m_nType == eTagType_Class
			 )
		{
			//写class声明到头文件
			if (pTopTag->m_bHasInterface == true)
			{
				TransIfClass(pTopTag, pFileH, pFileCPP);
			}
			else
			{
				//TransTag2Header(pTopTag, pFileH);
				TransClass(pTopTag, pFileH, pFileCPP);
			}

		}
		else
		{
			eTagType tagType = (eTagType)pTopTag->m_nType;
			TransTag2Header(pTopTag, pFileH);

			if (pTopTag->m_nType == eTagType_Define)
			{
				strTmp = "\r\n";
				fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
			}
		}
		//
	}


	return nRet;
}

int CTransInterfaceTags::TransConstructor2Client(CTag* pTagClass, FILE* pFileH, FILE* pFileCPP)
{
	string strContent;
	string strField;

	int nMaxFiledSize = 255;
	strField.resize(nMaxFiledSize);
	char *pField = (char*)strField.c_str();
	//
	sprintf(pField, "public:\r\n"
		);
	strContent = pField;
	fwrite(strContent.c_str(), strContent.size(), 1, pFileH);
	//写构造函数析构函数 头文件
	sprintf(pField, "\t%s();\r\n"
		,
		pTagClass->m_strName.c_str()
		);
	strContent = pField;
	fwrite(strContent.c_str(), strContent.size(), 1, pFileH);
	// 	sprintf(pField, "\tvirtual ~%s();\r\n"
	// 		,
	// 		pTagClass->m_strName.c_str()
	// 		);
	// 	strContent = pField;
	// 	fwrite(strContent.c_str(), strContent.size(), 1, pFileH);
	//
	//统计 有多少 个接口slot需要进行注册

	//写cpp
	//拼接服务名字
	string strServiceName;
	strServiceName = pTagClass->m_strName;
	int nPos = strServiceName.rfind(m_strClientSuffix);
	strServiceName = strServiceName.substr(0, nPos) + m_strServerSuffix;
	sprintf(pField, "%s::%s()\r\n"
		"{\r\n"
		"\tm_strName = \"%s\";\r\n"
		"\tm_strServiceName = \"%s\";\r\n"
		,
		pTagClass->m_strName.c_str(),
		pTagClass->m_strName.c_str(),
		pTagClass->m_strName.c_str(),
		strServiceName.c_str()
		);
	strContent = pField;
	//"\tRegisterFunction(string("Function"), (ServiceFunctionHandle)&CRpcDuplexService::OnFunction);"
	//注册 各种接口函数
	CTag* pTagMembers = pTagClass->GetProperty(eTagType_Members);

	list<CTag*>::iterator itemClassMember;
	for(itemClassMember = pTagMembers->m_lstChild.begin();
		itemClassMember != pTagMembers->m_lstChild.end();
		itemClassMember++)
	{
		CTag* pTagMember = *itemClassMember;
		if (pTagMember->m_nType != eTagType_IfSlot
			)
		{//不是接口
			continue;;
		}
		//去掉OnXXX 的On;
		string strFunctionName = pTagMember->m_strName;

		int nTrimSize = strlen("On");
		strFunctionName = pTagMember->m_strName.substr(nTrimSize, pTagMember->m_strName.size()-nTrimSize+1);
		//注册回调函数
		sprintf(pField, "\tRegisterFunction(string(\"%s\"), (ServiceFunctionHandle)&%s::%s);\r\n",
			strFunctionName.c_str(),
			pTagClass->m_strName.c_str(),
			pTagMember->m_strName.c_str()
			);
		strContent += pField;
	}

	//

	sprintf(pField, "}\r\n"
		);
	strContent += pField;

	fwrite(strContent.c_str(), strContent.size(), 1, pFileCPP);

	return 0;
}

int CTransInterfaceTags::TransDestructor2Client(CTag* pTagClass, FILE* pFileH, FILE* pFileCPP)
{
	string strContent;
	string strField;

	int nMaxFiledSize = 255;
	strField.resize(nMaxFiledSize);
	char *pField = (char*)strField.c_str();
	//

	sprintf(pField, "\tvirtual ~%s();\r\n\r\n"
		,
		pTagClass->m_strName.c_str()
		);
	strContent = pField;
	fwrite(strContent.c_str(), strContent.size(), 1, pFileH);
	//
	//统计 有多少 个接口slot需要进行注册

	sprintf(pField, "%s::~%s()\r\n"
		"{\r\n"
		"\t\r\n"
		,
		pTagClass->m_strName.c_str(),
		pTagClass->m_strName.c_str()
		);
	strContent = pField;

	sprintf(pField, "}\r\n"
		);
	strContent += pField;

	fwrite(strContent.c_str(), strContent.size(), 1, pFileCPP);

	return 0;
}

int CTransInterfaceTags::TransTag2Header(CTag* pTag, FILE* pFileH)
{
	string strTmp;
	strTmp = pTag->ToString() + "\r\n";
	//strTmp += "\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);

	return 0;
}

//
void CTransInterfaceTags::TransIfFunction(CTag* pTagClassFunction, FILE * pFileH, FILE * pFileCPP)
{
	string strTmp;
	string strField;


	int nMaxFiledSize = 2550;
	strField.resize(nMaxFiledSize);
	
	char *pField = (char*)strField.c_str();
	//写声明不写实现
	strTmp = "\t\/\/user should override this function\r\n"
		"\t";
	CTag* pTagParams(NULL);
	list<CTag*>::iterator item, itemTmp;
	for (item = pTagClassFunction->m_lstProperty.begin();
		item != pTagClassFunction->m_lstProperty.end();
		item++)
	{
		CTag *pTagProperty = *item;
		strTmp += pTagProperty->ToString();
		if (pTagProperty->m_nType == eTagType_Params)
		{
			pTagParams = pTagProperty;
		}
	}
	strTmp += ";\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	strTmp.clear();
	//写实现 
	for (item = pTagClassFunction->m_lstProperty.begin();
		item != pTagClassFunction->m_lstProperty.end();
		item++)
	{
		CTag *pTagProperty = *item;
		if (pTagProperty->m_nType == eTagType_Virtual
			|| pTagProperty->m_nType == eTagType_TokenIfFun
			)
		{// 
			 continue;
		}
		if (pTagProperty->m_nType == eTagType_FunctionName)
		{
			sprintf(pField, "%s::%s ", pTagClassFunction->m_pParent->m_pParent->m_strName.c_str(),  pTagProperty->m_strBody.c_str());
			strTmp += pField;
		}
		else if (pTagProperty->m_nType == eTagType_Params)
		{//写入所有参数
			pTagParams = pTagProperty;
			string strParams;

			TransParams(pTagParams,  strParams);


			strTmp += strParams;
		}
		else
		{
			sprintf(pField, "%s ", pTagProperty->m_strBody.c_str());
			strTmp += pField;
		}
	}
	//
	sprintf(pField, "%s", "\r\n{\r\n");
	strTmp += pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//填写代码部分 打包发送消息参数
	//填写函数开始
	strTmp = 
	"\tif (!m_pProcessor)\r\n"
	"\t	  return -1;\r\n"
	"\tif (!m_pSession) \r\n"
	"\t{\r\n"
	"\t	  m_pSession = m_pProcessor->GetSession();\r\n"
	"\t	  if (!m_pSession)\r\n"
	"\t	  {\r\n"
	"\t			return -2;\r\n"
	"\t	  }\r\n"
	"\t}\r\n"
	"\tint nRet = 0;\r\n";
	if (pTagClassFunction->m_bIsOneWay)
	{
		strTmp += "\tCRpcMessage rpcCall(eMessageType_Oneway);\r\n";
	}
	else
	{
		strTmp += "\tCRpcMessage rpcCall;\r\n";
	}
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//	
	string strBuildMessage;
	//strBuildMessage.resize(nMaxFiledSize);
	//char *pBuildMesage= &strBuildMessage.at(0);
	sprintf(pField, 
	"\trpcCall.SetService(m_strServiceName);\r\n"    //m_strName是在 构造函数里面填充的
	"\trpcCall.SetFunction(string(\"%s\"));\r\n" 
	"\trpcCall.SetObjectIdFrom(m_i64ObjectId);\r\n" 
	"\trpcCall.SetObjectIdTo(m_i64ObjectIdTo);\r\n", 
	pTagClassFunction->m_strName.c_str()
	);
	strBuildMessage += pField;
	//填充返回值
	sprintf(pField,
	"\tCFunctionParams *pParams = rpcCall.GetFunctionParams();\r\n"
	"\tCBuffer* pParamBuffer = pParams->GetBuffer();\r\n"
	"\tpParamBuffer->Write(nRet);\r\n"
	"\t\/\/返回值\r\n"
	"\t\/\/参数\r\n"
	);
	strBuildMessage += pField;
	
	//填充参数构建消息
	string strParams;
	list<CTag*>::iterator itemParam;
	for (itemParam = pTagParams->m_lstChild.begin();
		itemParam != pTagParams->m_lstChild.end();
		itemParam++)
	{
		CTag *pTagParam = *itemParam;
		eParamInOut paramInOut(eParamInOut_In);
		string strInOutP = "eParamInOut_In";
		if (pTagParam->m_lstProperty.front()->m_strName == "OUT_P")
		{
			paramInOut = eParamInOut_Out;
			strInOutP = "eParamInOut_Out";
		}
		CTagType * pTagType = (CTagType*)pTagParam->GetProperty(eTagType_VarType);
		if (pTagType->IsPointer())
		{
			sprintf(pField,
				"\tpParamBuffer->Write(*%s);\r\n"
				"\t\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		else{
			sprintf(pField,
				"\tpParamBuffer->Write(%s);\r\n"
				"\t\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		strParams += pField;
	}
	strBuildMessage += strParams;
	strBuildMessage += "\r\n";

	fwrite(strBuildMessage.c_str(), strBuildMessage.size(), 1, pFileCPP);
	
	string strSendRequest;
	if (pTagClassFunction->m_bIsOneWay)
	{
		strSendRequest = "\tnRet = m_pSession->Send(&rpcCall);\r\n";
	}
	else
	{
		strSendRequest = "\tnRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);\r\n";
	}
	//发送消息并且处理返回值
	strTmp = 

	"\tCRpcMessage *pRpcRet(NULL);\r\n"
	//"\tnRet = m_pSession->Send(&rpcCall,(CMessage**) &pRpcRet, m_nRequestTimeOut);\r\n"
	+ strSendRequest +
	"\tif (nRet < 0)\r\n"
	"\t{\r\n"
	"\t		\/\/debug(\"%s: false\\n\", __FUNCTION__);\r\n"
	"\t		return nRet;\r\n"
	"\t}\r\n"
	"\telse\r\n"
	"\t{\r\n"
	"\t//处理返参数 OUT_P类型 \r\n"
	"\t	int nRetValue(0);\r\n"
	"\t	if (pRpcRet)\r\n"
	"\t	{\r\n"
	"\t		CBuffer *pParamBuffer =  pRpcRet->GetFunctionParams()->GetBuffer();;\r\n"
	"\t		pParamBuffer->Read(nRetValue);\r\n"
	"\t		\r\n"
	;
	//处理返回参数 OUT_P
	//
	string strParamReturn;
	int nParamPos = 0, nOutParamCount(0);
	for (itemParam = pTagParams->m_lstChild.begin();
		itemParam != pTagParams->m_lstChild.end();
		itemParam++, nParamPos++)
	{
		CTag *pTagParam = *itemParam;
		eParamInOut paramInOut(eParamInOut_In);
		string strInOutP = "eParamInOut_In";
		if (pTagParam->m_lstProperty.front()->m_strName != "OUT_P")
		{
			continue;
		}

		CTagType* pTagParamType = dynamic_cast<CTagType*>(pTagParam->GetProperty(eTagType_VarType));
	 
		if (pTagParamType->IsRef())
		{
			sprintf(pField,
				"\t		pParamBuffer->Read(%s);\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		else if (pTagParamType->IsPointer())
		{
			sprintf(pField,
				"\t		pParamBuffer->Read(*%s);\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		else
		{
			//非法的传出参数
		}
		strParamReturn = strParamReturn + pField ;
		nOutParamCount++;
	}
	strTmp += strParamReturn;
	strTmp +=
	"			m_pSession->DestroyResponse(pRpcRet);\r\n"
	"			pRpcRet = NULL;\r\n"
	"			nRet = nRetValue;\r\n"
	"\t	}\r\n"
	"\t}\r\n"
	"\treturn nRet;\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	 
	strTmp.clear();
	sprintf(pField, "%s", "}\r\n");
	strTmp += pField;
	//
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
}

void CTransInterfaceTags::TransIfSlot(CTag* pTagClassMember, FILE * pFileH, FILE * pFileCPP)
{
	string strTmp;
	string strField;
	strField.resize(1024);
	char* pField = &strField.at(0);
	
	list<CTag*>::iterator item, itemTmp;
	CTag* pTagParams(NULL);
	//写声明不写实现
	/*strTmp = "\t\/\/user should override this function\r\n"
		"\t";
	
	
	for (item = pTagClassMember->m_lstProperty.begin();
		item != pTagClassMember->m_lstProperty.end();
		item++)
	{
		CTag *pTagProperty = *item;
		strTmp += pTagProperty->ToString();
		if (pTagProperty->m_nType == eTagType_Params)
		{
			pTagParams = pTagProperty;
		}
	}
	strTmp += ";\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);*/
	//
	TransFunction(pTagClassMember, pFileH, pFileCPP);

	pTagParams = pTagClassMember->GetProperty(eTagType_Params);
	//添加一个 同名函数。并写实现
	strTmp = "\t\/\/user should not override this function\r\n";
	sprintf(pField, "\tvirtual int %s(CFunctionParams* pParam);\r\n",
			pTagClassMember->m_strName.c_str()
			);
	strTmp += pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);


	sprintf(pField, "int %s::%s(CFunctionParams* pFunctionParam)\r\n"
		"{\r\n"
		,
		pTagClassMember->m_pParent->m_pParent->m_strName.c_str(),
		pTagClassMember->m_strName.c_str()
		);
	strTmp = pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//写入函数调用实现部分
	//实际调用回调函数 OnXXX
	sprintf(pField, 
		"\tCBuffer*	pBuffer = pFunctionParam->GetBuffer();\r\n"
		"\tint nRet(0);\r\n"
		"\tpBuffer->Read(nRet);\r\n"
		"\t\r\n"
		, pTagParams->m_lstChild.size()
		);
	strTmp = pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//参数调用
	string strParamsCall;
	//参数变量定义 和读取
	string strParamsVar;
	//参数调用
	int i=0;
	for (item = pTagParams->m_lstChild.begin();
		item != pTagParams->m_lstChild.end();
		item++, i++)
	{
		CTag* pTagParam = *item;
		CTagType* pTagParamType = dynamic_cast<CTagType*>(pTagParam->GetProperty(eTagType_VarType));

		if (i == 0)
		{	//第一个参数
		}
		else
		{
			strParamsCall += "\t";
		}
		if (pTagParamType->IsRef())
		{//引用的情况
			sprintf(pField, "\t%s %s; \r\n"
				"\tpBuffer->Read(%s);\r\n"
				, pTagParamType->GetBaseType().c_str()
				, pTagParam->m_strName.c_str()
				, pTagParam->m_strName.c_str()
				);
		}
		else{
			sprintf(pField, "\t%s %s; \r\n"
				"\tpBuffer->Read(%s);\r\n"
				, pTagParamType->m_strName.c_str()
				, pTagParam->m_strName.c_str()
				, pTagParam->m_strName.c_str()
				);
		}
		//变量参数 声明和读取

		strParamsVar += pField;
		//参数调用
		sprintf(pField, "\t%s "
			, pTagParam->m_strName.c_str()
			);
		strParamsCall += pField;




		itemTmp = item;
		itemTmp++;
		if (itemTmp != pTagParams->m_lstChild.end() )
		{
			strParamsCall += ", \r\n";
		}
		else
		{
			strParamsCall += "\r\n";
		}
		
	}
	//变量声明和读取
	fwrite(strParamsVar.c_str(), strParamsVar.size(), 1, pFileCPP);
	//函数体
	sprintf(pField, "\t\r\n"
		"\t\/\/call OnXXX function\r\n"
		"\tnRet= %s(%s"
		"\t);\r\n"
		, pTagClassMember->m_strName.c_str()
		, strParamsCall.c_str()
		);
	strTmp = pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//

	//对返回值写入
	//将数据序列化 到 pBuffer;, 只是序列化需要out 的param
	sprintf(pField, 
		"\tpBuffer->Clear();\r\n"
		"\t\/\/返回值\r\n"
		"\tpBuffer->Write(nRet);\r\n"
		);
	strTmp = pField;
	string strParamReturn;
	int nParamPos = 0, nOutParamCount(0);
	list<CTag*>::iterator itemParam;
	for (itemParam = pTagParams->m_lstChild.begin();
		itemParam != pTagParams->m_lstChild.end();
		itemParam++, nParamPos++)
	{
		CTag *pTagParam = *itemParam;
		
		if (pTagParam->m_lstProperty.front()->m_strName != "OUT_P")
		{
			continue;
		}

		CTagType* pTagParamType = dynamic_cast<CTagType*>(pTagParam->GetProperty(eTagType_VarType));

		if (pTagParamType->IsRef())
		{
			sprintf(pField,
				"\tpBuffer->Write(%s);\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		else if (pTagParamType->IsPointer())
		{
			sprintf(pField,
				"\tpBuffer->Write(*%s);\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		else
		{
			//非法的传出参数
			sprintf(pField,
				"\tpBuffer->Write(%s);\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
		}
		strParamReturn = strParamReturn + pField ;
		//需要释放 数组里面的指针
		if (pTagParamType->IsType(string("vector")) )
		{
			sprintf(pField,
				"\tpBuffer->Free(%s);\r\n"
				, 
				pTagParam->m_strName.c_str()
				);
			strParamReturn = strParamReturn + pField ;;
		}
		//
		nOutParamCount++;
	}
	strTmp += strParamReturn + "\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);


	sprintf(pField, "\treturn nRet;\r\n"
		"}\r\n"
		);
	strTmp = pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
}

int CTransInterfaceTags::TransParams(CTag* pTagParams,   string &strParams)
{
	int nRet = 0;
	list<CTag*>::iterator itemParam;
	strParams = "";
	string strField;
	int nMaxFieldLen(255);
	strField.resize(nMaxFieldLen);
	char * pField = (char*)strField.c_str();
	for (itemParam = pTagParams->m_lstChild.begin();
		itemParam != pTagParams->m_lstChild.end();
		itemParam++)
	{//所有的参数循环
		CTag* pTagParam = *itemParam;

		//参数的所有的域。
		list<CTag*>::iterator itemParamField;
		for (itemParamField = pTagParam->m_lstProperty.begin();
			itemParamField != pTagParam->m_lstProperty.end();
			itemParamField++)
		{
			CTag* pTagParamField = *itemParamField;
			sprintf(pField, "%s ", pTagParamField->m_strName.c_str());
			strParams += pField;
		}
		//
		list<CTag*>::iterator itemParamTmp = itemParam;
		itemParamTmp++;
		if (itemParamTmp != pTagParams->m_lstChild.end())
		{//, 
			sprintf(pField, "%s", ", ");
			strParams += pField;
		}
		//名字
		//
	}

	return nRet;
}

void CTransInterfaceTags::TransFunction(CTag* pTagClassFunction, FILE * pFileH, FILE * pFileCPP)
{
	string strDeclare;
	string strDefinition;
	string strField;

	strDeclare = "\t";
	strDefinition = "";

	int nMaxFiledSize = 255;
	strField.resize(nMaxFiledSize);

	char *pField = (char*)strField.c_str();
	CTag* pTagParams;
	list<CTag*>::iterator item;
	for (item = pTagClassFunction->m_lstProperty.begin();
		item != pTagClassFunction->m_lstProperty.end();
		item++)
	{
		CTag *pTagProperty = *item;
		if (pTagProperty->m_nType == eTagType_TokenIfFun
			)
		{// 
			//continue;
		}
		else if(pTagProperty->m_nType == eTagType_Virtual)
		{
			//头文件需要
			sprintf(pField, "%s ", pTagProperty->m_strBody.c_str());
			strDeclare += pField;
			//实现不需要
		}
		else if (pTagProperty->m_nType == eTagType_FunctionName)
		{
			sprintf(pField, "%s ",  pTagProperty->m_strBody.c_str());
			strDeclare += pField;

			sprintf(pField, "%s::%s ", pTagClassFunction->m_pParent->m_pParent->m_strName.c_str(),  pTagProperty->m_strBody.c_str());
			strDefinition += pField;

		}
		else if (pTagProperty->m_nType == eTagType_Params)
		{//写入所有参数
			string strParams;
			pTagParams = pTagProperty;
			TransParams(pTagParams,  strParams);

			strDeclare += strParams;
			strDefinition += strParams;
		}
		else
		{
			sprintf(pField, "%s ", pTagProperty->m_strBody.c_str());
			strDeclare += pField;
			strDefinition += pField;
		}
	}
	//declare
	sprintf(pField, ";\r\n");
	strDeclare += pField;
	//
	sprintf(pField, "%s", "\r\n{\r\n");
	strDefinition += pField;
	//填写代码部分 打包发送消息参数

	//
	//if (pTagClassFunction->m_nType == eTagType_Function)
	{//不是 构造和析构的时候
		sprintf(pField, "\t int nRet=0;\r\n\r\n" \
			"\t return nRet;\r\n");
		strDefinition += pField;
	}

	sprintf(pField, "%s", "}\r\n");
	strDefinition += pField;
	//
	fwrite(strDeclare.c_str(), strDeclare.size(), 1, pFileH);	
	fwrite(strDefinition.c_str(), strDefinition.size(), 1, pFileCPP);	
}

void CTransInterfaceTags::TransIfClass(CTag* pTopTag, FILE* pFileH, FILE* pFileCPP)
{
	string strTmp = pTopTag->ToString(eTagType_TokenBrancesLeft) + "\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	//写构造函数 和析构函数
	TransConstructor2Client(pTopTag, pFileH, pFileCPP);
	TransDestructor2Client(pTopTag, pFileH, pFileCPP);
	//转换 接口

	//class 写构造函数等？	
	CTag* pTagMembers(NULL);
	pTagMembers = pTopTag->GetProperty(eTagType_Members);


	//写入 各种接口函数
	list<CTag*>::iterator itemClassMember;
	for(itemClassMember = pTagMembers->m_lstChild.begin();
		itemClassMember != pTagMembers->m_lstChild.end();
		itemClassMember++)
	{
		CTag* pTagClassMember = *itemClassMember;
		if (pTagClassMember->m_nType < eTagType_Interface
			|| pTagClassMember->m_nType > eTagType_InterfaceEnd)
		{//不是接口
			continue;;
		}
		//转换接口函数
		if (pTagClassMember->m_nType == eTagType_IfFunction)
		{//写入到 目标文件
			//1客户端实现发送编码
			TransIfFunction(pTagClassMember, pFileH, pFileCPP);

			printf("%s: if function\n", __FUNCTION__);
		}
		//转换回调函数
		else if  (pTagClassMember->m_nType == eTagType_IfSlot)
		{//
			//客户端实现On 回调
			printf("%s: if slot\n", __FUNCTION__);
			//需要写入头文件 和cpp 添加一个函数 Onxx(CFunctionParam* pParams);
			//填写该函数体。在其中调用 本来的ifslot
			TransIfSlot(pTagClassMember, pFileH, pFileCPP);
		}
	}
	//写 class 结尾
	strTmp = pTopTag->ToString(eTagType_TokenBrancesRight, eTagType_Unknown) + "\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
}

void CTransInterfaceTags::TransClass(CTag* pClass, FILE* pFileH, FILE* pFileCPP)
{
	debug("%s: %s\n", __FUNCTION__, pClass->m_strName.c_str());
	//写入class header
	string strTmp ;
	string strField;
	int nMaxFiledSize = 1024;
	strField.resize(nMaxFiledSize);
	char *pField = &strField.at(0);

	strTmp = pClass->ToString(eTagType_TokenBrancesLeft) + "\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);

	strTmp = "public:\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	//写构造函数 和析构函数
	//应该是通过 tag结构添加构造函数
	//TransConstructor2Client(pClass, pFileH, pFileCPP);
	//TransDestructor2Client(pClass, pFileH, pFileCPP);
	//转换 接口

	//class 写构造函数等？	
	CTag* pTagMembers(NULL);
	pTagMembers = pClass->GetProperty(eTagType_Members);
	
	list<CTag*>::iterator itemMember;
	//写入序列化函数 
	//写入序列化函数。Write Read 函数
	strTmp = "\tint Write(CBuffer* pBuffer);\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	
	string strParamsWrite;
	for(itemMember = pTagMembers->m_lstChild.begin();
		itemMember != pTagMembers->m_lstChild.end();
		itemMember++)
	{
		CTag* pTagMember = *itemMember;
		if (pTagMember->m_nType != eTagType_Variant )
		{
			continue;
		}
		CTagType* pTagType = dynamic_cast<CTagType*>(pTagMember->GetProperty(eTagType_VarType));

		if ( pTagType->IsType(string("vector")) )
		{//write vector

		}
		else 
		{
			sprintf(pField, "\tnRet = pBuffer->Write(%s);\r\n"
				"\tif(nRet < 0)\r\n"
				"\t    return -1;\r\n"
				"\tnWriteLen += nRet;\r\n"
				, pTagMember->m_strName.c_str()
				);
		}
// 		
// 		else
// 		{
// 			sprintf(pField, "\tnRet = %s->Write(pBuffer);\r\n"
// 				"\tif(nRet < 0)\r\n"
// 				"\t    return -1;\r\n"
// 				"\tnWriteLen += nRet;\r\n"
// 				, pTagMember->m_strName.c_str());
// 		}
		strParamsWrite += pField;
	}

	sprintf(pField, "int %s::Write(CBuffer* pBuffer)\r\n"
		"{\r\n"
		"	int nRet=0;\r\n"
		"	int nWriteLen=0;\r\n"
		"%s"
		"	return nRet;\r\n"
		"}\r\n"
		, pClass->m_strName.c_str()
		, strParamsWrite.c_str()
		);
	strTmp = pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);
	//header
	strTmp = "\tint Read(CBuffer* pBuffer);\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
	//cpp
	string strParamsRead;
	for(itemMember = pTagMembers->m_lstChild.begin();
		itemMember != pTagMembers->m_lstChild.end();
		itemMember++)
	{
		CTag* pTagMember = *itemMember;
		if (pTagMember->m_nType != eTagType_Variant )
		{
			continue;
		}

		CTagType* pTagType =	dynamic_cast<CTagType*>(pTagMember->GetProperty(eTagType_VarType));
		if ( pTagType->IsType(string("vector")) )
		{
		}
		else  
		{
			sprintf(pField, "\tnRet = pBuffer->Read(%s);\r\n"
				"\tif(nRet < 0)\r\n"
				"\t    return -1;\r\n"
				"\tnReadLen += nRet;\r\n"
				,  pTagMember->m_strName.c_str());
		}

// 		else
// 		{
// 			sprintf(pField, "tnRet = %s->Read(pBuffer);\r\n"
// 				"\tif(nRet < 0)\r\n"
// 				"\t    return -1;"
// 				"\tnReadLen += nRet;"
// 				,  pTagMember->m_strName.c_str());
// 		}
		strParamsRead += pField;
	}

	sprintf(pField, "int %s::Read(CBuffer* pBuffer)\r\n"
		"{\r\n"
		"	int nRet=0;\r\n"
		"	int nReadLen = 0;\r\n"
		"%s"
		"	return nRet;\r\n"
		"}\r\n"
		, pClass->m_strName.c_str()
		, strParamsRead.c_str()
		);
	strTmp = pField;
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileCPP);


	for(itemMember = pTagMembers->m_lstChild.begin();
		itemMember != pTagMembers->m_lstChild.end();
		itemMember++)
	{
		CTag* pTagClassMember = *itemMember;
		//写入文档
		//转换接口函数
		
		if (pTagClassMember->m_nType == eTagType_Access)
		{
			strTmp = pTagClassMember->ToString( ) + ":\r\n";
			fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
		}
		else{
			strTmp = "\t";
			fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
			TransTag2Header(pTagClassMember, pFileH);
		}
	}

	//写 class 结尾
	strTmp = pClass->ToString(eTagType_TokenBrancesRight, eTagType_Unknown) + "\r\n";
	fwrite(strTmp.c_str(), strTmp.size(), 1, pFileH);
}

void CTransInterfaceTags::BuildFunctionWrite(CTag* pFunction)
{
	CTag* pTag = new CTag();
	pTag->m_strName = "int";
	pTag->m_strBody = "int";
	pTag->m_nType = eTagType_Return;
	pFunction->AppendProperty(pTag);
	pTag = new CTag();
	pTag->m_strName = "Write";
	pTag->m_strBody = "Write";
	pTag->m_nType = eTagType_FunctionName;
	pFunction->AppendProperty(pTag);
	pTag = new CTag();
	pTag->m_strName = "(";
	pTag->m_strBody = "(";
	pTag->m_nType = eTagType_TokenParenthesesLeft;
	pFunction->AppendProperty(pTag);
	CTag *pTagParams = new CTagParams();
	pTagParams->m_strName = "Params";
	pTagParams->m_strBody = "";
	pTagParams->m_nType = eTagType_Params;
	pFunction->AppendProperty(pTagParams);
	CTag* pTagParam = new CTag();
	pTagParam->m_strName = "pBuffer";
	pTagParam->m_strBody = "";
	pTagParam->m_nType = eTagType_Param;
	pTag = new CTag();
	pTag->m_strName = "CBuffer*";
	pTag->m_strBody = "CBuffer*";
	pTag->m_nType = eTagType_VarType;
	pTagParam->AppendProperty(pTag);
	pTag = new CTag();
	pTag->m_strName = "pBuffer";
	pTag->m_strBody = "pBuffer";
	pTag->m_nType = eTagType_VarName;
	pTagParam->AppendProperty(pTag);

	pTagParams->AppendChild(pTagParam);
	//
	pTag = new CTag();
	pTag->m_strName = ")";
	pTag->m_strBody = ")";
	pTag->m_nType = eTagType_TokenParenthesesRight;
	pFunction->AppendProperty(pTag);
}
