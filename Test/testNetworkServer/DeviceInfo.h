#pragma once
#include <string>

using namespace std;
namespace SimpleCpp{
class CBuffer;
}
using namespace SimpleCpp;
class CDeviceInfo
{
public:
	CDeviceInfo(void);
	virtual ~CDeviceInfo(void);
//实现这个接口就可以被序列化的发送到 rpc消息
	virtual int Read(CBuffer* pBuffer);
	virtual int Write(CBuffer* pBuffer);
public:

	string	m_strName;
	int		m_nId;
};
