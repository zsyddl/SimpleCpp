#ifndef ScpError_H
#define ScpError_H
#include <map>
using namespace std;
typedef enum _eScpError{
	eScpError_Min = -1,
	eScpError_SocketError=eScpError_Min-1000,

	eScpError_RemoteError=eScpError_Min-2000,

	eScpError_PackageError=eScpError_Min-3000,

	eScpError_Max,
}eScpError;
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
