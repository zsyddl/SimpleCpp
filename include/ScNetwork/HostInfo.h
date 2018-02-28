#pragma once
#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <list>
#include <string>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include "ScNetworkDefine.h"

#pragma comment(lib, "Iphlpapi.lib")
using namespace std;

//retrive ip address by domain name,how to get port
namespace SimpleCpp{

#define BUF_LEN_16 16
#define BUF_LEN_32 32
#define BUF_LEN_64 64
#define BUF_LEN_128 128

class SCNETWORK_API CHostInfo
{
public:
	CHostInfo();
	CHostInfo(const CHostInfo &hostInfo);
	CHostInfo & operator=(const CHostInfo &hostInfo);
	~CHostInfo();

	list<char *> Addresses();
	string HostName();
	void SetAddresses(const list<char *> &listAddresses);
	void SetHostName(const string &strHostName);

	static CHostInfo FromName(const string &strName);
	static string LocalDomainName();
	static string LocalHostName();

private:
	string m_strHostName;
	list<char *> m_listAddresses;
};

}

#endif