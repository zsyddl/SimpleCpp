#pragma once
#include "render.h"

class CDahuaRender :
	public CRender
{
public:
	CDahuaRender(void);
	virtual ~CDahuaRender(void);

	virtual int Open(HWND hWnd, int nPlayBuffer );
	//filterµÄÖ´ÐÐ º¯Êý 
	virtual int DoFilter();

protected:
	int m_nDhStreamHandle;
};
