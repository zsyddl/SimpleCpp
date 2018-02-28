#include "StdAfx.h"
#include "StreamIf.h"

CStreamIf::CStreamIf(void)
{
}

CStreamIf::~CStreamIf(void)
{
}

int CStreamIf::OnData(buffer& bufferData)
{
	//
	debug("%s: %d\n", __FUNCTION__, bufferData.size());
	//
	return 0;
}
