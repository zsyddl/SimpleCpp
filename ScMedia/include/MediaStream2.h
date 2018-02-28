#pragma once

#include "BasePipeLine.h"
#include "MediaSdkDef.h"

namespace MediaSdk{

	using namespace SimpleCpp;
class MEDIASDK_API CMediaStream2 :
	public CBasePipeLine
{
public:
	CMediaStream2(void);
	virtual ~CMediaStream2(void);
};

}