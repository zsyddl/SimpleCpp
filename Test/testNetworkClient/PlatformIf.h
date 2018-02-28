#pragma once
#include "ModuleIfC.h"

class CPlatformIf :
	public CModuleIFC
{
public:
	CPlatformIf(void);
	virtual ~CPlatformIf(void);

	//virtual int OnData( buffer& bufferData );
	
protected:
	FILE*	m_pFile;
};
