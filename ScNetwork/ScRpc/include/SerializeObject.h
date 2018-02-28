#ifndef SerializeObject_H
#define SerializeObject_H

#include "ScpDefine.h"

class CBuffer;
class SCP_API CSerializeObject
{
public:
	CSerializeObject(void);
	virtual ~CSerializeObject(void);

	virtual int Write(CBuffer* pBuffer);
	virtual int Read(CBuffer* pBuffer);
};

#endif
