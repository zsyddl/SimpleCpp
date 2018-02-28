#include "HostInfo.h"

namespace SimpleCpp{

CHostInfo::CHostInfo()
{

}

CHostInfo::CHostInfo(const CHostInfo &hostInfo)
{
	SetHostName(hostInfo.m_strHostName);
	
	int nLen = 0;
	char *pTemp = NULL;
	list<char *>::const_iterator iter;

	for (iter = hostInfo.m_listAddresses.begin(); iter != hostInfo.m_listAddresses.end(); iter++)
	{
		nLen = strlen(*iter);
		nLen += 1;
		pTemp = new char[nLen];
		if (NULL == pTemp)
		{
			return;
		}
		memset(pTemp, 0, nLen);
		memcpy(pTemp, *iter, nLen);
		m_listAddresses.push_back(pTemp);
	}
}

CHostInfo & CHostInfo::operator=(const CHostInfo &hostInfo)
{
	if (this == &hostInfo)
	{
		return *this;
	}

	//free previous memory
	char *pcTemp = NULL;
	list<char *>::iterator iter;
	list<char *>::iterator iterNext;

	for (iter = m_listAddresses.begin(); iter != m_listAddresses.end();)
	{
		iterNext = iter;
		iter++;
		pcTemp = *iterNext;
		delete pcTemp;
		pcTemp = NULL;
	}

	//assign value
	SetHostName(hostInfo.m_strHostName);

	int nLen = 0;
	char *pTemp = NULL;
	list<char *>::const_iterator constIter;
	for (constIter = hostInfo.m_listAddresses.begin(); constIter != hostInfo.m_listAddresses.end(); constIter++)
	{
		nLen = strlen(*constIter);
		nLen += 1;
		pTemp = new char[nLen];
		if (NULL == pTemp)
		{
			return *this;
		}
		memset(pTemp, 0, nLen);
		memcpy(pTemp, *constIter, nLen);
		m_listAddresses.push_back(pTemp);
	}

	return *this;
}

CHostInfo::~CHostInfo()
{
	char *pcTemp = NULL;
	list<char *>::iterator iter;
	list<char *>::iterator iterNext;
	
	for (iter = m_listAddresses.begin(); iter != m_listAddresses.end();)
	{
		iterNext = iter;
		iter++;
		pcTemp = *iterNext;
		delete pcTemp;
		pcTemp = NULL;
	}
}

list<char *> CHostInfo::Addresses()
{
	return m_listAddresses;
}

string CHostInfo::HostName()
{
	return m_strHostName;
}

void CHostInfo::SetAddresses(const list<char *> &listAddresses)
{
	m_listAddresses = listAddresses;
}

void CHostInfo::SetHostName(const string &strHostName)
{
	m_strHostName = strHostName;
}

//use ping www.sina.com verify this function
CHostInfo CHostInfo::FromName(const string &strName)
{
	char *pName = NULL;
	CHostInfo hostInfo;
	struct hostent *pHostInfo = NULL;

	pName = (char *)(strName.c_str());
	pHostInfo = gethostbyname(pName);
	if (NULL == pHostInfo)
	{
		hostInfo.SetHostName(strName);
		return hostInfo;
	}

	hostInfo.SetHostName(pHostInfo->h_name);

	char *pcTemp = NULL;
	char *pAddr = NULL;
	struct in_addr *pAddrIn; 
	list<char *> listAddresses;
	int nLen = 0;
	char **ppAddrList = pHostInfo->h_addr_list;
	while (*ppAddrList != NULL)
	{
		pAddrIn = (struct in_addr *)(*ppAddrList);
		pcTemp = inet_ntoa(*pAddrIn);
		nLen = strlen(pcTemp);
		nLen += 1;
		pAddr = new char[nLen];
		memset(pAddr, 0, nLen);
		memcpy(pAddr, pcTemp, strlen(pcTemp));
		listAddresses.push_back(pAddr);
		ppAddrList++;
	}
	
	hostInfo.SetAddresses(listAddresses);

	return hostInfo;
}

string CHostInfo::LocalDomainName()
{
	int nRet = 0;
	string strAdapterName,strAdapterAlias; 
	HKEY hKey, hSubKey, hNdiIntKey;

	char *pcSubKey = "SYSTEM\\CurrentControlSet\\Control\\Class";
	WCHAR wszClassName[256];  
	memset(wszClassName,0,sizeof(wszClassName));  
	MultiByteToWideChar(CP_UTF7, 0, pcSubKey, strlen(pcSubKey)+1, wszClassName, sizeof(wszClassName)/sizeof(wszClassName[0]));
	nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszClassName, 0, KEY_READ, &hKey);
	if(nRet != ERROR_SUCCESS) 
		return "";  

	DWORD dwIndex = 0; 
	DWORD dwBufSize = 256; 
	DWORD dwDataType; 
	TCHAR szSubKey[256]; 
	char szTemp1[50];
	char szTemp2[50];
	char szTemp3[50];
	char szTemp4[50];
	WCHAR wszTempt1[100];
	WCHAR wszTempt2[100];
	WCHAR wszTempt3[100];
	WCHAR wszTempt4[100];

