#ifndef DeviceGatewayIfS_H 
#define DeviceGatewayIfS_H  

#include "RpcService.h"
#include "Types.h"
#include "SerializeObject.h"
#include <string>
#ifdef DEVICEGATEWAYIF_EXPORTS 
#define DEVICEGATEWAYIF_API __declspec ( dllexport ) 

#else 
#define DEVICEGATEWAYIF_API __declspec ( dllimport ) 

#endif 
using namespace std; 
using namespace Scp; 
//接口函数 
#define IF_FUN 

//接口回调函数，槽函数 
#define IF_SLOT 

//标记为输出函数 
#define OUT_P 

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
// typedef enum _eMessageType{ 
// 	eMessageType_Login=0, 
// 	eMessageType_Logout, 
// }eMessageType; 
//基础数据类。无接口的类。充当结构体 
// 
class DEVICEGATEWAYIF_API CDeviceInfo : public CSerializeObject { 
public:
	int Write(CBuffer* pBuffer);
	int Read(CBuffer* pBuffer);
public :
	string m_strName ; 
	int m_nId ; 
	string m_strType ; 
} ; 
class DEVICEGATEWAYIF_API CDeviceGatewayIfS : public CRpcService { 
public:
	CDeviceGatewayIfS();
	virtual ~CDeviceGatewayIfS();

	virtual int OnLogin ( string& strUser , string& strPWD , OUT_P int& nReturn ) ;
	//user should not override this function
	virtual int OnLogin(CFunctionParams* pParam);
	virtual int OnGetDevices ( string& strName , OUT_P vector<CDeviceInfo*>& arrDevices ) ;
	//user should not override this function
	virtual int OnGetDevices(CFunctionParams* pParam);
} ; 
#endif 
