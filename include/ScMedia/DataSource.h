#pragma once
#include "datafilter.h"

typedef enum _ePlaySpeed{
		
}ePlaySpeed;

class CVivsMediaFile;
class CDataSource
{
public:
	CDataSource(void);
	virtual ~CDataSource(void);

	virtual long Open(char *pSzFile);
	virtual long Close();
	virtual long Start();
	virtual long Stop();
	virtual long Pause();
	virtual long SetSpeed(float fSpeed);
	virtual long GetTime();
	virtual long ReadFrame(unsigned char *pFrame, long &lTimeStamp);
protected:
	//virtual long DoFilter();
protected:
	float m_fPlaySpeed;//
	CVivsMediaFile *m_pVivsMediaFile;
};
