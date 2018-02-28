#include "StdAfx.h"
#include "DeviceInfo.h"
#include "Buffer.h"

CDeviceInfo::CDeviceInfo(void)
{
}

CDeviceInfo::~CDeviceInfo(void)
{
}
int CDeviceInfo::Read(CBuffer* pBuffer)
{
	int nWriten(0);
	nWriten += pBuffer->Read(m_strName);
	nWriten += pBuffer->Read(m_nId);

	return nWriten;
}
int CDeviceInfo::Write(CBuffer* pBuffer)
{
	int nWriten(0);
	nWriten += pBuffer->Write(m_strName);
	nWriten += pBuffer->Write(m_nId);

	return nWriten;
}