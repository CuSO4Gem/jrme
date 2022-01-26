#ifndef AUTOLOCK_H
#define AUTOLOCK_H

#include <mutex>

class AutoLock
{
public:
	AutoLock(std::mutex &iMutex):mMutex(iMutex)
	{
		mMutex.lock();
	}
	~AutoLock()
	{
		mMutex.unlock();
	}
private:
	std::mutex &mMutex;
    
};
#endif