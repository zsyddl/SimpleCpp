#include "Http.h"

namespace SimpleCpp{

CHttpMessageField::CHttpMessageField()
{

}

CHttpMessageField::~CHttpMessageField()
{

}

int CHttpMessageField::SetValue(int nKey, string strValue)
{
	m_strValue = strValue;
	m_nKey = nKey;

	return 0;
}

int CHttpMessageField::GetKey()
{
	return m_nKey;
}

string CHttpMessageField::GetValue()
{
	return m_strValue;
}

CHttpMessage::CHttpMessage()
{
	InitMapInt2String();
}

CHttpMessage::CHttpMessage(CHttpMessage &refHttpMessage)
{
	CHttpMessageField *pHttpMessageField = NULL;
	CHttpMessageField *pIterHttpMessageField = NULL;
	multimap<int, CHttpMessageField*>::iterator iter;

	m_nContentLength = refHttpMessage.m_nContentLength;
	m_strContentType = refHttpMessage.m_strContentType;
	m_strHttpVersion = refHttpMessage.m_strHttpVersion;

	InitMapInt2String();

	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		pIterHttpMessageField = iter->second;
		pHttpMessageField = new CHttpMessageField;
		pHttpMessageField->SetValue(pIterHttpMessageField->GetKey(), pIterHttpMessageField->GetValue());
	}

	m_strMessage = Build();
}

CHttpMessage::CHttpMessage(string strReqMessage)
{
	Parse(strReqMessage);
	m_strMessage = strReqMessage;
}

CHttpMessage::~CHttpMessage()
{
	CHttpMessageField* pTemp = NULL;
	multimap<int, CHttpMessageField*>::iterator iter;
	multimap<int, CHttpMessageField*>::iterator iterErase;
	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); )
	{
		pTemp = iter->second;
		iterErase = iter++;
		m_mapMessageField.erase(iterErase);

		delete pTemp;
	}
}

void CHttpMessage::InitMapInt2String()
{
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_CacheCtrl, "Cache-control"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Date, "Date"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Conn, "Connection"));

	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Accept, "Accept"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Accept_Charset, "Accept-Charset"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Accept_Encoding, "Accept-Encoding"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Accept_Language, "Accept-Language"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Authorization, "Authorization"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Host, "Host"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_UserAgent, "User-Agent"));

	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Location, "Location"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Server, "Server"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_WWWAuthenticate, "WWW-Authenticate"));

	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_ContentEncoding, "Content-Encoding"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_ContentLanguage, "Content-Language"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_ContentLength, "Content-Length"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_ContentType, "Content-Type"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_LastModified, "Last-Modified"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Expires, "Expries"));

	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_ETag, "ETag"));
	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Accept_Ranges, "Accept-Ranges"));

	m_mapInt2String.insert(map<int,char*>::value_type(eHttpMsgField_Text, "Text"));
}

string CHttpMessage::Version()
{
	return "";
}

int CHttpMessage::SetValue(int nKey, string strValue)
{
	CHttpMessageField *pTemp = new CHttpMessageField;

	if (NULL == pTemp)
	{
		return -1;
	}

	pTemp->SetValue(nKey, strValue);
	m_mapMessageField.insert(multimap<int,CHttpMessageField*>::value_type(nKey,pTemp));

	return 0;
}

string CHttpMessage::Value(int nKey)
{
	CHttpMessageField *pHttpMessageField = NULL;
	multimap<int, CHttpMessageField*>::iterator iter;

	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		if (nKey == iter->first)
		{
			pHttpMessageField = iter->second;
		}
	}

	if (NULL != pHttpMessageField)
	{
		return pHttpMessageField->GetValue();
	}

	return NULL;
}

int CHttpMessage::RemoveValue(int nKey)
{
	multimap<int, CHttpMessageField*>::iterator iter;

	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		if (nKey == iter->first)
		{
			m_mapMessageField.erase(iter);
			break;
		}
	}

	return 0;
}

string CHttpMessage::ToString()
{
	return m_strMessage;
}

string CHttpMessage::Build()
{
	string strTemp;
	string strBackSpace = " ";
	string strRef = ":";
	string strCrCl = "\r\n";
	string strReqText;
	CHttpMessageField *pHttpMsgField = NULL;
	multimap<int, CHttpMessageField*>::iterator iter;

	strTemp.append(m_strMethod);
	strTemp.append(strBackSpace);

	strTemp.append(m_strUrl);
	strTemp.append(strBackSpace);

	strTemp.append(m_strHttpVersion);
	strTemp.append(strCrCl);

	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		pHttpMsgField = iter->second;

		if (eHttpMsgField_Text == iter->first)
		{
			strReqText = pHttpMsgField->GetValue();
			continue;
		}
		strTemp.append(m_mapInt2String[iter->first]);
		strTemp.append(strRef);
		strTemp.append(pHttpMsgField->GetValue());
		strTemp.append(strCrCl);
	}

	if (strReqText.size() != 0)
	{
		strTemp.append(strCrCl);
		strTemp.append(strReqText);
	}

	strTemp.append(strCrCl);
	m_strMessage = strTemp;

	return strTemp;
}

