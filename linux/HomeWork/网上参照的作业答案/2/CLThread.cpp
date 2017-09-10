#include"CLThread.h"

CLThread::CLThread()
{
}
CLThread::~CLThread()
{
}
CLStatus CLThread::Run()
{
	int r=pthread_create(&m_ThreadID,NULL,StartFunctionOfThread,this);
	if(r!=0)
		return CLStatus(-1,0);
	return CLStatus(0,0);
}

void *CLThread::StartFunctionOfThread(void *pThis)
{
	CLThread *pThreadThis=(CLThread *)pThis;
	CLStatus s=pThreadThis->RunThreadFunction();
	return (void *)s.m_clReturnCode;
}
CLStatus CLThread::WaitForDeath()
{
	int r = pthread_join(m_ThreadID, 0);
	if(r != 0)
		return CLStatus(-1, 0);
	return CLStatus(0, 0);
}
