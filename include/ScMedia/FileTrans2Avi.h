#pragma once

#include "MediaStream.h"

class CFileTrans2Avi
{
public:
	CFileTrans2Avi(void);
	~CFileTrans2Avi(void);
	
	int Open(char *pVivsFile, char *pAviFile);
	int Start();
	int Pause();
	int Stop();
	int Close();


protected:
	
	CThread  *m_pThrTrans;
	eStreamState m_state;
};