int CHttpMessage::Parse(string strRsp)
{
	int nIndex = 0;
	int nEnterIndex = 0;
	int nHttpMsgFieldType = 0;
	string strCrCl = "\r\n";
	string str;
	string strSub;
	string strRspStatusLine;
	string strName;
	string strValue;
	CHttpMessageField *pHttpMsgField;

	m_strMessage = strRsp;
	nEnterIndex = strRsp.find_first_of(strCrCl);
	strRspStatusLine = strRsp.substr(0, nEnterIndex);

	char acTemp1[50];
	char acTemp2[50];
	char acTemp3[50];

	memset(acTemp1, 0, 50);
	memset(acTemp2, 0, 50);
	memset(acTemp3, 0, 50);
	if (strRspStatusLine.find("Get") != string::npos || strRspStatusLine.find("Post") != string::npos)
	{
		sscanf(strRspStatusLine.c_str(), "%s %s %s\r\n", &acTemp1, &acTemp2, &acTemp3);

		m_strMethod = acTemp1;
		m_strUrl = acTemp2;
		m_strHttpVersion = acTemp3;
	}
	else
	{
		sscanf(strRspStatusLine.c_str(), "%s %d %s\r\n", &acTemp1, &m_nRspStatusCode, &acTemp3);

		m_strHttpVersion = acTemp1;
		m_strRspStatusDes = acTemp3;
	}

	strRsp = strRsp.substr(nEnterIndex + 2, strRsp.size() - nEnterIndex - 2);
	while ((nEnterIndex = strRsp.find_first_of(strCrCl)) != -1)
	{
		strSub = strRsp.substr(0, nEnterIndex);
		nIndex = strSub.find_first_of(":");
		if (nIndex < 0)
		{
			strRsp = strRsp.substr(nEnterIndex + 2, strRsp.size() - nEnterIndex - 2);
			break;
		}

		strName = strSub.substr(0, nIndex);
		strValue = strSub.substr(nIndex + 1, strSub.size() - nIndex - 1);

		nHttpMsgFieldType = FindHttpMsgFieldType((char *)(strName.c_str()));

		pHttpMsgField = new CHttpMessageField;
		pHttpMsgField->SetValue(nHttpMsgFieldType, strValue);
		m_mapMessageField.insert(multimap<int,CHttpMessageField*>::value_type(nHttpMsgFieldType,pHttpMsgField));

		strRsp = strRsp.substr(nEnterIndex + 2, strRsp.size() - nEnterIndex - 2);
	}

	if (strRsp.size() != 0)
	{
		pHttpMsgField = new CHttpMessageField;
		pHttpMsgField->SetValue(eHttpMsgField_Text, strRsp);
		m_mapMessageField.insert(multimap<int,CHttpMessageField*>::value_type(eHttpMsgField_Text,pHttpMsgField));
	}

	multimap<int, CHttpMessageField*>::iterator iter;
	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		pHttpMsgField = iter->second;
		if (pHttpMsgField->GetKey() == eHttpMsgField_ContentLength)
		{
			string strLen = pHttpMsgField->GetValue();
			sscanf(strLen.c_str(), "%d", &m_nContentLength);
		}

		if (pHttpMsgField->GetKey() == eHttpMsgField_ContentType)
		{
			m_strContentType = pHttpMsgField->GetValue();
		}
	}

	return 0;
}

int CHttpMessage::FindHttpMsgFieldType(char *pcMsg)
{
	int nRet = -1;
	map<int,char*>::iterator iter;

	for (iter = m_mapInt2String.begin(); iter != m_mapInt2String.end(); iter++)
	{
		if (strcmp(pcMsg, iter->second) == 0)
		{
			nRet = iter->first;
			return nRet;
		}
	}

	return nRet;
}

int CHttpMessage::ContentLength()
{
	return m_nContentLength;
}

string CHttpMessage::ContentType()
{
	return m_strContentType;
}

bool CHttpMessage::HasContentType()
{
	if (m_strContentType.size() == 0)
	{
		return false;
	}
	
	return true;
}

bool CHttpMessage::HasContentLength()
{
	if (0 == m_nContentLength)
	{
		return false;
	}

	return true;
}

void CHttpMessage::SetContentLength(int nContentLength)
{
	m_nContentLength = nContentLength;
}

void CHttpMessage::SetContentType(string strContentType)
{
	m_strContentType = strContentType;
}

bool CHttpMessage::HasKey(string strKey)
{
	string strIterKey;
	multimap<int, CHttpMessageField*>::iterator iter;

	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		strIterKey = m_mapInt2String[iter->first];
		if (strcmp(strIterKey.c_str(), strKey.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}

list<string> CHttpMessage::Keys()
{
	list<string> strList;
	string strIterKey;
	multimap<int, CHttpMessageField*>::iterator iter;

	for (iter = m_mapMessageField.begin(); iter != m_mapMessageField.end(); iter++)
	{
		strIterKey = m_mapInt2String[iter->first];
		strList.push_back(strIterKey);
	}

	return strList;
}

CHttpRequest::CHttpRequest()
{

}

CHttpRequest::~CHttpRequest()
{

}

string CHttpRequest::Path()
{
	return m_strUrl;
}

string CHttpRequest::Method()
{
	return m_strMethod;
}

string CHttpRequest::Version()
{
	return m_strHttpVersion;
}

string CHttpRequest::ToString()
{
	if (m_strMessage.size() == 0)
	{
		m_strMessage = Build();
	}

	return m_strMessage;
}

void CHttpRequest::SetRequest(string strMethod, string strUrl, string strVersion)
{
	m_strMethod = strMethod;
	m_strUrl = strUrl;
	m_strHttpVersion = strVersion;
}

CHttpResponse::CHttpResponse()
{

}

CHttpResponse::~CHttpResponse()
{

}

int CHttpResponse::StatusCode()
{
	return m_nRspStatusCode;
}

string CHttpResponse::ReasonPhrase()
{
	return m_strRspStatusDes;
}

string CHttpResponse::Version()
{
	return m_strHttpVersion;
}

string CHttpResponse::ToString()
{
	return m_strMessage;
}

}