	memset(szTemp1, 0, 50);
	memset(szTemp2, 0, 50);
	memset(szTemp3, 0, 50);
	memset(szTemp4, 0, 50);
	memset(wszTempt1,0,sizeof(wszTempt1));
	memset(wszTempt2,0,sizeof(wszTempt2));
	memset(wszTempt3,0,sizeof(wszTempt3));
	memset(wszTempt4,0,sizeof(wszTempt4));

	memcpy(szTemp1, "Ndi\\Interfaces", strlen("Ndi\\Interfaces"));
	memcpy(szTemp2, "LowerRange", strlen("LowerRange"));
	memcpy(szTemp3, "DriverDesc", strlen("DriverDesc"));
	memcpy(szTemp4, "NetCfgInstanceID", strlen("NetCfgInstanceID"));

	MultiByteToWideChar(CP_UTF7, 0, szTemp1, strlen(szTemp1)+1, wszTempt1, sizeof(wszTempt1)/sizeof(wszTempt1[0]));
	MultiByteToWideChar(CP_UTF7, 0, szTemp2, strlen(szTemp2)+1, wszTempt2, sizeof(wszTempt2)/sizeof(wszTempt2[0]));
	MultiByteToWideChar(CP_UTF7, 0, szTemp3, strlen(szTemp3)+1, wszTempt3, sizeof(wszTempt3)/sizeof(wszTempt3[0]));
	MultiByteToWideChar(CP_UTF7, 0, szTemp4, strlen(szTemp4)+1, wszTempt4, sizeof(wszTempt4)/sizeof(wszTempt4[0]));

	LPWSTR lpTemp1 = wszTempt1;
	LPWSTR lpTemp2 = wszTempt2;
	LPWSTR lpTemp3 = wszTempt3;
	LPWSTR lpTemp4 = wszTempt4;
	unsigned char szData[256];

	while(RegEnumKeyEx(hKey, dwIndex++, szSubKey, &dwBufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) 
	{ 
		if(RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) 
		{      
			if(RegOpenKeyEx(hSubKey, lpTemp1, 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)  
			{  
				dwBufSize = 256;  
				if(RegQueryValueEx(hNdiIntKey, lpTemp2, 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)  
				{   
					if(strstr((char*)szData, "ethernet") != NULL)//  判断是不是以太网卡   
					{   
						dwBufSize = 256;   
						if(RegQueryValueEx(hSubKey, lpTemp3, 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)   
						{    
							strAdapterName = (char *)szData;    
							dwBufSize = 256;    
							if(RegQueryValueEx(hSubKey, lpTemp4, 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)    
							{    
								strAdapterAlias = (char *)szData;
								break;    
							}  
						}  
					}  
				}

				RegCloseKey(hNdiIntKey);  
			}  
			RegCloseKey(hSubKey); 
		}
		dwBufSize = 256; 
	}
	RegCloseKey(hKey);

	string strKeyName = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\"; 
	strKeyName += strAdapterAlias;

	char *pcKeyName = (char *)(strKeyName.c_str());
	WCHAR wszKeyName[256];  
	memset(wszKeyName,0,sizeof(wszKeyName));  
	MultiByteToWideChar(CP_UTF7, 0, pcKeyName, strlen(pcKeyName)+1, wszKeyName, sizeof(wszKeyName)/sizeof(wszKeyName[0]));
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, wszKeyName, 0, KEY_READ, &hKey) != ERROR_SUCCESS) 
		return "";    

	dwBufSize = 256;
	char szGateWay[50];
	WCHAR wszGateWay[100];

	memset(szGateWay, 0, 50);
	memset(wszGateWay, 0, 100);
	memcpy(szGateWay, "DefaultGateway", strlen("DefaultGateway"));
	MultiByteToWideChar(CP_UTF7, 0, szGateWay, strlen(szGateWay)+1, wszGateWay, sizeof(wszGateWay)/sizeof(wszGateWay[0]));

	memset(szGateWay, 0, 50);
	if(RegQueryValueEx(hKey, wszGateWay, 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
		strcpy(szGateWay,(LPCSTR)szData);
	RegCloseKey(hKey);
	
	//获取DNS服务器信息 
	FIXED_INFO *fi = (FIXED_INFO *)GlobalAlloc(GPTR,sizeof( FIXED_INFO));
	ULONG ulOutBufLen = sizeof(FIXED_INFO);
	DWORD ret = ::GetNetworkParams(fi, &ulOutBufLen);
	if(ret != ERROR_SUCCESS) 
	{
		GlobalFree(fi);
		fi = (FIXED_INFO *) GlobalAlloc( GPTR, ulOutBufLen );
		ret = ::GetNetworkParams(fi, &ulOutBufLen);
		if(ret != ERROR_SUCCESS)
		{
			return "";
		}
	} 

	char szDnsIp[50];
	memset(szDnsIp, 0, 50);
	strcpy(szDnsIp, fi->DnsServerList.IpAddress.String);
	IP_ADDR_STRING *pIPAddr = fi->DnsServerList.Next;
	if(pIPAddr != NULL)
		strcpy(szDnsIp, pIPAddr->IpAddress.String);

	string strDnsIp = string(szDnsIp);

	return strDnsIp;
}

string CHostInfo::LocalHostName()
{
	int nRet = 0;
	char acBuf[50];

	memset(acBuf, 0, 50);
	nRet = gethostname(acBuf, 50);
	
	string strName = acBuf;

	return strName;
}

}