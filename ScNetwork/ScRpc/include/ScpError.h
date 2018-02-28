#ifndef ScpError_H
#define ScpError_H
#include <map>
using namespace std;
typedef enum _eScpResult{
	eScpResult_Error = -1,
	eScpResult_NullPtr = eScpResult_Error-1000,

	eScpResult_SocketError=eScpResult_Error-2000,

	eScpResult_RemoteError=eScpResult_Error-3000,
	eScpResult_RemoteNoResponse,

	eScpResult_PackageError=eScpResult_Error-4000,

	eScpResult_Min,
}eScpResult;
class CScpError{
public:
	CScpError();
	~CScpError();
	
	string&	ErrorString(int nError);
protected:
	map<int, string>	m_mapError2String;
	string				m_strEmpty;
};

#endif
