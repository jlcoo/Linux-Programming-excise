#ifndef CLTHREAD_H
#define CLTHREAD_H

#include<pthread.h>
#include"CLStatus.h"
class CLThread
{
public:
	CLThread();
	virtual ~CLThread();
	CLStatus Run();
	CLStatus WaitForDeath();
private:
	static void *StartFunctionOfThread(void *);
protected:
	virtual CLStatus RunThreadFunction()=0;
	pthread_t m_ThreadID;
};

#endif
