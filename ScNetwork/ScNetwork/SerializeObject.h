#ifndef SerializeObject_H
#define SerializeObject_H

#include "ScpDefine.h"
namespace SimpleCpp{
class CBuffer;
}
using namespace SimpleCpp;;
class SCP_API CSerializeObject
{
public:
	CSerializeObject(void);
	virtual ~CSerializeObject(void);

	virtual int Write(CBuffer* pBuffer);
	virtual int Read(CBuffer* pBuffer);
};

#endif
