#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <map>
#include <set>
#include <list>
#include "ScNetworkDefine.h"
#include "Socket.h"
#include "Thread.h"
#include "AsyncSocket.h"
#include "Semaphore.h"
#include "mutex.h"
#include "Event.h"
#include "Socket.h"
#include "AsyncSocket.h"
#include "SelectPumper.h"
#include "IocpPumpEx.h"
using namespace std;
using namespace SimpleCpp;

namespace SimpleCpp{

#define BUF_LEN 10 * 1024

typedef enum SCNETWORK_API _eHttpMsgField
{
	eHttpMsgField_Invalid = -1,
	eHttpMsgField_Get,
	eHttpMsgField_Post,
	eHttpMsgField_Head,
	eHttpMsgField_Put,
	eHttpMsgField_Delete,
	eHttpMsgField_Trace,
	eHttpMsgField_Connect,
	eHttpMsgField_Option,

	eHttpMsgField_CacheCtrl = 8,
	eHttpMsgField_Date,
	eHttpMsgField_Conn,
	
	eHttpMsgField_Accept = 11,
	eHttpMsgField_Accept_Charset,
	eHttpMsgField_Accept_Encoding,
	eHttpMsgField_Accept_Language,
	eHttpMsgField_Authorization,
	eHttpMsgField_Host,
	eHttpMsgField_UserAgent,

	eHttpMsgField_Location = 18,
	eHttpMsgField_Server,
	eHttpMsgField_WWWAuthenticate,

	eHttpMsgField_ContentEncoding = 21,
	eHttpMsgField_ContentLanguage,
	eHttpMsgField_ContentLength,
	eHttpMsgField_ContentType,
	eHttpMsgField_LastModified,
	eHttpMsgField_Expires,

	eHttpMsgField_ETag = 27,
	eHttpMsgField_Accept_Ranges,

	eHttpMsgField_Text
}eHttpMsgField;

class SCNETWORK_API CHttpMessageField
{
	public:
		CHttpMessageField();
		virtual ~CHttpMessageField();

		int SetValue(int m_nKey, string strValue);
		int GetKey();
		string GetValue();

	private:
		int m_nKey;
		string m_strValue;
};

class SCNETWORK_API CHttpMessage
{
	public:
		CHttpMessage();
		CHttpMessage(CHttpMessage &CHttpMessage);
		CHttpMessage(string strReqMessage);
		virtual ~CHttpMessage();

		//common method
		virtual string Version();
		int ContentLength();
		string ContentType();
		bool HasContentType();
		bool HasContentLength();
		void SetContentLength(int nContentLength);
		void SetContentType(string strContentType);
		bool HasKey(string strKey);
		list<string> Keys();

		int SetValue(int nKey, string strValue);
		string Value(int nKey);
		int RemoveValue(int nKey);

		virtual string ToString();

		string Build();
		int Parse(string strRsp);

	protected:
		void InitMapInt2String();
		int FindHttpMsgFieldType(char *pcMsg);

		int m_nContentLength;
		string m_strContentType;
		string m_strHttpVersion;
		string m_strMessage;
		map<int,char*> m_mapInt2String;
		multimap<int, CHttpMessageField*> m_mapMessageField;

		//request
		string m_strMethod;
		string m_strUrl;

		//response
		int m_nRspStatusCode;
		string m_strRspStatusDes;
};

class SCNETWORK_API CHttpRequest:public CHttpMessage
{
	public:
		CHttpRequest();
		virtual ~CHttpRequest();

		string Path();
		string Method();
		virtual string Version();
		virtual string ToString();
		void SetRequest(string strMethod, string strUrl, string strVersion);

	private:
		
};

class SCNETWORK_API CHttpResponse:public CHttpMessage
{
	public:
		CHttpResponse();
		virtual ~CHttpResponse();
		
		int StatusCode();
		string ReasonPhrase();
		virtual string Version();
		virtual string ToString();

	private:
		
};

}

#endif