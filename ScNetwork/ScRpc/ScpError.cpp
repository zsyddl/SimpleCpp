
#include "ScpError.h"

CScpError::CScpError()
{
	m_mapError2String[eScpError_Min] = "最小值";
	m_mapError2String[eScpError_Max] = "最大值";
}

CScpError::~CScpError()
{
	m_mapError2String.clear();
}

string& CScpError::ErrorString(int nError)
{
	map<int, string> ::iterator item;
	item = m_mapError2String.find(nError);
	if (item == m_mapError2String.end())
	{
		return m_strEmpty;
	}
	return item->second;
}
