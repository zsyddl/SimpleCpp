#pragma once
//内部使用的日志类

//
class CLog
{
public:
	CLog(void);
	~CLog(void);

	long Open(char* pSzPath);
	long Close();

	long Write(int nLevel, char*pFormat, ...);
	
protected:
};
