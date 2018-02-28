#include "StdAfx.h"
#include "IfStream.h"

CIfStream::CIfStream(void)
{
}

CIfStream::~CIfStream(void)
{
}

int CIfStream::OnData(buffer& bufferData)
{
	//
	debug("%s: %d\n", __FUNCTION__, bufferData.size());
	//
	return 0;
}
