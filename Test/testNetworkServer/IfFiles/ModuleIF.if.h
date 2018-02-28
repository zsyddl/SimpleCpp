#ifndef __MODULE_IF_H
#define __MODULE_IF_H

#include "RpcService.h"
#include "Types.h"
#include "SerializeObject.h"
#include <string>
#include "DeviceInfo.h"
#include "ScBuffer.h"


#ifdef IF_EXPORTS
#define IF_API __declspec(dllexport)
#else
#define IF_API __declspec(dllimport)
#endif

using namespace std;
using namespace SimpleCpp;
//接口函数
#define IF_FUN 
//接口回调函数，槽函数
#define IF_SLOT
//标记为输出函数
#define OUT_P
#define ONE_WAY
//只是对接口函数和接口槽 进行翻译。其他函数保持不变
//模块接口， 经过解析之后 生成 服务 和客户端代码。.cpp
//IF_FUN 客户端自动生成代码，不需要手动编写的代码。
//       服务器端会自动生成OnXX回调函数函数无函数体
//IF_SLOT 客户端这个函数被回调，用户直接的代码写入其中就可以了
//		 服务器端自动生成发送消息代码，
// 不能使用 Variant的类型方式做参数。转换，只能直接使用序列化的方式交互。
// 否则在 参数为 class 或者 struct的情况下，导致无法传递结构体 让参数传递变得复杂并且低效

// 每个作为数据的结构体或者 class 生成函数 WriteTo 和 ReadFrom函数 作为序列化操作
//string vector<char> 或者 buffer  接口参数类型不能有指针
typedef enum _eMessageType{
	eMessageType_Login=0,
	eMessageType_Logout,
}eMessageType;
//基础数据类。无接口的类。充当结构体
//
// class IF_API CDeviceInfo1: public CSerializeObject
// {
// public:
// 
// 	string	m_strName;
// 	int		m_nId;
// };


class IF_API CModuleIF:public CRpcService
{
public:
	CModuleIF(void);
	virtual ~CModuleIF(void);
	//必须是int 返回类型必须是int
	//客户端主动调用的 不允许 char* 只能是 string 
	IF_FUN	virtual int Login(string& strUser, string& strPWD, OUT_P int& nReturn);
	 //客户端被动接收的消息回调函数，客户端不需要解析。直接被调用
	IF_FUN virtual int SendData(buffer& bufferData);
	//必须定义一个客户端 对象，用于发送消息
	IF_FUN	virtual int GetDeviceInfo(string& strName, OUT_P CDeviceInfo& deviceInfo );
	IF_FUN	virtual int GetDevices(string& strName, OUT_P vector<CDeviceInfo*>& lstDevices );

	IF_FUN	virtual int CreateStream(OUT_P int64& i64From, OUT_P int64& i64To);
	ONE_WAY IF_FUN virtual int KeepAlive1();
};

#endif