
#ifndef LOCK_H
#define LOCK_H

namespace SimpleCpp{

class CLock
{
public:
	CLock(void);
	virtual ~CLock(void);
	bool Lock();
	void Unlock();
	bool TryLock(long lTimeOut);

protected:

};
}


#endif